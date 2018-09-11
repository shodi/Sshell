#include <stdlib.h>

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 1024
#endif
#ifndef DELIMITERS
    #define DELIMITERS " \n\t\r"
#endif

#ifdef DT_DIR
    #undef DT_DIR
#endif
#ifdef DT_REG
    #undef DT_REG
#endif

#define DT_DIR 4
#define DT_REG 8