#include "ShaderManager.h"
#include "FlyCamera.h"

#include <fstream>
#include <sstream>

#define AMBIENT_LIGHT 0.25f
#define PI 3.141592654f

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

	light = glm::vec3(0);
}

void ShaderManager::ShutDown()
{

}

void ShaderManager::SetCamera(FlyCamera * l_camera)
{
	myCamera = l_camera;
}

int ShaderManager::GetShaderID(int l_shaderType)
{
	return programIDs[l_shaderType];
}

void ShaderManager::LoadShaders()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

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
			std::cout << infoLog;
		}

		programIDs[shader_Counter] = m_programID;
		shader_Counter++;

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		v_it++;
		f_it++;
	}
}

void ShaderManager::LoadFromFile(char* l_textName)
{
	std::fstream file;

	std::string text, c;

	file.open(l_textName);

	if (file.is_open())
	{
		std::getline(file, c);
		text += c + "\n";

		while (std::getline(file,c))
		{
			text += c;
		}

		if (l_textName[8] == 'V')
		{
			vShaders.push_back(text);
		}
		else
		{
			fShaders.push_back(text);
		}
	}
	else
	{
		std::cout << "File: " << l_textName << " - Is invalid." << std::endl;
	}

	file.close();
	text.clear();
}

void ShaderManager::Update(float l_deltaTime, std::vector<OpenGLInfo>* l_vecOpenGLInfo)
{
	std::vector<OpenGLInfo>::iterator it = l_vecOpenGLInfo->begin();

	OpenGLInfo l_openGLInfo = *it;

	timer += l_deltaTime; 
	
	int location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "time");
	glUniform1f(location, timer);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "heightScale");
	glUniform1f(location, l_deltaTime);
}

void ShaderManager::Draw(std::vector<OpenGLInfo>* l_vecOpenGLInfo, int l_textureIndexes[3])
{
	std::vector<OpenGLInfo>::iterator it = l_vecOpenGLInfo->begin();

	OpenGLInfo l_openGLInfo = *it;

	glUseProgram(programIDs[l_openGLInfo.m_ProgramID]);

	glActiveTexture(GL_TEXTURE0);
	if (l_textureIndexes[0] != -1)
	{
		glBindTexture(GL_TEXTURE_2D, l_textureIndexes[0]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glActiveTexture(GL_TEXTURE1);
	if (l_textureIndexes[1] != -1)
	{
		glBindTexture(GL_TEXTURE_2D, l_textureIndexes[1]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glActiveTexture(GL_TEXTURE2);
	if (l_textureIndexes[2] != -1)
	{
		glBindTexture(GL_TEXTURE_2D, l_textureIndexes[2]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	int location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "projectionViewWorldMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(myCamera->GetProjectionView()));
	
	light = glm::vec3(sin(glfwGetTime()), 0, cos(glfwGetTime()));

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "LightDir");
	glUniform3f(location, light.x, light.y, light.z);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "AmbientLight");
	glUniform1f(location, AMBIENT_LIGHT);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "LightColour");
	glUniform3f(location, 1, 1, 1);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "SpecPow");
	glUniform1f(location, 125.0f);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "roughness");
	glUniform1f(location, 0);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "CameraPos");
	glUniform3f(location, myCamera->GetPosition().x, myCamera->GetPosition().y, myCamera->GetPosition().z);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "lightType");
	glUniform1i(location, eLight);

	//
	//	SpotLightAngle is in radians.
	//
	//	1 = 57 degrees.
	//
	//	360 degrees = 2PI;
	//

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "SpotLightAngle");
	glUniform1f(location, PI * 0.33f);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "SpotLightFalloff");
	glUniform1f(location, PI * 0.05f);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "SpotLightCutoff");
	glUniform3f(location, 10.0f);

	location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "SpotLightPos");
	glUniform3f(location, glm::vec3(, , ));

	if (l_textureIndexes[0] != -1)
	{
		location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "diffuse");
		glUniform1i(location, 0);
	}

	if (l_textureIndexes[1] != -1)
	{
		location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "normal");
		glUniform1i(location, 1);
	}

	if (l_textureIndexes[2] != -1)
	{
		location = glGetUniformLocation(programIDs[l_openGLInfo.m_ProgramID], "specular");
		glUniform1i(location, 2);
	}

	for ( ; it != l_vecOpenGLInfo->end(); it++)
	{
		glBindVertexArray((*it).m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, (*it).m_faceCount * 3);
	}
}
