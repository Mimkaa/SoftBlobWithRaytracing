#include "Graphics.h"


Graphics::Graphics(HWND hWnd, int ScreenWidth, int ScreeHeight)
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
    scd.SampleDesc.Count = 1;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_DEBUG,
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
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;

    devcon->RSSetViewports(1, &viewport);

    // get the address of the back buffer (as a texture)
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target (pBackBuffer gives all the info about are the texels we have just retrieved)
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // depth stencil
    HRESULT hr;

    ID3D11Texture2D* pDepthStencil;
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = ScreenWidth;
    descDepth.Height = ScreeHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = dev->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
    if (FAILED(hr))
    {
        throw("mmmm");
    }



    D3D11_DEPTH_STENCIL_DESC dsDesc = {};

    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

   
    // Create depth stencil state
    ID3D11DepthStencilState* pDSState;
    hr = dev->CreateDepthStencilState(&dsDesc, &pDSState);

    if (FAILED(hr))
    {
        throw("mmmm");
    }

    devcon->OMSetDepthStencilState(pDSState, 1);

  

    // Bild depth stencil Desc

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;  // Use the same format as the texture
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;


    hr = dev->CreateDepthStencilView(pDepthStencil, &descDSV, &pDSSView);

    if (FAILED(hr))
    {
        throw("mmmm");
    }
    
   
    // Bind the depth stencil view  
    devcon->OMSetRenderTargets(1, backbuffer.GetAddressOf(), pDSSView.Get());

    pDepthStencil->Release();
    pDSState->Release();
   
   
}
void Graphics::ClearScreen(const float* colorBack)
{
    // clear the back buffer to a deep blue
    //const float colorBack[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    devcon->ClearRenderTargetView(backbuffer.Get(), colorBack);
    devcon->ClearDepthStencilView(pDSSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // switch the back buffer and the front buffer

}
void Graphics::Present()
{
    swapchain->Present(0, 0);
}


ID3D11Device* Graphics::GetDevicePtr()
{
    return dev.Get();
}

ID3D11DeviceContext* Graphics::GetContextPtr()
{
    return devcon.Get();
}


