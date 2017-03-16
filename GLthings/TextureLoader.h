#pragma once

#include <STB\stb_image.h>
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#define TEXTURE_LIMIT 50

struct GLFWwindow;

class FlyCamera;

class TextureLoader
{
public:
	
	~TextureLoader();

	static TextureLoader* GetInstance();

	int AddTexture(const char* l_textureFileName);

	void SetScreen(GLFWwindow* l_window);

	void SetCamera(FlyCamera* l_camera);

	void Update(float l_deltaTime);
	void Draw();

private:
	
	TextureLoader();

	static TextureLoader* instance;

	GLFWwindow* window;

	FlyCamera* myCamera;

	int textureIndex;

	unsigned int m_texture[TEXTURE_LIMIT];
	unsigned int m_program;
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
};

