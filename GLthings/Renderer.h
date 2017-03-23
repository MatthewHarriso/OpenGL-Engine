#pragma once
#include <map>
#include <vector>

class Object;
class ShaderManager;
class TextureManager;

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

	std::map<unsigned int, Object*> models;

	int bufferIndex;
};

