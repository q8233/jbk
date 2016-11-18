#include "StdAfx.h"
#include "cSokobanChar.h"


cSokobanChar::cSokobanChar(void)
	: m_vPosition(0, 0, 0)
	, m_vStart(0, 0, 0)
	, m_vDest(0, 0, 0)
	, m_fMoveTime(0.3f)
	, m_fPassedMoveTime(0.0f)
	, m_isMoving(false)
{
}

cSokobanChar::~cSokobanChar(void)
{
}

void cSokobanChar::SetPosition( D3DXVECTOR3& vPosition )
{
	m_vPosition = vPosition;
}

D3DXVECTOR3* cSokobanChar::GetPosition()
{
	return &m_vPosition;
}

void cSokobanChar::Update()
{
	m_fPassedMoveTime += g_pTimeManager->GetDeltaTime();
	
	if(m_fPassedMoveTime >= m_fMoveTime)
	{
		m_vPosition = m_vDest;
		m_isMoving = false;
	}
	else
	{
		float fDelta = m_fPassedMoveTime / m_fMoveTime;
		m_vPosition = (1.0f - fDelta) * m_vStart + fDelta * m_vDest;
	}

	if(m_isMoving)
		return;

	if (GetKeyState('A') & 0x8000)
	{
		m_vStart = m_vPosition;
		m_vDest = m_vPosition;
		m_vDest.x -= 1;
		m_isMoving = true;
		m_fPassedMoveTime = 0.0f;
	}
	else if (GetKeyState('D') & 0x8000)
	{
		m_vStart = m_vPosition;
		m_vDest = m_vPosition;
		m_vDest.x += 1;
		m_isMoving = true;
		m_fPassedMoveTime = 0.0f;
	}
	else if (GetKeyState('W') & 0x8000)
	{
		m_vStart = m_vPosition;
		m_vDest = m_vPosition;
		m_vDest.z += 1;
		m_isMoving = true;
		m_fPassedMoveTime = 0.0f;
	}
	else if (GetKeyState('S') & 0x8000)
	{
		m_vStart = m_vPosition;
		m_vDest = m_vPosition;
		m_vDest.z -= 1;
		m_isMoving = true;
		m_fPassedMoveTime = 0.0f;
	}
}
