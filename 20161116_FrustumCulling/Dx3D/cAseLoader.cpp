#include "StdAfx.h"
#include "cAseLoader.h"
#include "Asciitok.h"
#include "cMtlTex.h"
#include "cFrame.h"

cAseLoader::cAseLoader(void)
	: m_fp(NULL)
{
}

cAseLoader::~cAseLoader(void)
{
}

cFrame* cAseLoader::Load( char* szFullPath )
{
	cFrame* pRoot = NULL;
	fopen_s(&m_fp, szFullPath, "r");
	while(char* szToken = GetToken())
	{
		if (IsEqual(szToken, ID_SCENE))
		{
			SkipBlock();
		}
		else if(IsEqual(szToken, ID_MATERIAL_LIST))
		{
			ProcessMATERIAL_LIST();
		}
		else if(IsEqual(szToken, ID_GEOMETRY))
		{
			cFrame* pFrame = ProcessGEOMOBJECT();
			if(pRoot == NULL)
			{
				pRoot = pFrame;
			}
		}
		/*OutputDebugString(szToken);*/
	}
	fclose(m_fp);

	for each(auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}
	
	pRoot->CalcLocalTM(NULL);

	return pRoot;
}

char* cAseLoader::GetToken()
{
	bool isQuote = false;
	int nReadCount = 0;
	while(!feof(m_fp))
	{
		char c = fgetc(m_fp);
		if(c == '\"')
		{
			if(isQuote)
				break;
			isQuote = true;
			continue;
		}

		if(!isQuote && IsWhite(c))
		{
			if(nReadCount == 0) 
			{
				continue;
			}
			break;
		}
		m_szToken[nReadCount++] = c;
	}

	if(nReadCount == 0)
		return NULL;
	
	m_szToken[nReadCount] = '\0';

	return m_szToken;
}

int cAseLoader::GetInteger()
{
	return atoi(GetToken());
}

float cAseLoader::GetFloat()
{
	return (float)atof(GetToken());
}

bool cAseLoader::IsWhite( char c )
{
	return c < 33;
}

void cAseLoader::SkipBlock()
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
	} while (nLevel > 0);
}

bool cAseLoader::IsEqual( char* str1, char* str2 )
{
	return strcmp(str1, str2) == 0;
}

void cAseLoader::ProcessMATERIAL_LIST()
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MATERIAL_COUNT))
		{
			int nMtlCount = GetInteger();
			m_vecMtlTex.resize(nMtlCount);
			for (int i = 0; i < nMtlCount; ++i)
			{
				m_vecMtlTex[i] = new cMtlTex;
			}
		}
		else if (IsEqual(szToken, ID_MATERIAL))
		{
			int nMtlRef = GetInteger();
			ProcessMATERIAL(m_vecMtlTex[nMtlRef]);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL( OUT cMtlTex* pMtlTex )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_AMBIENT))
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMtl();
			stMtl.Ambient.r = GetFloat();
			stMtl.Ambient.g = GetFloat();
			stMtl.Ambient.b = GetFloat();
			stMtl.Ambient.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_DIFFUSE))
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMtl();
			stMtl.Diffuse.r = GetFloat();
			stMtl.Diffuse.g = GetFloat();
			stMtl.Diffuse.b = GetFloat();
			stMtl.Diffuse.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_SPECULAR))
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMtl();
			stMtl.Specular.r = GetFloat();
			stMtl.Specular.g = GetFloat();
			stMtl.Specular.b = GetFloat();
			stMtl.Specular.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_MAP_DIFFUSE))
		{
			ProcessMAP_DIFFUSE(pMtlTex);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMAP_DIFFUSE( OUT cMtlTex* pMtlTex )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_BITMAP))
		{
			pMtlTex->SetTexture(g_pTextureManager->GetTexture(GetToken()));
		}
	} while (nLevel > 0);
}

