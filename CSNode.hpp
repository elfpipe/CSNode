#ifndef CSNODE_hpp
#define CSNODE_hpp
//client/server transaction from samme node

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/file.h>

#include <vector>
#include <string>
#include <iostream>

#include "Buffer.cpp"
#include "Server.hpp"

using namespace std;

class CSNode {
private:
public:
    int bindSocket, port;
    bool hasBinding;

    Server server;
public:
    struct CSConnection {
        int connectionSocket;
        string identityString;
        Buffer readBuffer;
        bool isValid;
    };

public:
    bool doBind (int port);
    void unBind ();

public:
    CSNode () : server(this) { hasBinding = false; }
    ~CSNode () { server.endThread(); }

    CSConnection *waitForIncomming(int port);
    CSConnection *connectToPeer (const char *address, int port);
    void closeConnection (CSConnection *connection);

    string readSentence (CSConnection *connection, char stopCharacter = '\3');
    bool writeSentence (CSConnection *connection, string sentence);

    void serverCommand (CSConnection *connection);
    CSConnection *clientCommand (string command, CSConnection *connection);

    int clientPUSH (CSConnection *connection, const char *filename);
    int serverPUSH (CSConnection *connection, const char *filename); // PUSH from client

    void clientPUSHDIR (CSConnection *connection, const char *dirname);
    
    string remoteGETCWD (CSConnection *connection);
    void remoteCHDIR (CSConnection *connection, const char *dirname);
    void remoteMKDIR (CSConnection *connection, string dirname);
    string localGETCWD();
    void localCHDIR (string newdir);
};
#endif
