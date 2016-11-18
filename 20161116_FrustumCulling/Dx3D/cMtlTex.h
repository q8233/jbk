#pragma once

class cMtlTex : public cObject
{
private:
	SYNTHESIZE(DWORD, m_dwAttrID, AttrID);
	SYNTHESIZE_PASS_BY_REF(D3DMATERIAL9, m_stMtl, Mtl);
	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9, m_pTexture, Texture);

public:
	cMtlTex(void);
	~cMtlTex(void);
// 	void SetTexture(LPDIRECT3DTEXTURE9 pTexture)
// 	{
// 		if(m_pTexture != pTexture)
// 		{
// 			SAFE_RELEASE(m_pTexture);
// 			SAFE_ADDREF(pTexture);
// 			m_pTexture = pTexture;
// 		}
// 	}
};

