#include "stdafx.h"
#include "effectTest.h"


effectTest::effectTest()
{
}


effectTest::~effectTest()
{
}

HRESULT effectTest::init()
{
	IMAGEMANAGER->addImage("explosion", "image/explosion.bmp", 0, 0, 832, 62, true, RGB(255, 0, 255));

	_effectSample = new effect;
	_effectSample->init(IMAGEMANAGER->findImage("explosion"), 32, 62, 1, 0.3f);

	//EFFECTMANAGER »ç¿ë
	EFFECTMANAGER->addEffect("explosion2", "image/explosion.bmp", 832, 62, 32, 62, 1, 0.2, 300);

	return S_OK;
}

void effectTest::release()
{
	SAFE_DELETE(_effectSample);
}

void effectTest::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		_effectSample->startEffect(_ptMouse.x, _ptMouse.y);
	}
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		EFFECTMANAGER->play("explosion2", _ptMouse.x, _ptMouse.y);
	}

	_effectSample->update();
	EFFECTMANAGER->update();
}

void effectTest::render()
{
	_effectSample->render();
	EFFECTMANAGER->render();
}
