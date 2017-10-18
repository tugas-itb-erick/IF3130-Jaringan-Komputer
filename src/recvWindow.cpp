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
    return (msg.soh == SOH && msg.seqnum < MAXRECVBUFF && msg.stx == STX && 
        msg.etx == ETX && isChecksumValid(msg));
}

void increaseWindow(RecvWindow* window){
    window->received[window->rear] = false;
    window->rear = (window->rear + 1) % window->maxsize;
}

void decreaseWindow(RecvWindow* window){
    window->front = (window->front + 1) % window->maxsize;
}

// Insert data from window into process Buffer
void insertIntoProcessBuf(Byte data, QTYPE *queue, int sockfd, struct sockaddr_in sender_addr, int slen){
    queue->data[queue->rear++] = data;
    queue->rear %= queue->maxsize;
    queue->count++;

    // Sending XON and XOFF not implemented yet

    // if(queue->count > UPPERLIMIT && !send_xoff){
    //  cout<<"Buffer > minimum upperlimit. Sending XOFF"<<endl;
    //  Byte xoff;
    //  xoff = (Byte) XOFF;
    //  if(sendto(sockfd, &xoff, sizeof(Byte), 0, 
    //      (struct sockaddr *) &sender_addr, slen) == -1)
    //  {
    //      cout<<"Sending XOFF failed"<<endl;
    //  }
    //  send_xoff = true;
    // }
}