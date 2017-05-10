#pragma once

#define BUFFER_LENGTH 64

enum EPacketID
{
	EPacketID_Position,
	EPacketID_Chat,
	EPacketID_Health,
	EPacketID_ID,
	EPacketID_Ready
};

struct NetworkPacket
{
	EPacketID id = EPacketID_ID;
	unsigned char data[BUFFER_LENGTH] = " ";
	int playerId = -1;
};

struct Server_Vector2
{
	Server_Vector2()
	{
		m_x = 0.0f;
		m_y = 0.0f;
	}

	Server_Vector2(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	float m_x = 0.0f;
	float m_y = 0.0f;
};

struct Chat
{
	char textToSend[BUFFER_LENGTH];
};

struct Health
{
	int healthToSend = 0;
};

struct ID
{
	int idToSend = 0;
};

struct ReadyState
{
	bool readyState = false;
};