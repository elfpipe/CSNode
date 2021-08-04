#ifndef SERVER_hpp
#define SERVER_hpp

#include "Buffer.cpp"
#include "CSNode.hpp"
#include <pthread.h>
class CSNode;
class Server {
private:
    CSNode *node;
    Buffer readBuffer;
    int exitPipe[2];
public:
    Server (CSNode *node);
    void startThread ();
    static void *thread(void *dummy);
    void endThread();
};


// int main(int argc, char *argv[]) {
//     CSNode node;

//     if (argc < 2) {
//         printf("Usage : %s <port>\n", argv[0]);
//         return 0;
//     }

//     node.doBind (atoi(argv[1]));

//     Server server (&node);
//     server.startThread ();

//     printf("Thread started.\n");

//     char dummy[1024];
//     fgets (dummy, 1024, stdin);

//     node.unBind();

//     server.endThread();
//     pthread_exit(0);
// }
#endif //SERVER_hpp.
