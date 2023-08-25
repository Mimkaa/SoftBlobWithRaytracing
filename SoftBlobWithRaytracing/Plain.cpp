#include "Plain.h"

#include "PixelShader.h"
#include "Graphics.h"
#include <string>
#include "HelperFunctions.h"

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

    InitializeComputeShaderStuff(gfx, SCREEN_WIDTH, SCREEN_HEIGHT);


}

void Plain::UpdateTexture(Graphics& gfx)
{
    gfx.GetContextPtr()->CSSetShader(computeShader.Get(), nullptr, 0);

    gfx.GetContextPtr()->CSSetUnorderedAccessViews(0, 1, uav.GetAddressOf(), 0);

    gfx.GetContextPtr()->Dispatch(32, 32, 1);

    ID3D11UnorderedAccessView* pNullUAVs = nullptr;
    gfx.GetContextPtr()->CSSetUnorderedAccessViews(0, 1, &pNullUAVs, nullptr);

    


    

}
void Plain::InitializeComputeShaderStuff(Graphics& gfx, float Width, float Height)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

    ///////////////////////// Map's Texture
    // Initialize the  texture description.
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the texture description.
    // We will have our map be a square
    // We will need to have this texture bound as a render target AND a shader resource
    textureDesc.Width = SCREEN_WIDTH / 2;
    textureDesc.Height = SCREEN_HEIGHT/ 2;
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

    //load the first compute schader
    ID3DBlob* shaderBlob = nullptr;
    D3DX11CompileFromFile(L"ComputeShader.hlsl", 0, 0, "CSMain", "cs_5_0", 0, 0, 0, &shaderBlob, 0, 0);

    // Create and set the compute shader
   
    gfx.GetDevicePtr()->CreateComputeShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &computeShader);


    // create a structured buffer to pass as a resource for the compute shader
    const int size = SCREEN_WIDTH/2 * SCREEN_HEIGHT/2;



    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * size;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.StructureByteStride = sizeof(DirectX::XMFLOAT4); // Size of each element in the buffer

    

    gfx.GetDevicePtr()->CreateBuffer(&bufferDesc, NULL, &CSEditBuffer);

    // make it into a shader resource
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.ElementOffset = 0;
    srvDesc.Buffer.ElementWidth = bufferDesc.ByteWidth;

    gfx.GetDevicePtr()->CreateShaderResourceView(CSEditBuffer.Get(), &srvDesc, &CSBufferResource);

    // sampler
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    gfx.GetDevicePtr()->CreateSamplerState(&samplerDesc, &sampler);
   

}
