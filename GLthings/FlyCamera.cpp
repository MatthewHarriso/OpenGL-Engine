#include "FlyCamera.h"

FlyCamera::FlyCamera()
{
	mouseRightClick = false;

	newMouse_x = 0.1;
	newMouse_y = 0.1;

	up = glm::vec3(0, 1, 0);

	speed = 50;
}

FlyCamera::~FlyCamera()
{

}

void FlyCamera::Update(float l_deltaTime)
{
	//	input to manipulate the camera base class goes here.

	if (glfwGetKey(window, GLFW_KEY_W))
	{
		SetPosition(GetPosition() - glm::vec3(worldTransform[2]) * (l_deltaTime * speed));
	}

	if (glfwGetKey(window, GLFW_KEY_S))
	{
		SetPosition(GetPosition() + glm::vec3(worldTransform[2]) * (l_deltaTime * speed));
	}

	if (glfwGetKey(window, GLFW_KEY_A))
	{
		SetPosition(GetPosition() - glm::vec3(worldTransform[0]) * (l_deltaTime * speed));
	}

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		SetPosition(GetPosition() + glm::vec3(worldTransform[0]) * (l_deltaTime * speed));
	}

	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		SetPosition(GetPosition() + glm::vec3(worldTransform[1]) * (l_deltaTime * speed));
	}

	if (glfwGetKey(window, GLFW_KEY_E))
	{
		SetPosition(GetPosition() - glm::vec3(worldTransform[1]) * (l_deltaTime * speed));
	}
	
	double mouse_x = 0.1;
	double mouse_y = 0.1;

	glfwGetCursorPos(window, &mouse_x, &mouse_y);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		mouseRightClick = true;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		glm::vec4 up = viewTransform * glm::vec4(0, 1, 0, 0);
		glm::mat4 rotMatrix(1.0f);

		rotMatrix = glm::rotate((float)((newMouse_x -mouse_x) * (-speed * 0.25f)) * l_deltaTime, glm::vec3(up[0], up[1], up[2]));
		viewTransform = rotMatrix * viewTransform;

		rotMatrix = glm::rotate((float)((newMouse_y -mouse_y) * (-speed * 0.25f)) * l_deltaTime, glm::vec3(1, 0, 0));
		viewTransform = rotMatrix * viewTransform;

		worldTransform = glm::inverse(viewTransform);
		glfwSetCursorPos(window, newMouse_x, newMouse_y);
	}
	else
	{
		newMouse_x = mouse_x;
		newMouse_y = mouse_y;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (mouseRightClick)
		{
			glfwSetCursorPos(window, newMouse_x, newMouse_y);

			mouseRightClick = false;
		}
	}

	UpdateProjectionViewTransform();
}

void FlyCamera::SetSpeed(float l_speed)
{
	speed = l_speed;
}
