#pragma once

#include <AIE\Gizmos.h>
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

#include "TextureManager.h"
#include "VertexLoader.h"

#define OBJECT_LIMIT 50

struct GLFWwindow;

class Renderer;
class ShaderManager;
class Object;
class FlyCamera;
class Entity;

class Application
{

public:

	Application();
	~Application();

	bool Update();

	bool Startup();

	void Draw();

	void Shutdown();

private:

	GLFWwindow* window;

	FlyCamera* myCamera;

	Object* gameObjects[OBJECT_LIMIT];

	unsigned int objectCounter;

	Renderer* renderer;

	ShaderManager* shaderManager;

	TextureManager* textureManager;

	VertexLoader* vertexLoader;

	glm::mat4 view;
	glm::mat4 projection;
	
	float previousTime;
	float currentTime;
	float deltaTime;

	float secondTimer;

	int frameCounter;

	//
	//	0 = horizontal,
	//	1 = vertical.
	//
	int splitScreenOrientation;

	int ERROR_INDEX;
};

