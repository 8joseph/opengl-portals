#pragma once
#include <iostream>
#include "Drawable.h"
#include <vector>
#include "Portal.h"



class Scene
{
	public:
		void addObject(Drawable* object);
		void addPortal(Portal* portal);
		std::vector<Portal*> getPortals();
		void draw(Shader& shader);
	private:
		std::vector<Drawable*> sceneObjects;
		std::vector<Portal*> scenePortals;
};

