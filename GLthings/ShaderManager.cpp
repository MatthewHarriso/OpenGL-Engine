#include "ShaderManager.h"
#include <iostream>
#include <fstream>

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::Create()
{
	shader_Counter = 0;

	programID = 0;
}

void ShaderManager::ShutDown()
{

}

void ShaderManager::LoadShaders()
{
	std::fstream file;

	file.open("Shader_Default.txt");

	if (file.is_open())
	{
		char c;
		while (file.get(c))
		{
			std::cout << c;
		}

		std::streampos begin, end;

		begin = file.tellg();

		file.seekg(0, std::ios::end);

		end = file.tellg();

		file.read(shaders[shader_Counter], end - begin);
	}

	std::cout << shaders[shader_Counter] << std::endl;

	shader_Counter++;

	file.close();
}
