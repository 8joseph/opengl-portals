#include "Scene.h"

void Scene::addObject(Drawable* aObject)
{
	sceneObjects.push_back(aObject);
}

void Scene::draw(Shader& shader)
{
	for (auto* drawable : sceneObjects)
	{
		drawable->draw(shader);
	}
}