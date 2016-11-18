#include "StdAfx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager(void)
{
}


cTextureManager::~cTextureManager(void)
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( char* szTexturePath )
{
	return GetTexture(std::string(szTexturePath));
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( std::string sTexturePath )
{
	if (m_mapTexture.find(sTexturePath) == m_mapTexture.end())
	{
		LPDIRECT3DTEXTURE9 pTexture;
		D3DXCreateTextureFromFile(g_pD3DDevice,
			sTexturePath.c_str(),
			&pTexture);
		m_mapTexture[sTexturePath] = pTexture;
	}

	return m_mapTexture[sTexturePath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( char* szTexturePath, D3DXIMAGE_INFO* pImageInfo )
{
	if(pImageInfo == NULL)
		return GetTexture(std::string(szTexturePath));
	return GetTexture(std::string(szTexturePath), pImageInfo);
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( std::string sTexturePath, D3DXIMAGE_INFO* pImageInfo )
{
	if(pImageInfo == NULL)
		return GetTexture(sTexturePath);

	if (m_mapTexture.find(sTexturePath) == m_mapTexture.end())
	{
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		D3DXCreateTextureFromFileEx(
			g_pD3DDevice,
			sTexturePath.c_str(), 
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN, 
			D3DPOOL_MANAGED, 
			D3DX_FILTER_NONE, 
			D3DX_DEFAULT, 
			0,
			pImageInfo,
			NULL,
			&pTexture );
		m_mapImageInfo[sTexturePath] = *pImageInfo;
		m_mapTexture[sTexturePath] = pTexture;
	}
	else if (m_mapImageInfo.find(sTexturePath) == m_mapImageInfo.end())
	{
		SAFE_RELEASE(m_mapTexture[sTexturePath]);
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		D3DXCreateTextureFromFileEx(
			g_pD3DDevice,
			sTexturePath.c_str(), 
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN, 
			D3DPOOL_MANAGED, 
			D3DX_FILTER_NONE, 
			D3DX_DEFAULT, 
			0,
			pImageInfo,
			NULL,
			&pTexture );
		m_mapImageInfo[sTexturePath] = *pImageInfo;
		m_mapTexture[sTexturePath] = pTexture;
	}
	if(pImageInfo)
		*pImageInfo = m_mapImageInfo[sTexturePath];
	return m_mapTexture[sTexturePath];
}

void cTextureManager::Destroy()
{
	for each(auto it in m_mapTexture)
	{
		SAFE_RELEASE(it.second);
	}
}
