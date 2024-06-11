#include "EnemyCube.h"
#include <glad/glad.h>
#include "iostream"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Game.h"
#include "Bullet.h"
#include <vector>

glm::vec3 cubePositions[] = {
	glm::vec3(-10.0f, -0.5f, -10.0f),
	glm::vec3(20.0f,  -0.5f, -15.0f),
	glm::vec3(-10.5f, -0.5f, -20.5f),
	glm::vec3(-30.8f, -0.5f, -12.3f),
	glm::vec3(20.4f,  -0.5f, -30.5f),
	glm::vec3(-10.7f, -0.5f, -70.5f),
	glm::vec3(10.3f,  -0.5f, -20.5f),
	glm::vec3(10.5f,  -0.5f, -20.5f),
	glm::vec3(10.5f,  -0.5f, -10.5f),
	glm::vec3(-10.3f, -0.5f, -10.5f)
};


EnemyCube::EnemyCube(std::vector<std::string> fileName, int enemyIDInput, unsigned int textureId)
{

	enemyID = enemyIDInput;
	position = cubePositions[enemyID];
	//loadFiles(fileName);
	textureID = textureId;
	//initObject();
}

float cubeVertices[] = {
	
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,   1.0f, 1.0f,	//back
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,   0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,   0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,   0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,   1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,   1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,   1.0f,  1.0f, 1.0f,	//front
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,   1.0f,   0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,   1.0f,   0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,   1.0f,   0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,   1.0f,   1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,   1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,   0.0f,  1.0f, 0.0f,	//left
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,	 0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,	 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,	 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,	 0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,	 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,   0.0f,  0.0f, 0.0f,		//right
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,	 0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,	 0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,	 0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,	 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,	 0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,   0.0f,  0.0f, 1.0f,		//
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,	 0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,	 0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,	 0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,	 0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,	 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,   0.0f,  0.0f, 1.0f,		//
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,	 0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,	 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,	 0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,	 0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,	 0.0f,  0.0f, 1.0f,
};


unsigned int cubeTexture;
unsigned int VAOCube;

Shader* cubeShader;

glm::mat4 cubeProjection;
glm::mat4 cubeView;
glm::mat4 cubeModel;




void EnemyCube::loadFiles(std::vector<std::string> fileName)
{
	
	
	
	glGenTextures(3, textures);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName[0].c_str(), &width, &height, &nrChannels, 0);


	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load Cube texture" << std::endl;
	}
	stbi_image_free(data);


	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load(fileName[1].c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load Cube texture" << std::endl;
	}
	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE2);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load(fileName[2].c_str(), &width, &height, &nrChannels, 0);


	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load Cube texture" << std::endl;
	}
	stbi_image_free(data);

}