cFrame* cAseLoader::ProcessGEOMOBJECT()
{
	std::string sNodeName;

	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_NODE_NAME))
		{
			sNodeName = std::string(GetToken());
			m_mapFrame[sNodeName] = new cFrame;
		}
		else if (IsEqual(szToken, ID_NODE_PARENT))
		{
			std::string sParent(GetToken());
			m_mapFrame[sParent]->AddChild(m_mapFrame[sNodeName]);
		}
		else if (IsEqual(szToken, ID_NODE_TM))
		{
			ProcessNODE_TM(m_mapFrame[sNodeName]);
		}
		else if (IsEqual(szToken, ID_MESH))
		{
			ProcessMESH(m_mapFrame[sNodeName]);
		}
		else if (IsEqual(szToken, ID_TM_ANIMATION))
		{
			ProcessTM_ANIMATION(m_mapFrame[sNodeName]);
		}
		else if (IsEqual(szToken, ID_MATERIAL_REF))
		{
			m_mapFrame[sNodeName]->SetMtlTex(m_vecMtlTex[GetInteger()]);
		}
	} while (nLevel > 0);

	return m_mapFrame[sNodeName];
}

void cAseLoader::ProcessMESH( OUT cFrame* pFrame )
{
	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<ST_PNT_VERTEX> vecVertex;

	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_NUMVERTEX))
		{
			vecV.resize(GetInteger());
		}
		else if (IsEqual(szToken, ID_MESH_NUMFACES))
		{
			vecVertex.resize(GetInteger() * 3);
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX_LIST))
		{
			ProcessMESH_VERTEX_LIST(vecV);
		}
		else if (IsEqual(szToken, ID_MESH_FACE_LIST))
		{
			ProcessMESH_FACE_LIST(vecV, vecVertex);
		}
		else if (IsEqual(szToken, ID_MESH_NUMTVERTEX))
		{
			vecVT.resize(GetInteger());
		}
		else if (IsEqual(szToken, ID_MESH_TVERTLIST))
		{
			ProcessMESH_TVERTLIST(vecVT);
		}
		else if (IsEqual(szToken, ID_MESH_TFACELIST))
		{
			ProcessMESH_TFACELIST(vecVT, vecVertex);	
		}
		else if (IsEqual(szToken, ID_MESH_NORMALS))
		{
			ProcessMESH_NORMALS(vecVertex);
		}
	} while (nLevel > 0);

	D3DXMATRIXA16& matWorld = pFrame->GetWorldTM();
	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &matWorld);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}
	pFrame->SetVertex(vecVertex);
}

