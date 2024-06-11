#pragma once
#include "Bullet.h"
#include <vector>

class Terrain
{
public:
	Terrain();

	void loadFiles();

	void initObject();

	void renderObject(std::vector<Bullet> bullets, bool flashlight);
};

