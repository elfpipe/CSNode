#include "Buffer.cpp"
#include "CSNode.hpp"
#include <pthread.h>
#include <unistd.h>
#include <poll.h>
#include <iostream>
#include <strings.h>
#include <sys/select.h>
class Server {
private:
    CSNode *node;
    Buffer readBuffer;
    int exitPipe[2];
public:
    Server (CSNode *node) {
        this->node = node;
    }
    void startThread () {
        pthread_t threadHandle;
        pipe(exitPipe);
        pthread_create (&threadHandle, 0, thread, (void *)this);

    }
    static void *thread(void *dummy) {
        Server *_this = (Server *) dummy;

        fd_set rfds;
        struct timeval tv;
        int retval;

        FD_ZERO(&rfds);
        FD_SET(_this->node->bindSocket, &rfds);
        FD_SET(_this->exitPipe[0], &rfds);

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        while (1) {
            retval = select(1, &rfds, NULL, NULL, &tv);
            if (retval < 0) {
                perror("select");
                pthread_exit(0);
            }

            if (FD_ISSET(_this->node->bindSocket, &rfds)) {
                cout << "Icomming call...\n";

                CSNode::CSConnection *connection = _this->node->waitForIncomming (_this->node->port);
                if (connection) {
                    // cout << "<message> : " << _this->node->readSentence (connection, '\3') << "\n";
                    // _this->node->writeSentence (connection, "CLOSE");
                    _this->node->createServer (connection);
                    _this->node->closeConnection (connection);
                    cout << "Call completed.\n";

                }
            }
            if(FD_ISSET(_this->exitPipe[0], &rfds)) {
                cout << "exitPipe activatet (EXIT)\n";
                pthread_exit(0);
            }
        }
    }
    void endThread() {
        write (exitPipe[1], "hej\0", 4);
    }
};


int main(int argc, char *argv[]) {
    CSNode node;

    if (argc < 2) {
        printf("Usage : %s <port>\n", argv[0]);
        return 0;
    }

    node.doBind (atoi(argv[1]));

    Server server (&node);
    server.startThread ();

    printf("Thread started.\n");

    char dummy[1024];
    fgets (dummy, 1024, stdin);

    node.unBind();

    server.endThread();
    pthread_exit(0);
}