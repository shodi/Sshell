#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include "utils.h"

int change_directory(char **params) {
    if (params[1] == NULL) {
        fprintf(stderr, "Passe algum argumento para \"cd\"\n");
    } else {
        if(chdir(params[1]) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
    }
    return 1;
}

int remove_file(char **params) {
    char *file_name = params[1];
    int file_exists;
    volatile char confirmed;
    if (file_name == NULL) {
        fprintf(stderr, "Passe o nome do arquivo a ser deletado\n");
    } else {
        file_exists = access(file_name, F_OK);
        if (file_exists == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            return 1;
        }
        if(unlink(file_name) != 0) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
    }
    return 1;
}

char *get_file_type(unsigned char dir_type) {
    switch (dir_type) {
        case DT_DIR:
            return "\t(diretorio)";
        case DT_REG:
            return "\t(arquivo)";
        default:
            return "\t(desconhecido)";
    }
}

int list_directory(char **params) {
    char *dirname = params[1] == NULL ? "." : params[1];
    struct dirent* opened_dir;
    DIR* directory = opendir(dirname);
    if (directory == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }
loop:if ((opened_dir = readdir(directory)) != NULL) {
        printf("%s%s\n", opened_dir->d_name, get_file_type(opened_dir->d_type));
        goto loop;
    }
    closedir(directory);
    return 1;
}

char *readInput(void) {
    size_t buffersize = BUFFER_SIZE;
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
        buffer[counter] = character;
    }
    counter++;

    if (counter >= buffersize) {
        buffersize += BUFFER_SIZE;
        buffer = realloc(buffer, buffersize);
        if (!buffer) goto err;
    }
    goto loop;
}

char **formatParams(char *input) {
    size_t buffersize = BUFFER_SIZE;
    char *param;
    char **parameters = malloc(buffersize * sizeof(char *));
    unsigned int position = 0;
    if (!parameters) {
err:    fprintf(stderr, "Erro ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }
    
    param = strtok(input, DELIMITERS);
    while (param != NULL) {
        parameters[position] = param;
        param = strtok(NULL, DELIMITERS);
        position++;
    }
    parameters[position] = NULL;
    return parameters;
}

int execute_command(char **args) {
    char *command = args[0];
    if (strcmp(command, "cd") == 0) {
        return change_directory(args);
    } else if (strcmp(command, "ls") == 0) {
        return list_directory(args);
    } else if (strcmp(command, "rm") == 0) {
        return remove_file(args);
    }
    else {
        fprintf(stderr, "Comando \"%s\" nao reconhecido\n", command);
    }
    return 1;
}

void shell_loop(void) {
    char *input;
    char **params;
    int return_status;
    do {
        printf(">>> ");
        input = readInput();
        params = formatParams(input);
        return_status = execute_command(params);

        free(input);
        free(params);
    } while (return_status);
}

int main(int argc, char **argv) {

    shell_loop();

    return 0;
}