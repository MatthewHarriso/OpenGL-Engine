#pragma once

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_faceCount;
	unsigned int m_ProgramID;
};

struct OBJVertex
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};