#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Bullet.h"

class EnemyCube
{
public:

	bool operator==(const EnemyCube& other) const {
		return this->enemyID == other.enemyID; // You might need to adjust this based on your class structure
	}

	GLuint textures[3];
	int enemyID;
	bool steppingBack = false;
	int steps = 0;
	float cameraLength = 0;
	bool enemyToDirSet = true;
	unsigned int textureID;

	float size = 0.5f;
	glm::vec3 position;

	EnemyCube(std::vector<std::string> fileName, int enemyID, unsigned int textureId);

	void loadFiles(std::vector<std::string> fileName);

	void initObject();

	void renderObject(std::vector<Bullet> &bullets, bool flashlight, float angle, glm::vec3 currentPosition);

	void updateMovement();
};

