all: CSNode

CSNode: CSNode.hpp CSNode.cpp Buffer.cpp Strings.hpp Server3.cpp main3.cpp
	g++ main3.cpp CSNode.cpp Server3.cpp -pthread -o CSNode

clean:
	rm CSNode
