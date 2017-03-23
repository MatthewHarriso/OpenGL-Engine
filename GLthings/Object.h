#pragma once
#include <vector>

#include "Structures.h"

class VertexLoader;
class TextureLoader;

class Object
{
public:

	Object ();
	~Object ();

	void Startup ();

	void SetTexture (char* l_Texture);
	void SetMesh (char* l_Mesh);
	void SetParent (Object* l_Parent);
	void SetRenderIndex (unsigned int l_index);
	void SetShader(unsigned int l_shader);

	unsigned int GetRenderIndex ();

	std::vector<OpenGLInfo>* GetOpenGLInfo();

	void Update (float DeltaTime);

	void Draw ();

	void ShutDown ();

private:

	Object* Parent;

	unsigned int renderIndex;

	std::vector<OpenGLInfo>* _Mesh;
	unsigned int _Shader;
	unsigned int _Texture;

	TextureLoader* textureLoader;

	VertexLoader* vertexLoader;

};

