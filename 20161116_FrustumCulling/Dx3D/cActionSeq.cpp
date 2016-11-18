#include "StdAfx.h"
#include "cActionSeq.h"


cActionSeq::cActionSeq(void)
	: m_nCurrentActionIndex(0)
{
}

cActionSeq::~cActionSeq(void)
{
	for each(auto p in m_vecAction)
	{
		SAFE_RELEASE(p);
	}
}

void cActionSeq::Start()
{
	m_nCurrentActionIndex = 0;

	if(m_vecAction.size() > 0)
		m_vecAction[0]->Start();
}

void cActionSeq::Update()
{
	if (m_nCurrentActionIndex < m_vecAction.size() && 
		m_vecAction[m_nCurrentActionIndex])
	{
		m_vecAction[m_nCurrentActionIndex]->Update();
	}
}

void cActionSeq::OnActionFinish( cAction* pSender )
{	
	++m_nCurrentActionIndex;

	if(m_nCurrentActionIndex < m_vecAction.size())
		m_vecAction[m_nCurrentActionIndex]->Start();
	else if (m_pDelegate)
	{
		m_pDelegate->OnActionFinish(this);
	}

}

void cActionSeq::AddAction( cAction* pAction )
{
	if(pAction)
	{
		pAction->AddRef();
		pAction->SetDelegate(this);
		m_vecAction.push_back(pAction);
	}
}

cAction* cActionSeq::Create()
{
	return new cActionSeq;
}
