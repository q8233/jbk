#include "stdafx.h"
#include "pixelCollisionTest.h"


pixelCollisionTest::pixelCollisionTest()
{
}


pixelCollisionTest::~pixelCollisionTest()
{
}

HRESULT pixelCollisionTest::init()
{
	IMAGEMANAGER->addImage("mountain", "image/mountain.bmp", 922, 551, true, RGB(255, 0, 255));

	_ball = IMAGEMANAGER->addImage("ball", "image/ball.bmp", 60, 60, true, 0xff00ff);

	_x = CENTERX - 100;
	_y = CENTERY + 80;

	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	_proveY = _y + _ball->getHeight() / 2;

	return S_OK;
}

void pixelCollisionTest::release()
{
}

void pixelCollisionTest::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_x -= 2.0f;
	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_x += 2.0f;
	}

	_proveY = _y + _ball->getHeight() / 2;

	for (int i = _proveY - 50; i < _proveY + 50; i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("mountain")->getMemDC(), _x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (color != RGB(255, 0, 255))
		{
			_y = i - _ball->getHeight() / 2;
			break;
		}
	}


	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());
}

void pixelCollisionTest::render()
{
	IMAGEMANAGER->render("mountain", getMemDC());

	_ball->render(getMemDC(), _rc.left, _rc.top);

	Rectangle(getMemDC(), _x, _proveY, _x + 10, _proveY + 10);
}

