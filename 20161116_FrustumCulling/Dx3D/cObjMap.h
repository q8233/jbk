#pragma once

#include "iMap.h"

class cGroup;

class cObjMap : public iMap
{
private:
	std::vector<cGroup*>		m_vecGroup;
	std::vector<D3DXVECTOR3>	m_vecSurface;

public:
	cObjMap(void);
	virtual ~cObjMap(void);

	virtual void Load(char* szMap, char* szSurface, D3DXMATRIXA16* pmat);

	// iMap override
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;
	virtual void Render() override;
};

