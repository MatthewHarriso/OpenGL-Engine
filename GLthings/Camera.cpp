#include "Camera.h"

Camera::Camera()
{
	window = nullptr;

	worldTransform = glm::mat4(0);

	viewTransform = glm::mat4(0);

	projectionTransform = glm::mat4(0);

	projectionViewTransform = glm::mat4(0);
}

Camera::~Camera()
{

}

void Camera::SetPerspective(float l_fieldOfView, float l_aspectRatio, float l_near, float l_far)
{
	projectionTransform = glm::perspective(l_fieldOfView, l_aspectRatio, l_near, l_far);

	UpdateProjectionViewTransform();
}

void Camera::SetLookAt(glm::vec3 l_to)
{
	viewTransform = glm::lookAt(glm::vec3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]), l_to, glm::vec3(0, 1, 0));

	worldTransform = glm::inverse(viewTransform);

	UpdateProjectionViewTransform();
}

void Camera::SetLookAt(glm::vec3 l_from, glm::vec3 l_to, glm::vec3 l_up)
{
	viewTransform = glm::lookAt(l_from, l_to, l_up);

	worldTransform = glm::inverse(viewTransform);

	UpdateProjectionViewTransform();
}

void Camera::SetPosition(glm::vec3 l_position)
{
	worldTransform[3][0] = l_position.x;
	worldTransform[3][1] = l_position.y;
	worldTransform[3][2] = l_position.z;

	viewTransform = glm::inverse(worldTransform);

	UpdateProjectionViewTransform();
}

void Camera::SetScreen(GLFWwindow * l_window)
{
	window = l_window;
}


glm::vec3 Camera::GetPosition()
{
	//return glm::vec3(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]);
	return glm::vec3(worldTransform[3]);
}

glm::mat4 Camera::GetWorldTransform()
{
	return worldTransform;
}

glm::mat4 Camera::GetView()
{
	return viewTransform;
}

glm::mat4 Camera::GetProjection()
{
	return projectionTransform;
}

glm::mat4 Camera::GetProjectionView()
{
	return projectionViewTransform;
}

void Camera::UpdateProjectionViewTransform()
{
	projectionViewTransform = projectionTransform * viewTransform;
}
