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
	if (networkMode == Offline) { //we are not using networking - do nothing
		return;
	}
	else if (networkMode == Server) {
		//shutdown the connection on our end
		iResult = shutdown(clientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			std::cout << "Shutdown failed with error: " << WSAGetLastError() << std::endl;
			closesocket(listenSocket);
			WSACleanup();
			system("pause");
			return;
		}

		std::cout << "Connection closed" << std::endl;

		closesocket(clientSocket);
	}
	else if (networkMode == Client) {
		//shutdown the connection
		iResult = shutdown(connectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			std::cout << "Shutdown failed with error: " << WSAGetLastError() << std::endl;
			closesocket(connectSocket);
			WSACleanup();
			system("pause");
			return;
		}

		std::cout << "Connection closed" << std::endl;

		closesocket(connectSocket);
	}
	
	WSACleanup();
}

void NetworkManager::Run() {
	while (EngineManager::Instance()->GetIsRunning() == true) {
		if (networkMode == Offline) { //we are not using networking - do nothing
			return;
		}
		else if(networkMode == Server) {
			//listen
			iResult = listen(listenSocket, SOMAXCONN); //SOMAXCONN allows maximum number of connections
			if (iResult == SOCKET_ERROR) {
				std::cout << "Listen Socket failed with error: " << WSAGetLastError() << std::endl;
				closesocket(listenSocket);
				WSACleanup();
				system("pause");
				return;
			}

			std::cout << "Waiting for connection request..." << std::endl;

			//Accept a client socket
			clientSocket = accept(listenSocket, nullptr, nullptr);
			if (clientSocket == INVALID_SOCKET) {
				std::cout << "Accept failed with error: " << WSAGetLastError() << std::endl;
				closesocket(listenSocket);
				WSACleanup();
				system("pause");
				return;
			}

			std::cout << "Connected to client" << std::endl;

			closesocket(listenSocket);

			//receive until the peer shutdown the connect
			for (int x = 0; x < DEFAULT_BUFFER_LENGTH; x++) {
				recvbuf[x] = 0;
			}
			iResult = recv(clientSocket, recvbuf, DEFAULT_BUFFER_LENGTH, 0);
			if (iResult > 0) {
				std::cout << "Received string: " << recvbuf << std::endl;
			}
			else if (iResult == 0) {
				std::cout << "Connection closing..." << std::endl;
				break;
			}
			else {
				std::cout << "Receive failed with error: " << WSAGetLastError() << std::endl;
				closesocket(listenSocket);
				WSACleanup();
				system("pause");
				return;
			}
		}
		else if (networkMode == Client) {
			//Receive until the peer closes connection
			std::string message;
			message.clear();
			std::cout << "Input a sting to send (!exit to disconnect): ";
			std::cin >> message;
			int messageSize = strlen(message.c_str());
			//Send an initial buffer
			iResult = send(connectSocket, message.c_str(), messageSize, 0);
			if (iResult == SOCKET_ERROR) {
				std::cout << "Send failed with error: " << iResult << std::endl;
				closesocket(connectSocket);
				WSACleanup();
				system("pause");
				return;
			}
			std::cout << "Bytes sent: " << iResult << std::endl;
		}
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

	else if (networkMode == Server) { //set up the instance as a server
		//hints define the connection type and the address that will be used
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
	else if (networkMode == Client) {
		//hints define the connection type and the address that will be used
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC; //don't specificity the address family (IVP6 / IVP 4)
		hints.ai_socktype = SOCK_STREAM; //TCP works like a stream
		hints.ai_protocol = IPPROTO_TCP;

		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) { //move addresses till one succeeds
			//Create a socket for connecting to the server
			connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol); //find a open socket
			if (connectSocket == INVALID_SOCKET) {
				std::cout << "Socket failed with error: " << iResult << std::endl;
				WSACleanup();
				system("pause");
				return 1;
			}

			//Connect to the server
			iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(connectSocket);
				connectSocket = INVALID_SOCKET;
				continue;
			}
			std::cout << "Connected to server" << std::endl;
			break;
		}

		freeaddrinfo(result);
		if (connectSocket == INVALID_SOCKET) {
			std::cout << "Unable to connect to the server: " << iResult << std::endl;
			WSACleanup();
			system("pause");
			return 1;
		}
	}
	return true;
}
