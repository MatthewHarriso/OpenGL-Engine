#include "Renderer.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Update(float l_deltaTime)
{
	/*
	timer += l_deltaTime;

	unsigned int location = glGetUniformLocation(m_programID, "time");
	glUniform1f(location, timer);

	location = glGetUniformLocation(m_programID, "heightScale");
	glUniform1f(location, l_deltaTime);

	location = glGetUniformLocation(m_programID, "CameraPos");
	glUniform3f(location, myCamera->GetPosition().x, myCamera->GetPosition().y, myCamera->GetPosition().z);

	/*location = glGetUniformLocation(m_programID, "lightType");
	glUniform1i(location, eLight);*/
}

void Renderer::Draw()
{
	/*
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
	*/
}