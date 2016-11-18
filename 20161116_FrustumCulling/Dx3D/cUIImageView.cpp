#include "StdAfx.h"
#include "cUIImageView.h"


cUIImageView::cUIImageView(void)
	: m_pTexture(NULL)
{
}

cUIImageView::~cUIImageView(void)
{
	SAFE_RELEASE(m_pTexture);
}

void cUIImageView::Render( LPD3DXSPRITE pSprite )
{
	RECT rcSrc;
	SetRect(&rcSrc, 0, 0, m_stSize.fWidth, m_stSize.fHeight);
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_pTexture,
		&rcSrc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255));
	pSprite->End();

	cUIObject::Render(pSprite);
}
