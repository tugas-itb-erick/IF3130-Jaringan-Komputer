// File : header.cpp

#include <cstdio>
#include <cstdlib>
#include "header.h"

void error(std::string message) {
    perror(message.c_str());
    exit(EXIT_FAILURE);
}