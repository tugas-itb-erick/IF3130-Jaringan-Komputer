all: 
	make sendfile recvfile

sendfile: src/sender.cpp src/sendWindow.cpp src/header.cpp src/sendWindow.h src/header.h
	g++ -pthread -o sendfile src/sender.cpp src/sendWindow.cpp src/header.cpp -std=c++11

recvfile: src/receiver.cpp src/recvWindow.cpp src/header.cpp src/recvWindow.h src/header.h
	g++ -pthread -o recvfile src/receiver.cpp src/recvWindow.cpp src/header.cpp -std=c++11
