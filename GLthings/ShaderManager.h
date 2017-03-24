#pragma once
#include <vector>
#include <iostream>
#include "Structures.h"
#include <gl_core_4_4.h>
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

enum ShaderType
{
	ShaderType_DEFAULT,
	ShaderType_PHONG,
	ShaderType_LENGTH
};

class FlyCamera;

class ShaderManager
{
public:

	static ShaderManager* GetInstance();
	
	~ShaderManager();

	void Create();

	void ShutDown();

	void SetCamera(FlyCamera* l_camera);

	void LoadShaders();

	void LoadFromFile();

	void Update(float l_deltaTime, std::vector<OpenGLInfo>* l_openGLInfo);

	void Draw(std::vector<OpenGLInfo>* l_openGLInfo, int l_textureIndexes[3]);

private:

	ShaderManager();

	static ShaderManager* instance;

	std::vector<std::string> vShaders;
	std::vector<std::string> fShaders;

	glm::vec3 light;

	FlyCamera* myCamera;

	float timer;

	int eLight;

	int shader_Counter;

	unsigned int programIDs[ShaderType::ShaderType_LENGTH];

	unsigned int m_programID;
};

