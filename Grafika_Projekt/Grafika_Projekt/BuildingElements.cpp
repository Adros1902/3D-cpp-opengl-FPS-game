#include "BuildingElements.h"
#include <glad/glad.h>
#include "iostream"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Game.h"
#include <vector>
#include "Bullet.h"



float leftWB1TexEnd = 1.0f;
float leftWB1TexStart = 0.0f;
float leftWB1TexSide = 0.02f;
float leftWB1X = 3.0f;
float leftWB1Y = 1.5f;
float leftWB1Z = 0.1f;

//elements data
std::vector<float> leftWB1 = {
	//back //vertices					//texture
	 -leftWB1X, -leftWB1Y, -leftWB1Z, 	 0.0f,  0.0f, -1.0f,   leftWB1TexEnd, leftWB1TexEnd,
	  leftWB1X, -leftWB1Y, -leftWB1Z, 	 0.0f,  0.0f, -1.0f,   leftWB1TexStart, leftWB1TexEnd,
	  leftWB1X,  leftWB1Y, -leftWB1Z, 	 0.0f,  0.0f, -1.0f,   leftWB1TexStart, leftWB1TexStart,
	  leftWB1X,  leftWB1Y, -leftWB1Z, 	 0.0f,  0.0f, -1.0f,   leftWB1TexStart, leftWB1TexStart,
	 -leftWB1X,  leftWB1Y, -leftWB1Z, 	 0.0f,  0.0f, -1.0f,   leftWB1TexEnd, leftWB1TexStart,
	 -leftWB1X, -leftWB1Y, -leftWB1Z, 	 0.0f,  0.0f, -1.0f,   leftWB1TexEnd, leftWB1TexEnd,
	 -leftWB1X, -leftWB1Y,  leftWB1Z, 	 0.0f,  0.0f,  1.0f,	leftWB1TexEnd, leftWB1TexEnd,			 //front
	  leftWB1X, -leftWB1Y,  leftWB1Z, 	 0.0f,  0.0f,  1.0f,   leftWB1TexStart, leftWB1TexEnd,
	  leftWB1X,  leftWB1Y,  leftWB1Z, 	 0.0f,  0.0f,  1.0f,   leftWB1TexStart, leftWB1TexStart,
	  leftWB1X,  leftWB1Y,  leftWB1Z, 	 0.0f,  0.0f,  1.0f,   leftWB1TexStart, leftWB1TexStart,
	 -leftWB1X,  leftWB1Y,  leftWB1Z, 	 0.0f,  0.0f,  1.0f,   leftWB1TexEnd, leftWB1TexStart,
	 -leftWB1X, -leftWB1Y,  leftWB1Z, 	 0.0f,  0.0f,  1.0f,   leftWB1TexEnd, leftWB1TexEnd,
	 -leftWB1X,  leftWB1Y,  leftWB1Z, 	-1.0f,  0.0f,  0.0f,	 leftWB1TexSide, leftWB1TexStart,				 //left
	 -leftWB1X, -leftWB1Y,  leftWB1Z, 	-1.0f,  0.0f,  0.0f,   leftWB1TexSide, leftWB1TexEnd,
	 -leftWB1X, -leftWB1Y, -leftWB1Z, 	-1.0f,  0.0f,  0.0f,   leftWB1TexStart, leftWB1TexEnd,
	 -leftWB1X, -leftWB1Y, -leftWB1Z, 	-1.0f,  0.0f,  0.0f,   leftWB1TexStart, leftWB1TexEnd,
	 -leftWB1X,  leftWB1Y, -leftWB1Z, 	-1.0f,  0.0f,  0.0f,   leftWB1TexStart, leftWB1TexStart,
	 -leftWB1X,  leftWB1Y,  leftWB1Z, 	-1.0f,  0.0f,  0.0f,   leftWB1TexSide, leftWB1TexStart,
	  leftWB1X,  leftWB1Y,  leftWB1Z, 	 1.0f,  0.0f,  0.0f,	 leftWB1TexStart, leftWB1TexStart,		 //right
	  leftWB1X,  leftWB1Y, -leftWB1Z, 	 1.0f,  0.0f,  0.0f,   leftWB1TexSide, leftWB1TexStart,
	  leftWB1X, -leftWB1Y, -leftWB1Z, 	 1.0f,  0.0f,  0.0f,   leftWB1TexSide, leftWB1TexEnd,
	  leftWB1X, -leftWB1Y, -leftWB1Z, 	 1.0f,  0.0f,  0.0f,   leftWB1TexSide, leftWB1TexEnd,
	  leftWB1X, -leftWB1Y,  leftWB1Z, 	 1.0f,  0.0f,  0.0f,   leftWB1TexStart, leftWB1TexEnd,
	  leftWB1X,  leftWB1Y,  leftWB1Z,  	 1.0f,  0.0f,  0.0f,   leftWB1TexStart, leftWB1TexStart,
	 -leftWB1X, -leftWB1Y, -leftWB1Z, 	 0.0f, -1.0f,  0.0f,	 leftWB1TexStart, leftWB1TexEnd,			//top
	  leftWB1X, -leftWB1Y, -leftWB1Z, 	 0.0f, -1.0f,  0.0f,   leftWB1TexSide, leftWB1TexEnd,
	  leftWB1X, -leftWB1Y,  leftWB1Z, 	 0.0f, -1.0f,  0.0f,   leftWB1TexSide, leftWB1TexStart,
	  leftWB1X, -leftWB1Y,  leftWB1Z, 	 0.0f, -1.0f,  0.0f,   leftWB1TexSide, leftWB1TexStart,
	 -leftWB1X, -leftWB1Y,  leftWB1Z, 	 0.0f, -1.0f,  0.0f,   leftWB1TexStart, leftWB1TexStart,
	 -leftWB1X, -leftWB1Y, -leftWB1Z, 	 0.0f, -1.0f,  0.0f,   leftWB1TexStart, leftWB1TexEnd,
	 -leftWB1X,  leftWB1Y, -leftWB1Z, 	 0.0f,  1.0f,  0.0f,	 leftWB1TexStart, leftWB1TexEnd,			 //bottom
	  leftWB1X,  leftWB1Y, -leftWB1Z, 	 0.0f,  1.0f,  0.0f,   leftWB1TexSide, leftWB1TexEnd,
	  leftWB1X,  leftWB1Y,  leftWB1Z, 	 0.0f,  1.0f,  0.0f,   leftWB1TexSide, leftWB1TexStart,
	  leftWB1X,  leftWB1Y,  leftWB1Z, 	 0.0f,  1.0f,  0.0f,   leftWB1TexSide, leftWB1TexStart,
	 -leftWB1X,  leftWB1Y,  leftWB1Z, 	 0.0f,  1.0f,  0.0f,   leftWB1TexStart, leftWB1TexStart,
	 -leftWB1X,  leftWB1Y, -leftWB1Z, 	 0.0f,  1.0f,  0.0f,   leftWB1TexStart, leftWB1TexEnd,
};

