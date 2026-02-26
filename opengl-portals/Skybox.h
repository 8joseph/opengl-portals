#pragma once
#include <glad/glad.h>
#include<iostream>
#include <vector>
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ShaderClass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class Skybox
{
	public:
		Skybox();
		void draw(Camera camera);
	private:
		Shader* skyboxShader;
		unsigned int skyboxVAO;
		unsigned int skyboxVBO;
		unsigned int cubemapTexture; 
		unsigned int loadCubemap(std::vector<std::string> faces);
		unsigned int loadTexture(char const* path);

};

