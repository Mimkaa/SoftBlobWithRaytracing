#include "Plain.h"
#include "PixelShader.h"
#include "Graphics.h"
#include <string>
#include "HelperFunctions.h"
#include "App.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

Plain::Plain(Graphics& gfx)
{
    using namespace DirectX;
    // position and rotation
    XMVECTOR zeros = XMVectorZero();
    XMStoreFloat3(&angle, zeros);
    XMStoreFloat3(&position, zeros);


    // compute shader stuff
    InitializeComputeShaderStuff(gfx, SCREEN_WIDTH, SCREEN_HEIGHT);

    bindables.push_back(std::make_shared<Drawer>());

    bindables.push_back(std::make_shared<Topology>());

    auto vs = VertexShader(gfx, "VSTXShader.hlsl");

    bindables.push_back(std::make_shared<VertexShader>(vs));
    bindables.push_back(std::make_shared<PixelShader>(gfx, "PSTXShader.hlsl"));
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    bindables.push_back(std::make_shared<InputLayout>(gfx, ied, sizeof(ied) / sizeof(ied[0]), vs.GetBlob()));

    VERTEX vertices[] =
    {
        {-1.0f, 1.0f, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 0.0f, 0.0f},
        {1.0f, 1.0f, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 1.0f,0.0f},
        {1.0f, -1.0f, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 1.0f, 1.0f},
        {-1.0f, -1.0f, 0.0f, {1.0f,1.0f,1.0f,1.0f}, 0.0f, 1.0f}
    };


    // create the indices using an int array
    int indices[] =
    {
        0, 1, 2,    // side 1
        2, 3, 0,
    };
    num_indicies = sizeof(indices) / sizeof(int);

    veriticies = std::make_shared<ConstantBuffer<Plain::VERTEX>>(gfx, BufferType::VertexBuffer, vertices, sizeof(vertices) / sizeof(Plain::VERTEX));
    bindables.push_back(veriticies);

    indicies = std::make_shared<ConstantBuffer<int>>(gfx, BufferType::IndexBuffer, indices, sizeof(indices) / sizeof(int));
    bindables.push_back(indicies);
    Transformations def = {};
    tranforms = std::make_shared<ConstantBuffer<Transformations>>(gfx, BufferType::TransformBuffer, &def, sizeof(def) / sizeof(Transformations));
    bindables.push_back(tranforms);
   




}
void Plain::UpdateMatrices(Graphics& gfx, DirectX::XMFLOAT4X4 projection)
{
    using namespace DirectX;
    XMMATRIX translate = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
    XMMATRIX rotX = XMMatrixRotationX(angle.x);
    XMMATRIX rotY = XMMatrixRotationY(angle.y);
    XMMATRIX rotZ = XMMatrixRotationZ(angle.z);
    XMMATRIX rotation = XMMatrixMultiply(XMMatrixMultiply(rotX, rotY), rotZ);
    XMMATRIX model = XMMatrixMultiply(rotation, translate);
    XMFLOAT4X4 storedTranModel;
    XMStoreFloat4x4(&storedTranModel, XMMatrixTranspose(model));
    Transformations trans{ storedTranModel, projection };
    tranforms->Update(gfx, &trans, sizeof(trans) / sizeof(Transformations));
}




void Plain::SetPosition(DirectX::XMFLOAT3 pos_in)
{
    position = pos_in;
}

void Plain::SetRot(DirectX::XMFLOAT3 angles)
{
    angle = angles;
}

void Plain::BindAdditional(Graphics& gfx)
{
   
    // texturing

   
    gfx.GetContextPtr()->PSSetSamplers(0, 1, sampler.GetAddressOf());
    gfx.GetContextPtr()->PSSetShaderResources(0, 1, shaderResourseView.GetAddressOf());

    


}

