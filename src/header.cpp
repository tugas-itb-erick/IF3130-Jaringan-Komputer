#include <cstdio>
#include <cstdlib>
#include "header.h"

void error(char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}