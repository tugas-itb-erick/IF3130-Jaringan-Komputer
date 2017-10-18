// File : header.h


#ifndef _HEADER_H_
#define _HEADER_H_

#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>

/* ASCII Constants */
#define SOH 1
#define STX 2
#define ETX 3
#define ENQ 5
#define ACK 6
#define NAK 21
#define Endfile 26

/* XON/XOFF protocol */
#define XON (0x11)
#define XOFF (0x13)

void error(std::string msg);

typedef unsigned char Byte;

typedef struct QTYPE
{
	unsigned int count;
	unsigned int head;
	unsigned int back;
	unsigned int maxsize;
	Byte *data;
} QTYPE;

typedef struct Segment {
	Byte soh;
	unsigned int seqnum;
	Byte stx;
	Byte data;
	Byte etx;
	Byte checksum;
} Segment;

typedef struct Ack {
	Byte ack;
	unsigned int nextSeqnum;
	Byte advWinsize;
	Byte checksum;
} Ack;

#endif