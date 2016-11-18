#include "stdafx.h"
#include "ship.h"


ship::ship()
{
}


ship::~ship()
{
}

HRESULT ship::init(const char * imageName, int x, int y)
{
	_image = IMAGEMANAGER->findImage(imageName);
	_x = x;
	_y = y;
	_angle = ONE_RAD * 120;
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_speed = 0.f;
	_radius = _image->getFrameWidth() / 2;

	return S_OK;
}

void ship::release()
{
}

void ship::update()
{
	keyControl();
	move();
}

void ship::render()
{
	draw();
}

void ship::draw()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);

	char str[128];
	sprintf_s(str, "%d도 %3.2f", int(_angle * 180 / PI), _speed);
	TextOut(getMemDC(), _rc.left, _rc.top - 15, str, strlen(str));
}

void ship::keyControl()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT)) _angle += 0.06f;
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) _angle -= 0.06f;
	if (KEYMANAGER->isStayKeyDown(VK_UP)) _speed += 0.02f;
	if (KEYMANAGER->isStayKeyDown(VK_DOWN)) _speed -= 0.02f;
}

void ship::move()
{
	int frame;

	//_angle = _angle + PI16;
	if (_angle > PI2) _angle -= PI2;

	if (_angle < 0) _angle += PI2;

	frame = int(_angle / PI8);
	_image->setFrameX(frame);

	_x += cosf(_angle) * _speed;
	_y += -sinf(_angle) * _speed;

	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
}

void ship::focusDraw()
{
	//정보 출력
	char str[128];

	//위치
	sprintf_s(str, "좌표 : x : %d, y : %d", _rc.left, _rc.top);
	TextOut(getMemDC(), 400, 10, str, strlen(str));

	//각도
	sprintf_s(str, "각도 : %d도", int(_angle * 180 / PI));
	TextOut(getMemDC(), 400, 30, str, strlen(str));

	//스피드
	sprintf_s(str, "속도 : %d도", int(_speed));
	TextOut(getMemDC(), 400, 50, str, strlen(str));

	_image->frameRender(getMemDC(), CENTERX - _image->getFrameWidth() / 2,
		CENTERY - _image->getFrameHeight() / 2);
}

void ship::defaultDraw(RECT rcFocus)
{
	RECT rcSour;
	RECT rcTemp;
	int x, y;

	rcSour.left = rcFocus.left - (CENTERX - (rcFocus.right - rcFocus.left) / 2);
	rcSour.top = rcFocus.top - (CENTERY - (rcFocus.bottom - rcFocus.top) / 2);
	rcSour.right = rcSour.left + WINSIZEX;
	rcSour.bottom = rcSour.top + WINSIZEY;

	x = _rc.left - rcSour.left;
	y = _rc.top - rcSour.top;

	_image->frameRender(getMemDC(), x, y);
}
