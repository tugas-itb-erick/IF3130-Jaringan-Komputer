// File: sendWindow.h

#ifndef _SEND_WINDOW_H_
#define _SEND_WINDOW_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <string.h>
#include <cstring>
#include <bitset>
#include <time.h>
#include "header.h"

using namespace std;

#define DEFAULT_BUFFSIZE 256

typedef struct SendWindow {
	unsigned int count;
	unsigned int head;
	unsigned int back;
	unsigned int maxsize;

	Byte *data;
	bool *ack;

	clock_t *startTime;
} SendWindow;

//Send message
void sendSegment(Byte seqnum, Byte data, int sock, struct sockaddr_in receiverAddr, int slen);

//Add data to back of window
void putBack(Byte data, SendWindow* window);

//Remove head data from window
void delHead(SendWindow* window);

//Get string of CRC from bitstring
string createCRC(string bitStr);

//Generate BitString from message
string getBitString(Segment msg);

//Generate checksum from message to be sent
Byte getChecksum(Segment msg);

#endif
