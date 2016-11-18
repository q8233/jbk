#include "stdafx.h"
#include "starwarsScene.h"
#include "battle.h"
#include "carrier.h"

starwarsScene::starwarsScene()
{
}


starwarsScene::~starwarsScene()
{
}

HRESULT starwarsScene::init()
{
	_shipManager = new shipManager;
	_shipManager->init();

	return S_OK;
}

void starwarsScene::release()
{
	_shipManager->release();
	SAFE_DELETE(_shipManager);
}

void starwarsScene::update()
{
	_shipManager->update();
}


void starwarsScene::render()
{
	_shipManager->render();
}