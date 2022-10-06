#include "NetworkManager.h"
#include "EngineManager.h"

NetworkManager::NetworkManager() {
	networkMode = Offline;

	wsaData = WSADATA();
	iResult = true;
	iSendResult = true;

	result = nullptr;
	ptr = nullptr;
	hints = addrinfo();
	//server sockets
	listenSocket = INVALID_SOCKET;
	clientSocket = INVALID_SOCKET;
	//client sockets
	connectSocket = INVALID_SOCKET;

	for (int x = 0; x < DEFAULT_BUFFER_LENGTH; x++) {
		recvbuf[x] = 0;
	}
}

NetworkManager::~NetworkManager() {
	std::cout << "DESTORYTING\n";
}

void NetworkManager::Run() {
	while (EngineManager::Instance()->GetIsRunning() == true) {
		if (networkMode == Offline) { //we are not using networking - do nothing
			return;
		}
		std::cout << EngineManager::Instance()->GetTimer()->GetCurrentTicks() << std::endl;
	}
}

bool NetworkManager::Initialize(NetworkNode networkMode_) {
	networkMode = networkMode_;
	if (networkMode == Offline) { //we are not using networking - do nothing
		return true;
	}

	//initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2)/*make sure we use version 2.2*/, &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed: " << iResult << std::endl;
		system("pause");
		return false;
	}

	if (networkMode == Server) { //set up the instance as a server
		//set up hints
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM; //TCP works like a stream
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		//Resolve the server address and port
		iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
			WSACleanup();
			system("pause");
			return false;
		}

		//Create a socket for connecting to the server
		listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listenSocket == INVALID_SOCKET) {
			std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
			freeaddrinfo(result);
			WSACleanup();
			system("pause");
			return false;
		}

		//Setup the TCP listening socket
		iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			std::cout << "Bind failed with error: " << WSAGetLastError() << std::endl;
			freeaddrinfo(result);
			closesocket(listenSocket);
			WSACleanup();
			system("pause");
			return false;
		}
		freeaddrinfo(result);
	}

	return true;
}
