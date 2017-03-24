#include "TextureManager.h"

TextureManager* TextureManager::instance;

TextureManager::TextureManager()
{
	textureIndex = 0;

	for (int i = 0; i < TEXTURE_LIMIT; i++)
	{
		m_texture[i] = 0;
	}
}

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureManager();
	}

	return instance;
}

TextureManager::~TextureManager()
{

}

int TextureManager::AddTexture(const char * l_textureFileName)
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	unsigned char* data = stbi_load(l_textureFileName,
		&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture[textureIndex]);
	glBindTexture(GL_TEXTURE_2D, m_texture[textureIndex]);
	if (imageFormat == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
		if (imageFormat == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	textureIndex++;

	stbi_image_free(data);

	return m_texture[textureIndex - 1];
}

int TextureManager::AddNormal(const char * l_textureFileName)
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	unsigned char* data = stbi_load(l_textureFileName,
		&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture[textureIndex]);
	glBindTexture(GL_TEXTURE_2D, m_texture[textureIndex]);
	if (imageFormat == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
		if (imageFormat == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	textureIndex++;

	stbi_image_free(data);

	return m_texture[textureIndex - 1];
}

int TextureManager::GetTexture(int l_textureIndex)
{
	return m_texture[l_textureIndex];
}

void TextureManager::Update(float l_deltaTime)
{

}

void TextureManager::Draw()
{

}
