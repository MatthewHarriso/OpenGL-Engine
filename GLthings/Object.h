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

	unsigned int GetRenderIndex ();

	void Update (float DeltaTime);

	void Draw ();

	void ShutDown ();

private:

	Object* Parent;

	unsigned int renderIndex;

	std::vector<OpenGLInfo>* _Mesh;
	int _Shader;
	int _Texture;

	TextureLoader* textureLoader;

	VertexLoader* vertexLoader;

};

