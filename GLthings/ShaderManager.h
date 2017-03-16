#pragma once
#include <vector>

enum ShaderType
{
	ShaderType_DEFAULT,
	ShaderType_LIGHTING,
	ShaderType_LENGTH
};

class ShaderManager
{
public:

	ShaderManager();
	~ShaderManager();

	void Create();

	void ShutDown();

	void LoadShaders();

private:

	std::vector<char*> shaders;

	int shader_Counter;

	std::vector<unsigned int> IDs;

	unsigned int programID;
};

