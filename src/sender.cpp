// File: sender.cpp

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
#include "sendWindow.h"
using namespace std;

#define TIMEOUT 1000 // in ms

/* Arguments */
char* filename;
unsigned int windowsize;
unsigned int buffsize;
char* hostname;
int destination_port;

/* Socket */
struct sockaddr_in recvAddr;
int sock, slen = sizeof(recvAddr);
static Byte lastReceivedChar;

/* Window */
SendWindow window;

/* Function Declarations */
void* receiveResponse(void*);
void* receiveMessage(void*);

/* Create socket and configuration */
void setup(int argc, char* argv[]) {
	if (argc < 6) {
        printf("Usage: ./sendfile <filename> <windowsize> <buffersize> "
        "<hostname> <destination_port>\n");
        exit(EXIT_FAILURE);
    }
    filename = argv[1];
    windowsize = atoi(argv[2]);
    buffsize = DEFAULT_BUFFSIZE;
    hostname = argv[4];
	destination_port = atoi(argv[5]);
	
	Byte* buff;
	bool* ack;
	clock_t* startTime;
	buff = (Byte *) malloc(sizeof(*buff) * buffsize);
	ack = (bool *) malloc(sizeof(*ack) * buffsize);
	startTime = (clock_t *) malloc(sizeof(*startTime) * buffsize);
	window = {0,0,0, windowsize, buff, ack, startTime};

    // Creating UDP socket
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        error("Socket failed");
    }
  
    // Configure settings in address struct
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(destination_port);
	memset(recvAddr.sin_zero, '\0', sizeof(recvAddr.sin_zero));

	struct hostent* pHostInfo;
	long nHostAddress;
	pHostInfo = gethostbyname(hostname);
	if(!pHostInfo){
		error("Could not resolve host name");
	}
	memcpy(&recvAddr.sin_addr, pHostInfo->h_addr_list[0], pHostInfo->h_length);
}

int main(int argc, char* argv[]) {
    setup(argc, argv);

	// Create thread for receiving response
	pthread_t recvResponse_thread;
	if(pthread_create(&recvResponse_thread, NULL, &receiveResponse, NULL) < 0) {
		error("Error when creating thread");
	}

	FILE *fp;
	fp = fopen(filename, "r");

	if(fp == NULL){
		error("File not found, stopping...");
	}

	Byte ch;
	bool endfile = false;
	while(true){
		// Read message and put to window
		while(window.count <= window.maxsize / 2 && !endfile){
			if(lastReceivedChar != XOFF){
				if(fscanf(fp, "%c", &ch) == EOF){
					ch = Endfile; //Endfile sequence
					endfile = true;
				}
				putBack(ch, &window);
			}
		}

		// Iterate through window and send frame which have not been ACK
		for(int i = window.head; i!=window.back; i = (i+1) % window.maxsize){
			if(!window.ack[i]){
				double timeDif = (double)(clock() - window.startTime[i])/CLOCKS_PER_SEC * 1000;
				if(window.startTime[i]== -1 || timeDif>TIMEOUT){
					if(window.startTime[i] != -1)
						printf("Time out frame no %d\n",i);

					window.startTime[i] = clock();
					sendSegment(i, window.data[i], sock, recvAddr, slen);
					printf("Sent msgno-%d: '%c'\n", i, window.data[i]);
				}
			} else if(i == window.head && window.ack[i]){ // Slide Forward
				delHead(&window);
			}
		}

		if (endfile && window.head == window.back) {
			break;
		}
	}

	fclose(fp);
    
    return 0;
}

void* receiveResponse(void*){
	Ack response;
	while(true){
		if(recvfrom(sock, &response, sizeof(Ack), 0, 
				(struct sockaddr*)&recvAddr, (socklen_t*) &slen) == -1)
		{
			error("Error revfrom");
		}

		if(response.ack == ACK) {
			printf("ACK received for message no: %d\n", response.nextSeqnum);
			window.ack[response.nextSeqnum] = true;
		} else {
			printf("NAK received for message no: %d\n", response.nextSeqnum);
			
			// Resendings
			int seqnum = response.nextSeqnum;
			int data = window.data[seqnum];

			window.startTime[seqnum] = clock();
			sendSegment(seqnum, data, sock, recvAddr, slen);
			printf("Sent msgno-%d: '%c'\n", seqnum, data);
		}
	}
}

void* receiveMessage(void*){
	//Keep reading data from receiver until
	//Connection is terminated

	Byte ch;
	while(true) {
		if (recvfrom(sock, &ch, sizeof(Byte), 0, (struct sockaddr*)&recvAddr, (socklen_t*) &slen) == -1) {
			error("Error revfrom");
		}
		if (ch == XON)
			cout << "XON received" << endl;
		else if(ch == XOFF)
			cout << "XOFF received" << endl;

		lastReceivedChar = ch;
	}
}