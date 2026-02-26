#pragma once
#include <iostream>
#include "Drawable.h"
#include <vector>
#include "Portal.h"
#include "stb_image.h"
#include "Skybox.h"



class Scene
{
	public:
		void addObject(Drawable* object);
		void addPortal(Portal* portal);
		std::vector<Portal*> getPortals();
		void draw(Shader& shader, Camera camera);
	private:
		std::vector<Drawable*> sceneObjects;
		std::vector<Portal*> scenePortals;
		Skybox skybox;
};

