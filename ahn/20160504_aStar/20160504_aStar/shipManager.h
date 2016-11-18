#pragma once

#include "gameNode.h"
#include "battle.h"
#include "carrier.h"
#include "space.h"

enum SHIPKIND
{
	SHIPKIND_BATTLE,
	SHIPKIND_CARRIER,
	SHIPKIND_END
};

class shipManager : public gameNode
{
	ship* _ship[SHIPKIND_END];
	ship* _focusShip;

	space* _space;

public:
	shipManager();
	~shipManager();

	HRESULT init();
	void release();
	void update();
	void render();
};

