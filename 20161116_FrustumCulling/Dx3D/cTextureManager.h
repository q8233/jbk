#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	SINGLETONE(cTextureManager);

private:
	std::map<std::string, LPDIRECT3DTEXTURE9>	m_mapTexture;
	std::map<std::string, D3DXIMAGE_INFO>		m_mapImageInfo;

public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szTexturePath);
	LPDIRECT3DTEXTURE9 GetTexture(std::string sTexturePath);
	LPDIRECT3DTEXTURE9 GetTexture(char* szTexturePath, D3DXIMAGE_INFO* pImageInfo);
	LPDIRECT3DTEXTURE9 GetTexture(std::string sTexturePath, D3DXIMAGE_INFO* pImageInfo);

	void Destroy();
};

