#pragma once
#include <Windows.h>
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND handle);
	~Graphics();
	void RenderTarget();
	void initImGui(HWND window);
	void createNewFrame();
public:
	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* deviceContext{ nullptr };
	IDXGISwapChain* swapChain{ nullptr };
	ID3D11RenderTargetView* renderTargetView{ nullptr };
};

