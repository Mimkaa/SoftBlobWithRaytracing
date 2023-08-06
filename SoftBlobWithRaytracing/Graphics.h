#pragma once

class Graphics
{
friend class Bindable;
public:
    Graphics(HWND hWnd, int ScreenWidth, int ScreeHeight)
    {
        // create a struct to hold information about the swap chain
        DXGI_SWAP_CHAIN_DESC scd;

        // clear out the struct for use
        ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

        // fill the swap chain description struct
        scd.BufferCount = 1;                                    // one back buffer
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
        scd.BufferDesc.Width = ScreenWidth;                    // set the back buffer width
        scd.BufferDesc.Height = ScreeHeight;                  // set the back buffer height
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
        scd.OutputWindow = hWnd;                                // the window to be used
        scd.SampleDesc.Count = 4;                               // how many multisamples
        scd.Windowed = TRUE;                                    // windowed/full-screen mode
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

        // create a device, device context and swap chain using the information in the scd struct
        D3D11CreateDeviceAndSwapChain(NULL,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            NULL,
            NULL,
            NULL,
            D3D11_SDK_VERSION,
            &scd,
            &swapchain,
            &dev,
            NULL,
            &devcon);

        // Set the viewport
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = ScreenWidth;
        viewport.Height = ScreeHeight;

        devcon->RSSetViewports(1, &viewport);

        // get the address of the back buffer (as a texture)
        ID3D11Texture2D* pBackBuffer;
        swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

        // use the back buffer address to create the render target (pBackBuffer gives all the info about are the texels we have just retrieved)
        dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
        pBackBuffer->Release();

        // set the render target as the back buffer
        devcon->OMSetRenderTargets(1, &backbuffer, NULL);
    }
    void ClearScreen(const float* colorBack)
    {
        // clear the back buffer to a deep blue
        //const float colorBack[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        devcon->ClearRenderTargetView(backbuffer, colorBack);

        // switch the back buffer and the front buffer
  
    }
    void Present()
    {
        swapchain->Present(0, 0);
    }

public:
    ID3D11Device* GetDevicePtr()
    {
        return dev;
    }

    ID3D11DeviceContext* GetContextPtr()
    {
        return devcon;
    }


    ~Graphics()
    {
        swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
        dev->Release();
        devcon->Release();
        swapchain->Release();
        backbuffer->Release();
    }
private:
	IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
	ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
    ID3D11RenderTargetView* backbuffer;    // global declaration back buffer
};