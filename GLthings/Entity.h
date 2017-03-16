#pragma once

#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

class Entity
{
public:

	Entity();
	~Entity();

	void Update(float l_deltaTime);

	void SetParent(Entity* l_entity);

	void SetTransform(glm::vec3 l_transform);

	void SetRotateSpeed(float l_rotationSpeed);
	void SetOrbitDistance(float l_orbitDistance);
	void SetOrbitSpeed(float l_rotationSpeed);

	void SetColour(glm::vec4 l_colour);

	void SetDetails(glm::vec3 l_details);

	glm::vec3 GetPosition();
	
	glm::mat4 GetMatrix();
	glm::vec4 GetColour();

	float GetSize();

	int GetRows();
	int GetColumns();

private:

	Entity* parent;

	glm::mat4 transform;
	glm::mat4 localTransform;

	glm::mat4 rotationSpeed;
	glm::mat4 orbitSpeed;

	glm::vec4 colour;

	//
	//	This contains the size in x, the rows in y and columns in z.
	//
	glm::vec3 entityDetail;
};

