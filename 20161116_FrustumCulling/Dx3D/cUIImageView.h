#pragma once

class cUIImageView : public cUIObject
{
protected:
	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9, m_pTexture, Texture);

public:
	cUIImageView(void);
	virtual ~cUIImageView(void);

	// cUIObject override
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

