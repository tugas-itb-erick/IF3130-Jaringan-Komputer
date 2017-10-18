// File: sendWindow.cpp

#include "sendWindow.h"

using namespace std;

stringstream logsend;

void sendSegment(Byte segnum, Byte data, int sock, struct sockaddr_in receiverAddr, int slen) {
    Segment message;
    message.soh = SOH;
    message.seqnum = segnum;
    message.stx = STX;
    message.data = data;
    message.etx = ETX;
    message.checksum = getChecksum(message);

    if (sendto(sock, &message, sizeof(Segment), 0, (struct sockaddr*)&receiverAddr, slen) < 0) {
        cout << "Error sendto Segment" << endl;
    }
}

void putBack(Byte data, SendWindow* window) {
    unsigned int back = window->back;
    window->data[back] = data;
    window->ack[back] = false;
    window->startTime[back] = -1;
    window->back = (back + 1) % window->maxsize;
    window->count++;
}

void delHead(SendWindow* window) {
    window->head = (window->head + 1) % window->maxsize;
    window->count--;
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
      
    for (i=0; i<7; ++i) {
        result[6-i] = crc[i] ? '1' : '0';
    }
    result[7] = 0;

    return result;
}

//Generate BitString from message (not included checksum)
string getBitString(Segment msg) {
    string bitStr = "";
    bitStr += bitset<8>(msg.soh).to_string();
    bitStr += bitset<8>(msg.seqnum).to_string();
    bitStr += bitset<8>(msg.stx).to_string();
    bitStr += bitset<8>(msg.data).to_string();
    bitStr += bitset<8>(msg.etx).to_string();
    return bitStr;
}

Byte getChecksum(Segment msg) {
    string bitStr = getBitString(msg);
    string bitChecksum = createCRC(bitStr);
    Byte checksum = (Byte) (bitset<8>(bitChecksum).to_ulong());
    return checksum;
}
