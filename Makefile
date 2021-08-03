all: CSNode #CSNodeServer

CSNode: CSNode.hpp CSNode.cpp Buffer.cpp Strings.hpp main3.cpp
	g++ main3.cpp CSNode.cpp -o CSNode

CSNodeServer: CSNode.hpp CSNode.cpp Buffer.cpp Strings.hpp Server_poll.cpp
	g++ Server_poll.cpp CSNode.cpp -o CSNodeServer -o CSNodeServer -pthread

clean:
	rm CSNode CSNodeServer
