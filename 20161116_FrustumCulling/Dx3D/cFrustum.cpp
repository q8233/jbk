#include "StdAfx.h"
#include "cFrustum.h"


cFrustum::cFrustum(void)
{
	int n = 0;
	m_aProjVertex[n++] = D3DXVECTOR3(-1.0f, -1.0f,  0.0f);
	m_aProjVertex[n++] = D3DXVECTOR3(-1.0f,  1.0f,  0.0f);
	m_aProjVertex[n++] = D3DXVECTOR3( 1.0f,  1.0f,  0.0f);
	m_aProjVertex[n++] = D3DXVECTOR3( 1.0f, -1.0f,  0.0f);
	m_aProjVertex[n++] = D3DXVECTOR3(-1.0f, -1.0f,  1.0f);
	m_aProjVertex[n++] = D3DXVECTOR3(-1.0f,  1.0f,  1.0f);
	m_aProjVertex[n++] = D3DXVECTOR3( 1.0f,  1.0f,  1.0f);
	m_aProjVertex[n++] = D3DXVECTOR3( 1.0f, -1.0f,  1.0f);
}

cFrustum::~cFrustum(void)
{
}

void cFrustum::Update()
{
	D3DXMATRIXA16 matInvView, matInvProj, mat;
	
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matInvView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matInvProj);
	
	D3DXMatrixInverse(&matInvView, 0, &matInvView);
	D3DXMatrixInverse(&matInvProj, 0, &matInvProj);
	
	mat = matInvProj * matInvView;

	D3DXVECTOR3 aFrustumVertex[8];
	
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&aFrustumVertex[i], &m_aProjVertex[i], &mat);
	}
	int n = 0;
	D3DXPlaneFromPoints(&m_aPlane[n++], &aFrustumVertex[0], &aFrustumVertex[1], &aFrustumVertex[2]);
	D3DXPlaneFromPoints(&m_aPlane[n++], &aFrustumVertex[6], &aFrustumVertex[5], &aFrustumVertex[4]);
	D3DXPlaneFromPoints(&m_aPlane[n++], &aFrustumVertex[1], &aFrustumVertex[5], &aFrustumVertex[6]);
	D3DXPlaneFromPoints(&m_aPlane[n++], &aFrustumVertex[0], &aFrustumVertex[3], &aFrustumVertex[7]);
	D3DXPlaneFromPoints(&m_aPlane[n++], &aFrustumVertex[1], &aFrustumVertex[0], &aFrustumVertex[4]);
	D3DXPlaneFromPoints(&m_aPlane[n++], &aFrustumVertex[2], &aFrustumVertex[6], &aFrustumVertex[7]);
}

bool cFrustum::IsIn( ST_SPHERE* pSphere )
{
	for (int i = 0; i < 6; ++i)
	{
		if(D3DXPlaneDotCoord(&m_aPlane[i], &pSphere->vCenter) >= pSphere->fRadius)
			return false;
	}
	return true;
}
