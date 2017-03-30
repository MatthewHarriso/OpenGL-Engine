#include "Renderer.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "PostProcessing.h"
#include "Object.h"
#include "Structures.h"

Renderer* Renderer::instance;

Renderer::Renderer()
{
	currentObject = 0;

	bufferIndex = 0;

	shaderIndex = 0;

	indexDifference = 0;

	for (int i = 0; i < 5; i++)
	{
		shaderDifference[i] = -1;
	}

	shaderManager = ShaderManager::GetInstance();

	postProcessing = PostProcessing::GetInstance();
}

Renderer::~Renderer()
{

}

Renderer* Renderer::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Renderer();
	}

	return instance;
}

void Renderer::AddToQueue(Object* l_newObject)
{
	//	Safety check to make sure the map isn't empty.
	if (models.size() > 0)
	{
		//	Checks the map to see if the shaderID for the object to be added is the same as the first object added to the map.
		if (l_newObject->GetShader() == models.find(0)->second->GetShader())
		{
			//	Safety check to make sure that there is more then one object shaderID in the map.
			if ((&models.find(indexDifference + 1)) != nullptr && indexDifference + 1 < models.size())
			{
				//	If the shaderID is the same then we need to add the object to after the last object added with the same shaderID.
				//	Increase the difference between the first object with the same shaderID to the next object with another shaderID.
				indexDifference++;

				//	Add the first object of the other shaderID to the end of the map and add the new object to the end of objects in the map with the same shaderID.
				models.insert(std::map<unsigned int, Object*>::value_type(bufferIndex, models.find(indexDifference)->second));
				models.find(indexDifference)->second->SetRenderIndex(bufferIndex);

				models.insert(std::map<unsigned int, Object*>::value_type(indexDifference, l_newObject));
				l_newObject->SetRenderIndex(indexDifference);
			}
			else
			{
				//	If the safety check failed for more then one shaderID then we can safely assume that only one shaderID is being used for all objects.
				models.insert(std::map<unsigned int, Object*>::value_type(bufferIndex, l_newObject));
				l_newObject->SetRenderIndex(bufferIndex);
			}
		}
		else
		{
			//	If the new object has a different shaderID to the first object in the map then we assume there is only 2 shaderID's for the moment and add the new object to the end of the map.
			models.insert(std::map<unsigned int, Object*>::value_type(bufferIndex, l_newObject));
			l_newObject->SetRenderIndex(bufferIndex);
		}
	}
	else
	{
		//	If the map is empty we just add the new object to the map.
		models.insert(std::map<unsigned int, Object*>::value_type(bufferIndex, l_newObject));
		l_newObject->SetRenderIndex(bufferIndex);
	}

	//	Increase the index of how many objects we currently have in the map.
	bufferIndex++;
}

void Renderer::RemoveFromQueue(Object* l_Object)
{
	//	Remove the object from the render queue.
	models.erase(models.find(l_Object->GetRenderIndex()));
	bufferIndex--;
}

void Renderer::Update(float l_deltaTime)
{
	//	Updates the objects shader values depending on what object is currently being drawn.
	shaderManager->Update(l_deltaTime, (*models.find(currentObject)).second->GetOpenGLInfo());

	//
	//	I think this should be changed to sending the shaderManager Update an int of which shaderID is being used and then update the necessary values inside the update with a switch.
	//

	postProcessing->Update(l_deltaTime);
}

void Renderer::Draw()
{
	postProcessing->Draw();
	
	//	Goes through each object in the map and draws them to the screen.
	for (int i = 0; i < bufferIndex; i++)
	{
		currentObject = i;
		shaderManager->Draw((*models.find(currentObject)).second->GetOpenGLInfo(), (*models.find(currentObject)).second->GetTexture());
	}

	postProcessing->DrawPostProcessing(shaderManager->GetShaderID(ShaderType::ShaderType_POSTPROCESSING));
	/*
	for (int i = 0; i < bufferIndex; i++)
	{
		currentObject = i;
		shaderManager->Draw((*models.find(currentObject)).second->GetOpenGLInfo(), (*models.find(currentObject)).second->GetTexture());
	}
	*/
}