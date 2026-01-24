//a lot of code in this file is copied from learnopengl.com (as of january)
#pragma once
#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "ShaderClass.h"


struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
	public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> aVertices, std::vector<unsigned int> aIndices, std::vector<Texture> textures);
        void draw(Shader& shader);

    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh();
};

