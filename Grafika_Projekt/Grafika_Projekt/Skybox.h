#pragma once
#include <vector>
#include <string>
class Skybox
{
public:
	Skybox();

	void loadFiles(std::vector<std::string> faces);

	void initObject();

	void renderObject();
};