void Plain::UpdateTexture(Graphics& gfx)
{
    // increment step
    compShaderVars.x += 1;
    compShaderVars.y = float(SCREEN_WIDTH) / 2.0f;
    compShaderVars.z = float(SCREEN_WIDTH) / 2.0f;
    auto ii = compShaderVars;

    D3D11_MAPPED_SUBRESOURCE ms;
    gfx.GetContextPtr()->Map(CSBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, &compShaderVars, sizeof(COMPSHADERVARS) * 1);                 // copy the data
    gfx.GetContextPtr()->Unmap(CSBuffer.Get(), NULL);                                      // unmap the buffer


    ID3D11ShaderResourceView* nullSRV = nullptr;
    gfx.GetContextPtr()->PSSetShaderResources(0, 1, &nullSRV);


    gfx.GetContextPtr()->CSSetShaderResources(0, 1, CSBufferResource.GetAddressOf());

    gfx.GetContextPtr()->CSSetUnorderedAccessViews(0, 1, uav.GetAddressOf(), 0);

    gfx.GetContextPtr()->CSSetUnorderedAccessViews(1, 1, fluidUav.GetAddressOf(), 0);

    gfx.GetContextPtr()->CSSetUnorderedAccessViews(2, 1, fluidUavPrev.GetAddressOf(), 0);

    // shader chages
    gfx.GetContextPtr()->CSSetShader(DiffuceShader.Get(), nullptr, 0);
    for (int i = 0; i < 8; i++)
    {
        gfx.GetContextPtr()->Dispatch(400 / 16, 400 / 16, 1);
    }

    gfx.GetContextPtr()->CSSetShader(AdvectShader.Get(), nullptr, 0);
    gfx.GetContextPtr()->Dispatch(400 / 16, 400 / 16, 1);

    gfx.GetContextPtr()->CSSetShader(ClearDivergenceShader.Get(), nullptr, 0);
    for (int i = 0; i < 8; i++)
    {
        gfx.GetContextPtr()->Dispatch(400 / 16, 400 / 16, 1);
    }

    gfx.GetContextPtr()->CSSetShader(FinalFluidShader.Get(), nullptr, 0);
    gfx.GetContextPtr()->Dispatch(400 / 16, 400 / 16, 1);

    gfx.GetContextPtr()->CSSetShader(SwapBufferShader.Get(), nullptr, 0);
    gfx.GetContextPtr()->Dispatch(400 / 16, 400 / 16, 1);

    ID3D11UnorderedAccessView* pNullUAVs = nullptr;

    gfx.GetContextPtr()->CSSetUnorderedAccessViews(0, 1, &pNullUAVs, nullptr);
    gfx.GetContextPtr()->CSSetUnorderedAccessViews(1, 1, &pNullUAVs, 0);
    gfx.GetContextPtr()->CSSetUnorderedAccessViews(2, 1, &pNullUAVs, 0);
    gfx.GetContextPtr()->CSSetShaderResources(0, 1, &nullSRV);

    BindAdditional(gfx);


    

}
void Plain::InitializeComputeShaderStuff(Graphics& gfx, float Width, float Height)
{
    D3D11_TEXTURE2D_DESC textureDesc = {};
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};

    ///////////////////////// Map's Texture
    // Initialize the  texture description.
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the texture description.
    // We will have our map be a square
    // We will need to have this texture bound as a render target AND a shader resource
    textureDesc.Width = SCREEN_WIDTH / 2;
    textureDesc.Height = SCREEN_WIDTH / 2;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    // Create the texture
    gfx.GetDevicePtr()->CreateTexture2D(&textureDesc, NULL, &texture);

    /////////////////////// Map's Render Target
    // Setup the description of the render target view.
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    // Create the render target view.
    gfx.GetDevicePtr()->CreateRenderTargetView(texture.Get(), &renderTargetViewDesc, &renderTangetView);

    /////////////////////// Map's Shader Resource View
    // Setup the description of the shader resource view.
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    // Create the shader resource view.
    gfx.GetDevicePtr()->CreateShaderResourceView(texture.Get(), &shaderResourceViewDesc, &shaderResourseView);



    // Create the UAV
    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = textureDesc.Format; // Format should match your texture format
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    uavDesc.Texture2D.MipSlice = 0; // Mip level

    gfx.GetDevicePtr()->CreateUnorderedAccessView(texture.Get(), &uavDesc, &uav);

    //load the compute shaders
    ID3DBlob* DiffuceShaderBlob = nullptr;
    D3DX11CompileFromFile(L"DiffuceOperation.hlsl", 0, 0, "CSMain", "cs_5_0", 0, 0, 0, &DiffuceShaderBlob, 0, 0);
    gfx.GetDevicePtr()->CreateComputeShader(DiffuceShaderBlob->GetBufferPointer(), DiffuceShaderBlob->GetBufferSize(), nullptr, &DiffuceShader);

    ID3DBlob* AdvectShaderBlob = nullptr;
    D3DX11CompileFromFile(L"AdvectionOperation.hlsl", 0, 0, "CSMain", "cs_5_0", 0, 0, 0, &AdvectShaderBlob, 0, 0);
    gfx.GetDevicePtr()->CreateComputeShader(AdvectShaderBlob->GetBufferPointer(), AdvectShaderBlob->GetBufferSize(), nullptr, &AdvectShader);

    ID3DBlob* ClearDivergenceBlob = nullptr;
    D3DX11CompileFromFile(L"ClearDivergenceOperation.hlsl", 0, 0, "CSMain", "cs_5_0", 0, 0, 0, &ClearDivergenceBlob, 0, 0);
    gfx.GetDevicePtr()->CreateComputeShader(ClearDivergenceBlob->GetBufferPointer(), ClearDivergenceBlob->GetBufferSize(), nullptr, &ClearDivergenceShader);

    ID3DBlob* FinalFluidShaderBlob = nullptr;
    D3DX11CompileFromFile(L"FluidFinal.hlsl", 0, 0, "CSMain", "cs_5_0", 0, 0, 0, &FinalFluidShaderBlob, 0, 0);
    gfx.GetDevicePtr()->CreateComputeShader(FinalFluidShaderBlob->GetBufferPointer(), FinalFluidShaderBlob->GetBufferSize(), nullptr, &FinalFluidShader);

    ID3DBlob* SwapBufferShaderBlob = nullptr;
    D3DX11CompileFromFile(L"SwapFluidBuffers.hlsl", 0, 0, "CSMain", "cs_5_0", 0, 0, 0, &SwapBufferShaderBlob, 0, 0);
    gfx.GetDevicePtr()->CreateComputeShader(SwapBufferShaderBlob->GetBufferPointer(), SwapBufferShaderBlob->GetBufferSize(), nullptr, &SwapBufferShader);

    AdvectShaderBlob->Release();
    ClearDivergenceBlob->Release();
    FinalFluidShaderBlob->Release();
    AdvectShaderBlob->Release();
    SwapBufferShaderBlob->Release();

    // create a structured buffer to pass as a resource for the compute shader (simply variables)
    const int size = SCREEN_WIDTH/2 * SCREEN_HEIGHT/2;

    D3D11_BUFFER_DESC  bufferDescc = {};
    bufferDescc.ByteWidth = sizeof(DirectX::XMFLOAT4);           // Size of the buffer in bytes
    bufferDescc.Usage = D3D11_USAGE_DYNAMIC;      // How the buffer will be used
    bufferDescc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDescc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;              // CPU access flags
    bufferDescc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDescc.StructureByteStride = sizeof(DirectX::XMFLOAT4);

    HRESULT hrtt = gfx.GetDevicePtr()->CreateBuffer(&bufferDescc, nullptr, &CSBuffer);
    if (FAILED(hrtt))
    {
        throw std::runtime_error("Failed to create buffer. HRESULT: " + std::to_string(hrtt));
    }
    

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDescc = {};
    srvDescc.Format = DXGI_FORMAT_UNKNOWN;
    srvDescc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDescc.Buffer.FirstElement = 0;
    srvDescc.Buffer.NumElements = 1; // Number of elements in the buffer

   
    HRESULT hrt = gfx.GetDevicePtr()->CreateShaderResourceView(CSBuffer.Get(), &srvDescc, &CSBufferResource);
    if (FAILED(hrt))
    {
        throw("you are a loser");
    }

    // create a buffer for a fluid simulation and a shader resource and to pass it to the compute shader
    D3D11_BUFFER_DESC bbufferDesc = {};
    bbufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bbufferDesc.ByteWidth = sizeof(FLUIDSIMCELL) * SCREEN_WIDTH/2 * SCREEN_WIDTH/2;
    bbufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    bbufferDesc.CPUAccessFlags = 0;
    bbufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bbufferDesc.StructureByteStride = sizeof(FLUIDSIMCELL);


    
    HRESULT gg = gfx.GetDevicePtr()->CreateBuffer(&bbufferDesc, NULL, CSFluidBuffer.GetAddressOf());

    if (FAILED(gg))
    {
        throw("ddd");
    }
    

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDescc = {};
    uavDescc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDescc.Format = DXGI_FORMAT_UNKNOWN;
    uavDescc.Buffer.NumElements = SCREEN_WIDTH / 2 * SCREEN_WIDTH / 2;
    uavDescc.Buffer.FirstElement = 0;
    uavDescc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
    

   

    HRESULT hres = gfx.GetDevicePtr()->CreateUnorderedAccessView(CSFluidBuffer.Get(), &uavDescc, &fluidUav);
    if (FAILED(hres))
    {
        throw("ddd");
    }

    // buffer for the previous values of the fluid sim

    D3D11_BUFFER_DESC bbufferDescPrev = {};
    bbufferDescPrev.Usage = D3D11_USAGE_DEFAULT;
    bbufferDescPrev.ByteWidth = sizeof(FLUIDSIMCELL) * SCREEN_WIDTH / 2 * SCREEN_WIDTH / 2;
    bbufferDescPrev.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    bbufferDescPrev.CPUAccessFlags = 0;
    bbufferDescPrev.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bbufferDescPrev.StructureByteStride = sizeof(FLUIDSIMCELL);



    HRESULT gfg = gfx.GetDevicePtr()->CreateBuffer(&bbufferDescPrev, NULL, CSFluidBufferPrev.GetAddressOf());

    if (FAILED(gfg))
    {
        throw("ddd");
    }


    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesccPrev = {};
    uavDesccPrev.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesccPrev.Format = DXGI_FORMAT_UNKNOWN;
    uavDesccPrev.Buffer.NumElements = SCREEN_WIDTH / 2 * SCREEN_WIDTH / 2;
    uavDesccPrev.Buffer.FirstElement = 0;
    uavDesccPrev.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;




    HRESULT hrefs = gfx.GetDevicePtr()->CreateUnorderedAccessView(CSFluidBufferPrev.Get(), &uavDesccPrev, &fluidUavPrev);
    if (FAILED(hrefs))
    {
        throw("ddd");
    }

   
    // sampler
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = gfx.GetDevicePtr()->CreateSamplerState(&samplerDesc, &sampler);
    if (FAILED(hr))
    {
        throw("ddd");
    }

    

}
