#include "NetworkManager.h"
#include "EngineManager.h"

NetworkManager::NetworkManager() {
	networkMode = Offline;

	wsaData = WSADATA();
	iResult = true;
	iSendResult = true;

	server = sockaddr_in();
	client = sockaddr_in();
	//server sockets
	in = INVALID_SOCKET;
	//client sockets
	out = INVALID_SOCKET;

	sendbuf = NULL;

	actorBuffer = ActorBuffer();
}

NetworkManager::~NetworkManager() {
	Debug::Info("Shutting Down the NetworkManager", __FILE__, __LINE__);
	if (networkMode == Offline) { //we are not using networking - do nothing
		return;
	}
	else {
		std::cout << "Connection closed" << std::endl;

		closesocket(in);
		closesocket(out);
		WSACleanup();
	}
}

void NetworkManager::Run() {
	while (EngineManager::Instance()->GetIsRunning() == true) {
		if (networkMode == Server) {
			int bytesIn = recvfrom(in, (char*)&actorBuffer, sizeof(ActorBuffer), 0, (sockaddr*)&client, &clientLength);
			if (bytesIn == SOCKET_ERROR) {
				std::cout << "Error receiving from client " << WSAGetLastError() << std::endl;
				continue;
			}
			if (actorBuffer.ID == -1) {
				AddClientActor();
				continue;
			}


			std::cout << actorBuffer.position.x << std::endl;

			if (bytesIn > 0) {

				std::unique_lock<std::mutex> lock(transformUpdateMutex);
				printf("%f %f %f\n", actorBuffer.position.x, actorBuffer.position.y, actorBuffer.position.z);
				EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(actorBuffer.ID))->GetComponent<TransformComponent>()->SetPosition(actorBuffer.position);
				lock.unlock();

				actorBuffer.position = EngineManager::Instance()->GetActorManager()->GetActor<Actor>(std::to_string(actorBuffer.ID))->GetComponent<TransformComponent>()->GetPosition();
				//actorBuffer.name = actorName.c_str();
				
				sendbuf = (char*)&actorBuffer; //binary representation 
			}
			else {
				std::cout << "Receive failed with error: " << WSAGetLastError() << std::endl;
				this->~NetworkManager();
				return;
			}
		}
		else if (networkMode == Client) {
			actorBuffer.position = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition();
			actorBuffer.ID = actorID;

			sendbuf = (char*)&actorBuffer; //binary representation 

			int sendOk = sendto(out, sendbuf, sizeof(ActorBuffer), 0, (sockaddr*)&server, sizeof(server));
			if (sendOk == SOCKET_ERROR) {
				std::cout << "Send Error: " << WSAGetLastError() << std::endl;
			}

			if (sendOk > 0) {
				std::unique_lock<std::mutex> lock(transformUpdateMutex);
				printf("%f %f %f\n", actorBuffer.position.x, actorBuffer.position.y, actorBuffer.position.z);
				//EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->SetPosition(actorBuffer.position);
				lock.unlock();
			}
			else {
				std::cout << "Receive failed with error: " << WSAGetLastError() << std::endl;
				this->~NetworkManager();
				return;
			}
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
		// Create a socket, notice that it is a user datagram socket (UDP)
		in = socket(AF_INET, SOCK_DGRAM, 0); //SOCK_DGRAM is what makes it a udp socket

		// Create a server hint structure for the server
		server.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
		server.sin_family = AF_INET; // Address format is IPv4
		server.sin_port = htons(5400); //big endian system - most machines are big endian

		//Try and bind the socket to the IP and port
		if (bind(in, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) { //bind the socket
			std::cout << "Can't bind to socket " << WSAGetLastError() << std::endl;
		}

		std::cout << "Server Up!" << std::endl;

		clientLength = sizeof(client); // The size of the client information
	}
	else if (networkMode == Client) {
		out = socket(AF_INET, SOCK_DGRAM, 0);

		// Create a hint structure for the server
		serverLength = sizeof(server);
		server.sin_family = AF_INET; // AF_INET = IPv4 addresses
		server.sin_port = htons(5400); // Little to big endian conversion
		inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); //"127.0.0.1" is your own ip - Convert from string to byte array

		GetServerActorName();
	}
	return true;
}

void NetworkManager::GetServerActorName() {
	actorBuffer.ID = -1;

	//Send an initial buffer
	sendbuf = (char*)&actorBuffer; //binary representation 

	int sendOk = sendto(out, sendbuf, sizeof(ActorBuffer), 0, (sockaddr*)&server, sizeof(server));
	if (sendOk == SOCKET_ERROR) {
		std::cout << "Send Error: " << WSAGetLastError() << std::endl;
	}

	char actorNameBuffer[DEFAULT_BUFFER_LENGTH];

	ZeroMemory(actorNameBuffer, DEFAULT_BUFFER_LENGTH);

	while (true) {
		int bytesIn = recvfrom(out, actorNameBuffer, 1, 0, (sockaddr*)&server, &serverLength);
		if (bytesIn == SOCKET_ERROR) {
			std::cout << "Error receiving from client " << WSAGetLastError() << std::endl;
		}
		if (bytesIn > 0 && actorNameBuffer != "-1") {
			break;
		}
	}
	actorID = (int)actorNameBuffer[0] - 48;
	std::cout << actorID << std::endl;
}

void NetworkManager::AddClientActor() {
	std::string clientName = std::to_string(clientActors.size());

	std::unique_lock<std::mutex> lock(transformUpdateMutex);
	EngineManager::Instance()->GetActorManager()->AddActor<Actor>(clientName, new Actor(nullptr));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName)->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>(clientActorTemplateName.c_str()));
	//maybe make a spawn point actor? Just a though
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName)->AddComponent<TransformComponent>(nullptr, Vec3(12.5f, 7.5f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(3.0f,3.0f,3.0f));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName)->OnCreate();
	lock.unlock();

	clientActors[clientName] = EngineManager::Instance()->GetActorManager()->GetActor<Actor>(clientName);

	//ZeroMemory(sendbuf, sizeof(ActorBuffer));

	sendbuf = (char*)clientName.c_str();
	while (true) {
		int sendOk = sendto(in, sendbuf, clientName.size(), 0, (sockaddr*)&client, sizeof(client));
		if (sendOk == SOCKET_ERROR) {
			std::cout << "Send Error: " << WSAGetLastError() << std::endl;
		}
		else {
			break;
		}
	}

}