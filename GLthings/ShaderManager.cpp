#include "ShaderManager.h"
#include "FlyCamera.h"

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
	for (int i = 0; i < ShaderType::ShaderType_LENGTH; i++)
	{
		programIDs[i] = 0;
	}
	
	shader_Counter = 0;

	m_programID = 0;
}

void ShaderManager::ShutDown()
{

}

void ShaderManager::SetCamera(FlyCamera * l_camera)
{
	myCamera = l_camera;
}

void ShaderManager::LoadShaders()
{
	eLight = 1;

	timer = 0;

	std::vector<std::string>::iterator v_it = vShaders.begin();
	std::vector<std::string>::iterator f_it = fShaders.begin();

	for (std::vector<std::string>::iterator it = vShaders.begin(); it != vShaders.end(); it++)
	{
		const char* vsSource = (*v_it).data();

		const char* fsSource = (*f_it).data();

		int success = GL_FALSE;

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
		glCompileShader(vertexShader);
		glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
		glCompileShader(fragmentShader);

		m_programID = glCreateProgram();

		glAttachShader(m_programID, vertexShader);
		glAttachShader(m_programID, fragmentShader);
		glLinkProgram(m_programID);

		glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
		char* infoLog;
		if (success == GL_FALSE)
		{
			int infoLogLength = 0;

			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);

			infoLog = new char[infoLogLength];

			glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		}

		programIDs[shader_Counter] = m_programID;
		shader_Counter++;

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		v_it++;
		f_it++;
	}
}

void ShaderManager::LoadFromFile()
{
	std::fstream file;

	std::string text, c;

	file.open("Shaders/VShader_Default.txt");

	if (file.is_open())
	{
		std::getline(file, c);
		text += c + "\n";

		while (std::getline(file,c))
		{
			text += c;
		}

		vShaders.push_back(text);
	}

	file.close();

	file.open("Shaders/FShader_Default.txt");

	if (file.is_open())
	{
		std::getline(file, c);
		text += c + "\n";

		while (std::getline(file, c))
		{
			text += c;
		}

		fShaders.push_back(text);
	}

	file.close();
}

void ShaderManager::Update(float l_deltaTime)
{
	timer += l_deltaTime; 
	
	unsigned int location = glGetUniformLocation(m_programID, "time");
	glUniform1f(location, timer);

	location = glGetUniformLocation(m_programID, "heightScale");
	glUniform1f(location, l_deltaTime);
}

void ShaderManager::Draw(std::vector<OpenGLInfo>* l_vecOpenGLInfo)
{
	std::vector<OpenGLInfo>::iterator it = l_vecOpenGLInfo->begin();

	OpenGLInfo l_openGLInfo = *it;

	glUseProgram(l_openGLInfo.m_ProgramID);

	unsigned int location = glGetUniformLocation(l_openGLInfo.m_ProgramID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(myCamera->GetProjectionView()));

	location = glGetUniformLocation(l_openGLInfo.m_ProgramID, "LightDir");
	glUniform3f(location, 0.707, 0.707, 0.0);

	location = glGetUniformLocation(l_openGLInfo.m_ProgramID, "LightColour");
	glUniform3f(location, 1, 1, 1);

	location = glGetUniformLocation(l_openGLInfo.m_ProgramID, "SpecPow");
	glUniform1f(location, 100.0);

	location = glGetUniformLocation(l_openGLInfo.m_ProgramID, "CameraPos");
	glUniform3f(location, myCamera->GetPosition().x, myCamera->GetPosition().y, myCamera->GetPosition().z);

	location = glGetUniformLocation(l_openGLInfo.m_ProgramID, "lightType");
	glUniform1i(location, eLight);

	/*
	glBindVertexArray(l_openGLInfo.m_VAO);

	glDrawElements(GL_TRIANGLES, l_openGLInfo.m_faceCount, GL_UNSIGNED_INT, 0);
	*/

	for ( ; it != l_vecOpenGLInfo->end(); it++)
	{
		glBindVertexArray((*it).m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, (*it).m_faceCount * 3);
	}
}
