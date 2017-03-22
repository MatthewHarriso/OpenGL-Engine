#include "ShaderManager.h"
#include "FlyCamera.h"

#include <fstream>
#include <sstream>

#include <gl_core_4_4.h>

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

void ShaderManager::SetCamera(FlyCamera * l_camera)
{
	myCamera = l_camera;
}

void ShaderManager::LoadShaders()
{
	eLight = 1;

	timer = 0;

	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 position; \
							layout(location=1) in vec4 normal; \
							layout(location=2) in vec4 colour; \
							out vec4 vColour; \
							out vec4 vNormal; \
							out vec4 vPosition; \
							uniform mat4 projectionViewWorldMatrix; \
							uniform float time; \
							uniform float heightScale; \
							void main() \
							{ \
								vPosition = position; \
								vColour = vec4(78/255.0f, 46 / 255.0f, 40 / 255.0f, 1); \
								vec4 P = position; \
								P.y += sin((time * 10) + (-position.x * 0.125f)) * 4; \
								P.y += (heightScale + (position.x * 0.25f) * 0.75f) * cos(time * 2); \
								P.y += sin((time * 7.5f) + (abs(position.x * 0.05f))) * (cos(time * 2) * 2); \
								vNormal = normal; \
								gl_Position = projectionViewWorldMatrix * P;}";

	//vColour.b = -P.y; \
								//
//	This makes the image wave like an ocean/flag.
//

	const char* fsSource = shaders.begin;

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

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}

void ShaderManager::LoadFromFile()
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

void ShaderManager::Update(float l_deltaTime)
{
	timer += l_deltaTime;

	unsigned int location = glGetUniformLocation(m_programID, "time");
	glUniform1f(location, timer);

	location = glGetUniformLocation(m_programID, "heightScale");
	glUniform1f(location, l_deltaTime);

	location = glGetUniformLocation(m_programID, "CameraPos");
	glUniform3f(location, myCamera->GetPosition().x, myCamera->GetPosition().y, myCamera->GetPosition().z);

	location = glGetUniformLocation(m_programID, "lightType");
	glUniform1i(location, eLight);

	//					|
	//	Draw Function	|
	//					V

	glUseProgram(m_programID);

	unsigned int location = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(myCamera->GetProjectionView()));

	location = glGetUniformLocation(m_programID, "LightDir");
	glUniform3f(location, 0.707, 0.707, 0.0);
	location = glGetUniformLocation(m_programID, "LightColour");
	glUniform3f(location, 1, 1, 1);
	location = glGetUniformLocation(m_programID, "SpecPow");
	glUniform1f(location, 100.0);

	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	for (auto& gl : m_glInfo)
	{
		glBindVertexArray(gl.m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, gl.m_faceCount * 3);
	}
}
