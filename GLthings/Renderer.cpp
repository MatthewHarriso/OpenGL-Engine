#include "Renderer.h"
#include "ShaderManager.h"
#include "TextureLoader.h"
#include "Object.h"

Renderer::Renderer()
{
	bufferIndex = 0;

	shaderManager = ShaderManager::GetInstance();

	textureManager = TextureManager::GetInstance();
}

Renderer::~Renderer()
{

}

void Renderer::AddToQueue(Object* l_newObject)
{
	models.insert(std::map<unsigned int, Object*>::value_type(bufferIndex, l_newObject));
	l_newObject->SetRenderIndex(bufferIndex);
	bufferIndex++;
}

void Renderer::RemoveFromQueue(Object* l_Object)
{
	models.erase(models.find(l_Object->GetRenderIndex()));
	bufferIndex--;
}

void Renderer::Update(float l_deltaTime)
{
	
}

void Renderer::Draw()
{

}