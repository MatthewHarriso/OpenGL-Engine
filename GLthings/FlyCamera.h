#pragma once

#include "Camera.h"

#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

class FlyCamera : public Camera
{
public:

	FlyCamera();
	~FlyCamera();

	void Update(float l_deltaTime);

	void SetSpeed(float l_speed);

private:

	bool mouseRightClick;

	double newMouse_x;
	double newMouse_y;

	float speed;

	glm::vec3 up;
};

