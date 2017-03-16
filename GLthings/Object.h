#pragma once
#include <vector>

struct OpenGLInfo;

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

	void Update (float DeltaTime);

	void Draw ();

	void ShutDown ();

private:

	Object* Parent;

	std::vector<OpenGLInfo>* _Mesh;
	int _Shader;
	int _Texture;

	TextureLoader* textureLoader;

	VertexLoader* vertexLoader;

};

