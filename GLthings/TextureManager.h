#pragma once

#include <STB\stb_image.h>
#include <gl_core_4_4.h>

#define TEXTURE_LIMIT 50

class TextureManager
{
public:
	
	~TextureManager();

	static TextureManager* GetInstance();

	int AddTexture(const char* l_textureFileName);
	int AddNormal(const char* l_textureFileName);

	int GetTexture(int l_textureIndex);

	void Update(float l_deltaTime);
	void Draw();

private:
	
	TextureManager();

	static TextureManager* instance;

	int textureIndex;

	unsigned int m_texture[TEXTURE_LIMIT];

};

