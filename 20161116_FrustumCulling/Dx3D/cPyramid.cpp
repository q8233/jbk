#include "StdAfx.h"
#include "cPyramid.h"


cPyramid::cPyramid(void)
{
}

cPyramid::~cPyramid(void)
{
}

void cPyramid::Setup(D3DXCOLOR c, D3DXMATRIXA16* pmat /*= NULL*/)
{
	ST_PN_VERTEX v;
	
	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-1, -1,  1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); m_vecVertex.push_back(v);

	if(pmat)
	{	
		for(size_t i = 0; i < m_vecVertex.size(); ++i)
		{
			D3DXVec3TransformCoord(&m_vecVertex[i].p,
				&m_vecVertex[i].p,
				pmat); 
		}
	}

	for(size_t i = 0; i < m_vecVertex.size(); i += 3)
	{
		D3DXVECTOR3 n;
		D3DXVECTOR3 v01 = m_vecVertex[i + 1].p - m_vecVertex[i].p;
		D3DXVECTOR3 v02 = m_vecVertex[i + 2].p - m_vecVertex[i].p;
		D3DXVec3Cross(&n, &v01, &v02); 
		D3DXVec3Normalize(&n, &n);
		m_vecVertex[i].n = m_vecVertex[i + 1].n = m_vecVertex[i + 2].n = n;
	}

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = c;
}

void cPyramid::Render()
{
	//D3DXMATRIXA16 matWorld;
	//D3DXMatrixTranslation(&matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PN_VERTEX));
}
