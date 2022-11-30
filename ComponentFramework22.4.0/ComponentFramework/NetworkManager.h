#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h> //include after WinSock2.h

#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFFER_LENGTH 512

#include <mutex>
#include "Vector.h"
#include <unordered_map>

#include "Actor.h"

enum NetworkNode {
	Offline, Server, Client
};

struct ActorBuffer {
	ActorBuffer() { ID = -1; position = MATH::Vec3(); orientation = Quaternion(); }
	unsigned int ID;
	MATH::Vec3 position;
	MATH::Quaternion orientation;
};

struct ServerBuffer {
	unsigned int numOfActors;
	unsigned int ID[5];
	MATH::Vec3 position[5];
	MATH::Quaternion orientation[5];
};

class NetworkManager {
public:
	NetworkManager();
	~NetworkManager();
	void Run();
	bool Initialize(NetworkNode networkMode_);
private:
	//Client Variables & Functions
	void GetServerActorName();
	void ReceiveServerBuffers();
	unsigned int actorID;

	//Server Variables & Functions
	void AddClientActor();
	void AddClientSession(void* data);
	std::string clientActorTemplateName;

	std::unordered_map<std::string, Ref<Actor>> clientActors;

	//server socket
	SOCKET listenSocket;

	//General Networking Variables
	ActorBuffer actorBuffer;
	NetworkNode networkMode;

	WSADATA wsaData; //https://learn.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-wsadata
	int iResult; //check if the start up failed

	struct addrinfo* result, * ptr, hints;

	//connection socket
	SOCKET connectSocket;

	std::mutex transformUpdateMutex;
	std::mutex serverBufferMutex;

	char* sendbuf;
	int iSendResult;
};