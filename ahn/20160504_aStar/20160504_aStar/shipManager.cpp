#include "stdafx.h"
#include "shipManager.h"


shipManager::shipManager()
{
}


shipManager::~shipManager()
{
}

HRESULT shipManager::init()
{
	_space = new space;
	_space->init();

	//배틀
	_ship[SHIPKIND_BATTLE] = new battle;
	_ship[SHIPKIND_BATTLE]->init("battle", WINSIZEX / 3, CENTERY);

	_ship[SHIPKIND_CARRIER] = new carrier;
	_ship[SHIPKIND_CARRIER]->init("carrier", WINSIZEX / 3 * 2, CENTERY);

	//포커스
	_focusShip = _ship[SHIPKIND_BATTLE];
	_space->setShip(_focusShip);

	return S_OK;
}

void shipManager::release()
{
	for (int i = 0; i < SHIPKIND_END; i++)
	{
		_ship[i]->release();
		SAFE_DELETE(_ship[i]);
	}


}

void shipManager::update()
{
	//포커스 변환
	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		_focusShip = _ship[SHIPKIND_BATTLE];
		_space->setShip(_focusShip);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		_focusShip = _ship[SHIPKIND_CARRIER];
		_space->setShip(_focusShip);
	}

	//쉽 업데이트
	for (int i = 0; i < SHIPKIND_END; i++)
	{
		_ship[i]->update();
	}

	//현재 포커스로 맞춰진 쉽 컨트롤
	_focusShip->keyControl();

}

void shipManager::render()
{
	_space->render();

	for (int i = 0; i < SHIPKIND_END; i++)
	{
		//포커스 쉽과 같으면...
		if (_focusShip == _ship[i])
		{
			_focusShip->focusDraw();
		}
		else
		{
			_ship[i]->defaultDraw(_focusShip->getRect());
		}
	}
}
