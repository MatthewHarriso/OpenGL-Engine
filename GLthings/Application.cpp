#include "Application.h"

#include "FlyCamera.h"
#include "Object.h"
#include "ShaderManager.h"
#include "Renderer.h"

#include <stdio.h>

#include <iostream>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define PLAYERS 1
#define CAMERA_SPEED 25

Application::Application()
{
	
}

Application::~Application()
{

}

void Application::Shutdown()
{
	delete myCamera;

	delete textureManager;
	
	delete vertexLoader;

	for (int i = 0; i < OBJECT_LIMIT; i++)
	{
		if (gameObjects[i] != nullptr)
		{
			delete gameObjects[i];
		}
	}

	/*delete sun;
	delete sunMoon;
	delete earth;
	delete earthMoon;
	delete sunMoonMoon;*/

	Gizmos::destroy();

	glfwDestroyWindow(window);

	glfwTerminate();
}

bool Application::Startup()
{
	if (glfwInit() == false)
	{
		ERROR_INDEX = -1;

		return false;
	}

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();

		ERROR_INDEX = -2;

		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);

		glfwTerminate();

		ERROR_INDEX = -3;

		return false;
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//					|
	//	Camera Setup	|
	//					V

	myCamera = new FlyCamera();

	myCamera->SetPerspective(glm::pi<float>() * 0.3f, (SCREEN_WIDTH / SCREEN_HEIGHT) * 1.75f, 1.5f, 500.f);

	myCamera->SetLookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	myCamera->SetSpeed(CAMERA_SPEED);

	myCamera->SetScreen(window);

	//					^
	//	Camera Setup	|
	//					|

	view = glm::lookAt(glm::vec3(20, 20, 20), glm::vec3(0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.0f);
	
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();

	//				|
	//	Objects		|
	//				v

	objectCounter = 0;

	for (int i = 0; i < OBJECT_LIMIT; i++)
	{
		gameObjects[i] = new Object();
		gameObjects[i]->Startup();
	}

	gameObjects[objectCounter]->SetMesh("models/soulspear.obj");
	gameObjects[objectCounter]->SetTexture("textures/soulspear_diffuse.tga");
	gameObjects[objectCounter]->SetNormal("textures/soulspear_normal.tga");
	gameObjects[objectCounter]->SetShader(ShaderType::ShaderType_DEFAULT);

	objectCounter++;

	//				^
	//	Objects		|
	//				|

	/*sun = new Entity();
	sunMoon = new Entity();
	earth = new Entity();
	earthMoon = new Entity();
	sunMoonMoon = new Entity();

	Gizmos::create();

	//					|
	//	Entities Setup	|
	//					v

	//	SUN ENTITY

	sun->SetColour(glm::vec4(1, 1, 0, 1));

	sun->SetDetails(glm::vec3(2, 30, 30));

	sun->SetRotateSpeed(0.025f);

	//	SUN'S MOON ENTITY

	sunMoon->SetColour(glm::vec4(1, 1, 1, 1));

	sunMoon->SetDetails(glm::vec3(0.5f, 15, 15));

	sunMoon->SetRotateSpeed(-0.05f);
	sunMoon->SetOrbitSpeed(-0.0249f);
	sunMoon->SetOrbitDistance(3.25f);

	sunMoon->SetParent(sun);

	//	EARTH ENTITY

	earth->SetColour(glm::vec4(0, 0.5f, 0.75f, 0.9f));

	earth->SetDetails(glm::vec3(1.25f, 30, 30));

	earth->SetRotateSpeed(0.1f);
	earth->SetOrbitSpeed(0.05f);
	earth->SetOrbitDistance(8.f);

	earth->SetParent(sun);

	//	EARTH'S MOON ENTITY

	earthMoon->SetColour(glm::vec4(0.46f, 0.45f, 0.44f, 0.9f));

	earthMoon->SetDetails(glm::vec3(0.5f, 15, 15));

	earthMoon->SetRotateSpeed(-0.1f);
	earthMoon->SetOrbitSpeed(-0.05f);
	earthMoon->SetOrbitDistance(2.25f);

	earthMoon->SetParent(earth);

	//	SUN'S MOON'S MOON

	sunMoonMoon->SetColour(glm::vec4(0.25f, 0.25f, 0.25f, 0.75f));

	sunMoonMoon->SetDetails(glm::vec3(0.15f, 5, 5));

	sunMoonMoon->SetRotateSpeed(0.5f);
	sunMoonMoon->SetOrbitSpeed(0.5);
	sunMoonMoon->SetOrbitDistance(0.75f);

	sunMoonMoon->SetParent(sunMoon);

	//					^
	//	Entities Setup	|
	//					|

	*/

	glfwSwapInterval(0);
	
	previousTime = 0;

	secondTimer = 0;

	frameCounter = 0;

	splitScreenOrientation = 1;

	glClearColor(0.25f, 0.25f, 0.25f, 1);

	//	Shader Manager
	shaderManager = ShaderManager::GetInstance();

	shaderManager->SetCamera(myCamera);

	shaderManager->LoadFromFile();

	shaderManager->LoadShaders();

	//	Texture Renderer.
	textureManager = TextureManager::GetInstance();

	textureManager->AddTexture("compiling.png");
	textureManager->AddTexture("test.jpg");
	//	textureLoader->AddTexture("compilingNM.png");

	//	Vertex Renderer.

	vertexLoader = VertexLoader::GetInstance();

	vertexLoader->SetCamera(myCamera);

	vertexLoader->GenerateGrid(100, 100);

	//	Vertex Renderer.

	//	Renderer.

	renderer = Renderer::GetInstance();

	renderer->AddToQueue(gameObjects[0]);

	//	Renderer.

	return true;
}

