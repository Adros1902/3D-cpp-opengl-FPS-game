#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"
#include "Bullet.h"

class BuildingElements
{
public:
	unsigned int elVBO;
	unsigned int elTexture;
	unsigned int elVAO;
	Shader* elShader;

	glm::mat4 elProjection;
	glm::mat4 elView;
	glm::mat4 elModel;
	glm::mat4 tempModel;
	float angles;
	glm::vec3 rotation;
	bool isRotable;
	glm::mat3 normalMatrix;
	bool isRoof = false;
	GLuint textures[2];
	unsigned int textureID;

	float sizeX, sizeY, sizeZ;

	glm::vec3 position;

	glm::vec3 startPosition;
	void changeModelRot(float radians, glm::vec3 axis);
	BuildingElements(std::vector<std::string> fileName, glm::vec3 position);
	BuildingElements(std::vector<std::string> fileName, glm::vec3 position, float anglesIn, glm::vec3 rotationIn);
	BuildingElements(std::vector<std::string> fileName, glm::vec3 position, float anglesIn, glm::vec3 rotationIn, float isRoof);
	void loadFiles(std::vector<std::string> fileName);
	void initObject(std::vector<float> vertices);
	void renderObject(std::vector<Bullet>& bullets, bool flashlight);

};

