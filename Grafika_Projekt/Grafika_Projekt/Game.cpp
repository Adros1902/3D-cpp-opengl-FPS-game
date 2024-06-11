#include "Game.h"
#include <glad/glad.h>
#include "iostream"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Terrain.h"
#include "Skybox.h"
#include "EnemyCube.h"
#include "Bullet.h"
#include "BuildingElementsData.h"
#include "BuildingElements.h"
#include "Light.h"
#include <random>
#include <unordered_map>
#include "Model.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	std::cout << width << " " << height;
}
//camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float gravity = 10.0f;
bool isJumping = false;
float jumpDuration = 0.8f;
float startJumpTime = 0.0f;
float jumpStartVelocity = 3.0f;

Game::Game()
{
}

glm::vec3 Game::getCameraPos()
{
	return cameraPos;
}

glm::vec3 Game::getCameraFront()
{
	return cameraFront;
}

glm::vec3 Game::getCameraUp()
{
	return cameraUp;
}

void Game::playerJumpProcess()
{
	if (isJumping) {
		double currentTime = glfwGetTime();
		double elapsedTime = currentTime - startJumpTime;

		if (elapsedTime >= jumpDuration) {
			isJumping = false;
			return;
		}

		double jumpHeight = jumpStartVelocity * elapsedTime - 0.5 * gravity * elapsedTime * elapsedTime;
		glm::vec3 cameraPos = Game::getCameraPos();

		if (jumpHeight <= 0.0f)
		{
			cameraPos.y = 0.0f;
			isJumping = false;
		}
		else {
			cameraPos.y = jumpHeight;
		}

		Game::updateCameraPos(cameraPos);
	}
}

std::vector<std::string> texturesForEnemies;

bool flashlightOn = true;
float playerSize = 0.5f;
float playerHeight = 0.15f;
int wallID = 0;


glm::vec3 tempCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


bool blockMovement = false;


std::vector<EnemyCube> walterVector;
std::vector<Bullet> bullets;
std::vector<BuildingElements> buildingElementsVector;
std::vector<Model> modelsVector;

int bulletCounter = 0;
float playerRadius = 0.2f;

bool Game::checkCollisionSphereAABB(EnemyCube& enemy)
{
	glm::vec3 camPos = tempCameraPos;
	const float x = std::max(enemy.position.x - enemy.size, std::min(camPos.x, enemy.position.x + enemy.size));
	const float y = std::max(enemy.position.y - enemy.size, std::min(camPos.y, enemy.position.y + enemy.size));
	const float z = std::max(enemy.position.z - enemy.size, std::min(camPos.z, enemy.position.z + enemy.size));

	const float distance = (x - camPos.x) * (x - camPos.x) + (z - camPos.z) * (z - camPos.z) + (y - camPos.y) * (y - camPos.y);
	//std::cout << "distance: " << distance << " radius^2 = " << playerRadius * playerRadius << std::endl;
	return distance < playerRadius * playerRadius;
}

bool Game::chechCollisionForPlayer()
{
	for (int i = 0; i < walterVector.size(); i++)
	{
		if (checkCollisionSphereAABB(walterVector[i])) {
			return true;
		}
	}

	for (int i = 0; i < buildingElementsVector.size(); i++)
	{
		if (checkCollisionPlayerWall(buildingElementsVector[i])) {
			return true;
		}
	}
	for (int i = 0; i < modelsVector.size(); i++)
	{
		if (checkModelPlayerCollision(modelsVector[i])) {
			return true;
		}
	}

	return false;
}

