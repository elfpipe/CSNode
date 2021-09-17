#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <fcntl.h>
#include <unistd.h>

#include "Buffer.cpp"

#include <iostream>
using namespace std;
int main() {
    Buffer buffer;

    // srand (time(NULL));

    // const int bufSize = 4096;
    // char input[bufSize];
    // for(int i = 0; i < 10000; i++) {
    //     int f = rand() % bufSize;
    //     printf("fill : %d\r", i);
    //     buffer.fill(input, f);
    //     int bytes;
    //     while((bytes = buffer.readBytes(input, 0)) > 0)
    //         ; //printf("read : %d\n", bytes);
    // }
    // printf("\n");

    for (int i = 0; i < 1000000; i++) {
        int fd = open ("../calculator/calculator",  O_RDONLY);
        if(!fd) { perror("open"); continue; }

        //calculate file size
        int size = lseek (fd, 0, SEEK_END);
        lseek (fd, 0, SEEK_SET);
        char sizebuf[128];
        sprintf(sizebuf, "%d", size);

        int totalSent = 0, totalRead = 0, bytes;
        const int bufSize = 4096;
        char input[bufSize];
        while (totalSent == totalRead && totalRead < size && (bytes = read(fd, input, bufSize)) > 0) {
            if (bytes < 0) perror ("read");
            totalRead += bytes;
            cout << "-";
            //first fill the buffer (in case it was already non-empty)
            buffer.fill(input, bytes);
            //...then extract from it
            cout << "'";
            while((bytes = buffer.readBytes(input, bufSize)) > 0) {
  //              cout << ",";
 //               bytes = send(connection->connectionSocket, buffer, bytes, 0);
  //              if(bytes < 0) perror("send");
                totalSent += bytes;
                cout << ".";
            }
            cout << "_";
        }
    }
    return 0;
}