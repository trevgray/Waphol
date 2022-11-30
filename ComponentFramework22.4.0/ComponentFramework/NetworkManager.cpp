#include "NetworkManager.h"
#include "EngineManager.h"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

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

	actorBuffer = ActorBuffer();
}

NetworkManager::~NetworkManager() {
	Debug::Info("Shutting Down the NetworkManager", __FILE__, __LINE__);
	if (networkMode == Offline) { //we are not using networking - do nothing
		return;
	}
	else {
		//shutdown the connection on our end
		iResult = shutdown(listenSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			std::cout << "Shutdown failed with error: " << WSAGetLastError() << std::endl;
		}

		std::cout << "Connection closed" << std::endl;

		closesocket(listenSocket);
		closesocket(connectSocket);
		WSACleanup();
	}
}

void NetworkManager::Run() {
	while (EngineManager::Instance()->GetIsRunning() == true) {
		if (networkMode == Server) {
			//listen
			iResult = listen(listenSocket, SOMAXCONN); //SOMAXCONN allows maximum number of connections
			if (iResult == SOCKET_ERROR) {
				std::cout << "Listen Socket failed with error: " << WSAGetLastError() << std::endl;
				this->~NetworkManager();
				return;
			}
			std::cout << "Waiting for connection request..." << std::endl;

			while (connectSocket = accept(listenSocket, nullptr, nullptr)) {
				//Accept a client socket
				if (connectSocket == INVALID_SOCKET) {
					std::cout << "Accept failed with error: " << WSAGetLastError() << std::endl;
					this->~NetworkManager();
					return;
				}

				std::cout << "Connected to client" << std::endl;

				AddClientActor();

				//create a thread and start it
				std::thread clientThread(&NetworkManager::AddClientSession, this, (void*)connectSocket);
				clientThread.detach();
			}

		}
		else if (networkMode == Client) {

			if (connectSocket == INVALID_SOCKET) {
				for (ptr = result; ptr != NULL; ptr = ptr->ai_next) { //move addresses till one succeeds
					//Create a socket for connecting to the server
					connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol); //find a open socket
					if (connectSocket == INVALID_SOCKET) {
						std::cout << "Socket failed with error: " << iResult << std::endl;
						this->~NetworkManager();
						return;
					}

					//Connect to the server
					iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
					if (iResult == SOCKET_ERROR) {
						closesocket(connectSocket);
						connectSocket = INVALID_SOCKET;
						continue;
					}
					std::cout << "Connected to server" << std::endl;

					GetServerActorName();

					//create a thread and start it
					std::thread bufferThread(&NetworkManager::ReceiveServerBuffers, this);
					bufferThread.detach();

					break;
				}

				freeaddrinfo(result);
				if (connectSocket == INVALID_SOCKET) {
					std::cout << "Unable to connect to the server: " << iResult << std::endl;
					this->~NetworkManager();
					return;
				}
			}

			actorBuffer.position = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition();
			actorBuffer.orientation = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetQuaternion();
			actorBuffer.ID = actorID;

			sendbuf = (char*)&actorBuffer; //binary representation 

			iResult = send(connectSocket, sendbuf, sizeof(ActorBuffer), 0);
			if (iResult == SOCKET_ERROR) {
				std::cout << "Send failed with error: " << iResult << std::endl;
				this->~NetworkManager();
				return;
			}
			std::this_thread::sleep_for(1ms);
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
			this->~NetworkManager();
			return false;
		}

		//Create a socket for connecting to the server
		listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listenSocket == INVALID_SOCKET) {
			std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
			freeaddrinfo(result);
			this->~NetworkManager();
			return false;
		}

		//Setup the TCP listening socket
		iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			std::cout << "Bind failed with error: " << WSAGetLastError() << std::endl;
			freeaddrinfo(result);
			this->~NetworkManager();
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
				this->~NetworkManager();
				return false;
			}
		}
	}
	return true;
}

void NetworkManager::GetServerActorName() {
	char actorNameBuffer[DEFAULT_BUFFER_LENGTH];

	ZeroMemory(actorNameBuffer, DEFAULT_BUFFER_LENGTH);

	iResult = recv(connectSocket, actorNameBuffer, DEFAULT_BUFFER_LENGTH, 0);
	if (iResult > 0) {
		actorID = (unsigned int)actorNameBuffer[0] - 48;

		std::unique_lock<std::mutex> lock(transformUpdateMutex);
		EngineManager::Instance()->GetActorManager()->AddActor<Actor>("9", new Actor(nullptr));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("9")->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>(clientActorTemplateName.c_str()));
		//maybe make a spawn point actor? Just a though
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("9")->AddComponent<TransformComponent>(nullptr, Vec3(12.5f, 7.5f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(3.0f, 3.0f, 3.0f));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("9")->OnCreate();
		lock.unlock();
	}
	else {
		std::cout << "Receive failed with error: " << WSAGetLastError() << std::endl;
		this->~NetworkManager();
		return;
	}
}

