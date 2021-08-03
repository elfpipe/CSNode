#include "CSNode.hpp"
#include <iostream>
#include "Strings.hpp"

int safe_send(int socket, const char *message)
{
    int bytes = send (socket, message, strlen(message), 0);
    send (socket, "\3", 1, 0);
    return bytes;
}

int do_PUSH (CSNode &node, CSNode::CSConnection *connection, const char *filename)
{
    int fd = open (filename,  O_RDONLY);
    if (fd < 0) {
        node.writeSentence(connection, "0");
	    perror ("open");
	    return -1;
    }

    //calculate file size
    int size = lseek (fd, 0, SEEK_END);
    lseek (fd, 0, SEEK_SET);
    char sizebuf[128];

    //send file size as string
    sprintf(sizebuf, "%d", size);
    node.writeSentence(connection, sizebuf);

    printf("<send file> : %s , size=%s\n", filename, sizebuf);

    int i;
    for (i = 0; i < size; i++) {
        //construct string with single byte
        char byte;
        int len = read(fd, &byte, 1);
        if(len != 1) break;
        len = send(connection->connectionSocket, &byte, 1, 0);
        if(len != 1) break;
        // char bytebuf[32];
        // sprintf(bytebuf, "%d", byte);
        // node.writeSentence(connection, bytebuf);
    }
    if (i == size)
        printf("<PUSH> : file sent\n");
    else
        printf("<PUSH> : incomplete send\n");

    close (fd);
    return 0;
}

CSNode::CSConnection *do_command(CSNode &node, string command, CSNode::CSConnection *connection = 0) {
    astream a(command);
    string stripped = a.get('\n');
    a.setString(stripped);
    vector<string> argv = a.split(' ');
    string keyword = argv[0];

    cout << "argv[0]: '" << argv[0] << "'\n";

    if(!keyword.compare("SERVE")) {
        if(argv.size() < 2) {
            cout << "Usage : SERVE <port>\n";
        } else {
            cout << "(*) Serving calls on port " << argv[1] << "....\n";

            CSNode::CSConnection *newConnection = node.waitForIncomming (atoi(argv[1].c_str()));
            if(newConnection) {
                cout << "Gracefully accepted call from " << newConnection->identityString << " :)\n";
                connection = newConnection;
                node.createServer (connection);
            }
        }
    } else if(!keyword.compare("CALL")) {
        if(argv.size() < 3) {
            cout << "Usage : CALL <address> <port>\n";
        } else {
            if (connection) {
                cout << "Please close previous connection to " << connection->identityString << " first. (CALL)\n";
                return connection;
            }

            CSNode::CSConnection *newConnection = node.connectToPeer(argv[1].c_str(), atoi(argv[2].c_str()));

            if (newConnection) {
                cout << "Successfully connected to " << argv[1] << "\n";
                cout << "Sending credentials to " << argv[1] << "\n";
                // -- bla bla --
                cout << "Host accepted call.\n";
                connection = newConnection;
                //node.createServer (connection);
            } else cout << "Failed to connect to " << argv[1] << "\n";
        }
        return connection;
    } else if (!keyword.compare("MESSAGE")) {
        if(connection) {
            node.writeSentence (connection, stripped);
        } else cout << "No connection\n";
    } else if (!keyword.compare("CLOSE")) {
        if(connection) {
            node.writeSentence(connection, "CLOSE");
            node.closeConnection(connection);
            cout << "Connection closed\n";
            connection = 0;
        } else cout << "Not connected.\n";
    } else if (!keyword.compare("EXIT")) {
        if (connection) {
            node.writeSentence (connection, "CLOSE");
            node.closeConnection (connection);
        }
        cout << "Connection closed. Exit.\n";
        exit(0);
    } else if (!keyword.compare("PUSH")) {
        if(connection) {
            node.writeSentence (connection, stripped);
            do_PUSH (node, connection, argv[1].c_str());
        } else cout << "No connection\n";
    } else if (!keyword.compare("PULL")) {

    }
    return connection;
}

int main(int argc, char *argv[]) {
    CSNode node;
    CSNode::CSConnection *connection = 0;

    printf("Commands: SERVE CALL MESSAGE CLOSE EXIT PUSH PULL\n");
    
    do {
        char command[1024] = "";

        printf("> ");
        fgets(command, 1023, stdin);
    	connection = do_command(node, command, connection);
    } while(true);

    //return 0;
}