#pragma once
#include <vector>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ShaderClass.h"
#include "Mesh.h"
#include "stb_image.h"
#include "Drawable.h"

class Model : public Drawable
{
	public:
		Model(const char* path, glm::vec3 startPosition = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 startRotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 startScale = glm::vec3(1.0f, 1.0f, 1.0f));
		void draw(Shader &shader);

	private:
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<Texture> textures_loaded;

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene *scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);
};

