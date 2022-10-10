#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h> //include after WinSock2.h

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFFER_LENGTH 512

enum NetworkNode {
	Offline, Server, Client
};

class NetworkManager {
public:
	NetworkManager();
	~NetworkManager();
	void Run();
	bool Initialize(NetworkNode networkMode_);
private:
	NetworkNode networkMode;

	WSADATA wsaData; //https://learn.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-wsadata
	int iResult; //check if the start up failed

	struct addrinfo* result, * ptr, hints;
	
	//server sockets
	SOCKET listenSocket;
	//connection sockets
	SOCKET connectSocket;

	char recvbuf[DEFAULT_BUFFER_LENGTH];
	int iSendResult;
};