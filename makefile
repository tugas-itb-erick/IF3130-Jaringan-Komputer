all: 
	make bin/sendfile bin/recvfile

bin/sendfile: src/sender.cpp src/sendWindow.cpp src/header.cpp src/sendWindow.h src/header.h
	g++ -pthread -o bin/sendfile src/sender.cpp src/sendWindow.cpp src/header.cpp -std=c++11

bin/recvfile: src/receiver.cpp src/recvWindow.cpp src/header.cpp src/recvWindow.h src/header.h
	g++ -pthread -o bin/recvfile src/receiver.cpp src/recvWindow.cpp src/header.cpp -std=c++11
