#include "Scene.h"


void Scene::addObject(Drawable* aObject)
{
	sceneObjects.push_back(aObject);
}

void Scene::addPortal(Portal* aPortal)
{
	scenePortals.push_back(aPortal);
}

std::vector<Portal*> Scene::getPortals()
{
	return scenePortals;
}

void Scene::draw(Shader& shader, Camera camera)
{
	skybox.draw(camera); 
	shader.use();
	for (auto* drawable : sceneObjects)
	{
		drawable->draw(shader);
	}


}

