#include "Server.h"
#include <iostream>
#include "DataTypes.h"

Server::Server()
{
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		clientSockets[i] = INVALID_SOCKET;
		isAllPlayersReady[i] = false;
	}

	listenSocket = INVALID_SOCKET;
	recvBufLen = DEFAULT_BUFLEN;

	currentConnectionID = 0;

	packetSend = new NetworkPacket;
	packetRecv = new NetworkPacket;
	pos = new Server_Vector2;
	hlth = new Health;
	cht = new Chat;
	idNumber = new ID;

	packetSend->id = EPacketID_ID;

	idNumber->idToSend = 0;

	memcpy(packetSend->data, idNumber, sizeof(ID));

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);

		WSACleanup();
	}

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);

		WSACleanup();
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	//If iMode!=0, non-blocking mode is enabled.
	u_long iMode = 1;

	for (int i = 0; i < MAX_CONNECTIONS; i++)
		ioctlsocket(clientSockets[i], FIONBIO, &iMode);

	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error at socket (): %d\n", WSAGetLastError());

		freeaddrinfo(result);
		WSACleanup();
	}

	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());

		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
	}
}

Server::~Server()
{
	delete packetSend;
	delete packetRecv;
	delete pos;
	delete hlth;
	delete cht;
	delete idNumber;
}

void Server::Update()
{
	//
	//	ERROR HERE
	//
	//	This listen (listenSocket, SOMAXCONN) command waits for a connection and once the client connects it keeps running this for the first client and fills up clientSockets[] with just the first client.
	//
	if (listen(listenSocket, SOMAXCONN))
		if (currentConnectionID < MAX_CONNECTIONS)
			ConnectionFound();

	for (int i = 0; i < currentConnectionID; i++)
	{
		//
		//	ERROR HERE
		//
		//	The client sends a packet when you click play game to send a bool but the server only ever sees an ID packet for some reason.
		//
		recv(clientSockets[i], (char*)packetRecv, sizeof(NetworkPacket), 0);

		switch (packetRecv->id)
		{
		case EPacketID_Position:
			SendingData((Server_Vector2*)packetRecv->data, i);

			for (int j = 0; j < currentConnectionID; j++)
				if (j != i)
					iSendResult = send(clientSockets[currentConnectionID], (const char*)packetSend, sizeof(NetworkPacket), 0);

			break;

		case EPacketID_Chat:
			SendingData((Chat*)packetRecv->data, i);

			for (int j = 0; j < currentConnectionID; j++)
				if (j != i)
					iSendResult = send(clientSockets[currentConnectionID], (const char*)packetSend, sizeof(NetworkPacket), 0);

			break;

		case EPacketID_Health:
			SendingData((Health*)packetRecv->data, i);

			for (int j = 0; j < currentConnectionID; j++)
				if (j != i)
					iSendResult = send(clientSockets[currentConnectionID], (const char*)packetSend, sizeof(NetworkPacket), 0);

			break;

		case EPacketID_ID:
			//
			//	ERROR HERE
			//
			//	When this ID data gets sent it is never the right data, it is always either no data or some random number.
			//
			SendingData((ID*)packetRecv->data, i);

			iSendResult = send(clientSockets[currentConnectionID], (const char*)packetSend, sizeof(NetworkPacket), 0);

			break;

		case EPacketID_Ready:
			isAllPlayersReady[i] = (bool)packetRecv->data;
			if (allPlayersReady)
			{
				for (int j = 0; j < currentConnectionID; j++)
				{
					SendingData((ReadyState*)packetRecv->data, i);
					iSendResult = send(clientSockets[currentConnectionID], (const char*)packetSend, sizeof(NetworkPacket), 0);
				}
			}
		}

		printf("recieved: %d\n", packetRecv->id);
		printf("recieved: %s\n", (char*)packetRecv->data);
	}

	for (int i = 0; i < MAX_CONNECTIONS; i++)
		if (!isAllPlayersReady[i])
			allPlayersReady = false;
		else
			allPlayersReady = true;
}

void Server::ConnectionFound()
{
	//
	//	ERROR HERE
	//
	//	So this is where the connections are assigned and also the id is sent out but the id is always -1 on the clients side even though currentConnectionID is never -1.
	//
	clientSockets[currentConnectionID] = accept(listenSocket, NULL, NULL);

	idNumber->idToSend = currentConnectionID;

	SendingData(idNumber, currentConnectionID);

	iSendResult = send(clientSockets[currentConnectionID], (const char*)packetSend, sizeof(NetworkPacket), 0);

	currentConnectionID++;
}

int Server::CurrentConnections()
{
	return currentConnectionID;
}

void Server::ShutdownServer()
{
	for (int i = 0; i < currentConnectionID; i++)
		iResult = shutdown(clientSockets[i], SD_SEND);

	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());

		for (int i = 0; i < currentConnectionID; i++)
			closesocket(clientSockets[i]);

		WSACleanup();
	}

	for (int i = 0; i < currentConnectionID; i++)
		closesocket(clientSockets[i]);

	WSACleanup();
}

void Server::SendingData(Server_Vector2* l_Pos, int l_PlayerID)
{
	packetSend->id = EPacketID_Position;
	memcpy(packetSend->data, l_Pos, sizeof(Server_Vector2));
	packetSend->playerId = l_PlayerID;
}

void Server::SendingData(Chat* l_Chat, int l_PlayerID)
{
	packetSend->id = EPacketID_Chat;
	memcpy(packetSend->data, l_Chat, sizeof(Chat));
	packetSend->playerId = l_PlayerID;
}

void Server::SendingData(Health* l_Health, int l_PlayerID)
{
	packetSend->id = EPacketID_Health;
	memcpy(packetSend->data, l_Health, sizeof(Health));
	packetSend->playerId = l_PlayerID;
}

void Server::SendingData(ID* l_ID, int l_PlayerID)
{
	packetSend->id = EPacketID_ID;
	memcpy(packetSend->data, l_ID, sizeof(ID));
	packetSend->playerId = l_PlayerID;
}

void Server::SendingData(ReadyState* l_Ready, int l_PlayerID)
{
	packetSend->id = EPacketID_Ready;
	memcpy(packetSend->data, l_Ready, sizeof(ReadyState));
	packetSend->playerId = l_PlayerID;
}

bool Server::IsEveryoneReady()
{
	return allPlayersReady;
}
