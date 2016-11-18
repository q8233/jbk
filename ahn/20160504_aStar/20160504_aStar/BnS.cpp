#include "stdafx.h"
#include "BnS.h"


BnS::BnS()
{
}


BnS::~BnS()
{
}


HRESULT BnS::init()
{
	_swordMan = IMAGEMANAGER->addImage("bns", "image/올려베기&승천.bmp", 14800, 700, true, RGB(255, 0, 255));

	//애니메이션 테스트1
	//디폴트 애니메이션
	_ani1 = new animation;
	_ani1->init(_swordMan->getWidth(), _swordMan->getHeight(), _swordMan->getWidth() / 37, 700);
	_ani1->setDefPlayFrame(true, true);
	_ani1->setFPS(8);
	
	//애니메이션 테스트2
	_ani2 = new animation;
	_ani2->init(_swordMan->getWidth(), _swordMan->getHeight(), _swordMan->getWidth() / 37, 700);
	_ani2->setPlayFrame(0, 10, true, true);
	_ani2->setFPS(8);

	//애니메이션 테스트3
	int arrAni[] = { 3, 7, 14, 25, 8, 30, 21, 22, 11 };
	_ani3 = new animation;
	_ani3->init(_swordMan->getWidth(), _swordMan->getHeight(), _swordMan->getWidth() / 37, 700);
	_ani3->setPlayFrame(arrAni, _countof(arrAni), true);
	_ani3->setFPS(8);

	return S_OK;
}

void BnS::release()
{
	SAFE_DELETE(_ani1);
	SAFE_DELETE(_ani2);
	SAFE_DELETE(_ani3);
}

void BnS::update()
{
	if (KEYMANAGER->isOnceKeyDown('A'))	_ani1->start();

	if (KEYMANAGER->isOnceKeyDown('S')) _ani2->start();

	if (KEYMANAGER->isOnceKeyDown('D')) _ani3->start();

	_ani1->frameUpdate(TIMEMANAGER->getElapsedTime());
	_ani2->frameUpdate(TIMEMANAGER->getElapsedTime());
	_ani3->frameUpdate(TIMEMANAGER->getElapsedTime());
}

void BnS::render()
{
	_swordMan->aniRender(getMemDC(), 0, 100, _ani1);

	_swordMan->aniRender(getMemDC(), 450, 100, _ani2);

	_swordMan->aniRender(getMemDC(), 900, 100, _ani3);
}