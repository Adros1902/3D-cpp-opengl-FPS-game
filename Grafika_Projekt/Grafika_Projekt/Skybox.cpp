#include "Skybox.h"
#include <vector>
#include <string>
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MainWindow.h"
#include "Game.h"

float skyboxVertices[] = {
   -1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f,  1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f,  1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f,  1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,

   -1.0f, -1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, -1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
    1.0f,  1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
    1.0f,  1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f,  1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f, -1.0f,  1.0f, 1.0f, 0.94f, 0.79f,

   -1.0f,  1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f,  1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f, -1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f,  1.0f,  1.0f, 1.0f, 0.94f, 0.79f,

    1.0f,  1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
    1.0f,  1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, -1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
    1.0f,  1.0f,  1.0f, 1.0f, 0.94f, 0.79f,

   -1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, -1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, -1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f, -1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f, -1.0f, -1.0f, 1.0f, 0.94f, 0.79f,

   -1.0f, 1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, 1.0f, -1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, 1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
    1.0f, 1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f, 1.0f,  1.0f, 1.0f, 0.94f, 0.79f,
   -1.0f, 1.0f, -1.0f, 1.0f, 0.94f, 0.79f,      
};

std::vector<std::string> faces
{
    "textures/skybox/posx.jpg",
    "textures/skybox/negx.jpg",
    "textures/skybox/posy.jpg",
    "textures/skybox/negy.jpg",
    "textures/skybox/posz.jpg",
    "textures/skybox/negz.jpg"
};



unsigned int skyboxTextureID;
unsigned int VAOSkybox;
Shader* skyboxShader;

glm::mat4 projection1;
glm::mat4 view1;

Skybox::Skybox()
{
    loadFiles(faces);
    initObject();
}

void Skybox::loadFiles(std::vector<std::string> faces)
{
    glGenTextures(1, &skyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::initObject()
{
    unsigned int VBOSkybox;
    glGenBuffers(1, &VBOSkybox);

    glBindBuffer(GL_ARRAY_BUFFER, VBOSkybox);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    skyboxShader = new Shader("shaders/skybox.vs", "shaders/skybox.fs");

    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

    skyboxShader->use();
    //terrainShader->setInt("texture1", 0);

    glGenVertexArrays(1, &VAOSkybox);

    glBindVertexArray(VAOSkybox);

    glBindBuffer(GL_ARRAY_BUFFER, VAOSkybox);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);

    projection1 = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    skyboxShader->setMat4("projection", projection1);
}

void Skybox::renderObject()
{
    glDepthMask(GL_FALSE);
    skyboxShader->use();
    view1 = glm::lookAt(Game::getCameraPos(), Game::getCameraPos() + Game::getCameraFront(), Game::getCameraUp());
    view1 = glm::mat4(glm::mat3(view1));
    skyboxShader->setMat4("view", view1);
    skyboxShader->setMat4("projection", projection1);
    skyboxShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    glBindVertexArray(VAOSkybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}