bool Application::Update()
{
	currentTime = (float)glfwGetTime();
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	frameCounter++;

	secondTimer += deltaTime;

	if (secondTimer >= 1.0f)
	{
		secondTimer -= 1;

		std::cout << "FPS: " << frameCounter << std::endl;
		
		frameCounter = 0;
	}

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		//Game Logic and update code here.
		//Also our render code.

		/*
		sun->Update(deltaTime);

		sunMoon->Update(deltaTime);

		earth->Update(deltaTime);

		earthMoon->Update(deltaTime);

		sunMoonMoon->Update(deltaTime);
		
		*/


		if (glfwGetKey(window, GLFW_KEY_ENTER))
		{
			shaderManager->LoadFromFile();

			shaderManager->LoadShaders();
		}

		renderer->Update(deltaTime);

		myCamera->Update(deltaTime);
		
		for (unsigned int i = 0; i < objectCounter; i++)
		{
			gameObjects[i]->Update(deltaTime);
		}

		textureManager->Update(currentTime);

		vertexLoader->Update(deltaTime);

		return true;
	}

	return false;
}

void Application::Draw()
{
	//Enables the depth biffer.

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Gizmos::draw(myCamera2->GetProjectionView());
	//Gizmos::clear();

	//	Texture Renderer.
	//textureLoader->Draw();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	renderer->Draw();

	for (unsigned int i = 0; i < objectCounter; i++)
	{
		gameObjects[i]->Draw();
	}

	vertexLoader->Draw();

	/*
	Gizmos::addTransform(glm::mat4(1));

	Gizmos::addSphere(sun->GetPosition(), sun->GetSize(), sun->GetRows(), sun->GetColumns(), sun->GetColour(), &sun->GetMatrix());
	Gizmos::addSphere(sunMoon->GetPosition(), sunMoon->GetSize(), sunMoon->GetRows(), sunMoon->GetColumns(), sunMoon->GetColour(), &sunMoon->GetMatrix());
	Gizmos::addSphere(earth->GetPosition(), earth->GetSize(), earth->GetRows(), earth->GetColumns(), earth->GetColour(), &earth->GetMatrix());
	Gizmos::addSphere(earthMoon->GetPosition(), earthMoon->GetSize(), earthMoon->GetRows(), earthMoon->GetColumns(), earthMoon->GetColour(), &earthMoon->GetMatrix());
	Gizmos::addSphere(sunMoonMoon->GetPosition(), sunMoonMoon->GetSize(), sunMoonMoon->GetRows(), sunMoonMoon->GetColumns(), sunMoonMoon->GetColour(), &sunMoonMoon->GetMatrix());
	*/

	glm::vec4 white(0);
	glm::vec4 black(1, 0, 1, 1);

	for (int i = 0; i < 21; i++)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
			glm::vec3(-10 + i, 0, -10),
			i == 10 ? white : black);

		Gizmos::addLine(glm::vec3(10, 0, -10 + i),
			glm::vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	//if (PLAYERS == 1)
	//{
	//	if (splitScreenOrientation == 0)
	//	{
	//		glViewport(0, 0, SCREEN_WIDTH / PLAYERS, SCREEN_HEIGHT);
	//	}
	//	else
	//	{
	//		glViewport(0, SCREEN_HEIGHT - (SCREEN_HEIGHT / PLAYERS), SCREEN_WIDTH, SCREEN_HEIGHT / PLAYERS);
	//	}
	//}
	//else
	//	if (PLAYERS > 1)
	//{
	//	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//}

	Gizmos::draw(myCamera->GetProjectionView());

	/*if (PLAYERS == 1)
	{
		if (splitScreenOrientation == 0)
		{
			glViewport(0 + (SCREEN_WIDTH / PLAYERS), 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
		else
		{
			glViewport(0, SCREEN_HEIGHT - ((SCREEN_HEIGHT / PLAYERS) * 2), SCREEN_WIDTH, SCREEN_HEIGHT / PLAYERS);
		}
	}
	else
		if (PLAYERS > 1)
	{
		glViewport(SCREEN_WIDTH - (SCREEN_WIDTH / 10), 0, SCREEN_WIDTH - (SCREEN_WIDTH / 10), SCREEN_HEIGHT - (SCREEN_HEIGHT / 10));
	}*/

	
	glfwSwapBuffers(window);

	glfwPollEvents();
}
