// ClientServidor.cpp : Defines the entry point for the console application.
//
#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>
#include<iostream>
#include <thread>

#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

void client(const char* serverIp, const char* enviar)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,*ptr = NULL,hints;
	
	int iResult;	

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(serverIp, DEFAULT_PORT, &hints, &result);
	
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);		

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		
		break;
	}

	freeaddrinfo(result);

	// Send an initial buffer
	iResult = send(ConnectSocket, enviar, (int)strlen(enviar), 0);
	
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);	

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
	
	
}

void server () {
	
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);	

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);	

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);	
	freeaddrinfo(result);
	iResult = listen(ListenSocket, SOMAXCONN);
	
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	
	// No longer need server socket
	closesocket(ListenSocket);

	// Receive
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);		

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	
	for (int i = 0; i < DEFAULT_BUFLEN; i++) {
		printf("%c", recvbuf[i]);
	}
	printf("\n");

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();    
}
/*
int main() {

	std::string temp = "HOla";
	const char* toSend = temp.c_str();
	std::thread primer (client, "127.0.0.1", toSend);
	std::thread tambeprimer(server);
	
	primer.join();
	tambeprimer.join();

	return 0;
}*/