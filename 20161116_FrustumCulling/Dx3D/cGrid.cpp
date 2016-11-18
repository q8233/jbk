#include "StdAfx.h"
#include "cGrid.h"
#include "cPyramid.h"

cGrid::cGrid(void)
	: m_pVB(NULL)
	, m_nNumLine(0)
{
}

cGrid::~cGrid(void)
{
	SAFE_RELEASE(m_pVB);

	for each(auto p in m_vecPyramid)
	{
		SAFE_RELEASE(p);
	}
}

void cGrid::Setup(int nLine)
{
	std::vector<ST_PC_VERTEX>	vecVertex;

	int nHalf = nLine / 2;
	
	float fWidth = 1.0f;
	float fMax = fWidth * nHalf;
	
	D3DXCOLOR c;

	vecVertex.reserve(nHalf * 4 * 2 + 4);
	for (int i = 1; i <= nHalf; ++i)
	{
		c = D3DCOLOR_XRGB(128, 128, 128);
		if (i % 5 == 0)
		{
			c = D3DCOLOR_XRGB(255, 255, 255);
		}
		vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0,  i * fWidth), c));
		vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0,  i * fWidth), c));

		vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0, -i * fWidth), c));
		vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0, -i * fWidth), c));

		vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i * fWidth, 0, -fMax), c));
		vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i * fWidth, 0,  fMax), c));

		vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i * fWidth, 0, -fMax), c));
		vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i * fWidth, 0,  fMax), c));
	}

	c = D3DXCOLOR(1, 0, 0, 1);
	vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0,  0), c));
	vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0,  0), c));

	c = D3DXCOLOR(0, 1, 0, 1);
	vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0, -fMax, 0), c));
	vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0,  fMax, 0), c));

	c = D3DXCOLOR(0, 0, 1, 1);
	vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0, 0, -fMax), c));
	vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0, 0,  fMax), c));

	g_pD3DDevice->CreateVertexBuffer(vecVertex.size() * sizeof(ST_PC_VERTEX),
		D3DUSAGE_WRITEONLY,
		ST_PC_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	ST_PC_VERTEX* pV = NULL;
	m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PC_VERTEX));
	m_pVB->Unlock();

	m_nNumLine = vecVertex.size() / 2;

	D3DXMATRIXA16 matS, matR, matWorld;

	cPyramid* pPyramid = NULL;

	c = D3DCOLOR_XRGB(255, 0, 0);
	D3DXMatrixScaling(&matS, 0.1f, 2.0f, 0.1f);
	D3DXMatrixRotationZ(&matR, D3DX_PI / 2.0f);
	matWorld = matS * matR;
	pPyramid = new cPyramid;
	pPyramid->Setup(c, &matWorld);
	m_vecPyramid.push_back(pPyramid);

	c = D3DCOLOR_XRGB(0, 255, 0);
	D3DXMatrixScaling(&matS, 0.1f, 2.0f, 0.1f);
	D3DXMatrixRotationZ(&matR, D3DX_PI);
	matWorld = matS * matR;
	pPyramid = new cPyramid;
	pPyramid->Setup(c, &matWorld);
	m_vecPyramid.push_back(pPyramid);

	c = D3DCOLOR_XRGB(0, 0, 255);
	D3DXMatrixScaling(&matS, 0.1f, 2.0f, 0.1f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	matWorld = matS * matR;
	pPyramid = new cPyramid;
	pPyramid->Setup(c, &matWorld);
	m_vecPyramid.push_back(pPyramid);
}

void cGrid::Render( )
{
	DWORD dwLighting = 0;
	g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &dwLighting);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTexture(0, NULL);
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PC_VERTEX));
	g_pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_nNumLine);

// 	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
// 		vecVertex.size() / 2,
// 		&vecVertex[0],
// 		sizeof(ST_PC_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	for each(auto p in m_vecPyramid)
	{
		p->Render();
	}

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, dwLighting);
}
