#include "StdAfx.h"
#include "cActionRepeat.h"


cActionRepeat::cActionRepeat(void)
	: m_pAction(NULL)
{
}

cActionRepeat::~cActionRepeat(void)
{
	SAFE_RELEASE(m_pAction);
}

void cActionRepeat::Update()
{
	if(m_pAction)
	{
		m_pAction->Update();
	}
}

void cActionRepeat::OnActionFinish( cAction* pSender )
{
	if(m_pAction)
		m_pAction->Start();
}

cAction* cActionRepeat::Create()
{
	return new cActionRepeat;
}
