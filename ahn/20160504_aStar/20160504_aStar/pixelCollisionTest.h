#pragma once

#include "gameNode.h"

class pixelCollisionTest : public gameNode
{
private:
	image* _mountain;
	image* _ball;

	RECT _rc; //보여주기 용

	float _x;
	float _y;

	int _proveY; //y축 탐색용

	bool _isCollision;

public:
	pixelCollisionTest();
	~pixelCollisionTest();

	HRESULT init();
	void release();
	void update();
	void render();
};

