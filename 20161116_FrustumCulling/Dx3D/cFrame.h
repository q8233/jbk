#pragma once
#include "cMtlTex.h"

class cFrame
{
private:
	std::vector<cFrame*> m_vecChild;
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matLocal, LocalTM);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_POS_SAMPLE>, m_vecPosTrack, PosTrack);
	SYNTHESIZE_PASS_BY_REF(std::vector<ST_ROT_SAMPLE>, m_vecRotTrack, RotTrack);
	
public:
	cFrame(void);
	~cFrame(void);

	void Update(int nKeyFrame, D3DXMATRIXA16* pParentWorld);
	void Render();
	void AddChild(cFrame* pChild);
	void Destroy();
	void CalcLocalTM(D3DXMATRIXA16* pParentWorldTM);
	void LocalTraslationMatrix(int nKeyFrame, OUT D3DXMATRIXA16& mat);
	void LocalRotationMatrix(int nKeyFrame, OUT D3DXMATRIXA16& mat);
};

