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

#define MAXTRANSBUFF 10
#define TIMEOUT 1000 // in ms

/* Arguments */
char* filename = "input.txt";
int windowsize = MAXTRANSBUFF;
int buffsize = 256;
char* buff;
char* destination_ip = "127.0.0.1";
int destination_port = 8080;

/* Socket */
struct sockaddr_in recvAddr;
int sock, slen = sizeof(recvAddr);
static Byte lastReceivedChar;
socklen_t addrSize;

Byte buf[MAXTRANSBUFF];
bool ack[MAXTRANSBUFF];
clock_t startTime[MAXTRANSBUFF];
SendWindow window = {0,0,0, MAXTRANSBUFF, buf, ack, startTime};




void* receiveResponse(void*);
void* receiveMessage(void*);

/* Create socket and connection */
void setup() {
    buff = (char *)malloc(sizeof(*buff) * buffsize);

    // Creating UDP socket
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        error("Socket failed");
    }
  
    // Configure settings in address struct
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(destination_port);
    recvAddr.sin_addr.s_addr = inet_addr(destination_ip);
    memset(recvAddr.sin_zero, '\0', sizeof(recvAddr.sin_zero)); 
      
    // Initialize size variable to be used later on
    addrSize = sizeof(recvAddr);
}

int main(int argc, char* argv[]) {
    /*if (argc < 6) {
        printf("Usage: ./sendfile <filename> <windowsize> <buffersize> "
        "<hostname> <destination_port>\n");
        exit(EXIT_FAILURE);
    }

    filename = argv[1];
    windowsize = atoi(argv[2]);
    buffsize = atoi(argv[3]);
    destination_ip = argv[4];
    destination_port = atoi(argv[5]);*/

    setup();

	// Create thread for receiving response
	pthread_t recvResponse_thread;
	if(pthread_create(&recvResponse_thread, NULL, &receiveResponse, NULL) < 0){
		cout<<"Error creating thread"<<endl;
		exit(EXIT_FAILURE);
	}

	//Main thread
	//Keep sending bytes until EOF while not XOFF
	FILE *fp;
	fp = fopen(filename, "r");

	if(fp == NULL){
		cout<<"Text File not found, terminating..."<<endl;
		exit(EXIT_FAILURE);
	}

	Byte ch;
	int msgno = 0;
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
		for(int i = window.front; i!=window.rear; i = (i+1) % window.maxsize){
			if(!window.ack[i]){
				double timeDif = (double)(clock() - window.startTime[i])/CLOCKS_PER_SEC * 1000;
				if(window.startTime[i]== -1 || timeDif>TIMEOUT){
					if(window.startTime[i] != -1)
						printf("Time out frame no %d\n",i);

					window.startTime[i] = clock();
					sendSegment(i, window.data[i], sock, recvAddr, slen);
					printf("Sent msgno-%d: '%c'\n", i, window.data[i]);
				}
			}
			else if(i == window.front && window.ack[i]){
				//Slide
				delHead(&window);
			}
		}

		if(endfile && window.front == window.rear)
			break; // All file sent
	}

	fclose(fp);
    
    return 0;
}

void* receiveResponse(void*){
	// Haven't implemented checksum for receiving response

	Ack response;
	while(true){
		if(recvfrom(sock, &response, sizeof(Ack), 0, 
				(struct sockaddr*)&recvAddr, (socklen_t*) &slen) == -1)
			{
				cout<<"Error receiving byte"<<endl;
				exit(EXIT_FAILURE);
			}

		if(response.ack == ACK){
			printf("ACK received for message no: %d\n", response.nextSeqnum);
			window.ack[response.nextSeqnum] = true;
		}
		else{
			printf("NAK received for message no: %d\n", response.nextSeqnum);
			
			// Resendings
			int msgno = response.nextSeqnum;
			int data = window.data[msgno];

			window.startTime[msgno] = clock();
			sendSegment(msgno, data, sock, recvAddr, slen);
			printf("Sent msgno-%d: '%c'\n", msgno, data);
		}
	}
}

void* receiveMessage(void*){
	//Keep reading data from receiver until
	//Connection is terminated

	Byte ch;
	while(true){
		if(recvfrom(sock, &ch, sizeof(Byte), 0, 
			(struct sockaddr*)&recvAddr, (socklen_t*) &slen) == -1)
		{
			cout<<"Error receiving byte"<<endl;
			exit(EXIT_FAILURE);
		}
		if(ch == XON)
			cout<<"XON received"<<endl;
		else if(ch == XOFF)
			cout<<"XOFF received"<<endl;

		lastReceivedChar = ch;
	}
}