#pragma once
#include <glm/glm.hpp>>
#include <string>
#include <vector>
#include "Shader.h"
#include <assimp/types.h>
#include "Bullet.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
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
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(Shader& shader, std::vector<Bullet>& bullets, bool flashlight);
private:
    //  render data
    unsigned int VAO, VBO, EBO;
    glm::mat4 modelProjection;
    glm::mat4 modelView;
    glm::mat4 modelModel;
    glm::vec3 positionModel;

    glm::vec3 position;

    glm::vec3 startPosition;

    void setupMesh();
};