bool Game::checkCollisionEnemies(EnemyCube & enemyForCheck) {

	for (int i = 0; i < walterVector.size(); i++)
	{
		if (walterVector[i].enemyID != enemyForCheck.enemyID) {
			bool collisionX = walterVector[i].position.x - walterVector[i].size <= enemyForCheck.position.x + enemyForCheck.size &&
				walterVector[i].position.x + walterVector[i].size >= enemyForCheck.position.x - enemyForCheck.size;

			bool collisionY = walterVector[i].position.y - walterVector[i].size <= enemyForCheck.position.y + enemyForCheck.size &&
				walterVector[i].position.y + walterVector[i].size >= enemyForCheck.position.y - enemyForCheck.size;

			bool collisionZ = walterVector[i].position.z - walterVector[i].size <= enemyForCheck.position.z + enemyForCheck.size &&
				walterVector[i].position.z + walterVector[i].size >= enemyForCheck.position.z - enemyForCheck.size;
			if (collisionX && collisionY && collisionZ)
			{
				if (enemyForCheck.cameraLength > walterVector[i].cameraLength)
				{
					enemyForCheck.steppingBack = true;
					return true;
				}
			}
		}
	}

	if (checkCollisionSphereAABB(enemyForCheck)) {
		return true;
	}

	for (int i = 0; i < buildingElementsVector.size(); i++)
	{
		
		bool collisionX = enemyForCheck.position.x - enemyForCheck.size <= buildingElementsVector[i].position.x + buildingElementsVector[i].sizeX &&
			enemyForCheck.position.x + enemyForCheck.size >= buildingElementsVector[i].position.x - buildingElementsVector[i].sizeX;

		bool collisionY = enemyForCheck.position.y - enemyForCheck.size <= buildingElementsVector[i].position.y + buildingElementsVector[i].sizeY &&
			enemyForCheck.position.y + enemyForCheck.size >= buildingElementsVector[i].position.y - buildingElementsVector[i].sizeY;

		bool collisionZ = enemyForCheck.position.z - enemyForCheck.size <= buildingElementsVector[i].position.z + buildingElementsVector[i].sizeZ &&
			enemyForCheck.position.z + enemyForCheck.size >= buildingElementsVector[i].position.z - buildingElementsVector[i].sizeZ;
		if (collisionX && collisionY && collisionZ) {
			return true;
		}
	}

	for (int i = 0; i < modelsVector.size(); i++)
	{

		bool collisionX = enemyForCheck.position.x - enemyForCheck.size <= modelsVector[i].position.x + modelsVector[i].sizeX &&
			enemyForCheck.position.x + enemyForCheck.size >= modelsVector[i].position.x - modelsVector[i].sizeX;

		bool collisionY = enemyForCheck.position.y - enemyForCheck.size <= modelsVector[i].position.y + modelsVector[i].sizeY &&
			enemyForCheck.position.y + enemyForCheck.size >= modelsVector[i].position.y - modelsVector[i].sizeY;

		bool collisionZ = enemyForCheck.position.z - enemyForCheck.size <= modelsVector[i].position.z + modelsVector[i].sizeZ &&
			enemyForCheck.position.z + enemyForCheck.size >= modelsVector[i].position.z - modelsVector[i].sizeZ;
		if (collisionX && collisionY && collisionZ) {
			return true;
		}
	}

	return false;
}

bool Game::checkBulletCollision(Bullet& bullet, EnemyCube& enemy)
{
	bool collisionX = bullet.currentPosition.x - bullet.size <= enemy.position.x + enemy.size &&
		bullet.currentPosition.x + bullet.size >= enemy.position.x - enemy.size;

	bool collisionY = bullet.currentPosition.y - bullet.size <= enemy.position.y + enemy.size &&
		bullet.currentPosition.y + bullet.size >= enemy.position.y - enemy.size;

	bool collisionZ = bullet.currentPosition.z - bullet.size <= enemy.position.z + enemy.size &&
		bullet.currentPosition.z + bullet.size >= enemy.position.z - enemy.size;

	return collisionX && collisionY && collisionZ;
}

bool Game::checkBulletCollisionWithBuilding(Bullet& bullet, BuildingElements& obj)
{
	bool collisionX = bullet.currentPosition.x - bullet.size <= obj.position.x + obj.sizeX &&
		bullet.currentPosition.x + bullet.size >= obj.position.x - obj.sizeX;

	bool collisionY = bullet.currentPosition.y - bullet.size <= obj.position.y + obj.sizeY &&
		bullet.currentPosition.y + bullet.size >= obj.position.y - obj.sizeY;

	bool collisionZ = bullet.currentPosition.z - bullet.size <= obj.position.z + obj.sizeZ &&
		bullet.currentPosition.z + bullet.size >= obj.position.z - obj.sizeZ;

	return collisionX && collisionY && collisionZ;
}

