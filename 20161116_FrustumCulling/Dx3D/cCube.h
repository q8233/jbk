#pragma once

class cCube
{
private:
	std::vector<ST_PNT_VERTEX>	m_vecVertex;
	LPDIRECT3DTEXTURE9			m_pTexture;
	std::vector<cCube*>			m_vecChild;
	//float						m_fAngleSpeed;
	float						m_fAngle;
	D3DMATERIAL9				m_stMtl;

	SYNTHESIZE(float, m_fAngleSpeed, AngleSpeed);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vTranslation, Translation);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);
public:
	cCube(void);
	~cCube(void);

// 	float GetAngleSpeed()
// 	{
// 		return m_fAngleSpeed;
// 	}
// 
// 	void SetAngleSpeed(float f)
// 	{
// 		m_fAngleSpeed = f;
// 	}

	void Setup(D3DXMATRIXA16* pmat);
	void Render();
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);
	void AddChild(cCube* pChild);
	void Update(D3DXMATRIXA16* pmatParentWorldTM);
	void Destroy();
};

