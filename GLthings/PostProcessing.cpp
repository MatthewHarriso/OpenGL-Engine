#include "PostProcessing.h"
#include "FlyCamera.h"

#include <iostream>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

PostProcessing* PostProcessing::instance = nullptr;

PostProcessing::PostProcessing()
{

}

void PostProcessing::Create()
{
	m_fbo = 0;
	m_fboDepth = 0;
	m_fboTexture = 0;

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	
	// TODO: we attach render targets here, code on following pages 
	
	// create a texture and bind it
	glGenTextures(1, &m_fboTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);

	// specify texture format for storage
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// attach it to the framebuffer as the first colour attachment
	// the FBO MUST still be bound
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fboTexture, 0);

	// setup and bind a 24bit depth buffer as a render buffer
	glGenRenderbuffers(1, &m_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, SCREEN_WIDTH, SCREEN_HEIGHT);
	// while the FBO is still bound
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };

	glDrawBuffers(1, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error!\n");
	}

	// unbind the FBO so that we can render to the back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::vec2 texelSize = 1.0f / glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
	glm::vec2 halfTexel = texelSize * 0.5f;

	float vertexData[] = 
	{ 
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
		-1, 1, 0, 1, halfTexel.x, 1 - halfTexel.y,

		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, -1, 0, 1, 1 - halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
	};

	//unsigned int indexData[] = { 0, 1, 2, 0, 2, 3, };

	//unsigned int m_ibo;

	glGenVertexArrays(1, &tempArray.m_VAO);
	glBindVertexArray(tempArray.m_VAO);

	glGenBuffers(1, &tempArray.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, tempArray.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertexData, GL_STATIC_DRAW);

	//glGenBuffers(1, &m_ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

PostProcessing* PostProcessing::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new PostProcessing();
	}

	return instance;
}

PostProcessing::~PostProcessing()
{

}

void PostProcessing::SetCamera(FlyCamera * l_camera)
{
	m_camera = l_camera;
}

void PostProcessing::Update(float l_deltaTime)
{
}

void PostProcessing::Draw ()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.75f, 0.75f, 0.75f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw our meshes, or gizmos, to the render target
}

void PostProcessing::DrawPostProcessing(unsigned int l_programID)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glClearColor(0.25f, 0.25f, 0.25f, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(l_programID);

	//int loc = glGetUniformLocation(l_programID, "projectionView");
	//glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_camera->GetProjectionView()[0][0]));

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);

	glUniform1i(glGetUniformLocation(l_programID, "screenTexture"), 0);

	glBindVertexArray(tempArray.m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
