#pragma once
class cSokobanChar
{
private:
	bool		m_isMoving;
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3	m_vStart;
	D3DXVECTOR3	m_vDest;
	float		m_fMoveTime;
	float		m_fPassedMoveTime;
	
public:
	cSokobanChar(void);
	~cSokobanChar(void);

	void Update();
	void SetPosition(D3DXVECTOR3& vPosition);
	D3DXVECTOR3* GetPosition();
};

