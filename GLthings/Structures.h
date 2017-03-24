#pragma once

#include <glm/glm.hpp>

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
	float tx, ty, tz;
	float u, v;
	float s, t;
};

struct Vertex
{
	glm::vec4 tangent;
	glm::vec4 bitangent;
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec4 colour;
	glm::vec2 texcoord;
};