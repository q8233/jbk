#pragma once

class cUIObject : public cObject
{
private:
	std::vector<cUIObject*> m_vecChild;
	SYNTHESIZE(int, m_nTag, Tag);
	SYNTHESIZE(D3DXVECTOR3, m_vLocalPos, LocalPos);
	SYNTHESIZE(ST_SIZE, m_stSize, Size);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);

public:
	cUIObject(void);
	virtual ~cUIObject(void);

	virtual void AddChild(cUIObject* pChild);
	virtual void Update();
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual cUIObject* GetChildByTag(int nTag);
};

