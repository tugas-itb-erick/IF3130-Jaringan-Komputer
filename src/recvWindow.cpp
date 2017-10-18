// File: recvWindow.cpp

#include "recvWindow.h"
using namespace std;

void sendACK(Byte ack, int sock, struct sockaddr_in senderAddr, int slen, unsigned int seqnum, int checksum) {
	Ack response;
    response.ack = ack;
    response.nextSeqnum = seqnum;
    response.checksum = checksum;

	if(sendto(sock, &response, sizeof(Ack), 0, (struct sockaddr *) &senderAddr, slen) < 0) {	
        cout << "Error sendto ACK" << endl;
    }
}

void increaseWindow(RecvWindow* window){
    window->received[window->back] = false;
    window->back = (window->back + 1) % window->maxsize;
}

void decreaseWindow(RecvWindow* window){
    window->head = (window->head + 1) % window->maxsize;
}

string createCRC(string bitStr) {
    static char result[8];
    char crc[7];
    int  i;
    char invert;
   
    for (i=0; i<7; ++i){
        crc[i] = 0; 
    }                   
   
    for (i=0; i<bitStr.length(); i++) {
        invert = ('1' == bitStr[i]) ^ crc[6];
        crc[6] = crc[5] ^ invert;
        crc[5] = crc[4];
        crc[4] = crc[3] ^ invert;
        crc[3] = crc[2];
        crc[2] = crc[1] ^ invert;
        crc[1] = crc[0];
        crc[0] = invert;
    }
      
    for (i=0; i<7; ++i){
        result[6-i] = crc[i] ? '1' : '0';
    }
    result[7] = 0;

    return result;
}

//Generate BitString from message (not include checksum)
string getBitString(Segment msg){
    string bitStr = "";
    bitStr += bitset<8>(msg.soh).to_string();
    bitStr += bitset<8>(msg.seqnum).to_string();
    bitStr += bitset<8>(msg.stx).to_string();
    bitStr += bitset<8>(msg.data).to_string();
    bitStr += bitset<8>(msg.etx).to_string();
    bitStr += bitset<7>(msg.checksum).to_string();

    return bitStr;
}

bool isChecksumValid(Segment msg){
    string bitStr = getBitString(msg);
    string bitCheckSum = createCRC(bitStr);

    return (bitset<7>(bitCheckSum).to_ulong()) == 0;
}

bool isFrameValid(Segment msg){
    return (msg.soh == SOH && msg.seqnum < DEFAULT_BUFFSIZE && msg.stx == STX && 
        msg.etx == ETX && isChecksumValid(msg));
}

// Insert data from window into process Buffer
void insertIntoProcessBuf(Byte data, QTYPE *queue, int sock, struct sockaddr_in senderAddr, int slen){
    queue->data[queue->back++] = data;
    queue->back %= queue->maxsize;
    queue->count++;
}