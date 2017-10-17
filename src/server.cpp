#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
using namespace std;

#define ARGUMENTS 5
#define LOCALHOST "127.0.0.1"

/* Arguments */
char* filename;
int windowsize;
char* buff;
int buffsize = 256;
int port = 8080;

/* Socket */
struct sockaddr_in serverAddr, clientAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size, client_addr_size;
int udpSocket;
int nBytes;

char* hello = "Reply from server";

/* Print error message */
void error(char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/* Create socket and bind */
void setup() {
    buff = (char *)malloc(sizeof(*buff) * buffsize);

    // Creating UDP socket
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        error("Socket failed");
    }
      
    // Configure settings in address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(LOCALHOST);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
      
    // Bind socket with address struct
    if (bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        error("Bind failed");
    }

    addr_size = sizeof(serverStorage);
}

void slidingWindow() {
    
}

int main(int argc, char* argv[]) {
    /*if (argc < ARGUMENTS) {
        printf("Usage: ./recvfile <filename> <windowsize> <buffersize> <port>\n");
        exit(EXIT_FAILURE);
    }
    
    filename = argv[1];
    windowsize = atoi(argv[2]);
    buffsize = atoi(argv[3]);
    port = atoi(argv[4]);
    */

    setup();

    while(true) {
        /* Try to receive any incoming UDP datagram. Address and port of 
          requesting client will be stored on serverStorage variable */
        nBytes = recvfrom(udpSocket, buff, buffsize, 0, (struct sockaddr *) &serverStorage, &addr_size);
    
        /*Convert message received to uppercase*/
        for(int i=0;i<nBytes-1;i++){
            buff[i] = toupper(buff[i]);
        }
    
        /*Send uppercase message back to client, using serverStorage as the address*/
        sendto(udpSocket, buff, nBytes, 0, (struct sockaddr *) &serverStorage, addr_size);
    }

    return 0;
}