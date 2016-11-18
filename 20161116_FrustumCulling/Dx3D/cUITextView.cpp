#include "StdAfx.h"
#include "cUITextView.h"


cUITextView::cUITextView(void)
	: m_sText("")
	, m_pFont(NULL)
	, m_dwDrawTextFormat(DT_LEFT | DT_TOP)
	, m_dwColor(D3DCOLOR_XRGB(255, 255, 255))
{
}

cUITextView::~cUITextView(void)
{
	SAFE_RELEASE(m_pFont);
}

void cUITextView::Render( LPD3DXSPRITE pSprite )
{
	RECT rc;
	SetRect(&rc,
		m_matWorld._41,
		m_matWorld._42,
		m_matWorld._41 + m_stSize.fWidth,
		m_matWorld._42 + m_stSize.fHeight);

	m_pFont->DrawText(NULL, 
		m_sText.c_str(), 
		m_sText.length(), 
		&rc,
		m_dwDrawTextFormat, 
		m_dwColor);

	cUIObject::Render(pSprite);
}
