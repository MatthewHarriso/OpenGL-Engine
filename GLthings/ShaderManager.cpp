#include "ShaderManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

ShaderManager* ShaderManager::instance = nullptr;

ShaderManager::ShaderManager()
{
	Create();
}

ShaderManager* ShaderManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ShaderManager();
	}

	return instance;
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

	std::string text, c;

	file.open("Shaders/Shader_Default.txt");


	if (file.is_open())
	{
		std::getline(file, c);
		text += c + "\n";

		while (std::getline(file,c))
		{
			text += c;
		}

		shaders.push_back(text);
		
		shader_Counter++;
	}

	file.close();
}

/*
std::string fline, ftext;
std::ifstream fshaderText("data/shaders/shaderStandardFSource.txt");

if (vshaderText.is_open())
{
	std::getline(vshaderText, vline);
	vtext += vline + "\n";
	while (std::getline(vshaderText, vline))
	{
		vtext += vline;

	}
	strcpy_s(shaderStandardVSource, 2048, vtext.c_str());
	vshaderText.close();

}
else std::cout << "Unable to open file";
*/

/*std::streampos ShaderManager::fileSize(const char* filePath) {

	std::streampos fsize = 0;
	std::fstream file(filePath, std::ios::binary);

	fsize = file.tellg();
	file.seekg(0, std::ios::end);
	fsize = file.tellg() - fsize;
	file.close();

	return fsize;
}*/