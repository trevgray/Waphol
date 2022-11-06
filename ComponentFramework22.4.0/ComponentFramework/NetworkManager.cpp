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
	//client sockets
	connectSocket = INVALID_SOCKET;

	sendbuf = NULL;
}

NetworkManager::~NetworkManager() {
	Debug::Info("Shutting Down the NetworkManager", __FILE__, __LINE__);
	if (networkMode == Offline) { //we are not using networking - do nothing
		return;
	}
	else {
		//shutdown the connection on our end
		iResult = shutdown(connectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			std::cout << "Shutdown failed with error: " << WSAGetLastError() << std::endl;
			closesocket(listenSocket);
			WSACleanup();
			//system("pause");
			return;
		}

		std::cout << "Connection closed" << std::endl;

		closesocket(connectSocket);
		WSACleanup();

		std::cout << "BRUH" << std::endl;
	}
}

void NetworkManager::Run() {
	while (EngineManager::Instance()->GetIsRunning() == true) {
		if (networkMode == Server) {
			if (connectSocket == INVALID_SOCKET) { //when the listen socket is null
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
				connectSocket = accept(listenSocket, nullptr, nullptr);
				if (connectSocket == INVALID_SOCKET) {
					std::cout << "Accept failed with error: " << WSAGetLastError() << std::endl;
					closesocket(listenSocket);
					WSACleanup();
					system("pause");
					return;
				}

				std::cout << "Connected to client" << std::endl;

				AddClientActor();

				closesocket(listenSocket);
			}

			iResult = recv(connectSocket, (char*)&actorBuffer, sizeof(ActorBuffer), 0);
			if (iResult > 0) {

				std::unique_lock<std::mutex> lock(transformUpdateMutex);
				printf("%f %f %f\n", actorBuffer.position.x, actorBuffer.position.y, actorBuffer.position.z);
				EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->SetPosition(actorBuffer.position);
				lock.unlock();

				actorBuffer.position = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition();
				
				sendbuf = (char*)&actorBuffer; //binary representation 

				iResult = send(connectSocket, sendbuf, sizeof(ActorBuffer), 0);
				if (iResult == SOCKET_ERROR) {
					std::cout << "Send failed with error: " << iResult << std::endl;
					closesocket(connectSocket);
					WSACleanup();
					system("pause");
					return;
				}
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
			GetServerActorName();

			//Receive until the peer closes connection
			actorBuffer.position = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition();

			//Send an initial buffer
			sendbuf = (char*)&actorBuffer; //binary representation 

			iResult = send(connectSocket, sendbuf, sizeof(ActorBuffer), 0);
			if (iResult == SOCKET_ERROR) {
				std::cout << "Send failed with error: " << iResult << std::endl;
				closesocket(connectSocket);
				WSACleanup();
				system("pause");
				return;
			}

			iResult = recv(connectSocket, (char*)&actorBuffer, sizeof(ActorBuffer), 0);
			if (iResult > 0) {

				std::unique_lock<std::mutex> lock(transformUpdateMutex);
				printf("%f %f %f\n", actorBuffer.position.x, actorBuffer.position.y, actorBuffer.position.z);
				EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->SetPosition(actorBuffer.position);
				lock.unlock();
			}
			else {
				std::cout << "Receive failed with error: " << WSAGetLastError() << std::endl;
				closesocket(listenSocket);
				WSACleanup();
				system("pause");
				return;
			}
			//std::cout << "Bytes sent: " << iResult << std::endl;
		}
		else if (networkMode == Offline) { //we are not using networking - do nothing
			return;
		}
	}
}

bool NetworkManager::Initialize(NetworkNode networkMode_) {
	networkMode = networkMode_;
	if (networkMode == Offline) { //we are not using networking - do nothing
		return true;
	}

	clientActorTemplateName = "NPC";

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

		//Resolve the server address and port
		iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result); //localhost connections be to the own computer - this is where you enter the ip
		{ //result gets full of how to connection to the computer
			if (iResult != 0) {
				std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
				WSACleanup();
				system("pause");
				return 1;
			}
		}

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

void NetworkManager::GetServerActorName() {
	char actorNameBuffer[DEFAULT_BUFFER_LENGTH];

	ZeroMemory(actorNameBuffer, DEFAULT_BUFFER_LENGTH);

	iResult = recv(connectSocket, actorNameBuffer, DEFAULT_BUFFER_LENGTH, 0);
	if (iResult > 0) {
		actorName = actorNameBuffer;
		std::cout << actorName << std::endl;
	}
	else {
		std::cout << "Receive failed with error: " << WSAGetLastError() << std::endl;
		closesocket(connectSocket);
		WSACleanup();
		system("pause");
		return;
	}
}

void NetworkManager::AddClientActor() {
	std::string clientName = "Client" + std::to_string(clientActors.size());
	EngineManager::Instance()->GetActorManager()->AddActor<Actor>(clientName, new Actor(nullptr));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName)->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>(clientActorTemplateName.c_str()));
	//maybe make a spawn point actor? Just a though
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName)->AddComponent<TransformComponent>(nullptr, Vec3(), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3());
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName)->OnCreate();

	clientActors[clientName] = EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName);

	//ZeroMemory(sendbuf, sizeof(ActorBuffer));

	sendbuf = (char*)clientName.c_str();

	iResult = send(connectSocket, sendbuf, clientName.size(), 0);
	if (iResult == SOCKET_ERROR) {
		std::cout << "Send failed with error: " << iResult << std::endl;
		closesocket(connectSocket);
		WSACleanup();
		system("pause");
		return;
	}
}