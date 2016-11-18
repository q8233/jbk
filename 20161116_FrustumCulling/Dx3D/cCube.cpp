#include "StdAfx.h"
#include "cCube.h"


cCube::cCube(void)
	: m_pTexture(NULL)
	, m_fAngleSpeed(0.0f)
	, m_fAngle(0.0f)
	, m_vTranslation(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCube::~cCube(void)
{
	SAFE_RELEASE(m_pTexture);
}

void cCube::Setup(D3DXMATRIXA16* pmat)
{
	std::vector<D3DXVECTOR3>	vecVertex;

	vecVertex.reserve(8);
	vecVertex.push_back(D3DXVECTOR3(-1.0f, -1.0f, -1.0));
	vecVertex.push_back(D3DXVECTOR3(-1.0f,  1.0f, -1.0));
	vecVertex.push_back(D3DXVECTOR3( 1.0f,  1.0f, -1.0));
	vecVertex.push_back(D3DXVECTOR3( 1.0f, -1.0f, -1.0));
	vecVertex.push_back(D3DXVECTOR3(-1.0f, -1.0f,  1.0));
	vecVertex.push_back(D3DXVECTOR3(-1.0f,  1.0f,  1.0));
	vecVertex.push_back(D3DXVECTOR3( 1.0f,  1.0f,  1.0));
	vecVertex.push_back(D3DXVECTOR3( 1.0f, -1.0f,  1.0));

	std::vector<D3DXVECTOR2>	vecTexCoord;
	vecTexCoord.push_back(D3DXVECTOR2(0, 1));
	vecTexCoord.push_back(D3DXVECTOR2(0, 0));
	vecTexCoord.push_back(D3DXVECTOR2(1, 0));

	vecTexCoord.push_back(D3DXVECTOR2(0, 1));
	vecTexCoord.push_back(D3DXVECTOR2(1, 0));
	vecTexCoord.push_back(D3DXVECTOR2(1, 1));

	std::vector<int>		vecIndex;
	vecIndex.reserve(36);
	// Àü
	vecIndex.push_back(0); // ÁÂÇÏ (0, 1)
	vecIndex.push_back(1); // ÁÂ»ó (0, 0)
	vecIndex.push_back(2); // ¿ì»ó (1, 0)

	vecIndex.push_back(0); // ÁÂÇÏ (0, 1)
	vecIndex.push_back(2); // ¿ì»ó (1, 0)
	vecIndex.push_back(3); // ¿ìÇÏ (1, 1)

	// ÈÄ
	vecIndex.push_back(7); // ÁÂÇÏ
	vecIndex.push_back(6); // ÁÂ»ó
	vecIndex.push_back(5); // ¿ì»ó

	vecIndex.push_back(7); // ÁÂÇÏ
	vecIndex.push_back(5); // ¿ì»ó
	vecIndex.push_back(4); // ¿ìÇÏ

	// ÁÂ
	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);

	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);

	// ¿ì
	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);

	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);

	// »ó
	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);

	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);

	// ÇÏ
	vecIndex.push_back(4);
	vecIndex.push_back(0);
	vecIndex.push_back(3);

	vecIndex.push_back(4);
	vecIndex.push_back(3);
	vecIndex.push_back(7);

	std::vector<D3DXVECTOR3> vecNormal;
	vecNormal.push_back(D3DXVECTOR3( 0, 0,-1));
	vecNormal.push_back(D3DXVECTOR3( 0, 0, 1));
	vecNormal.push_back(D3DXVECTOR3(-1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3( 1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3( 0, 1, 0));
	vecNormal.push_back(D3DXVECTOR3( 0,-1, 0));

	m_vecVertex.reserve(36);
	int nTexCoordIndex = 0;
	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		ST_PNT_VERTEX v;
		v.p = vecVertex[vecIndex[i]];
		if(pmat)
		{
			D3DXVec3TransformCoord(&v.p, &v.p, pmat);
		}
		v.n = vecNormal[i / 6];
		v.t = vecTexCoord[i % 6];
		m_vecVertex.push_back(v);
	}

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
}

void cCube::Render()
{
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PNT_VERTEX));

	for each(auto p in m_vecChild)
	{
		p->Render();
	}
}

void cCube::SetTexture( LPDIRECT3DTEXTURE9 pTexture )
{
	if(!m_pTexture && pTexture)
	{
		m_pTexture = pTexture;
		m_pTexture->AddRef();
	}
}

void cCube::AddChild( cCube* pChild )
{
	m_vecChild.push_back(pChild);
}

void cCube::Update(D3DXMATRIXA16* pmatParentWorldTM)
{
	m_fAngle += m_fAngleSpeed * g_pTimeManager->GetDeltaTime();

	if(m_fAngle > D3DX_PI / 4.0f)
	{
		m_fAngle = D3DX_PI / 4.0f;
		m_fAngleSpeed *= -1;
	}

	if(m_fAngle < -D3DX_PI / 4.0f)
	{
		m_fAngle = -D3DX_PI / 4.0f;
		m_fAngleSpeed *= -1;
	}

	D3DXMATRIXA16 matR;
	D3DXMatrixRotationX(&matR, m_fAngle);

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vTranslation.x, m_vTranslation.y, m_vTranslation.z);

	D3DXMATRIXA16 matLocalTM = matR * matT;
	if(pmatParentWorldTM)
	{
		m_matWorld = matLocalTM * *pmatParentWorldTM;
	}
	else
	{
		m_matWorld = matLocalTM;
	}

	for each(auto p in m_vecChild)
	{
		p->Update(&m_matWorld);
	}
}

void cCube::Destroy()
{
	for each(auto p in m_vecChild)
	{
		p->Destroy();
	}
	delete this;
}

