#pragma once

#include "gameNode.h"

class pixelCollisionTest : public gameNode
{
private:
	image* _mountain;
	image* _ball;

	RECT _rc; //�����ֱ� ��

	float _x;
	float _y;

	int _proveY; //y�� Ž����

	bool _isCollision;

public:
	pixelCollisionTest();
	~pixelCollisionTest();

	HRESULT init();
	void release();
	void update();
	void render();
};

