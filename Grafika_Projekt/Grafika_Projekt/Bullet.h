#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
class Bullet
{
public:

	unsigned int VAOBullet;
	unsigned int VBOBullet;

	Shader* bulletShader;

	float bulletSpeed = 0.05f;
	float size = 0.005f;
	int bulletID = 0;
	float range = 35.0f;

	bool operator==(const Bullet& other) const {
		return this->bulletID == other.bulletID; // You might need to adjust this based on your class structure
	}

	glm::vec3 currentPosition;
	glm::vec3 directionCam;
	glm::vec3 startPoint;
	
	Bullet(glm::vec3 startPointIn, glm::vec3 directionCamIn, int bulletIDIn);

	void initObject();

	void renderObject();

	bool outOfRange();
};

