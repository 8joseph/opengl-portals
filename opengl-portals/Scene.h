#pragma once
#include <iostream>
#include "Drawable.h"
#include <vector>



class Scene
{
	public:
		void addObject(Drawable* object);
		void draw(Shader& shader);
	private:
		std::vector<Drawable*> sceneObjects;
};

