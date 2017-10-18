// File: receiver.cpp

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
#include "recvWindow.h"
using namespace std;

#define LOCALHOST "127.0.0.9"

/* Delay to adjust speed of consuming buffer, in seconds */
#define DELAY 100
/* Define receive buffer size */
#define RXQSIZE 8
/* Define minimum upperlimit */
#define UPPERLIMIT 4
/* Define maximum lowerlimit */
#define LOWERLIMIT 2

Byte rxbuf[RXQSIZE];
QTYPE rcvq = { 0, 0, 0, RXQSIZE, rxbuf };
QTYPE *rxq = &rcvq;
bool send_xoff = false;

/* Arguments */
char* filename;
unsigned int windowsize;
unsigned int buffsize;
int port;

/* Socket */
struct sockaddr_in recvAddr, senderAddr;
int slen = sizeof(senderAddr);
int udpSocket;
int byteCounter = 0; //Number of bytes received
int byteConsumed = 0;

// Window
RecvWindow window;

/* Functions declaration */
static Byte rcvchar(int sockfd, QTYPE *queue);
static Byte q_get(QTYPE *);
void* consumeBuffer(void*);

/* Create socket and bind */
void setup(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Usage: ./recvfile <filename> <windowsize> <buffersize> <port>\n");
        exit(EXIT_FAILURE);
    }
    
    filename = argv[1];
    windowsize = atoi(argv[2]);
    buffsize = DEFAULT_BUFFSIZE;
    port = atoi(argv[4]);

    Byte* buff;
    bool* received;
    buff = (Byte *) malloc(sizeof(*buff) * buffsize);
    received = (bool *) malloc(sizeof(*received) * buffsize);
    window = {0, buffsize/2 - 1, windowsize, buff, received};

    // Creating UDP socket
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        error("Socket failed");
    }
      
    // Configure settings in address struct
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(port);
    recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(recvAddr.sin_zero, '\0', sizeof recvAddr.sin_zero);  
      
    // Bind socket with address struct
    if (bind(udpSocket, (struct sockaddr *) &recvAddr, sizeof(recvAddr)) < 0) {
        error("Bind failed");
    }
}

int main(int argc, char* argv[]) {
    setup(argc, argv);

	/* Create child thread, for consuming buffer */
	pthread_t child_thread;
	if(pthread_create(&child_thread, NULL, &consumeBuffer, NULL) < 0){
        error("Error when creating thread");
    }
    
    FILE *fp;
	fp = fopen(filename, "w");

	// Main thread for receiving message using
	// sliding window protocol
	Segment message;
    while(true){
	    if(recvfrom(udpSocket, &message, sizeof(Segment), 0, 
				(struct sockaddr *) &senderAddr, (socklen_t *) &slen) == -1) {
            error("Error recvfrom");
	    }

    	// Send response back
	    if(isFrameValid(message) && rcvq.count < RXQSIZE){
	    	int msgno = message.seqnum;
	    	Byte data = message.data;
	    	window.data[msgno] = data;

	    	sendACK(ACK, udpSocket, senderAddr, slen, msgno, message.checksum);
	    	if(data != Endfile){
                fprintf(fp, "%c", data);
		    	printf("Frame no: %d received (Byte received: %c)\n", msgno, data);
		    	window.received[msgno] = true;
	    	}
		    else
		    	break;
	    }
	    else if(rcvq.count >= RXQSIZE){
	    	cout<<"Fail: Buffer Full, not sending ACK"<<endl;
	    }
	    else{
	    	cout<<"Fail: Wrong checksum. Sending NAK"<<endl;
	    	sendACK(NAK, udpSocket, senderAddr, slen, message.seqnum, message.checksum);
	    }

	    for(int i = window.head; i != window.back; i = (i+1)%window.maxsize){
	    	if(i == window.head && window.received[i]){
	    		insertIntoProcessBuf(window.data[i], rxq, udpSocket, senderAddr, slen);
	    		decreaseWindow(&window);
	    		increaseWindow(&window);
	    	}
	    }
	}
	while(rxq->count!=0){
		//Do Nothing, waiting buffer being consumed
    }
    
    fclose(fp);

    return 0;
}

// q_get returns a pointer to the buffer where data is read 
// or NULL if buffer is empty.

static Byte q_get(QTYPE *queue) {
	Byte *current;
	/* Nothing in the queue */
	if (!queue->count) return '\0';
	
	/*
	Retrieve data from buffer, save it to "current" and "data"
	If the number of characters in the receive buffer is below certain
	level, then send XON.
	Increment head index and check for wraparound.
	*/
	Byte c = queue->data[queue->head++];
	queue->head %= queue->maxsize;
	queue->count--;

	if(c != Endfile){
		printf("Consumed byte %d: '%c'\n",++byteConsumed, c);

	// XON and XOFF not yet implemented

	// if(queue->count == LOWERLIMIT && send_xoff){
	 //    	cout<<"Buffer < maximum lowerlimit. Sending XON"<<endl;
	 //    	Byte xon;
	 //    	xon = (Byte) XON;
	 //    	if(sendto(sockfd, &xon, sizeof(Byte), 0, 
	 //    		(struct sockaddr *) &sender_addr, slen) == -1)
	 //    	{
	 //    		cout<<"Sending XON failed"<<endl;
	 //    	}
	 //    	send_xoff = false;
		// }
	}

	return c;
}

void* consumeBuffer(void*){
	//Consume Buffer with delay
	while(true){
		q_get(rxq);
		usleep((rand()%5+1) * DELAY * 1000); //usleep parameter is useconds
	}
}
