#pragma once
#include "Object3D.h"
#include <glm/glm.hpp>
#include "ShaderClass.h"

class Drawable : public Object3D
{
	public:
		Drawable(glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale) : Object3D(aPosition, aRotation, aScale) {};
		virtual void draw(Shader& shader) = 0;
};