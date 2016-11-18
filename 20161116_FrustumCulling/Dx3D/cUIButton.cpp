#include "StdAfx.h"
#include "cUIButton.h"


cUIButton::cUIButton(void)
	: m_eButtonStatus(E_NORMAL)
	, m_pDelegate(NULL)
{
	for (int i = 0; i < E_COUNT; ++i)
	{
		m_aTexture[i] = NULL;
	}
}


cUIButton::~cUIButton(void)
{
}

void cUIButton::SetTexture( char* szNor, char* szOvr, char* szSel )
{
	D3DXIMAGE_INFO stImageInfo;
	if(m_aTexture[E_NORMAL] == NULL)
	{
		SAFE_ADDREF(m_aTexture[E_NORMAL]);
		m_aTexture[E_NORMAL] = g_pTextureManager->GetTexture(szNor, &stImageInfo);
	}
	
	if(m_aTexture[E_MOUSE_OVER] == NULL)
	{
		SAFE_ADDREF(m_aTexture[E_MOUSE_OVER]);
		m_aTexture[E_MOUSE_OVER] = g_pTextureManager->GetTexture(szOvr, &stImageInfo);
	}

	if(m_aTexture[E_SELECTED] == NULL)
	{
		SAFE_ADDREF(m_aTexture[E_SELECTED]);
		m_aTexture[E_SELECTED] = g_pTextureManager->GetTexture(szSel, &stImageInfo);
	}

	m_stSize.fWidth = stImageInfo.Width;
	m_stSize.fHeight = stImageInfo.Height;
}

void cUIButton::Update()
{
	cUIObject::Update();

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT rc;
	SetRect(&rc,
		m_matWorld._41,
		m_matWorld._42,
		m_matWorld._41 + m_stSize.fWidth,
		m_matWorld._42 + m_stSize.fHeight);

	if (PtInRect(&rc, pt))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_eButtonStatus == E_MOUSE_OVER)
			{
				m_eButtonStatus = E_SELECTED;
			}
		}
		else
		{
			if(m_eButtonStatus == E_SELECTED)
			{
				if(m_pDelegate)
					m_pDelegate->OnClick(this);
			}
			m_eButtonStatus = E_MOUSE_OVER;
		}
	}
	else
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
		}
		else
		{
			m_eButtonStatus = E_NORMAL;
		}
	}
}

void cUIButton::Render( LPD3DXSPRITE pSprite )
{
	RECT rc;
	SetRect(&rc, 0, 0, m_stSize.fWidth, m_stSize.fHeight);
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_aTexture[m_eButtonStatus],
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255));
	pSprite->End();

	cUIObject::Render(pSprite);
}
