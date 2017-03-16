#pragma once

#include <AIE\Gizmos.h>
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

#include "TextureLoader.h"
#include "VertexLoader.h"

struct GLFWwindow;

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

	Entity* sun;
	Entity* sunMoon;
	Entity* earth;
	Entity* earthMoon;
	Entity* sunMoonMoon;

	TextureLoader* textureLoader;

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

