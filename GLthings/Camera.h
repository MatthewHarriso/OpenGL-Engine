#pragma once

#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

#include <glfw3.h>

struct GLFWwindow;

class Camera
{

public:

	Camera();
	~Camera();

	virtual void Update(float l_deltaTime) = 0;

	void SetPerspective(float l_fieldOfView, float l_aspectRatio, float l_near, float l_far);
	void SetLookAt(glm::vec3 l_from, glm::vec3 l_to, glm::vec3 l_up);
	void SetLookAt(glm::vec3 l_to);
	void SetPosition(glm::vec3 l_position);
	void SetScreen(GLFWwindow* l_window);

	glm::vec3 GetPosition();
	glm::mat4 GetWorldTransform();
	glm::mat4 GetView();
	glm::mat4 GetProjection();
	glm::mat4 GetProjectionView();

	float GetNearPlane();

protected:

	void UpdateProjectionViewTransform();

	GLFWwindow* window;

	glm::mat4 worldTransform;
	glm::mat4 viewTransform;
	glm::mat4 projectionTransform;
	glm::mat4 projectionViewTransform;

	float nearPlane;
};
