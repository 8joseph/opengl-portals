#pragma once
#include <iostream>
#include "Model.h"
#include "Camera.h"
class Scene
{
	public:
		std::vector<Model> getModels();
		void addModel(Model newModel);
		void renderScene();
	private:
		std::vector<Model> sceneModels;
};

