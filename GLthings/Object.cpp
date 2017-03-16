#include "Object.h"
#include "TextureLoader.h"
#include "VertexLoader.h"

Object::Object()
{
	
}

Object::~Object()
{
	
}

void Object::Startup()
{
	vertexLoader = VertexLoader::GetInstance();
	textureLoader = TextureLoader::GetInstance();

	_Mesh = nullptr;
	_Shader = -1;
	_Texture = -1;

	Parent = nullptr;
}

void Object::ShutDown()
{
	if (Parent != nullptr)
	{
		delete Parent;
	}
}

void Object::SetTexture(char * l_Texture)
{
	_Texture = textureLoader->AddTexture(l_Texture);
}

void Object::SetMesh(char * l_Mesh)
{
	_Mesh = vertexLoader->LoadGeometry(l_Mesh);
}

void Object::SetParent(Object * l_Parent)
{

}

void Object::Update(float DeltaTime)
{

}

void Object::Draw()
{

}
