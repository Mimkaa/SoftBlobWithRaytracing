#pragma once
#include "Graphics.h"
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include "DirectXMath.h"

class Graphics
{
friend class Bindable;
public:
    Graphics(HWND hWnd, int ScreenWidth, int ScreeHeight);
    
    
    void ClearScreen(const float* colorBack);
    
    void Present();
    
protected:
    ID3D11Device* GetDevicePtr();
   

    ID3D11DeviceContext* GetContextPtr();
    
public:
    virtual ~Graphics();
private:
	IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
	ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
    ID3D11RenderTargetView* backbuffer;    // global declaration back buffer
};