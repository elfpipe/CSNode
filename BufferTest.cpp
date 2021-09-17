#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

#include "Buffer.cpp"

int main() {
    Buffer buffer;

    srand (time(NULL));

    const int bufSize = 4096;
    char input[bufSize];
    for(int i = 0; i < 10000; i++) {
        int f = rand() % bufSize;
        printf("fill : %d\r", i);
        buffer.fill(input, f);
        int bytes;
        while((bytes = buffer.readBytes(input, 0)) > 0)
            ; //printf("read : %d\n", bytes);
    }
    printf("\n");
    return 0;
}