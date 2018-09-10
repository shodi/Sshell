#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

char *readInput() {
    int buffersize = BUFFER_SIZE;
    int character;
    int counter = 0;
    char *buffer = malloc(sizeof(char) * buffersize);
    if (!buffer) {
err:    fprintf(stderr, "Erro ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }
    
loop: character = getchar();
    if (character == EOF || character == '\n') {
        buffer[counter] = '\n';
        return buffer;
    } else {
        buffer[counter] = c;
    }
    counter++;

    if (counter >= buffersize) {
        buffersize += BUFFER_SIZE;
        buffer = realloc(buffer, buffersize);
        if (!buffer) goto err;
    }
    goto loop;
}

int main(int argc, char **argv) {

    char *input;
    return 0;
}