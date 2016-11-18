#pragma once

#include "gameNode.h"
#include "shipManager.h"

class starwarsScene : public gameNode
{
private:
	shipManager* _shipManager;

public:
	starwarsScene();
	~starwarsScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