void EnemyCube::initObject()
{
	unsigned int VBOCube;
	glGenBuffers(1, &VBOCube);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	cubeShader = new Shader("shaders/trianglePosAdv.vs", "shaders/triangleColor.fs");
	cubeShader->use();
	//texture
	//cubeShader->setInt("texture1", 2);
	//std::cout << textureID << std::endl;
	//for (int i = 0; i < 3; i++)
	//{
	//	std::cout << textures[i] << std::endl;
	//}


	cubeShader->setInt("currentUsedLights", 0);
	cubeShader->setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
	cubeShader->setVec3("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
	cubeShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	cubeShader->setVec3("dirLight.direction", -0.0f, -1.0f, -0.3f);

	cubeShader->setVec3("material.ambient", 0.878f, 0.878f, 1.0f);
	cubeShader->setVec3("material.diffuse", 0.878f, 0.878f, 1.0f);
	cubeShader->setVec3("material.specular", 0.2f, 0.2f, 0.2f);
	cubeShader->setFloat("material.shininess", 8.0f);

	cubeShader->setVec3("colorForBullets.ambient", 1.0f, 0.235f, 0.235f);
	cubeShader->setVec3("colorForBullets.diffuse", 1.0f, 0.235f, 0.235f);
	cubeShader->setVec3("colorForBullets.specular", 1.0f, 0.235f, 0.235f);
	cubeShader->setFloat("colorForBullets.shininess",16.0f);


	cubeShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	cubeShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	cubeShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	cubeShader->setFloat("spotLight.constant", 1.0f);
	cubeShader->setFloat("spotLight.linear", 0.045f);
	cubeShader->setFloat("spotLight.quadratic", 0.0075f);

	cubeShader->setVec3("bulbMaterial.ambient", 0.0f, 1.0f, 0.0f);
	cubeShader->setVec3("bulbMaterial.diffuse", 0.0f, 1.0f, 0.0f);
	cubeShader->setVec3("bulbMaterial.specular", 0.0f, 1.0f, 0.0f);
	cubeShader->setFloat("bulbMaterial.shininess", 4.0f);

	glGenVertexArrays(1, &VAOCube);

	glBindVertexArray(VAOCube);

	//glBindBuffer(GL_ARRAY_BUFFER, VAOCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(2);
	cubeProjection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
	//std::cout << "Enemy ID: " << enemyID << " Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
}

void EnemyCube::renderObject(std::vector<Bullet> &bullets, bool flashlight, float angle, glm::vec3 currentPosition)
{
	//std::cout << "Enemy ID: " << enemyID << " Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
	glActiveTexture(GL_TEXTURE0 + textureID);
	glBindTexture(GL_TEXTURE_2D, textures[textureID]);
	cubeShader->use();
	cubeShader->setInt("texture1", textureID);
	
	cubeView = glm::lookAt(Game::getCameraPos(), Game::getCameraPos() + Game::getCameraFront(), Game::getCameraUp());
	cubeShader->setMat4("view", cubeView);
	cubeShader->setMat4("projection", cubeProjection);
	cubeShader->setVec3("lightColor", ambientLightColor);
	position = currentPosition;

	glBindVertexArray(VAOCube);

	cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(cubeModel, currentPosition);
	cubeModel = glm::rotate(cubeModel, angle, glm::vec3(0.0f, -1.0f, 0.0f));
	cubeShader->setMat4("model", cubeModel);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(cubeModel)));
	cubeShader->setVec3("lightPos", testLightPos);
	cubeShader->setMat3("normalMatrix", normalMatrix);
	cubeShader->setVec3("viewPos", Game::getCameraPos());
	cubeShader->setInt("currentUsedLights", bullets.size());
	cubeShader->setBool("flashlightOn", flashlight);

	for (int i = 0; i < bullets.size(); i++)
	{
		std::string tempConstName = "";
		tempConstName += "pointLights[";
		tempConstName += std::to_string(i);
		tempConstName += "]";
		cubeShader->setVec3(tempConstName + ".position", bullets[i].currentPosition);
		cubeShader->setFloat(tempConstName + ".constant", 1.0f);
		cubeShader->setFloat(tempConstName + ".linear", 0.7f);
		cubeShader->setFloat(tempConstName + ".quadratic", 1.8f);
		cubeShader->setVec3(tempConstName + ".ambient", 0.05f, 0.05f, 0.05f);
		cubeShader->setVec3(tempConstName + ".diffuse", 0.35f, 0.35f, 0.35f);
		cubeShader->setVec3(tempConstName + ".specular", 0.25f, 0.25f, 0.25f);
	}
	glm::vec3 bulbDiffuse(sin(glfwGetTime() *2));
	cubeShader->setVec3("lightBulb.position", -6.0f, 1.3f, -6.5f);
	cubeShader->setVec3("lightBulb.ambient", 0.1f, 0.1f, 0.1f);
	cubeShader->setVec3("lightBulb.diffuse", bulbDiffuse);
	cubeShader->setVec3("lightBulb.specular", 0.1f, 0.1f, 0.1f);
	cubeShader->setFloat("lightBulb.constant", 1.0f);
	cubeShader->setFloat("lightBulb.linear", 0.35f);
	cubeShader->setFloat("lightBulb.quadratic", 0.44f);

	cubeShader->setVec3("spotLight.position", Game::getCameraPos());
	cubeShader->setVec3("spotLight.direction", Game::getCameraFront());
	cubeShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(18.5f)));
	cubeShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(35.0f)));

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

