#pragma once
#include "iMap.h"

class cMtlTex;

class cHeightMap : public iMap
{
private:
	cMtlTex*					m_pMtlTex;
	std::vector<D3DXVECTOR3>	m_vecFace;
	int							m_nTileN;
	LPD3DXMESH					m_pMesh;

public:
	cHeightMap(void);
	virtual ~cHeightMap(void);

	void Load(char* szFolder, char* szRaw, char* szTexture, DWORD dwBytePerPixel = 1);

	// iMap override
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;
	virtual void Render() override;
};

