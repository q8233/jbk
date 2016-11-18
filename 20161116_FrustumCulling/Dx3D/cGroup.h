#pragma once
#include "cMtlTex.h"

class cGroup
{
private:
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	int						m_nNumTri;
	//SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	cGroup(void);
	~cGroup(void);

	void Render();
	void SetVertex(std::vector<ST_PNT_VERTEX>& vecVertex);
};

