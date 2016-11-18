#pragma once
class cRay
{
private:
	D3DXVECTOR3 m_vRayOrg;
	D3DXVECTOR3 m_vRayDir;

public:
	cRay(void);
	~cRay(void);

	static cRay RayAtViewSpace(int x, int y);
	static cRay RayAtWorldSpace(int x, int y);
	bool IsPicked(ST_SPHERE* pSphere);
};

