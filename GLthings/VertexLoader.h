#pragma once

#include <gl_core_4_4.h>
#include "tiny_obj_loader.h"
#include <vector>
#include <map>
#include <string>

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_faceCount;
};

struct OBJVertex
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

class FlyCamera;

class VertexLoader
{
public:

	~VertexLoader();
	
	static VertexLoader* GetInstance();

	std::vector<OpenGLInfo>* CreateOpenGLBuffers(tinyobj::attrib_t &attribs, std::vector<tinyobj::shape_t> &shapes);

	void SetCamera(FlyCamera* l_camera);

	void GenerateGrid(unsigned int rows, unsigned int cols);

	std::vector<OpenGLInfo>* LoadGeometry(const char* l_textureFileName);

	void Update(float l_deltaTime);

	void Draw();

private:
	
	VertexLoader();

	static VertexLoader* instance;

	std::map<const std::string, std::vector<OpenGLInfo>*> models;

	FlyCamera* myCamera;

	float timer;

	int eLight;

	unsigned int indexCount;

	unsigned int m_programID;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
};

