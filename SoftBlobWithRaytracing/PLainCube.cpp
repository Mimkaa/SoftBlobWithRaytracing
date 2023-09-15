#include "PlainCube.h"
#include "App.h"
#include "PixelShader.h"
#include "Graphics.h"
#include <string>
#include "HelperFunctions.h"
#include "db_perlin.hpp"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600


PlainCube::PlainCube(Graphics& gfx)
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

    veriticies = std::make_shared<ConstantBuffer<PlainCube::VERTEX>>(gfx, BufferType::VertexBuffer, vertices, sizeof(vertices) / sizeof(PlainCube::VERTEX));
    bindables.push_back(veriticies);

    indicies = std::make_shared<ConstantBuffer<int>>(gfx, BufferType::IndexBuffer, indices, sizeof(indices) / sizeof(int));
    bindables.push_back(indicies);
    Transformations def = {};
    tranforms = std::make_shared<ConstantBuffer<Transformations>>(gfx, BufferType::TransformBuffer, &def, sizeof(def) / sizeof(Transformations));
    bindables.push_back(tranforms);

    compShaderVars.lifeTimeVars.y = 0.0f;
    compShaderVars.lifeTimeVars.x = 0.0f;
    compShaderVars.vars.x = 0.0f;
    compShaderVars.lifeTimeVars.w = 0.0f;


}
void PlainCube::UpdateMatrices(Graphics& gfx, DirectX::XMFLOAT4X4 projection, DirectX::XMFLOAT4X4 view)
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
    Transformations trans{ storedTranModel, projection, view };
    tranforms->Update(gfx, &trans, sizeof(trans) / sizeof(Transformations));
}




void PlainCube::SetPosition(DirectX::XMFLOAT3 pos_in)
{
    position = pos_in;
}

void PlainCube::SetRot(DirectX::XMFLOAT3 angles)
{
    angle = angles;
}

void PlainCube::BindAdditional(Graphics& gfx)
{

    // texturing


    gfx.GetContextPtr()->PSSetSamplers(0, 1, sampler.GetAddressOf());
    gfx.GetContextPtr()->PSSetShaderResources(0, 1, shaderResourseView.GetAddressOf());




}

void PlainCube::UpdateTexture(Graphics& gfx)
{
    // increment step
    compShaderVars.vars.x += 1;

    D3D11_MAPPED_SUBRESOURCE ms;
    gfx.GetContextPtr()->Map(CSBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, &compShaderVars, sizeof(COMPSHADERVARS) * 1);                 // copy the data
    gfx.GetContextPtr()->Unmap(CSBuffer.Get(), NULL);                                      // unmap the buffer


    ID3D11ShaderResourceView* nullSRV = nullptr;
    gfx.GetContextPtr()->PSSetShaderResources(0, 1, &nullSRV);


    gfx.GetContextPtr()->CSSetShaderResources(0, 1, CSBufferResource.GetAddressOf());

    gfx.GetContextPtr()->CSSetUnorderedAccessViews(0, 1, uav.GetAddressOf(), 0);

   

    // shader chages
    gfx.GetContextPtr()->CSSetShader(ComputeShader.Get(), nullptr, 0);
    gfx.GetContextPtr()->Dispatch(400 / 16, 400 / 16, 1);
    

    
    ID3D11UnorderedAccessView* pNullUAVs = nullptr;

    gfx.GetContextPtr()->CSSetUnorderedAccessViews(0, 1, &pNullUAVs, nullptr);
    gfx.GetContextPtr()->CSSetShaderResources(0, 1, &nullSRV);

    BindAdditional(gfx);




}
void PlainCube::InitializeComputeShaderStuff(Graphics& gfx, float Width, float Height)
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
    ID3DBlob* ComputeShaderBlob = nullptr;
    D3DX11CompileFromFile(L"ComputeShader.hlsl", 0, 0, "CSMain", "cs_5_0", 0, 0, 0, &ComputeShaderBlob, 0, 0);
    gfx.GetDevicePtr()->CreateComputeShader(ComputeShaderBlob->GetBufferPointer(), ComputeShaderBlob->GetBufferSize(), nullptr, &ComputeShader);

    ComputeShaderBlob->Release();

    // create a structured buffer to pass as a resource for the compute shader (simply variables)
    const int size = SCREEN_WIDTH / 2 * SCREEN_HEIGHT / 2;

    D3D11_BUFFER_DESC  bufferDescc = {};
    bufferDescc.ByteWidth = sizeof(COMPSHADERVARS);           // Size of the buffer in bytes
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
    srvDescc.Buffer.NumElements = 2; // Number of elements in the buffer


    HRESULT hrt = gfx.GetDevicePtr()->CreateShaderResourceView(CSBuffer.Get(), &srvDescc, &CSBufferResource);
    if (FAILED(hrt))
    {
        throw("you are a loser");
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




ID3D11Texture2D* PlainCube::GiveTexture()
{
    return texture.Get();
}

