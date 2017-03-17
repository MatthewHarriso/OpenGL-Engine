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

	static ShaderManager* GetInstance();
	
	~ShaderManager();

	void Create();

	void ShutDown();

	void LoadShaders();

	//std::streampos fileSize(const char * filePath);

private:

	ShaderManager();

	static ShaderManager* instance;

	std::vector<std::string> shaders;

	int shader_Counter;

	std::vector<unsigned int> IDs;

	unsigned int programID;
};
