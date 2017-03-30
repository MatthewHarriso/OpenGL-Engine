#pragma once

#include "gl_core_4_4.h"
#include "Structures.h"

class FlyCamera;

class PostProcessing
{
public:
	
	static PostProcessing* GetInstance();

	~PostProcessing();

	void Create();

	void SetCamera(FlyCamera* l_camera);

	void Update(float l_deltaTime);

	void Draw();

	void DrawPostProcessing(unsigned int l_programID);

private:

	PostProcessing();

	static PostProcessing* instance;

	unsigned int m_fbo;
	unsigned int m_fboDepth;
	unsigned int m_fboTexture;

	OpenGLInfo tempArray;

	FlyCamera* m_camera;
};

