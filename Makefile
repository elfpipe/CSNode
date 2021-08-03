all: CSNodeClient CSNodeServer

CSNodeClient: CSNode.hpp CSNode.cpp Buffer.cpp Strings.hpp main3.cpp
	g++ main3.cpp CSNode.cpp -o CSNodeClient

CSNodeServer: CSNode.hpp CSNode.cpp Buffer.cpp Strings.hpp Server_poll.cpp
	g++ Server_poll.cpp CSNode.cpp -o CSNodeServer -o CSNodeServer -pthread

clean:
	rm CSNodeClient CSNodeServer
