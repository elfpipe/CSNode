#include <unistd.h>
#include <iostream>
#include <strings.h>
#include "CSNode.hpp"
#include "Server.hpp"
Server::Server (CSNode *node) {
    this->node = node;
}
void Server::startThread () {
    cout << "Accepting calls...\n";
    CSNode::CSConnection *connection = node->waitForIncomming (node->port);
    if (connection) {
        cout << "Incomming call(!)...\n";
        // cout << "<message> : " << _this->node->readSentence (connection, '\3') << "\n";
        // _this->node->writeSentence (connection, "CLOSE");
        node->serverCommand (connection);
        node->closeConnection (connection);
        cout << "Call completed.\n";
    }
}
void *Server::thread(void *dummy) {
    return 0;
}
void Server::endThread() {
}
