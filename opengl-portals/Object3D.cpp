#include "Object3D.h"
#include <glm/gtc/matrix_transform.hpp>
Object3D::Object3D(glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale)
{
	position = aPosition;
	rotation = aRotation;
	scale = aScale;
}

glm::mat4 Object3D::getModelMatrix()
{
	glm::mat4 returnModelMatrix = glm::mat4(1.0f);
	returnModelMatrix = glm::translate(returnModelMatrix, position);

	returnModelMatrix = glm::rotate(returnModelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	returnModelMatrix = glm::rotate(returnModelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	returnModelMatrix = glm::rotate(returnModelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));

	returnModelMatrix = glm::scale(returnModelMatrix, scale);

	return returnModelMatrix;
}

void Object3D::setPosition(glm::vec3 newPosition)
{
	position = newPosition;
}
void Object3D::setRotation(glm::vec3 newRotation)
{
	rotation = newRotation;
}
void Object3D::setScale(glm::vec3 newScale)
{
	scale = newScale;
}

glm::vec3 Object3D::getPosition()
{
	return position;
}

glm::vec3 Object3D::getRotation()
{
	return rotation;
}

glm::vec3 Object3D::getScale()
{
	return scale;
}