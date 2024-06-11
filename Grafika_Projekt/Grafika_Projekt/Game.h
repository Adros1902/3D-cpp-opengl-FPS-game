#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EnemyCube.h"
#include "Bullet.h"
#include <vector>
#include "BuildingElements.h"
#include "Model.h"

const float screenWidth = 1920.0f;
const float screenHeight = 1080.0f;
const glm::vec3 ambientLightColor(1.0f, 1.0f, 1.0f);
const glm::vec3 testLightPos(1.5f, 1.0f, 1.5f);

class Game
{
public:
	Game();

	float playerSize = 0.75f;
	float playerHeight = 0.25f;
	glm::mat4 modelProjection;
	glm::mat4 modelView;
	glm::mat4 modelModel;
	glm::vec3 positionModel;

	int enemyCounter = 0;
	//Map map;
	

	static glm::vec3 getCameraPos();
	static glm::vec3 getCameraFront();
	static glm::vec3 getCameraUp();

	void playerJumpProcess();

	static bool checkCollisionSphereAABB(EnemyCube& enemy);

	static bool chechCollisionForPlayer();

	bool checkCollisionEnemies(EnemyCube & enemyForCheck);

	bool checkBulletCollision(Bullet& bullet, EnemyCube& enemy);

	bool checkBulletCollisionWithBuilding(Bullet& bullet, BuildingElements& obj);

	bool checkBulletCollisionWithModel(Bullet& bullet, Model& obj);
	
	static bool checkCollisionPlayerWall(BuildingElements& obj);

	static bool checkModelPlayerCollision(Model& model);

	void deleteKilledEnemyAndBullet();

	glm::vec3 enemyMovement(EnemyCube& enemy, glm::vec3 playerPos, std::vector<EnemyCube>& enemyVector);

	bool checkOverLap(glm::vec2 posOther, float size, EnemyCube& thisEnemy);

	static void updateCameraPos(const glm::vec3& newPos);
	static void updateCameraFront(const glm::vec3& newFront);
	static void updateCameraUp(const glm::vec3& newUp);
	glm::vec2 computeNormalSeparation(std::vector<EnemyCube>& enemyVector, EnemyCube& thisEnemy);

	void initShadows();
	void renderShadows();
	Shader genModelShader();

	int game();

};

