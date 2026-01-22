#pragma once
#include <glm/glm.hpp>

class Camera {
public:
	Camera(glm::vec3 alocation, glm::vec3 aUp = glm::vec3(0.0f, 1.0f, 0.0f), float aYaw = -90.0f, float aPitch = 0.0f);
	glm::mat4 getViewMatrix();
	void updatePitchAndYaw(float aPitch, float aYaw);
	glm::vec3 getLocation();
	void setLocation(glm::vec3 newLocation);
	glm::vec3 getUp();
	glm::vec3 getFront();
private:
	glm::vec3 location;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;

	float yaw;
	float pitch;
};