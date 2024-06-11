#pragma once
class Shadows
{
	//in development
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;

public:
	void initShadows();

	void renderShadows();
};