bool Game::checkBulletCollisionWithModel(Bullet& bullet, Model& obj)
{
	bool collisionX = bullet.currentPosition.x - bullet.size <= obj.position.x + obj.sizeX &&
		bullet.currentPosition.x + bullet.size >= obj.position.x - obj.sizeX;

	bool collisionY = bullet.currentPosition.y - bullet.size <= obj.position.y + obj.sizeY &&
		bullet.currentPosition.y + bullet.size >= obj.position.y - obj.sizeY;

	bool collisionZ = bullet.currentPosition.z - bullet.size <= obj.position.z + obj.sizeZ &&
		bullet.currentPosition.z + bullet.size >= obj.position.z - obj.sizeZ;

	return collisionX && collisionY && collisionZ;
}

bool Game::checkCollisionPlayerWall(BuildingElements& obj) 
{
	glm::vec3 camPos = tempCameraPos;
	const float x = std::max(obj.position.x - obj.sizeX, std::min(camPos.x, obj.position.x + obj.sizeX));
	const float y = std::max(obj.position.y - obj.sizeY, std::min(camPos.y, obj.position.y + obj.sizeY));
	const float z = std::max(obj.position.z - obj.sizeZ, std::min(camPos.z, obj.position.z + obj.sizeZ));

	const float distance = (x - camPos.x) * (x - camPos.x) + (z - camPos.z) * (z - camPos.z) + (y - camPos.y) * (y - camPos.y);
	//std::cout << "distance: " << distance << " radius^2 = " << playerRadius * playerRadius << std::endl;
	return distance < playerRadius * playerRadius;
}

bool Game::checkModelPlayerCollision(Model& model) 
{
	glm::vec3 camPos = tempCameraPos;
	const float x = std::max(model.position.x - model.sizeX, std::min(camPos.x, model.position.x + model.sizeX));
	const float y = std::max(model.position.y - model.sizeY, std::min(camPos.y, model.position.y + model.sizeY));
	const float z = std::max(model.position.z - model.sizeZ, std::min(camPos.z, model.position.z + model.sizeZ));

	const float distance = (x - camPos.x) * (x - camPos.x) + (z - camPos.z) * (z - camPos.z) + (y - camPos.y) * (y - camPos.y);
	//std::cout << "distance: " << distance << " radius^2 = " << playerRadius * playerRadius << std::endl;
	return distance < playerRadius * playerRadius;
}

void Game::deleteKilledEnemyAndBullet()
{
	std::vector<EnemyCube> cubesToDelete;
	std::vector<Bullet> bulletsToDelete;
	//check bullet collision
	for (int i = 0; i < walterVector.size(); i++)
	{
		for (int j = 0; j < bullets.size(); j++) {
			bool hit = checkBulletCollision(bullets[j], walterVector[i]);
			if (hit) {
				cubesToDelete.push_back(walterVector[i]);
				bulletsToDelete.push_back(bullets[j]);
			}
		}
	}

	//collision with walls etc
	for (int i = 0; i < buildingElementsVector.size(); i++)
	{
		for (int j = 0; j < bullets.size(); j++)
		{
			bool hit = checkBulletCollisionWithBuilding(bullets[j], buildingElementsVector[i]);
			if (hit) {
				bulletsToDelete.push_back(bullets[j]);
			}
		}
	}

	for (int i = 0; i < modelsVector.size(); i++)
	{
		for (int j = 0; j < bullets.size(); j++)
		{
			bool hit = checkBulletCollisionWithModel(bullets[j], modelsVector[i]);
			if (hit) {
				bulletsToDelete.push_back(bullets[j]);
			}
		}
	}

	for (int i = 0; i < cubesToDelete.size(); i++) {
		for (int j = 0; j < walterVector.size(); j++) {
			if (walterVector[j].enemyID == cubesToDelete[i].enemyID) {
				walterVector.erase(walterVector.begin() + j);
				break;
			}
		}
	}

	for (int i = 0; i < bulletsToDelete.size(); i++) {
		for (int j = 0; j < bullets.size(); j++) {
			if (bullets[j].bulletID == bulletsToDelete[i].bulletID) {
				bullets.erase(bullets.begin() + j);
				break;
			}
		}
	}

	//bullet out of range
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].outOfRange()) {
			bulletsToDelete.push_back(bullets[i]);
		}
	}

	for (int i = 0; i < bulletsToDelete.size(); i++) {
		auto bulletToDelete = std::find(bullets.begin(), bullets.end(), bulletsToDelete[i]);
		if (bulletToDelete != bullets.end()) {
			bullets.erase(bulletToDelete);
			break;
		}
	}
	

	cubesToDelete.clear();
	bulletsToDelete.clear();
}

