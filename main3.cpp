#include "CSNode.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    CSNode node;
    CSNode::CSConnection *connection = 0;

    printf("Commands: SERVE UNSERVE CALL CLOSE MESSAGE EXIT PUSH PULL\n");
    
    do {
        char command[1024] = "";

        printf("> ");
        fgets(command, 1023, stdin);
    	connection = node.clientCommand(command, connection);
    } while(true);

    //return 0;
}