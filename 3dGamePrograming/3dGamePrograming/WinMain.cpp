
// 2016.11.22

#include "stdafx.h"
#include <mmsystem.h> 
#pragma comment(lib,"winmm") 

LPDIRECT3D9 g_pD3D = NULL; 
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL; 

LPD3DXMESH g_pMesh = NULL; 
D3DMATERIAL9* g_pMeshMaterials = NULL; 
LPDIRECT3DTEXTURE9* g_pMeshTextures = NULL; 
DWORD g_dwNumMaterials = 0L; 


HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	
	D3DPRESENT_PARAMETERS d3dpp; 

	ZeroMemory(&d3dpp, sizeof(d3dpp)); 
	

	d3dpp.Windowed = TRUE; 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; 

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}


	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	return S_OK;
}

HRESULT InitGeometry()
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	char* xFileName = "바이킹.x";
	char* xFilePath= "엑스파일\\바이킹.x";
	char* txFilePath = "엑스파일\\바이킹.bmp";

	if (FAILED(D3DXLoadMeshFromX(xFileName, D3DXMESH_SYSTEMMEM,
		g_pd3dDevice,NULL,
		&pD3DXMtrlBuffer,NULL,
		&g_dwNumMaterials,
		&g_pMesh)))
	{
		if (FAILED(D3DXLoadMeshFromX(xFilePath,
			D3DXMESH_SYSTEMMEM,
			g_pd3dDevice,NULL,
			&pD3DXMtrlBuffer,NULL,&g_dwNumMaterials,
			&g_pMesh)))
		{
			MessageBox(NULL, "Could not find xFile",
				"Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}

	
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	
	g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials]; 

	g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials]; 

	for (DWORD i = 0; i < g_dwNumMaterials; i++)
	{
		g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

		d3dxMaterials[i].pTextureFilename = txFilePath;

		g_pMeshTextures[i] = NULL;

		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,
				d3dxMaterials[i].pTextureFilename,
				&g_pMeshTextures[i])))
			{
				const TCHAR* strPrefix = TEXT(txFilePath);
				const int lenPrefix = lstrlen(strPrefix);
				TCHAR strTexture[MAX_PATH];
				lstrcpyn(strTexture, strPrefix, MAX_PATH);
				lstrcpyn(strTexture + lenPrefix,
					d3dxMaterials[i].pTextureFilename,
					MAX_PATH - lenPrefix);

				if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,
					strTexture,
					&g_pMeshTextures[i])))
				{

					MessageBox(NULL, "Could not find texture map",
						"Meshes.exe", MB_OK);
				}
			}
		}
	}

	pD3DXMtrlBuffer->Release();
	
	return S_OK;
}


VOID Cleanup()
{
	if (g_pMeshMaterials != NULL)
		delete[] g_pMeshMaterials;

	if (g_pMeshTextures)
	{
		for (DWORD i = 0; i < g_dwNumMaterials; i++)
		{
			if (g_pMeshTextures[i])
				g_pMeshTextures[i]->Release();
		}
		delete[] g_pMeshTextures;
	}

	if (g_pMesh != NULL)
		g_pMesh->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}


VOID SetupMatrices()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f); // Y축을 중심으로 회전행렬 생성
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);


	// 리그 오브 레전드
	//D3DXVECTOR3 vEyePt(0.0f,100.0f, -50.0f); 
	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f,0.0f); 
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); 

	// 스타 크래프트 2
	D3DXVECTOR3 vEyePt(0.0f, 10.0f, -5.0f); 
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f); 
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); 


	D3DXMATRIXA16 matProj;

	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

}


VOID Render()
{

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112), 1.0f, 0);


	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		
		SetupMatrices();

	
		for (DWORD i = 0; i < g_dwNumMaterials; i++)
		{
		
			g_pd3dDevice->SetMaterial(&g_pMeshMaterials[i]);
			g_pd3dDevice->SetTexture(0, g_pMeshTextures[i]);

			g_pMesh->DrawSubset(i);
		}

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL,NULL,NULL,NULL);
}


LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3DxFileTest", NULL };

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("D3DxFileTest", "D3DxFileTest",
		WS_OVERLAPPEDWINDOW, 100, 10, 1000, 800,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitGeometry()))
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			MSG msg;

			ZeroMemory(&msg, sizeof(msg));

			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
				
			}
		}
	}

	UnregisterClass("D3DxFileTest", wc.hInstance);
	return 0;
}

