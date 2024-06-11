#include "Terrain.h"
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MainWindow.h"
#include "Game.h"
#include "Bullet.h"
#include <vector>
#include <GLFW/glfw3.h>

float terrainVertices[] = {
   -100.0f, -1.0f, -100.0f,  0.0f, 1.0f, 0.0f,  00.0f, 00.0f,
	100.0f, -1.0f, -100.0f,  0.0f, 1.0f, 0.0f,  20.0f, 00.0f,
	100.0f, -1.0f,  100.0f,  0.0f, 1.0f, 0.0f,  00.0f, 20.0f,
	100.0f, -1.0f,  100.0f,  0.0f, 1.0f, 0.0f,  00.0f, 20.0f,
	-100.0f, -1.0f,  100.0f, 0.0f, 1.0f, 0.0f,  20.0f, 20.0f,
	-100.0f, -1.0f, -100.0f, 0.0f, 1.0f, 0.0f,  20.0f, 00.0f,

};

int width, height, nrChannels;
unsigned char* data;
unsigned int textureTerrain;
unsigned int VAOterrain;
Shader* terrainShader;

//camera etc.
glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;

Terrain::Terrain()
{
	loadFiles();
	initObject();
}

void Terrain::loadFiles()
{
	glGenTextures(1, &textureTerrain);
	glBindTexture(GL_TEXTURE_2D, textureTerrain);
	//glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("sand_01_diff_4k.jpg", &width, &height, &nrChannels, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrain);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
}

void Terrain::initObject()
{
	unsigned int VBOTerrain;
	glGenBuffers(1, &VBOTerrain);

	glBindBuffer(GL_ARRAY_BUFFER, VBOTerrain);

	glBufferData(GL_ARRAY_BUFFER, sizeof(terrainVertices), terrainVertices, GL_STATIC_DRAW);

	terrainShader = new Shader("shaders/trianglePosAdv.vs", "shaders/triangleColor.fs");
	terrainShader->use();
	terrainShader->setInt("currentUsedLights", 0);
	terrainShader->setVec3("dirLight.ambient", 0.06f, 0.06f, 0.06f);
	terrainShader->setVec3("dirLight.diffuse", 0.08f, 0.08f, 0.08f);
	terrainShader->setVec3("dirLight.specular", 0.1f, 0.1f, 0.1f);
	terrainShader->setVec3("dirLight.direction", -0.0f, -1.0f, -0.3f);

	terrainShader->setVec3("material.ambient", 0.878f, 0.878f, 1.0f);
	terrainShader->setVec3("material.diffuse", 0.878f, 0.878f, 1.0f);
	terrainShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	terrainShader->setFloat("material.shininess", 32.0f);

	terrainShader->setVec3("colorForBullets.ambient", 1.0f, 0.235f, 0.235f);
	terrainShader->setVec3("colorForBullets.diffuse", 1.0f, 0.235f, 0.235f);
	terrainShader->setVec3("colorForBullets.specular", 1.0f, 0.235f, 0.235f);
	terrainShader->setFloat("colorForBullets.shininess", 16.0f);

	terrainShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	terrainShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	terrainShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	terrainShader->setFloat("spotLight.constant", 1.0f);
	terrainShader->setFloat("spotLight.linear", 0.045f);
	terrainShader->setFloat("spotLight.quadratic", 0.0075f);



	terrainShader->setVec3("bulbMaterial.ambient", 0.0f, 1.0f, 0.0f);
	terrainShader->setVec3("bulbMaterial.diffuse", 0.0f, 1.0f, 0.0f);
	terrainShader->setVec3("bulbMaterial.specular", 0.0f, 1.0f, 0.0f);
	terrainShader->setFloat("bulbMaterial.shininess", 4.0f);

	glGenVertexArrays(1, &VAOterrain);

	glBindVertexArray(VAOterrain);

	glBufferData(GL_ARRAY_BUFFER, sizeof(terrainVertices), terrainVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(2);

	projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
}

void Terrain::renderObject(std::vector<Bullet> bullets, bool flashlight)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrain);
	terrainShader->use();
	view = glm::lookAt(Game::getCameraPos(), Game::getCameraPos() + Game::getCameraFront(), Game::getCameraUp());
	terrainShader->setMat4("view", view);
	terrainShader->setMat4("projection", projection);
	terrainShader->setVec3("lightColor", ambientLightColor);
	terrainShader->setVec3("lightPos", testLightPos);

	glBindVertexArray(VAOterrain);

	model = glm::mat4(1.0f);
	terrainShader->setMat4("model", model);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
	
	terrainShader->setMat3("normalMatrix", normalMatrix);
	terrainShader->setVec3("viewPos", Game::getCameraPos());

	terrainShader->setInt("currentUsedLights", bullets.size());

	for (int i = 0; i < bullets.size(); i++)
	{
		std::string tempConstName = "";
		tempConstName += "pointLights[";
		tempConstName += std::to_string(i);
		tempConstName += "]";
		terrainShader->setVec3(tempConstName + ".position", bullets[i].currentPosition);
		terrainShader->setFloat(tempConstName + ".constant", 1.0f);
		terrainShader->setFloat(tempConstName + ".linear", 0.7f);
		terrainShader->setFloat(tempConstName + ".quadratic", 1.8f);
		terrainShader->setVec3(tempConstName + ".ambient", 0.05f, 0.05f, 0.05f);
		terrainShader->setVec3(tempConstName + ".diffuse", 0.35f, 0.35f, 0.35f);
		terrainShader->setVec3(tempConstName + ".specular", 0.25f, 0.25f, 0.25f);
	}
	terrainShader->setBool ("flashlightOn", flashlight);

	//lightbulb in building
	glm::vec3 bulbDiffuse(sin(glfwGetTime() * 2));
	terrainShader->setVec3("lightBulb.position", -6.0f, 1.3f, -6.5f);
	terrainShader->setVec3("lightBulb.ambient", 0.1f, 0.1f, 0.1f);
	terrainShader->setVec3("lightBulb.diffuse", bulbDiffuse);
	terrainShader->setVec3("lightBulb.specular", 0.1f, 0.1f, 0.1f);
	terrainShader->setFloat("lightBulb.constant", 1.0f);
	terrainShader->setFloat("lightBulb.linear", 0.35f);
	terrainShader->setFloat("lightBulb.quadratic", 0.44f);


	terrainShader->setVec3("spotLight.position", Game::getCameraPos());
	terrainShader->setVec3("spotLight.direction", Game::getCameraFront());
	terrainShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(18.5f)));
	terrainShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(35.0f)));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
