#pragma once

class cPyramid : public cGameObject
{
private:
	std::vector<ST_PN_VERTEX>	m_vecVertex;
	D3DMATERIAL9				m_stMtl;

public:
	cPyramid(void);
	~cPyramid(void);

	void Setup(D3DXCOLOR c, D3DXMATRIXA16* pmat = NULL);
	void Render();
};

