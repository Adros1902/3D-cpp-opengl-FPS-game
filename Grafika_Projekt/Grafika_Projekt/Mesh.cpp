#include "Mesh.h"
#include "Game.h"
#include "Bullet.h"
#include <vector>
#include <glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::draw(Shader& shader, std::vector<Bullet>& bullets, bool flashlight)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
	shader.use();
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); 
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
		std::cout << "Binding texture: " << name + number << " to unit " << i << " with ID " << textures[i].id << std::endl;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
	//dodane
    std::cout << "MESH" << std::endl;
	modelView = glm::lookAt(Game::getCameraPos(), Game::getCameraPos() + Game::getCameraFront(), Game::getCameraUp());
	shader.setMat4("view", modelView);
	shader.setInt("currentUsedLights", 0);
	shader.setMat4("projection", modelProjection);
	shader.setVec3("lightColor", ambientLightColor);
	shader.setVec3("lightPos", testLightPos);

	modelModel = glm::mat4(1.0f);
	modelModel = glm::translate(modelModel, glm::vec3(-3.0f, -2.0f, 12.0f));
	modelModel = glm::rotate(modelModel, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
	modelModel = glm::rotate(modelModel, glm::radians(200.0f), glm::vec3(0.0, 0.0, 1.0));
	glm::vec3 scale(1.0f, 1.0f, 1.0f);
	modelModel = glm::scale(modelModel, scale);
	shader.setMat4("model", modelModel);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelModel)));
	shader.setMat3("normalMatrix", normalMatrix);

	shader.setBool("flashlightOn", flashlight);
	shader.setInt("currentUsedLights", bullets.size());

	for (int i = 0; i < bullets.size(); i++)
	{
		std::string tempConstName = "";
		tempConstName += "pointLights[";
		tempConstName += std::to_string(i);
		tempConstName += "]";
		shader.setVec3(tempConstName + ".position", bullets[i].currentPosition);
		shader.setFloat(tempConstName + ".constant", 1.0f);
		shader.setFloat(tempConstName + ".linear", 0.7f);
		shader.setFloat(tempConstName + ".quadratic", 1.8f);
		shader.setVec3(tempConstName + ".ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3(tempConstName + ".diffuse", 0.35f, 0.35f, 0.35f);
		shader.setVec3(tempConstName + ".specular", 0.05f, 0.05f, 0.05f);
	}
	glm::vec3 bulbDiffuse(sin(glfwGetTime() * 2));
	shader.setVec3("lightBulb.position", -6.0f, 1.3f, -6.5f);
	shader.setVec3("lightBulb.ambient", 0.1f, 0.1f, 0.1f);
	shader.setVec3("lightBulb.diffuse", bulbDiffuse);
	shader.setVec3("lightBulb.specular", 0.1f, 0.1f, 0.1f);
	shader.setFloat("lightBulb.constant", 1.0f);
	shader.setFloat("lightBulb.linear", 0.22f);
	shader.setFloat("lightBulb.quadratic", 0.20f);

	shader.setVec3("spotLight.position", Game::getCameraPos());
	shader.setVec3("spotLight.direction", Game::getCameraFront());
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(18.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(35.0f)));
    // draw mesh
    
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);

	startPosition = glm::vec3(glm::vec3(-3.0f, 1.0f, -12.0f));

    modelProjection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);

	modelModel = glm::mat4(1.0f);
	modelModel = glm::translate(modelModel, startPosition);
	glm::vec4 tempPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 worldPosition = modelModel * tempPosition;
	position = glm::vec3(worldPosition);
	position = startPosition;
}
