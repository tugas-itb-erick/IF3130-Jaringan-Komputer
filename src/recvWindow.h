// File: recvWindow.h

#ifndef _RECV_WINDOW_H_
#define _RECV_WINDOW_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <bitset>
#include <cstring>
#include "header.h"

#define DEFAULT_BUFFSIZE 256 

using namespace std;

typedef struct RecvWindow {
	unsigned int head;
	unsigned int back;
	unsigned int maxsize;

	Byte *data;
	bool *received;
} RecvWindow;

void sendACK(Byte ack, int sock, struct sockaddr_in senderAddr, int slen, unsigned int seqnum, int checksum);

void increaseWindow(RecvWindow* window);
void decreaseWindow(RecvWindow* window);

string createCRC(string bitStr);
string getBitString(Segment msg);
bool isChecksumCorrect(Segment msg);
bool isFrameValid(Segment msg);

void insertIntoProcessBuf(Byte data, QTYPE *queue, int sock, struct sockaddr_in senderAddr, int slen);

#endif