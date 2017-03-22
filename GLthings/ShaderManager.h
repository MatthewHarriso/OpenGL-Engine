#pragma once
#include <vector>
#include <iostream>

enum ShaderType
{
	ShaderType_DEFAULT,
	ShaderType_LIGHTING,
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

	void Update(float l_deltaTime);

private:

	ShaderManager();

	static ShaderManager* instance;

	std::vector<std::string> shaders;

	FlyCamera* myCamera;

	float timer;

	int eLight;

	int shader_Counter;

	std::vector<unsigned int> IDs;

	unsigned int programID;
};
