all: 
	make bin/client bin/server

bin/client: src/client.cpp
	g++ -pthread -o bin/client src/client.cpp

bin/server: src/server.cpp
	g++ -pthread -o bin/server src/server.cpp
