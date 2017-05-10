#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment ( lib, "Ws2_32.lib" )

#define DEFAULT_BUFLEN 512
#define MAX_CONNECTIONS 4
#define DEFAULT_PORT "27015"

struct NetworkPacket;
struct Server_Vector2;
struct Chat;
struct Health;
struct ID;
struct ReadyState;

class Server
{
public:

	Server();
	~Server();

	void Update();

	void ConnectionFound();

	int CurrentConnections();

	void ShutdownServer();

	void SendingData(Server_Vector2* l_Pos, int l_PlayerID);
	void SendingData(Chat* l_Chat, int l_PlayerID);
	void SendingData(Health* l_Health, int l_PlayerID);
	void SendingData(ID* l_ID, int l_PlayerID);
	void SendingData(ReadyState* l_Ready, int l_PlayerID);

	bool IsEveryoneReady();

private:

	struct addrinfo *result = NULL, hints;

	WSADATA wsaData;

	SOCKET listenSocket;
	SOCKET clientSockets[MAX_CONNECTIONS];

	int iResult, iSendResult;
	int recvBufLen;

	char recvBuf[DEFAULT_BUFLEN];
	char sendBuf[DEFAULT_BUFLEN];

	int currentConnectionID;

	NetworkPacket* packetSend;
	NetworkPacket* packetRecv;
	Server_Vector2* pos;
	Health* hlth;
	Chat* cht;
	ID* idNumber;
	ReadyState* stateOfReadiness;

	bool isAllPlayersReady[MAX_CONNECTIONS];
	bool allPlayersReady;
};