#include "stdafx.h"
#include "bullets.h"


missile::missile()
{
}


missile::~missile()
{
}

HRESULT missile::init(int bulletMax, float range)
{
	_range = range;

	for (int i = 0; i < bulletMax; i++)
	{
		tagBullet bullet;
		ZeroMemory(&bullet, sizeof(tagBullet)); //memset�� ���� �޸𸮸� 0���� �ʱ�ȭ ��Ų��.
		bullet.bulletImage = new image;
		bullet.bulletImage->init("image/missile.bmp", 26, 124, true, RGB(255, 0, 255));
		bullet.speed = 5.0f;
		bullet.fire = false;

		_vBullet.push_back(bullet);
	}

	return S_OK;
}

void missile::release()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		SAFE_DELETE(_viBullet->bulletImage);
		_viBullet = _vBullet.erase(_viBullet);
	}
}

void missile::update()
{
	move();
}

void missile::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->fire) continue;

		_viBullet->bulletImage->render(getMemDC(), _viBullet->rc.left,
			_viBullet->rc.top, 0, 0, _viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());
	}
}

//�߻� �Լ�
void missile::fire(float x, float y)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->fire)continue;
		//�� �Ʒ��� �����Դٴ� ���� �߻簡 ���� �� �� �̻��� �̶�� ����?

		_viBullet->fire = true;
		_viBullet->x = _viBullet->fireX = x;
		_viBullet->y = _viBullet->fireY = y;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());
		break;
	}
}

//�̻��� ��������
void missile::move()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->fire) continue;
		//�� �Ʒ��� �����Դٴ� ���� �߻�� �̻����� �ִٴ� �Ŵ�.

		_viBullet->y -= _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());

		if (_range < MY_UTIL::getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet->fire = false;
		}
	}
}

void missile::removeMissile(int arrNum)
{
	SAFE_DELETE(_vBullet[arrNum].bulletImage);
	_vBullet.erase(_vBullet.begin() + arrNum);
}

bullet::bullet()
{

}

bullet::~bullet()
{

}

HRESULT bullet::init(const char* imageName, int bulletMax, float range)
{
	_imageName = imageName;
	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}

void bullet::release()
{

}

void bullet::update()
{
	move();
}

void bullet::render()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->bulletImage->render(getMemDC(), _viBullet->rc.left, _viBullet->rc.top);
	}
}


void bullet::fire(float x, float y, float angle, float speed)
{
	if (_bulletMax < _vBullet.size()) return;

	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));

	bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.radius = bullet.bulletImage->getFrameWidth() / 2;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.rc = RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getFrameWidth(),
		bullet.bulletImage->getFrameHeight());

	_vBullet.push_back(bullet);
}


void bullet::move()
{
	//���Ϳ� ��Ƶ� �༮�� ��������
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());

		//��Ÿ� ������ ������ �����ش�
		if (_range < MY_UTIL::getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else
		{
			_viBullet++;
		}
	}
}

void bullet::removeBullet(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}