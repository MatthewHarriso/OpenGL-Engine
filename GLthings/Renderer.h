#pragma once
#include <map>
#include <vector>

class Object;
class ShaderManager;
class TextureManager;
class PostProcessing;

class Renderer
{
public:

	static Renderer* GetInstance();
	
	~Renderer ();

	void AddToQueue (Object* l_newObject);

	void RemoveFromQueue (Object* l_Object);

	void Update (float l_deltaTime);

	void Draw ();

private:
	
	Renderer ();

	static Renderer* instance;

	ShaderManager* shaderManager;
	TextureManager* textureManager;
	PostProcessing* postProcessing;

	std::map<unsigned int, Object*> models;

	//	What object is currently being drawn in the map.
	int currentObject;

	//	How many current objects are in the map.
	int bufferIndex;

	//	How many different shaders are used for each object.
	int shaderIndex;
	//	How far apart the objects that use different shaders are.
	int indexDifference;
	//	Each different shaders increases the array by one size,
	//	and when a shader of the same type is added it gets added straight after the same types.
	//	This ensure all shaders of the same type load at the same time.
	int shaderDifference[5];
};

