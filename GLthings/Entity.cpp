#include "Entity.h"

Entity::Entity()
{
	parent = nullptr;
}

Entity::~Entity()
{

}

void Entity::Update(float l_deltaTime)
{
	localTransform = localTransform * rotationSpeed;
	localTransform = orbitSpeed * localTransform;

	if (parent != nullptr)
	{
		transform = parent->GetMatrix() * localTransform;
	}
	else
	{
		transform = localTransform;
	}
}

void Entity::SetParent(Entity* l_entity)
{
	if (l_entity != nullptr)
	{
		parent = l_entity;
	}
}

void Entity::SetTransform(glm::vec3 l_transform)
{

}

void Entity::SetRotateSpeed(float l_rotationSpeed)
{
	rotationSpeed = glm::rotate(l_rotationSpeed, glm::vec3(0, 1, 0));
}

void Entity::SetOrbitDistance(float l_orbitDistance)
{
	localTransform[3][0] = l_orbitDistance;
	localTransform[3][2] = l_orbitDistance;
}

void Entity::SetOrbitSpeed(float l_rotationSpeed)
{
	orbitSpeed = glm::rotate(l_rotationSpeed, glm::vec3(0, 1, 0));
}

void Entity::SetColour(glm::vec4 l_colour)
{
	colour = l_colour;
}

void Entity::SetDetails(glm::vec3 l_details)
{
	entityDetail = l_details;
}

glm::vec3 Entity::GetPosition()
{
	return glm::vec3(transform[3][0], transform[3][1], transform[3][2]);
}

glm::mat4 Entity::GetMatrix()
{
	return transform;
}

glm::vec4 Entity::GetColour()
{
	return colour;
}

float Entity::GetSize()
{
	return entityDetail[0];
}
//I am Matt and I am a poopy butt hole

int Entity::GetRows()
{
	return (int)entityDetail[1];
}

int Entity::GetColumns()
{
	return (int)entityDetail[2];
}
