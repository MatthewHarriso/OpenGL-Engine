#include "Object.h"
#include "TextureLoader.h"
#include "VertexLoader.h"

Object::Object()
{
	Startup();
}

Object::~Object()
{
	
}

void Object::Startup()
{
	renderIndex = 0;

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

void Object::SetParent(Object * l_parent)
{
	Parent = l_parent;
}

void Object::SetRenderIndex(unsigned int l_index)
{
	renderIndex = l_index;
}

void Object::SetShader(unsigned int l_shader)
{
	std::vector<OpenGLInfo>::iterator it = _Mesh->begin();
	(*it).m_ProgramID = l_shader;
}

unsigned int Object::GetRenderIndex()
{
	return renderIndex;
}

std::vector<OpenGLInfo>* Object::GetOpenGLInfo()
{
	return _Mesh;
}

void Object::Update(float DeltaTime)
{

}

void Object::Draw()
{

}
