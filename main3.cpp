#include "CSNode.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
    CSNode node;
    CSNode::CSConnection *connection = 0;

	if(argc > 1) {
		for(int arg = 1; arg < argc; arg++) {
			if(!strcmp(argv[arg], "SCRIPT") ){
				string text;
				ifstream file(argv[++arg]);
				while (std::getline(file, text)) {
					if(!text.compare("REPEAT")) {
						file.close();
						file.open(argv[arg]);
					}
					else connection = node.clientCommand(text, connection);
				}
			} else {
				string command = argv[arg];
				if(arg + 1 < argc) {
					command += " ";
					command += argv[++arg];
					//command += "\n";
				}
				std::cout << "Running command : " << command << "\n";
				connection = node.clientCommand(command, connection);
			}
		}
	}

    printf("Commands: SERVE UNSERVE CALL CLOSE MESSAGE PUSH PUSHDIR PULL CHDIR CWDIR MKDIR EXIT\n");
    
    do {
        char command[1024] = "";

        printf("> ");
        fgets(command, 1023, stdin);
    	connection = node.clientCommand(command, connection);
    } while(true);

    //return 0;
}
