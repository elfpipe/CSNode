all: CSNode

CSNode: CSNode.hpp CSNode.cpp Buffer.cpp Strings.hpp Server2.cpp main3.cpp
	g++ main3.cpp CSNode.cpp Server2.cpp -pthread -o CSNode

clean:
	rm CSNode
