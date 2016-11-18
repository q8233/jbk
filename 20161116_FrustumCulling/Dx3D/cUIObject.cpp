#include "StdAfx.h"
#include "cUIObject.h"


cUIObject::cUIObject(void)
	: m_vLocalPos(0, 0, 0)
	, m_stSize(0, 0)
	, m_pParent(NULL)
	, m_nTag(-1)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cUIObject::~cUIObject(void)
{
	for each(auto p in m_vecChild)
	{
		p->Release();
	}
}

void cUIObject::AddChild( cUIObject* pChild )
{
	SAFE_ADDREF(pChild);
	if(pChild)
	{
		pChild->SetParent(this);
		m_vecChild.push_back(pChild);
	}
}

void cUIObject::Update()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._41 = m_vLocalPos.x;
	m_matWorld._42 = m_vLocalPos.y;
	m_matWorld._43 = m_vLocalPos.z;
	if (m_pParent)
	{
		m_matWorld._41 += m_pParent->m_matWorld._41;
		m_matWorld._42 += m_pParent->m_matWorld._42;
		m_matWorld._43 += m_pParent->m_matWorld._43;
	}

	for each(auto p in m_vecChild)
	{
		p->Update();
	}
}

void cUIObject::Render(LPD3DXSPRITE pSprite)
{
	for each(auto p in m_vecChild)
	{
		p->Render(pSprite);
	}
}

cUIObject* cUIObject::GetChildByTag( int nTag )
{
	if (m_nTag == nTag)
		return this;

	cUIObject* pChild = NULL;
	for each(auto p in m_vecChild)
	{
		pChild = p->GetChildByTag(nTag);
		if(pChild)
			return pChild;
	}
	return NULL;
}
