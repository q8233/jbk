#pragma once

class cPyramid;

class cGrid
{
private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	int							m_nNumLine;
	std::vector<cPyramid*>		m_vecPyramid;

public:
	cGrid(void);
	~cGrid(void);

	void Setup(int nLine);
	void Render();
};

