#pragma once
#include <vector>

#include "Structures.h"

class VertexLoader;
class TextureManager;

class Object
{
public:

	Object ();
	~Object ();

	void Startup ();

	void SetTexture (char* l_Texture);
	void SetNormal (char* l_Texture);
	void SetSpecular (char* l_Texture);
	void SetMesh (char* l_Mesh);
	void SetParent (Object* l_Parent);
	void SetRenderIndex (unsigned int l_index);
	void SetShader (int l_shader);

	unsigned int GetRenderIndex ();

	int GetShader();

	int* GetTexture();

	std::vector<OpenGLInfo>* GetOpenGLInfo();

	void Update (float DeltaTime);

	void Draw ();

	void ShutDown ();

private:

	Object* Parent;

	unsigned int renderIndex;

	std::vector<OpenGLInfo>* _Mesh;
	int _Shader;
	int _Texture[3];

	TextureManager* textureManager;

	VertexLoader* vertexLoader;

};