void NetworkManager::ReceiveServerBuffers() {
	while (EngineManager::Instance()->GetIsRunning() == true) {
		ActorBuffer serverBuffer;
		iResult = recv(connectSocket, (char*)&serverBuffer, sizeof(ActorBuffer), 0);
		if (iResult > 0) {
			bool checkActor = true;
			for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
				if (std::to_string(serverBuffer.ID) == actor.first) {
					checkActor = false;
				}
			}
			if (checkActor == true) {
				std::unique_lock<std::mutex> lock(serverBufferMutex);
				EngineManager::Instance()->GetActorManager()->AddActor<Actor>(std::to_string(serverBuffer.ID), new Actor(nullptr));
				EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(serverBuffer.ID))->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>(clientActorTemplateName.c_str()));
				//maybe make a spawn point actor? Just a though
				EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(serverBuffer.ID))->AddComponent<TransformComponent>(nullptr, Vec3(12.5f, 7.5f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(3.0f, 3.0f, 3.0f));
				EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(serverBuffer.ID))->OnCreate();
				lock.unlock();
			}

			//SET THE ACTORS VARIABLES
			//std::unique_lock<std::mutex> lock(serverBufferMutex);
			std::cout << "MUTEX 3" << std::endl;
			printf("%f %f %f\n", serverBuffer.position.x, serverBuffer.position.y, serverBuffer.position.z);
			EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(serverBuffer.ID))->GetComponent<TransformComponent>()->SetPosition(serverBuffer.position);
			EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(serverBuffer.ID))->GetComponent<TransformComponent>()->setOrientation(serverBuffer.orientation);
			//lock.unlock();
		}
		else {
			std::cout << "Receive failed with error: " << WSAGetLastError() << std::endl;
			this->~NetworkManager();
			return;
		}
		//std::cout << "Bytes sent: " << iResult << std::endl;
	}
}

void NetworkManager::AddClientActor() {
	std::string clientName = std::to_string(clientActors.size());

	std::unique_lock<std::mutex> lock(transformUpdateMutex);
	EngineManager::Instance()->GetActorManager()->AddActor<Actor>(clientName, new Actor(nullptr));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName)->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>(clientActorTemplateName.c_str()));
	//maybe make a spawn point actor? Just a though
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName)->AddComponent<TransformComponent>(nullptr, Vec3(12.5f, 7.5f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(3.0f, 3.0f, 3.0f));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName)->OnCreate();
	lock.unlock();

	clientActors[clientName] = EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName);

	sendbuf = (char*)clientName.c_str();

	iResult = send(connectSocket, sendbuf, clientName.size(), 0);
	if (iResult == SOCKET_ERROR) {
		std::cout << "Send failed with error: " << iResult << std::endl;
		this->~NetworkManager();
		return;
	}
}

void NetworkManager::AddClientSession(void* data) {
	SOCKET clientSocket = (SOCKET)data;
	ActorBuffer clientActorBuffer;
	char* clientSendbuf;
	int iClientResult;

	int thisClientID;

	while (EngineManager::Instance()->GetIsRunning() == true) {
		iClientResult = recv(clientSocket, (char*)&clientActorBuffer, sizeof(ActorBuffer), 0);
		if (iClientResult > 0) {

			std::unique_lock<std::mutex> lock(transformUpdateMutex);
			printf("%f %f %f\n", clientActorBuffer.position.x, clientActorBuffer.position.y, clientActorBuffer.position.z);
			EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(clientActorBuffer.ID))->GetComponent<TransformComponent>()->SetPosition(clientActorBuffer.position);
			EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(clientActorBuffer.ID))->GetComponent<TransformComponent>()->setOrientation(clientActorBuffer.orientation);
			lock.unlock();

			thisClientID = clientActorBuffer.ID;

			//SEND # OF ACTORS
			/*int numOfActors = 1;
			iClientResult = send(clientSocket, (char*)&numOfActors, sizeof(int), 0);
			if (iClientResult == SOCKET_ERROR) {
				std::cout << "Send failed with error: " << iClientResult << std::endl;
				break;
			}*/

			//SERVER ACTOR
			clientActorBuffer.ID = 9;
			clientActorBuffer.position = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition();
			clientActorBuffer.orientation = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetQuaternion();

			clientSendbuf = (char*)&clientActorBuffer; //binary representation 

			iClientResult = send(clientSocket, clientSendbuf, sizeof(ActorBuffer), 0);
			if (iClientResult == SOCKET_ERROR) {
				std::cout << "Send failed with error: " << iClientResult << std::endl;
				break;
			}
			//CLIENT ACTORS
			for (auto actor : clientActors) {
				if (EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(thisClientID)) != actor.second) {
					clientActorBuffer.ID = std::stoi(actor.first);
					clientActorBuffer.position = EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(clientActorBuffer.ID))->GetComponent<TransformComponent>()->GetPosition();
					clientActorBuffer.orientation = EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(clientActorBuffer.ID))->GetComponent<TransformComponent>()->GetQuaternion();

					clientSendbuf = (char*)&clientActorBuffer; //binary representation 

					iClientResult = send(clientSocket, clientSendbuf, sizeof(ActorBuffer), 0);
					if (iClientResult == SOCKET_ERROR) {
						std::cout << "Send failed with error: " << iClientResult << std::endl;
						break;
					}
				}
			}
		}
		else {
			std::cout << "Connection closing..." << std::endl;
			EngineManager::Instance()->GetActorManager()->RemoveActor(std::to_string(thisClientID));
			clientActors.erase(std::to_string(thisClientID));
			break;
		}
	}

	//shutdown the connection on our end
	iClientResult = shutdown(clientSocket, SD_SEND);
	if (iClientResult == SOCKET_ERROR) {
		std::cout << "Shutdown failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
	}

	closesocket(clientSocket);
}