std::vector<float> roof = {
   -10.0f, -1.0f, -10.0f,  0.0f, -1.0f, 0.0f,  00.0f, 00.0f,
	10.0f, -1.0f, -10.0f,  0.0f, -1.0f, 0.0f,  5.0f, 00.0f,
	10.0f, -1.0f,  10.0f,  0.0f, -1.0f, 0.0f,  00.0f, 5.0f,
	10.0f, -1.0f,  10.0f,  0.0f, -1.0f, 0.0f,  00.0f, 5.0f,
   -10.0f, -1.0f,  10.0f,  0.0f, -1.0f, 0.0f,  5.0f, 5.0f,
   -10.0f, -1.0f, -10.0f,  0.0f, -1.0f, 0.0f,  5.0f, 00.0f,
};


int ID;

BuildingElements::BuildingElements(std::vector<std::string> fileName , glm::vec3 position)
{
	startPosition = position;
	isRotable = false;
	loadFiles(fileName);
	initObject(leftWB1);
	
	
}

BuildingElements::BuildingElements(std::vector<std::string> fileName, glm::vec3 position, float anglesIn, glm::vec3 rotationIn)
{
	startPosition = position;
	isRotable = true;
	angles = anglesIn;
	rotation = rotationIn;
	loadFiles(fileName);
	initObject(leftWB1);
	
	
}

