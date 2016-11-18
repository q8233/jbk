#pragma once

class cUIButton;

class iUIButtonDelegate
{
public:
	virtual void OnClick(cUIButton* pSender) = 0;
};

class cUIButton : public cUIObject
{
protected:
	enum eButtonStatus
	{
		E_NORMAL,
		E_MOUSE_OVER,
		E_SELECTED,
		E_COUNT
	};
	
	LPDIRECT3DTEXTURE9	m_aTexture[E_COUNT];
	eButtonStatus		m_eButtonStatus;
	SYNTHESIZE(iUIButtonDelegate*, m_pDelegate, Delegate);
public:
	cUIButton(void);
	virtual ~cUIButton(void);

	virtual void SetTexture(char* szNor, char* szOvr, char* szSel);

	// cUIObject override
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

