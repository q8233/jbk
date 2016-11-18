#pragma once

class cCamera;
class cGrid;
class cCharController;
class iMap;
class cSkinnedMesh;
class cFrustum;

class cMainGame
{
private:
	cCamera*					m_pCamera;
	cGrid*						m_pGrid;
	cCharController*			m_pCharController;
	iMap*						m_pMap;
	std::vector<cSkinnedMesh*>	m_vecSkinnedMesh;
	cSkinnedMesh*				m_pSkinnedMesh;
	cFrustum*					m_pFrustum;


public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();
};