BuildingElements::BuildingElements(std::vector<std::string> fileName, glm::vec3 position, float anglesIn, glm::vec3 rotationIn, float roof)
{
	isRoof = roof;
	startPosition = position;
	isRotable = true;
	angles = anglesIn;
	rotation = rotationIn;
	loadFiles(fileName);
	initObject(leftWB1);


}

void BuildingElements::loadFiles(std::vector<std::string> fileName)
{
	glGenTextures(2, textures);

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

}

void BuildingElements::changeModelRot(float radians, glm::vec3 axis) {
	elModel = glm::mat4(1.0f);
	elModel = glm::translate(elModel, startPosition);
	elModel = glm::rotate(elModel, glm::radians(radians), axis);
	//tempModel = elModel;

	elShader->setMat4("model",elModel);
}

void BuildingElements::initObject(std::vector<float> vertices)
{
	
	glGenBuffers(1, &elVBO);
	glBindBuffer(GL_ARRAY_BUFFER, elVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	elShader = new Shader("shaders/trianglePosAdv.vs", "shaders/triangleColor.fs");
	elShader->use();
	elShader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	elShader->setVec3("light.diffuse", 0.3f, 0.3f, 0.3f);
	elShader->setVec3("light.specular", 0.5f, 0.5f, 0.5f);

	elShader->setVec3("dirLight.ambient", 0.15f, 0.15f, 0.15f);
	elShader->setVec3("dirLight.diffuse", 0.25f, 0.25f, 0.25f);
	elShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	elShader->setVec3("dirLight.direction", -0.0f, -1.0f, -0.3f);

	elShader->setVec3("material.ambient", 0.878f, 0.878f, 1.0f);
	elShader->setVec3("material.diffuse", 0.878f, 0.878f, 1.0f);
	elShader->setVec3("material.specular", 0.2f, 0.2f, 0.2f);
	elShader->setFloat("material.shininess", 8.0f);

	elShader->setVec3("colorForBullets.ambient", 1.0f, 0.235f, 0.235f);
	elShader->setVec3("colorForBullets.diffuse", 1.0f, 0.235f, 0.235f);
	elShader->setVec3("colorForBullets.specular", 1.0f, 0.235f, 0.235f);
	elShader->setFloat("colorForBullets.shininess", 2.0f);
	
	elShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	elShader->setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
	elShader->setVec3("spotLight.specular", 0.3f, 0.3f, 0.3f);
	elShader->setFloat("spotLight.constant", 1.0f);
	elShader->setFloat("spotLight.linear", 0.045f);
	elShader->setFloat("spotLight.quadratic", 0.0075f);

	elShader->setVec3("bulbMaterial.ambient", 0.0f, 1.0f, 0.0f);
	elShader->setVec3("bulbMaterial.diffuse", 0.0f, 1.0f, 0.0f);
	elShader->setVec3("bulbMaterial.specular", 0.0f, 1.0f, 0.0f);
	elShader->setFloat("bulbMaterial.shininess", 4.0f);
	

	glGenVertexArrays(1, &elVAO);
	glBindVertexArray(elVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(2);
	
	elProjection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);
	


	if(isRotable && !isRoof){
		sizeX = leftWB1Z;
		sizeY = leftWB1Y;
		sizeZ = leftWB1X;
		
		elModel = glm::mat4(1.0f);
		elModel = glm::translate(elModel, startPosition);
		elModel = glm::rotate(elModel, glm::radians(angles), rotation);
		glm::vec4 tempPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 worldPosition = elModel * tempPosition;
		position = glm::vec3(worldPosition);
		//std::cout << "world pos:" << position.x << position.y << position.z << std::endl;

	}
	else if (isRoof) {
		sizeX = leftWB1X;
		sizeY = leftWB1Z;
		sizeZ = leftWB1Y;
		elModel = glm::mat4(1.0f);
		elModel = glm::translate(elModel, startPosition);
		elModel = glm::rotate(elModel, glm::radians(angles), rotation);
		glm::vec4 tempPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 worldPosition = elModel * tempPosition;
		position = glm::vec3(worldPosition);
	}
	else
	{
		sizeX = leftWB1X;
		sizeY = leftWB1Y;
		sizeZ = leftWB1Z;
		elModel = glm::mat4(1.0f);
		elModel = glm::translate(elModel, startPosition);
		glm::vec4 tempPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 worldPosition = elModel * tempPosition;
		position = glm::vec3(worldPosition);
		position = startPosition;
		//std::cout << "second pos: " << position.x << position.y << position.z << std::endl;
	}
	
}

void BuildingElements::renderObject(std::vector<Bullet> & bullets, bool flashlight)
{
	if (isRoof) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		elShader->use();
		elShader->setInt("texture1", 1);
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		elShader->use();
		elShader->setInt("texture1", 0);
	}

	
	elView = glm::lookAt(Game::getCameraPos(), Game::getCameraPos() + Game::getCameraFront(), Game::getCameraUp());
	elShader->setMat4("view", elView);
	elShader->setInt("currentUsedLights", 0);
	elShader->setMat4("projection", elProjection);
	elShader->setVec3("lightColor", ambientLightColor);
	elShader->setVec3("lightPos", testLightPos);


	glBindVertexArray(elVAO);
	
	
	if (isRotable == false) {
		elModel = glm::mat4(1.0f);
		elModel = glm::translate(elModel, startPosition); 
		elShader->setMat4("model", elModel);
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(elModel)));
		elShader->setMat3("normalMatrix", normalMatrix);
		
	}
	else {
		elModel = glm::mat4(1.0f);
		elModel = glm::translate(elModel, startPosition);
		elModel = glm::rotate(elModel, glm::radians(angles), rotation);
		elShader->setMat4("model", elModel);
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(elModel)));
		elShader->setMat3("normalMatrix", normalMatrix);
	}
	elShader->setBool("flashlightOn", flashlight);
	elShader->setInt("currentUsedLights", bullets.size());

	for (int i = 0; i < bullets.size(); i++)
	{
		std::string tempConstName = "";
		tempConstName += "pointLights[";
		tempConstName += std::to_string(i);
		tempConstName += "]";
		elShader->setVec3(tempConstName + ".position", bullets[i].currentPosition);
		elShader->setFloat(tempConstName + ".constant", 1.0f);
		elShader->setFloat(tempConstName + ".linear", 0.7f);
		elShader->setFloat(tempConstName + ".quadratic", 1.8f);
		elShader->setVec3(tempConstName + ".ambient", 0.05f, 0.05f, 0.05f);
		elShader->setVec3(tempConstName + ".diffuse", 0.35f, 0.35f, 0.35f);
		elShader->setVec3(tempConstName + ".specular", 0.05f, 0.05f, 0.05f);
	}
	glm::vec3 bulbDiffuse(sin(glfwGetTime() * 2));
	elShader->setVec3("lightBulb.position", -6.0f, 1.3f, -6.5f);
	elShader->setVec3("lightBulb.ambient", 0.1f, 0.1f, 0.1f);
	elShader->setVec3("lightBulb.diffuse", bulbDiffuse);
	elShader->setVec3("lightBulb.specular", 0.1f, 0.1f, 0.1f);
	elShader->setFloat("lightBulb.constant", 1.0f);
	elShader->setFloat("lightBulb.linear", 0.22f);
	elShader->setFloat("lightBulb.quadratic", 0.20f);

	elShader->setVec3("spotLight.position", Game::getCameraPos());
	elShader->setVec3("spotLight.direction", Game::getCameraFront());
	elShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(18.5f)));
	elShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(35.0f)));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
}




