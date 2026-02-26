#pragma once
#include <glm/glm.hpp>
#include "Object3D.h"

class Camera : public Object3D {
public:
	Camera(glm::vec3 startPos, glm::vec3 aUp = glm::vec3(0.0f, 1.0f, 0.0f), float aYaw = -90.0f, float aPitch = 0.0f);
	glm::mat4 getViewMatrix();
	void updatePitchAndYaw(float aPitch, float aYaw);
	void setPitchAndYaw(float aPitch, float aYaw);
	void setPosition(glm::vec3 newLocation);
	glm::vec3 getUp();
	glm::vec3 getFront();

private:
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;
};