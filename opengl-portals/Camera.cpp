#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 startPos, glm::vec3 aUp, float aYaw, float aPitch)
	: Object3D(startPos, glm::vec3(aPitch, aYaw, 0.0f), glm::vec3(1.0f))
{
	position = startPos;
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = aUp;

	rotation.y = aYaw;
	rotation.x = aPitch;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + cameraFront, cameraUp);
}


//maybe change this in the future: have a getPitchAndYaw and setPitchAndYaw, it seems kind of unintuiative to update? idk
void Camera::updatePitchAndYaw(float aPitch, float aYaw)
{
	rotation.x += aPitch;
	rotation.y += aYaw;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (rotation.x > 89.0f)
		rotation.x = 89.0f;
	if (rotation.x < -89.0f)
		rotation.x = -89.0f;
	
	glm::vec3 front;
	front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	front.y = sin(glm::radians(rotation.x));
	front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	cameraFront = glm::normalize(front);
}

void Camera::setPitchAndYaw(float aPitch, float aYaw)
{
	rotation.x = aPitch;
	rotation.y = aYaw;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (rotation.x > 89.0f)
		rotation.x = 89.0f;
	if (rotation.x < -89.0f)
		rotation.x = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	front.y = sin(glm::radians(rotation.x));
	front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	cameraFront = glm::normalize(front);
}

void Camera::setPosition(glm::vec3 newPos)
{
	position = newPos;
}

glm::vec3 Camera::getUp()
{
	return cameraUp;
}

glm::vec3 Camera::getFront()
{
	return cameraFront;
}