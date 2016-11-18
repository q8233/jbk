#include "StdAfx.h"
#include "cHeightMap.h"
#include "cMtlTex.h"

cHeightMap::cHeightMap(void)
	: m_pMtlTex(NULL)
	, m_nTileN(0)
	, m_pMesh(NULL)
{
}


cHeightMap::~cHeightMap(void)
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pMesh);
}

void cHeightMap::Load( char* szFolder, char* szRaw, char* szTexture, DWORD dwBytePerPixel /*= 1*/ )
{
	std::vector<ST_PNT_VERTEX> vecVertex;
	std::string sFolder(szFolder);
	std::string sRaw(szRaw);
	
	FILE* fp = NULL;
	fopen_s(&fp, (sFolder + "/" + sRaw).c_str(), "rb");
	
	fseek(fp, 0, SEEK_END);
	int nNumByte = ftell(fp);
	int nSqrtByte = (int)sqrt(nNumByte + 0.0001f);
	assert(nSqrtByte * nSqrtByte == nNumByte && "정사각 형태의 Raw파일만 사용 가능합니다.");
	int nNumRowVertex = nSqrtByte / dwBytePerPixel;
	int nNumColVertex = nNumRowVertex;
	m_nTileN = nNumRowVertex - 1; //256
	fseek(fp, 0, SEEK_SET);
	
	m_vecFace.reserve(nNumRowVertex * nNumRowVertex);
	vecVertex.reserve(nNumRowVertex * nNumRowVertex);
	for (int z = 0; z < nNumRowVertex; ++z)
	{
		for (int x = 0; x < nNumRowVertex; ++x)
		{
			ST_PNT_VERTEX v;
			v.p = D3DXVECTOR3(x, (float)(fgetc(fp)) / 10.f, z);
			v.t = D3DXVECTOR2(x / (float)m_nTileN, z / (float)m_nTileN);
			v.n = D3DXVECTOR3(0, 1, 0);
			vecVertex.push_back(v);
			m_vecFace.push_back(v.p);
			if(dwBytePerPixel > 1)
			{
				for (int  i = 0; i < dwBytePerPixel - 1; ++i)
				{
					fgetc(fp);
				}
			}
		}
	}
	fclose(fp);

	for (int z = 1; z < m_nTileN; ++z)
	{
		for (int x = 1; x < m_nTileN; ++x)
		{
			int l = (z + 0) * nNumColVertex + x - 1;
			int r = (z + 0) * nNumColVertex + x + 1;
			int u = (z + 1) * nNumColVertex + x + 0;
			int d = (z - 1) * nNumColVertex + x + 0;
			D3DXVECTOR3 lr = m_vecFace[r] - m_vecFace[l];
			D3DXVECTOR3 du = m_vecFace[u] - m_vecFace[d];
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);
			int i = z * nNumColVertex + x;
			vecVertex[i].n = n;
		}
	}

	std::vector<DWORD> vecIndex;
	vecIndex.reserve(m_nTileN * m_nTileN * 2 * 3);
	for (int z = 0; z < m_nTileN; ++z)
	{
		for (int x = 0; x < m_nTileN; ++x)
		{
			int _0 = (z + 0) * nNumColVertex + x;
			int _1 = (z + 1) * nNumColVertex + x;
			int _2 = (z + 0) * nNumColVertex + x + 1;
			int _3 = (z + 1) * nNumColVertex + x + 1;
			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);

			vecIndex.push_back(_3);
			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
		}
	}

	D3DXCreateMeshFVF(vecIndex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	memset(pA, 0, sizeof(DWORD) * (vecIndex.size() / 3));
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT | 
		D3DXMESHOPT_VERTEXCACHE |
		D3DXMESHOPT_COMPACT,
		&vecAdj[0],
		NULL,
		NULL,
		NULL);

	m_pMtlTex = new cMtlTex;
	D3DMATERIAL9 stMtl;
	ZeroMemory(&stMtl, sizeof(D3DMATERIAL9));
	stMtl.Ambient = stMtl.Diffuse = stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_pMtlTex->SetMtl(stMtl);
	std::string sTexture(szTexture);
	m_pMtlTex->SetTexture(g_pTextureManager->GetTexture(sFolder + "/" + sTexture));
}

bool cHeightMap::GetHeight( IN float x, OUT float& y, IN float z )
{
	if (x < 0 || x > m_nTileN || z < 0 || z > m_nTileN)
	{
		y = 0.0f;
		return false;
	}
	
	int nX = (int)x;
	int nZ = (int)z;

	// 1--3
	// |\ |
	// | \|
	// 0--2
	int _0 = (nZ + 0) * (m_nTileN + 1) + nX;
	int _1 = (nZ + 1) * (m_nTileN + 1) + nX;
	int _2 = (nZ + 0) * (m_nTileN + 1) + nX + 1;
	int _3 = (nZ + 1) * (m_nTileN + 1) + nX + 1;

	float fDeltaX = x - nX;
	float fDeltaZ = z - nZ;
	if (fDeltaX + fDeltaZ < 1)
	{
		D3DXVECTOR3 _01 = m_vecFace[_1] - m_vecFace[_0];
		D3DXVECTOR3 _02 = m_vecFace[_2] - m_vecFace[_0];
		y = (_01 * fDeltaZ + _02 * fDeltaX).y;
		y += m_vecFace[_0].y;
	}
	else
	{
		fDeltaX = 1.0f - fDeltaX;
		fDeltaZ = 1.0f - fDeltaZ;

		D3DXVECTOR3 _31 = m_vecFace[_1] - m_vecFace[_3];
		D3DXVECTOR3 _32 = m_vecFace[_2] - m_vecFace[_3];
		y = (_31 * fDeltaX + _32 * fDeltaZ).y;
		y += m_vecFace[_3].y;
	}
	return true;
}

void cHeightMap::Render()
{
	D3DXMATRIXA16 matI;
	D3DXMatrixIdentity(&matI);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
	g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMtl());
	m_pMesh->DrawSubset(0);
}