void cAseLoader::ProcessMESH_VERTEX_LIST( OUT std::vector<D3DXVECTOR3>& vecV )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX))
		{
			int nIndex = GetInteger();
			vecV[nIndex].x = GetFloat();
			vecV[nIndex].z = GetFloat();
			vecV[nIndex].y = GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_FACE_LIST( IN std::vector<D3DXVECTOR3>& vecV, OUT std::vector<ST_PNT_VERTEX>& vecVertex )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_FACE))
		{
			int nFaceIndex = GetInteger();
			GetToken(); // A:
			int nA = GetInteger();
			GetToken(); // B:
			int nB = GetInteger();
			GetToken(); // C:
			int nC = GetInteger();
			vecVertex[nFaceIndex * 3].p = vecV[nA];
			vecVertex[nFaceIndex * 3 + 1].p = vecV[nC];
			vecVertex[nFaceIndex * 3 + 2].p = vecV[nB];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TVERTLIST( OUT std::vector<D3DXVECTOR2>& vecVT )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_TVERT))
		{
			int nIndex = GetInteger();
			vecVT[nIndex].x = GetFloat();
			vecVT[nIndex].y = 1.0f - GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TFACELIST( IN std::vector<D3DXVECTOR2>& vecVT, OUT std::vector<ST_PNT_VERTEX>& vecVertex )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_TFACE))
		{
			int nFaceIndex = GetInteger();
			int nA = GetInteger();
			int nB = GetInteger();
			int nC = GetInteger();
			vecVertex[nFaceIndex * 3].t = vecVT[nA];
			vecVertex[nFaceIndex * 3 + 1].t = vecVT[nC];
			vecVertex[nFaceIndex * 3 + 2].t = vecVT[nB];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_NORMALS( OUT std::vector<ST_PNT_VERTEX>& vecVertex )
{
	int nFaceIndex = 0;
	int nVertexCount = 0;
	int aMod[] = {0, 2, 1};

	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_FACENORMAL))
		{
			nFaceIndex = GetInteger();
			nVertexCount = 0;
		}
		else if (IsEqual(szToken, ID_MESH_VERTEXNORMAL))
		{
			GetToken();
			vecVertex[nFaceIndex * 3 + aMod[nVertexCount]].n.x = GetFloat();
			vecVertex[nFaceIndex * 3 + aMod[nVertexCount]].n.z = GetFloat();
			vecVertex[nFaceIndex * 3 + aMod[nVertexCount]].n.y = GetFloat();
			++nVertexCount;
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessNODE_TM( OUT cFrame* pFrame )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_TM_ROW0))
		{
			D3DXMATRIXA16& matWorld = pFrame->GetWorldTM();
			matWorld._11 = GetFloat();
			matWorld._13 = GetFloat();
			matWorld._12 = GetFloat();
			matWorld._14 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW1))
		{
			D3DXMATRIXA16& matWorld = pFrame->GetWorldTM();
			matWorld._31 = GetFloat();
			matWorld._33 = GetFloat();
			matWorld._32 = GetFloat();
			matWorld._34 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW2))
		{
			D3DXMATRIXA16& matWorld = pFrame->GetWorldTM();
			matWorld._21 = GetFloat();
			matWorld._23 = GetFloat();
			matWorld._22 = GetFloat();
			matWorld._24 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW3))
		{
			D3DXMATRIXA16& matWorld = pFrame->GetWorldTM();
			matWorld._41 = GetFloat();
			matWorld._43 = GetFloat();
			matWorld._42 = GetFloat();
			matWorld._44 = 1.0f;
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessTM_ANIMATION( OUT cFrame* pFrame )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_POS_TRACK))
		{
			ProcessPOS_TRACK(pFrame);
		}
		else if (IsEqual(szToken, ID_ROT_TRACK))
		{
			ProcessROT_TRACK(pFrame);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessPOS_TRACK( OUT cFrame* pFrame )
{
	std::vector<ST_POS_SAMPLE> vecPosTrack;

	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_POS_SAMPLE))
		{
			ST_POS_SAMPLE s;
			s.nKeyFrame = GetInteger();
			s.p.x = GetFloat();
			s.p.z = GetFloat();
			s.p.y = GetFloat();
			vecPosTrack.push_back(s);
		}
	} while (nLevel > 0);

	pFrame->SetPosTrack(vecPosTrack);
}

void cAseLoader::ProcessROT_TRACK( OUT cFrame* pFrame )
{
	std::vector<ST_ROT_SAMPLE> vecRotTrack;

	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_ROT_SAMPLE))
		{
			ST_ROT_SAMPLE s;

			s.nKeyFrame = GetInteger();
			s.q.x = GetFloat();
			s.q.z = GetFloat();
			s.q.y = GetFloat();
			s.q.w = GetFloat();
			
			// Axis Angle To Quaternion
			s.q.x = s.q.x * sinf(s.q.w / 2.0f);
			s.q.z = s.q.z * sinf(s.q.w / 2.0f);
			s.q.y = s.q.y * sinf(s.q.w / 2.0f);
			s.q.w = cosf(s.q.w / 2.0f);

			// ´©Àû
			if (!vecRotTrack.empty())
			{
				s.q = vecRotTrack.back().q * s.q;
			}

			vecRotTrack.push_back(s);
		}
	} while (nLevel > 0);

	pFrame->SetRotTrack(vecRotTrack);
}
