#include "StdAfx.h"
#include "cRay.h"


cRay::cRay(void)
	: m_vRayOrg(0, 0 ,0)
	, m_vRayDir(0, 0 ,0)
{
}


cRay::~cRay(void)
{
}

cRay cRay::RayAtViewSpace( int x, int y )
{
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);
	D3DXMATRIXA16 matProj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	cRay r;
	r.m_vRayDir.x = ( 2.0f * x / vp.Width - 1) / matProj._11;
	r.m_vRayDir.y = (-2.0f * y / vp.Height + 1) / matProj._22;
	r.m_vRayDir.z = 1.0f;
	D3DXVec3Normalize(&r.m_vRayDir, &r.m_vRayDir);
	return r;
}

cRay cRay::RayAtWorldSpace( int x, int y )
{
	cRay r = cRay::RayAtViewSpace(x, y);
	D3DXMATRIXA16 matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);
	D3DXVec3TransformNormal(&r.m_vRayDir, &r.m_vRayDir, &matInvView);
	D3DXVec3TransformCoord(&r.m_vRayOrg, &r.m_vRayOrg, &matInvView);
	D3DXVec3Normalize(&r.m_vRayDir, &r.m_vRayDir);
	return r;
}

bool cRay::IsPicked( ST_SPHERE* pSphere )
{
	assert(pSphere && "구 정보는 줘야지 검사하지 ");

	D3DXVECTOR3 q = m_vRayOrg - pSphere->vCenter;
	D3DXVECTOR3 v = m_vRayDir;

	// (q.v) * (q.v) - v.v * (q.q - r*r)
	float qq = D3DXVec3Dot(&q, &q);
	float qv = D3DXVec3Dot(&q, &v);
	float vv = D3DXVec3Dot(&v, &v);
	float rr = pSphere->fRadius * pSphere->fRadius;
	float f = qv * qv - vv * (qq - rr);
	return qv * qv - vv * (qq - rr) > 0;
}
