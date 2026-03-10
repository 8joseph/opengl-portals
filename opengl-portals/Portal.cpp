#include "Portal.h"
#include <iostream>

const static GLfloat vertices[] = 
			{ -0.3, -0.5, 0,
			  -0.3,  0.5, 0,
			   0.3,  0.5, 0,
			   0.3, -0.5, 0 };		

static GLubyte indices[] = 
			{0,1,2,
			 0,2,3 };

Portal::Portal(glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale):Drawable(aPosition, aRotation, aScale)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glBindVertexArray(0);
	linkedPortal = NULL;

}

void Portal::setLinkedPortal(Portal* portal)
{
	linkedPortal = portal;
}

Portal* Portal::getLinkedPortal()
{
	//if there is no linked portal, return self so there is no crash and at least something is rendered
	if (linkedPortal)
	{
		return linkedPortal;
	}
	else {
		std::cout << "no linked portal";
		return this;
	}
	
}

void Portal::draw(Shader& shader)

{

	glm::mat4 model = getModelMatrix();
	shader.use();
	shader.setMat4("model", model);


	glBindVertexArray(VAO);


	//wireframe mode (just for debugging atm)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0);

}

