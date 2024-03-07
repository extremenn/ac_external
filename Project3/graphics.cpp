#include "graphics.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

Graphics::Graphics(HWND handle)
{
	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = 68U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = handle;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	);

	ID3D11Texture2D* backBuffer{ nullptr };
	swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (backBuffer) {
		device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
		backBuffer->Release();
	}

}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (swapChain) {
		swapChain->Release();
	}
	if (deviceContext) {
		deviceContext->Release();
	}
	if (renderTargetView) {
		renderTargetView->Release();
	}
}

void Graphics::RenderTarget()
{
	ImGui::Render();

	constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
	deviceContext->OMSetRenderTargets(1U, &renderTargetView, nullptr);
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void Graphics::initImGui(HWND window)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, deviceContext);
}

void Graphics::createNewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

