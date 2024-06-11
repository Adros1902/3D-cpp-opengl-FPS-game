#include "Map.h"
#include "BuildingElements.h"
#include <vector>
#include <queue>


Map::Map(std::vector<BuildingElements>& buildingVector)
{
	createMap(buildingVector);
}


Map::~Map()
{
	for (int i = 0; i < mapResolution; i++) {
		delete[] mapObjects[i];
	}
	delete[] mapObjects;
}

void Map::createMap(std::vector<BuildingElements>& buildingVector)
{
	mapObjects = new Node* [mapResolution];
	for (int i = 0; i < mapResolution; i++)
	{
		mapObjects[i] = new Node[mapResolution];
	}

	for (float i = 0; i < mapResolution; i++)
	{
		for (float j = 0; j < mapResolution; j++) {
			for (int k = 0; k < buildingVector.size(); k++)
			{//problem z rozdzielczoscia
				if ((buildingVector[k].position.x + mapSize - buildingVector[k].sizeX) + 20.0f <= i 
					&& (buildingVector[k].position.x + mapSize + buildingVector[k].sizeX) + 20.0f >= i
					&& (buildingVector[k].position.z + mapSize + buildingVector[k].sizeZ) + 20.0f >= j
					&& (buildingVector[k].position.z + mapSize - buildingVector[k].sizeZ) + 20.0f <= j 
					&& !buildingVector[k].isRoof)
				{
					Node tempNode;
					tempNode.direction = 0;
					tempNode.distance = INT_MAX;
					mapObjects[(int)i][(int)j] = tempNode;
					break;
				}
				else {
					Node tempNode;
					tempNode.direction = 9;
					tempNode.distance = INT_MAX;
					mapObjects[(int)i][(int)j] = tempNode;
				}
			}
		}
	}
	std::cout << "Generated Map" << std::endl;
}

void Map::setTargetAndUpdateFLow(int targetXNew, int targetYNew)
{
	if (targetX != targetXNew || targetY != targetYNew) {
		targetX = targetXNew;
		targetY = targetYNew;

		//Target in bounds?
		if (targetX >= 0 && targetX < mapResolution &&
			targetY >= 0 && targetY < mapResolution) {

			for (int i = 0; i < mapResolution; i++)
			{
				for(int j = 0; j < mapResolution; j++){
					mapObjects[i][j].direction = 9;
					mapObjects[i][j].distance = INT_MAX;
				}
			}

			//Calculate the flow field.
			calculateDistance();
			calculateFlowDirections();
		}
	}
	for (float i = 0; i < mapResolution; i++)
	{
		for (float j = 0; j < mapResolution; j++) {
			std::cout << mapObjects[(int)i][(int)j].distance << " ";
		}
		std::cout << std::endl;
	}
}

void Map::calculateDistance()
{
	std::queue<std::pair<int, int>> listToCheck;

	mapObjects[targetX][targetY].distance = 0;
	float temp = ((float)targetX ) - 20;
	float tempZ = ((float)targetY ) - 20;
	std::cout << "TargetX: " << temp << " Target Z: " << tempZ << std::endl;
	std::pair<int, int> pointOnMap;
	pointOnMap.first = targetX;
	pointOnMap.second = targetY;
	listToCheck.push(pointOnMap);
	

	const int listNeighbors[][2] = { { -1, 0}, {1, 0}, {0, -1}, {0, 1} };

	while (!listToCheck.empty()) {
		std::pair<int, int> currentPoint = listToCheck.front();
		listToCheck.pop();
		//to nie dziala, bo ma byc tylko gora, dol lewo prawo
		for (int i = 0; i < 4; i++)
		{
				currentPoint.first += listNeighbors[i][0];
				currentPoint.second += listNeighbors[i][1];

				if (currentPoint.first >= 0 && currentPoint.first < mapResolution
					&& currentPoint.second >= 0 && currentPoint.second < mapResolution)
				{
					if (mapObjects[currentPoint.first][currentPoint.second].direction != 0 
						&& mapObjects[currentPoint.first][currentPoint.second].distance == INT_MAX)
					{
						mapObjects[currentPoint.first][currentPoint.second].distance = mapObjects[pointOnMap.first][pointOnMap.second].distance + 1;
						listToCheck.push(currentPoint);
					}
				}
		}
	}
}

void Map::calculateFlowDirections()
{
	const int listNeighbors[][2] = {
		{-1, 0}, {-1, 1}, {0, 1}, {1, 1},
		{1, 0}, {1, -1}, {0, -1}, {-1, -1} };

	for (int i = 0; i < mapResolution; i++)
	{
		for (int j = 0; j < mapResolution; j++) {
			if (mapObjects[i][j].distance != INT_MAX)
			{
				int flowFieldBest = mapObjects[i][j].distance;

				for (int k = 0; k < 8; k++)
				{
					int offsetX = listNeighbors[k][0];
					int offsetY = listNeighbors[k][1];

					std::pair<int, int> currentPoint;
					currentPoint.first = i + offsetX;
					currentPoint.second = j + offsetY;

					if (currentPoint.first >= 0 && currentPoint.first < mapResolution
						&& currentPoint.second >= 0 && currentPoint.second < mapResolution)
					{
						if (mapObjects[currentPoint.first][currentPoint.second].distance < flowFieldBest) {
							flowFieldBest = mapObjects[currentPoint.first][currentPoint.second].distance;
							switch (offsetY) {
							case -1:
								switch (offsetX) {
								case -1:
									mapObjects[i][j].direction = 8; // down-left
									break;
								case 0:
									mapObjects[i][j].direction = 7; // down
									break;
								case 1:
									mapObjects[i][j].direction = 6; // down-right
									break;
								}
								break;
							case 0:
								switch (offsetX) {
								case -1:
									mapObjects[i][j].direction = 1; // left
									break;
								case 0:
									// no move - obstacle
									break;
								case 1:
									mapObjects[i][j].direction = 5; // right
									break;
								}
								break;
							case 1:
								switch (offsetX) {
								case -1:
									mapObjects[i][j].direction = 2; // up-left
									break;
								case 0:
									mapObjects[i][j].direction = 3; // up
									break;
								case 1:
									mapObjects[i][j].direction = 4; // up-right
									break;
								}
								break;
							}
						}
					}
				}
			}
		}
	}

}

glm::vec2 Map::getFlowNormal(int x, int y)
{

	if (x >= 0 && x < mapResolution && y >= 0 && y < mapResolution)
	{
		switch (mapObjects[x][y].direction) {
		case 1:
			return glm::normalize(glm::vec2(-1.0f, 0.0f)); // left
		case 2:
			return glm::normalize(glm::vec2(-1.0f, 1.0f)); // up-left
		case 3:
			return glm::normalize(glm::vec2(0.0f, 1.0f)); // up
		case 4:
			return glm::normalize( glm::vec2(1.0f, 1.0f)); // up-right
		case 5:
			return glm::normalize(glm::vec2(1.0f, 0.0f)); // right
		case 6:
			return glm::normalize(glm::vec2(1.0f, -1.0f)); // down-right
		case 7:
			return glm::normalize(glm::vec2(0.0f, -1.0f)); // down
		case 8:
			return glm::normalize(glm::vec2(-1.0f, -1.0f)); // down-left
		}
	}

	return glm::vec2();
}

bool Map::isMovePossible(int x, int y)
{
	if (x >= 0 && x < mapResolution && y >= 0 && y < mapResolution)
	{
		if (mapObjects[x][y].direction == 0)
		{
			return false;
		}
	}
	return true;
}

