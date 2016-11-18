#pragma once

class iMap;

class cCharController
{
private:
	D3DXVECTOR3					m_vPosition;
	D3DXVECTOR3					m_vDirection;
	float						m_fAngle;
	float						m_fSpeed;
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);

public:
	cCharController(void);
	~cCharController(void);

	void Update(iMap* pMap = NULL);

	D3DXVECTOR3* GetPosition()
	{
		return &m_vPosition;
	}
	
};