void Game::updateCameraPos(const glm::vec3& newPos)
{
	cameraPos = newPos;
}

void Game::updateCameraFront(const glm::vec3& newFront)
{
	cameraFront = newFront;
}

void Game::updateCameraUp(const glm::vec3& newUp)
{
	cameraUp = newUp;
}



void Game::initShadows()
{
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	//const unsigned int 
}
void Game::renderShadows()
{
}
Shader Game::genModelShader()
{
	Shader modelShader("shaders/importMesh.vs", "shaders/importMesh.fs");
	modelShader.use();

	std::cout << "Model Shader" << std::endl;

	modelShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	modelShader.setVec3("light.diffuse", 0.3f, 0.3f, 0.3f);
	modelShader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);

	modelShader.setVec3("dirLight.ambient", 0.15f, 0.15f, 0.15f);
	modelShader.setVec3("dirLight.diffuse", 0.25f, 0.25f, 0.25f);
	modelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	modelShader.setVec3("dirLight.direction", -0.0f, -1.0f, -0.3f);

	modelShader.setVec3("material.ambient", 0.878f, 0.878f, 1.0f);
	modelShader.setVec3("material.diffuse", 0.878f, 0.878f, 1.0f);
	modelShader.setVec3("material.specular", 0.2f, 0.2f, 0.2f);
	modelShader.setFloat("material.shininess", 8.0f);

	modelShader.setVec3("colorForBullets.ambient", 1.0f, 0.235f, 0.235f);
	modelShader.setVec3("colorForBullets.diffuse", 1.0f, 0.235f, 0.235f);
	modelShader.setVec3("colorForBullets.specular", 1.0f, 0.235f, 0.235f);
	modelShader.setFloat("colorForBullets.shininess", 2.0f);

	modelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	modelShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
	modelShader.setVec3("spotLight.specular", 0.3f, 0.3f, 0.3f);
	modelShader.setFloat("spotLight.constant", 1.0f);
	modelShader.setFloat("spotLight.linear", 0.045f);
	modelShader.setFloat("spotLight.quadratic", 0.0075f);

	modelShader.setVec3("bulbMaterial.ambient", 0.0f, 1.0f, 0.0f);
	modelShader.setVec3("bulbMaterial.diffuse", 0.0f, 1.0f, 0.0f);
	modelShader.setVec3("bulbMaterial.specular", 0.0f, 1.0f, 0.0f);
	modelShader.setFloat("bulbMaterial.shininess", 4.0f);

	modelProjection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);

	modelModel = glm::mat4(1.0f);
	modelModel = glm::translate(modelModel, glm::vec3(3.0f, 0.0f, -12.0f));
	glm::vec3 scale(3.0f, 3.0f, 3.0f);
	modelModel = glm::scale(modelModel, scale);
	glm::vec4 tempPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 worldPosition = modelModel * tempPosition;
	positionModel = glm::vec3(worldPosition);
	positionModel = glm::vec3(-3.0f, 0.0f, -12.0f);
	modelShader.setMat4("model", modelModel);
	modelShader.setMat4("projection", modelProjection);
	return modelShader;
}


