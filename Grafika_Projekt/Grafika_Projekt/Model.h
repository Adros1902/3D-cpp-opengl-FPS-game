#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
    Model(std::string path);
    void draw(Shader& shader, std::vector<Bullet>& bullets, bool flashlight);
    glm::vec3 position;
    float sizeX, sizeY, sizeZ;
private:
    std::vector<Texture> textures_loaded;
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

