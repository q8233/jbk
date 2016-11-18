#pragma once

#include "cAction.h"

class cGameObject : public cObject
{
private:
	D3DXVECTOR3 m_vPrevPosition;

	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);

public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void Update();
};