static bool flashlightKeyPressed = false;
void processInput(GLFWwindow* window)
{
	//std::cout << blockMovement << std::endl;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//std::cout << "x = " << Game::getCameraPos().x << " y = " << Game::getCameraPos().y << " z = " << Game::getCameraPos().z << std::endl;
	float cameraSpeed = static_cast<float>(6.0 * deltaTime);

	static double lastFrameTime = glfwGetTime();
	double currentFrameTime = glfwGetTime();
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
	static double debounceDelay = 0.5;
	static double lastKeyPressTime = 0;

	glm::vec3 front = glm::normalize(glm::vec3(Game::getCameraFront().x, 0, Game::getCameraFront().z));
	glm::vec3 right = glm::normalize(glm::cross(front, Game::getCameraUp()));
	

	tempCameraPos = Game::getCameraPos();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		tempCameraPos += cameraSpeed * front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		tempCameraPos -= cameraSpeed * front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		tempCameraPos -= cameraSpeed * right;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		tempCameraPos += cameraSpeed * right;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!isJumping)
		{
			isJumping = true;
			startJumpTime = glfwGetTime();
		}
		
	}
	

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (!flashlightKeyPressed) {
			flashlightOn = !flashlightOn;
			flashlightKeyPressed = true;
		}
	}
	else 
	{
		flashlightKeyPressed = false;
	}

	if (!Game::chechCollisionForPlayer()) {
		//std::cout << "Brak kolizji!" << std::endl;
		Game::updateCameraPos(tempCameraPos);
	}
	else {
		//std::cout << "Kolizja!" << std::endl;
	}
}

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
double lastX = 800.0 / 2.0;
double lastY = 600.0 / 2.0;
float fov = 45.0f;
std::unordered_map<int, double> lastRotationTimes;


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Game::updateCameraFront(glm::normalize(direction));
}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << "Mouse left button is clicked" << std::endl;
		glm::vec3 tempCamPosForBullet = Game::getCameraPos() + glm::vec3(0.1f, -0.1f, -0.1f);
		Bullet bullet(tempCamPosForBullet, glm::normalize(Game::getCameraFront()), bulletCounter);
		bulletCounter++;
		bullets.push_back(bullet);
	}
}
int frameCount = 0;
double lastTime = 0.0;

void updateFPS(GLFWwindow* window) {
	double currentTime = glfwGetTime();
	frameCount++;

	if (currentTime - lastTime >= 1.0) {
		double fps = frameCount / (currentTime - lastTime);
		std::cout << "FPS: " << fps << std::endl;

		frameCount = 0;
		lastTime = currentTime;
	}
}
float temp = 1.0f;

float calculateRotationForEnemy(float targetX, float targetY, float originX, float originY, bool rotDirection) {
	float rotation = atan2(targetY - originY, targetX - originX);

		rotation += sin(glfwGetTime() * 2) / 4;

	return rotation;
}

float lastUpdateTime = 0.0;

glm::vec3 Game::enemyMovement(EnemyCube& enemy, glm::vec3 playerPos, std::vector<EnemyCube> &enemyVector) {

	float currentTime = glfwGetTime();
	float updateTimeInterval = 0.3f;
	float enemyPosToReturnX = enemy.position.x;
	float enemyPosToReturnZ = enemy.position.z;
	if (currentTime - lastUpdateTime >= updateTimeInterval ) {
		float dirX = playerPos.x - enemy.position.x;
		float dirZ = playerPos.z - enemy.position.z;
		float length = sqrt(dirX * dirX + dirZ * dirZ);
		enemy.cameraLength = length;
		//normalized direction
		if (length != 0.0f) {
			dirX = dirX / length;
			dirZ = dirZ / length;
		}
		
		if (enemy.steppingBack) {
			enemy.steps--;
			if (enemy.steps == 0) {
				enemy.steppingBack = false;
				enemy.enemyToDirSet = true;
			}
			
		}


		if (!checkCollisionEnemies(enemy) && !enemy.steppingBack) {
			float enemySpeed = 0.02f;
			enemyPosToReturnX += dirX * enemySpeed;
			enemyPosToReturnZ += dirZ * enemySpeed;
		} else{
			if (enemy.enemyToDirSet) {
				enemy.steps = 10;
				enemy.enemyToDirSet = false;
			}
			float enemySpeed = 0.03f;
			enemyPosToReturnX -= dirX * enemySpeed;
			enemyPosToReturnZ -= dirZ * enemySpeed;
		}
	}
	
	return glm::vec3(enemyPosToReturnX, -0.5f, enemyPosToReturnZ);
}

bool Game::checkOverLap(glm::vec2 posOther, float sizeOther, EnemyCube& thisEnemy)
{
	glm::vec2 pos(thisEnemy.position.x, thisEnemy.position.z);
	return glm::length(posOther - pos) <= (sizeOther + thisEnemy.size) / 2.0f;
}

bool rotateRight = true;

// Czas od ostatniej zmiany kierunku
double lastDirectionChangeTime = 0.0;

