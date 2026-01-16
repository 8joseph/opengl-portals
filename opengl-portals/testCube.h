#pragma once
#include <glm/glm.hpp>
#include "ShaderClass.h"

class testCube
{
	public:
		testCube(glm::vec3 location);
		void draw(Shader shader);

	private:
		glm::vec3 location;

};

