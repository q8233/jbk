#include "StdAfx.h"
#include "cActionMove.h"


cActionMove::cActionMove(void)
	: m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
{
}

cActionMove::~cActionMove(void)
{
}

void cActionMove::Start()
{
	m_fPassedTime = 0;
	if(m_pTarget)
	{
		m_pTarget->SetPosition(m_vFrom);
	}
}

void cActionMove::Update()
{
	m_fPassedTime += g_pTimeManager->GetDeltaTime();

	if(m_fPassedTime > m_fActionTime)
	{
		if (m_pDelegate)
		{
			m_pDelegate->OnActionFinish(this);
		}
		return;
	}
	if (m_pTarget)
	{
		float t = m_fPassedTime / m_fActionTime;
		D3DXVECTOR3 p;
		D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
		m_pTarget->SetPosition(p);
	}
}

cAction* cActionMove::Create()
{
	return new cActionMove;
}
