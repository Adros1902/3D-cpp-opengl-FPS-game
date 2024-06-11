#include "Bullet.h"
#include <glad/glad.h>
#include "iostream"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "MainWindow.h"
#include "Game.h"

Bullet::Bullet(glm::vec3 startPointIn, glm::vec3 directionCamIn, int bulletIDIn)
{
	bulletID = bulletIDIn;
	directionCam = directionCamIn;
	currentPosition = startPointIn;
	startPoint = startPointIn;
	initObject();
}

float bulletVertices[] =
{
	-0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f, //back
	-0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	 -0.01f,  0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	- 0.01f,  0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f,  0.01f, -0.01f, 1.0f, 0.0f, 0.0f,

	-0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f, //front
	-0.01f, -0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f, -0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f, -0.01f,  0.01f, 1.0f, 0.0f, 0.0f,

	-0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f, //left
	-0.01f, -0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f,  0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f,

	 0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f, //right
	 0.01f,  0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f, -0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f,	

	-0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f, //
	 0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f, -0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f, -0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f, -0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f, -0.01f, -0.01f, 1.0f, 0.0f, 0.0f,	

	-0.01f,  0.01f, -0.01f, 1.0f, 0.0f, 0.0f,//
	 0.01f,  0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	 0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f,  0.01f,  0.01f, 1.0f, 0.0f, 0.0f,
	-0.01f,  0.01f, -0.01f, 1.0f, 0.0f, 0.0f,
};



glm::mat4 bulletProjection;
glm::mat4 bulletView;
glm::mat4 bulletModel;

void Bullet::initObject()
{
	
	glGenBuffers(1, &VBOBullet);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBullet);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bulletVertices), bulletVertices, GL_STATIC_DRAW);

	bulletShader = new Shader("shaders/bullet.vs", "shaders/bullet.fs");

	glGenVertexArrays(1, &VAOBullet);

	glBindVertexArray(VAOBullet);

	glBufferData(GL_ARRAY_BUFFER, sizeof(bulletVertices), bulletVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(1);


	bulletProjection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
}

void Bullet::renderObject()
{
	bulletShader->use();
	bulletView = glm::lookAt(Game::getCameraPos(), Game::getCameraPos() + Game::getCameraFront(), Game::getCameraUp());
	bulletShader->setMat4("view", bulletView);
	bulletShader->setMat4("projection", bulletProjection);

	glBindVertexArray(VAOBullet);

	bulletModel = glm::mat4(1.0f);
	currentPosition += bulletSpeed * directionCam;
	bulletModel = glm::translate(bulletModel, currentPosition);
	bulletShader->setMat4("model", bulletModel);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

bool Bullet::outOfRange()
{
	if (currentPosition.x > startPoint.x + range || currentPosition.x < startPoint.x - range ||
		currentPosition.y > startPoint.y + range || currentPosition.y < -1.0f ||
		currentPosition.z > startPoint.z + range || currentPosition.z < startPoint.z - range) 
	{
		return true;
	}

	return false;
}