int Game::game()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Walter", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	texturesForEnemies.push_back("saul.jpg");
	texturesForEnemies.push_back("finger.jpg");
	texturesForEnemies.push_back("walter2.jpg");

	glEnable(GL_DEPTH_TEST);

	//MSAA
	glEnable(GL_MULTISAMPLE);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	Skybox skybox;

	Terrain terrain;

	
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<unsigned int> distribution(0, 2);
	

	Model cottageModel("textures/models/sheet/scan_door_brick_wall_building_4/scene.gltf");
	modelsVector.push_back(cottageModel);
	
	for (int i = 0; i < 10; i++)
	{
		
		unsigned int randomInt = distribution(gen);
		EnemyCube walterCube(texturesForEnemies, enemyCounter, randomInt);
		if (i == 0)
		{
			walterCube.loadFiles(texturesForEnemies);
		}
		walterCube.initObject();
		
		enemyCounter++;
		walterVector.push_back(walterCube);
	}

	//generate building elements

	//building 1
	std::vector<std::string> textures;
	textures.push_back("textures/concrete_4k.jpg");
	textures.push_back("textures/wood_4k.jpg");
	glm::vec3 newPosition(-3.0f, 0.0f, -8.0f);
	BuildingElements leftWB1(textures, glm::vec3(-3.0f, 0.0f, -8.0f), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	BuildingElements backWB1(textures, glm::vec3(-6.0f, 0.0f, -11.0f));
	BuildingElements rightWB1(textures, glm::vec3(-9.0f, 0.0f, -8.0f), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	BuildingElements roofWB1(textures, glm::vec3(-6.0f, 1.5f, -9.5f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f), true);
	BuildingElements roof2WB1(textures, glm::vec3(-6.0f, 1.5f, -6.5f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f), true);
	buildingElementsVector.push_back(leftWB1);
	buildingElementsVector.push_back(backWB1);
	buildingElementsVector.push_back(rightWB1);
	buildingElementsVector.push_back(roofWB1);
	buildingElementsVector.push_back(roof2WB1);
	int frames = 0;
	double lastTime = glfwGetTime();
	double allTime =  glfwGetTime();

	double lastUpdateTime = glfwGetTime(); // Inicjalizacja czasu ostatniej aktualizacji
	const double updateInterval = 0.5;
	Shader modelShader = genModelShader();
	


	while (!glfwWindowShouldClose(window))
	{
		updateFPS(window);
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		playerJumpProcess();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		double currentTime = glfwGetTime();


		if (currentTime - lastDirectionChangeTime >= 5.0)
		{
			rotateRight = !rotateRight;
			lastDirectionChangeTime = currentTime;
		}

		if (walterVector.size() == 0)
		{
			enemyCounter = 0;
			for (int i = 0; i < 10; i++)
			{
				
				unsigned int randomInt = distribution(gen);
				EnemyCube walterCube(texturesForEnemies, enemyCounter, randomInt);
				if (i == 0)
				{
					walterCube.loadFiles(texturesForEnemies);
				}
				walterCube.initObject();

				enemyCounter++;
				walterVector.push_back(walterCube);
			}
		}
		

		
		skybox.renderObject();
		
		
		cottageModel.draw(modelShader, bullets, flashlightOn);
		
		glm::vec3 playerPos = getCameraPos();
		std::cout << "X: " << playerPos.x << " Y:" << playerPos.y << " Z: " << playerPos.z << std::endl;
		//render enemies
		for (unsigned int i = 0; i < walterVector.size(); i++)
		{
			float rotAngle = calculateRotationForEnemy(playerPos.x, playerPos.z, walterVector[i].position.x, walterVector[i].position.z, rotateRight);
			glm::vec3 newEnemyPos = enemyMovement( walterVector[i], playerPos, walterVector);
			walterVector[i].renderObject(bullets, flashlightOn, rotAngle, newEnemyPos);
			std::cout << "WAlter " << i << " X:" << newEnemyPos.x << " Y : " << newEnemyPos.y << " Z : " << newEnemyPos.z << std::endl;
		}

		//render active bullets
		for (int i = 0; i < bullets.size(); i++) {
			bullets[i].renderObject();
		}

		for (int i = 0; i < buildingElementsVector.size(); i++) {
			buildingElementsVector[i].renderObject(bullets, flashlightOn);
		}

		terrain.renderObject(bullets, flashlightOn);

		deleteKilledEnemyAndBullet();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();



	return 0;
}


