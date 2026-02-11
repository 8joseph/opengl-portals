#pragma once
#include "Drawable.h"
class Portal : public Drawable
{
	public:
		Portal(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 startRotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 startScale = glm::vec3(1.0f, 1.0f, 1.0f));
		void setLinkedPortal(Portal* portal);
		void draw(Shader& shader);
	private:
		Portal *linkedPortal;
		unsigned int VAO, VBO, EBO;
};

