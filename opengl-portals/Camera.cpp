#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 aLocation, glm::vec3 aUp, float aYaw, float aPitch)
{
	location = aLocation;
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = aUp;

	yaw = aYaw;
	pitch = aPitch;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(location, location + cameraFront, cameraUp);
}


//maybe change this in the future: have a getPitchAndYaw and setPitchAndYaw, it seems kind of unintuiative to update? idk
void Camera::updatePitchAndYaw(float aPitch, float aYaw)
{
	pitch += aPitch;
	yaw += aYaw;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}

glm::vec3 Camera::getLocation()
{
	return location;
}

void Camera::setLocation(glm::vec3 newLocation)
{
	location = newLocation;
}

glm::vec3 Camera::getUp()
{
	return cameraUp;
}

glm::vec3 Camera::getFront()
{
	return cameraFront;
}