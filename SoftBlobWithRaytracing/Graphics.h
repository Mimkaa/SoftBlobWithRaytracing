#pragma once
#include "Graphics.h"
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include "DirectXMath.h"
#include <wrl/client.h>

namespace Win = Microsoft::WRL;

class Graphics
{
friend class Bindable;
public:
    Graphics(HWND hWnd, int ScreenWidth, int ScreeHeight);
    
    
    void ClearScreen(const float* colorBack);
    
    void Present();
public:
    

    ID3D11Device* GetDevicePtr();
   

    ID3D11DeviceContext* GetContextPtr();
    

private:
	Win::ComPtr<IDXGISwapChain> swapchain;             // the pointer to the swap chain interface
    Win::ComPtr <ID3D11Device> dev;                     // the pointer to our Direct3D device interface
    Win::ComPtr < ID3D11DeviceContext> devcon;           // the pointer to our Direct3D device context
    Win::ComPtr < ID3D11RenderTargetView> backbuffer;    // global declaration back buffer
    Win::ComPtr < ID3D11DepthStencilView> pDSSView;
};