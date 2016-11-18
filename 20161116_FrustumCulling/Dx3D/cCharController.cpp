#include "StdAfx.h"
#include "cCharController.h"
#include "iMap.h"

cCharController::cCharController(void)
	: m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
	, m_fAngle(0.0f)
	, m_fSpeed(0.1f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCharController::~cCharController(void)
{
}

void cCharController::Update(iMap* pMap)
{
	if (GetKeyState('A') & 0x8000)
	{
		m_fAngle -= 0.1f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_fAngle += 0.1f;
	}

	m_vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIXA16 matRotY;
	D3DXMatrixRotationY(&matRotY, m_fAngle);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matRotY);

	D3DXVECTOR3 p = m_vPosition;
	if (GetKeyState('W') & 0x8000)
	{
		p = p + m_vDirection * m_fSpeed;
	}
	if (GetKeyState('S') & 0x8000)
	{
		p = p - m_vDirection * m_fSpeed;
	}
	
	if(pMap && pMap->GetHeight(p.x, p.y, p.z))
	{
		m_vPosition = p;
	}

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matRotY * matT;
// 
// 
// 	D3DLIGHT9 stLight;
// 	stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
// 	stLight.Type = D3DLIGHT_SPOT;
// 	D3DXVECTOR3 vDir;
// 	D3DXVec3Normalize(&vDir, &m_vDirection);
// 	stLight.Direction = vDir;
// 	stLight.Position = m_vPosition;
// 	stLight.Theta = D3DX_PI / 8;
// 	stLight.Phi = D3DX_PI / 6;
// 	stLight.Range = 100.f;
// 	g_pD3DDevice->SetLight(1, &stLight);
// 	g_pD3DDevice->LightEnable(1, true);
}
