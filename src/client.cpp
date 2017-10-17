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
#include <sstream>
using namespace std;

#define ARGUMENTS 6

/* Arguments */
char* filename = "input.txt";
int windowsize = 4;
int buffsize = 256;
char* buff;
char* destination_ip = "127.0.0.1";
int destination_port = 8080;

/* Socket */
int sock, nBytes;
struct sockaddr_in serverAddr;
socklen_t addrSize;

char* hello = "Hello from client";

/* Print error message */
void error(char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/* Create socket and connection */
void setup() {
    // Read file and save to buffer
    ifstream file(filename);
    stringstream sstr;
    while(file >> sstr.rdbuf());
    cout << sstr.str() << endl;

    buff = (char *)malloc(sizeof(*buff) * buffsize);

    // Creating UDP socket
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        error("Socket failed");
    }
  
    // Configure settings in address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(destination_port);
    serverAddr.sin_addr.s_addr = inet_addr(destination_ip);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero)); 
      
    // Initialize size variable to be used later on
    addrSize = sizeof(serverAddr);
}

void slidingWindow() {

}

int main(int argc, char* argv[]) {
    /*if (argc < ARGUMENTS) {
        printf("Usage: ./sendfile <filename> <windowsize> <buffersize> "
        "<destination_ip> <destination_port>\n");
        exit(EXIT_FAILURE);
    }

    filename = argv[1];
    windowsize = atoi(argv[2]);
    buffsize = atoi(argv[3]);
    destination_ip = argv[4];
    destination_port = atoi(argv[5]);*/

    setup();

    while(true) {
        printf("Type a sentence to send to server: ");
        fgets(buff, 1024, stdin);
    
        nBytes = strlen(buff) + 1;
        
        /*Send message to server*/
        sendto(sock, buff, nBytes, 0, (struct sockaddr*) &serverAddr, addrSize);
    
        /*Receive message from server*/
        nBytes = recvfrom(sock, buff, 1024, 0, NULL, NULL);
    
        printf("Received from server: %s\n",buff);
    
    }
    
    return 0;
}