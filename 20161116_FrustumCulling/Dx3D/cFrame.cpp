#include "StdAfx.h"
#include "cFrame.h"

cFrame::cFrame(void)
	: m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cFrame::~cFrame(void)
{
	SAFE_RELEASE(m_pMtlTex);
}

void cFrame::Update( int nKeyFrame, D3DXMATRIXA16* pParentWorld )
{
	D3DXMATRIXA16 matT, matR;
	LocalTraslationMatrix(nKeyFrame, matT);
	LocalRotationMatrix(nKeyFrame, matR);
	
	m_matLocal = matR * matT;

	m_matWorld = m_matLocal;
	if (pParentWorld)
	{
		m_matWorld *= *pParentWorld;
	}

	for each(auto p in m_vecChild)
	{
		p->Update(nKeyFrame, &m_matWorld);
	}
}

void cFrame::Render()
{
	if(m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMtl());
		//g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX));
	}
	
	for each(auto p in m_vecChild)
	{
		p->Render();
	}
}

void cFrame::AddChild( cFrame* pChild )
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	for each(auto p in m_vecChild)
	{
		p->Destroy();
	}
	delete this;
}

void cFrame::CalcLocalTM( D3DXMATRIXA16* pParentWorldTM )
{
	if(pParentWorldTM)
	{
		//W * IPW = L * PW * IPW;
		D3DXMATRIXA16 matInvParentWorld;
		D3DXMatrixInverse(&matInvParentWorld, 0, pParentWorldTM);
		m_matLocal = m_matWorld * matInvParentWorld;
	}
	else
	{
		m_matLocal = m_matWorld;
	}

	for each(auto p in m_vecChild)
	{
		p->CalcLocalTM(&m_matWorld);
	}
}

void cFrame::LocalTraslationMatrix( int nKeyFrame, OUT D3DXMATRIXA16& mat )
{
	D3DXMatrixIdentity(&mat);

	if(m_vecPosTrack.empty())
	{
		mat._41 = m_matLocal._41;
		mat._42 = m_matLocal._42;
		mat._43 = m_matLocal._43;
	}
	else if (nKeyFrame <= m_vecPosTrack.front().nKeyFrame)
	{
		mat._41 = m_vecPosTrack.front().p.x;
		mat._42 = m_vecPosTrack.front().p.y;
		mat._43 = m_vecPosTrack.front().p.z;
	}
	else if (nKeyFrame >= m_vecPosTrack.back().nKeyFrame)
	{
		mat._41 = m_vecPosTrack.back().p.x;
		mat._42 = m_vecPosTrack.back().p.y;
		mat._43 = m_vecPosTrack.back().p.z;
	}
	else
	{
		int nNext = 0;
		for (size_t i = 0; i < m_vecPosTrack.size(); ++i)
		{
			if(nKeyFrame < m_vecPosTrack[i].nKeyFrame)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;
		float t = (nKeyFrame - m_vecPosTrack[nPrev].nKeyFrame)
			/ (float)(m_vecPosTrack[nNext].nKeyFrame - m_vecPosTrack[nPrev].nKeyFrame);
		D3DXVECTOR3 p;
		D3DXVec3Lerp(&p, &m_vecPosTrack[nPrev].p, &m_vecPosTrack[nNext].p, t);
		mat._41 = p.x;
		mat._42 = p.y;
		mat._43 = p.z;
	}
}

void cFrame::LocalRotationMatrix( int nKeyFrame, OUT D3DXMATRIXA16& mat )
{
	D3DXMatrixIdentity(&mat);

	if(m_vecRotTrack.empty())
	{
		mat = m_matLocal;
		mat._41 = 0.0f;
		mat._42 = 0.0f;
		mat._43 = 0.0f;
	}
	else if (nKeyFrame <= m_vecRotTrack.front().nKeyFrame)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().q);
	}
	else if (nKeyFrame >= m_vecRotTrack.back().nKeyFrame)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().q);
	}
	else
	{
		int nNext = 0;
		for (size_t i = 0; i < m_vecRotTrack.size(); ++i)
		{
			if(nKeyFrame < m_vecRotTrack[i].nKeyFrame)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;
		float t = (nKeyFrame - m_vecRotTrack[nPrev].nKeyFrame)
			/ (float)(m_vecRotTrack[nNext].nKeyFrame - m_vecRotTrack[nPrev].nKeyFrame);
		
		D3DXQUATERNION q;
		D3DXQuaternionSlerp(&q, &m_vecRotTrack[nPrev].q, &m_vecRotTrack[nNext].q, t);
		D3DXMatrixRotationQuaternion(&mat, &q);
	}
}
