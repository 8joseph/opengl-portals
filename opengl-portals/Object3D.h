#pragma once
#include <glm/glm.hpp>

class Object3D
{
	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		Object3D(glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale);
		glm::mat4 getModelMatrix();
		void setPosition(glm::vec3 newPosition);
		void setRotation(glm::vec3 newRotation);
		void setScale(glm::vec3 newScale);
		glm::vec3 getPosition();
		glm::vec3 getRotation();
		glm::vec3 getScale();
};

