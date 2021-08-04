#include <unistd.h>
#include <poll.h>
#include <iostream>
#include <strings.h>
#include "CSNode.hpp"
#include "Server.hpp"
Server::Server (CSNode *node) {
    this->node = node;
}
void Server::startThread () {
    pthread_t threadHandle;
    pipe(exitPipe);
    pthread_create (&threadHandle, 0, thread, (void *)this);

}
#define MAX(x,y) ((x) < (y) ? (y) : (x))
void *Server::thread(void *dummy) {
    Server *_this = (Server *) dummy;

    fd_set rfds;
    struct timeval tv;
    int retval;


    while (1) {
        FD_ZERO(&rfds);
        FD_SET(_this->node->bindSocket, &rfds);
        FD_SET(_this->exitPipe[0], &rfds);

        int maxfd = MAX(_this->node->bindSocket, _this->exitPipe[0]);

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        retval = select(maxfd+1, &rfds, 0, 0, &tv);
        if (retval < 0) {
            perror("select");
            pthread_exit(0);
        }

        if (FD_ISSET(_this->node->bindSocket, &rfds)) {
            cout << "Incomming call...\n";
            CSNode::CSConnection *connection = _this->node->waitForIncomming (_this->node->port);
            if (connection) {
                // cout << "<message> : " << _this->node->readSentence (connection, '\3') << "\n";
                // _this->node->writeSentence (connection, "CLOSE");
                _this->node->serverCommand (connection);
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
void Server::endThread() {
    write (exitPipe[1], "hej\0", 4);
}
