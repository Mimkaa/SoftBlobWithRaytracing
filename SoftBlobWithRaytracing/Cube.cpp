#include "Cube.h"

Cube::Cube(Graphics& gfx)
{
    using namespace DirectX;
    // position and rotation
    XMVECTOR zeros = XMVectorZero();
    XMStoreFloat3(&angle, zeros);
    XMStoreFloat3(&position, zeros);

    bindables.push_back(std::make_shared<Drawer>());

    bindables.push_back(std::make_shared<Topology>());

    auto vs = VertexShader(gfx, "VSShader.hlsl");

    bindables.push_back(std::make_shared<VertexShader>(vs));
    bindables.push_back(std::make_shared<PixelShader>(gfx, "PSNOTXShader.hlsl"));
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    bindables.push_back(std::make_shared<InputLayout>(gfx, ied, sizeof(ied) / sizeof(ied[0]), vs.GetBlob()));
    float side = 1.0f;
    VERTEX vertices[] =
    {
        // near
        { DirectX::XMFLOAT3{ -side,-side,-side },{0.0f, 0.0f, -1.0f}, DirectX::XMFLOAT2{0.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//0
        { DirectX::XMFLOAT3{ side,-side,-side},  {0.0f, 0.0f, -1.0f}, DirectX::XMFLOAT2{1.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//1
        { DirectX::XMFLOAT3{ -side,side,-side},  {0.0f, 0.0f, -1.0f}, DirectX::XMFLOAT2{0.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//2
        { DirectX::XMFLOAT3{ side,side,-side},   {0.0f, 0.0f, -1.0f}, DirectX::XMFLOAT2{ 1.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//3

        // far
        { DirectX::XMFLOAT3{ -side,-side,side}, DirectX::XMFLOAT3{0, 0, 1.0f}, DirectX::XMFLOAT2{ 0.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//4
        { DirectX::XMFLOAT3{ side,-side,side}, DirectX::XMFLOAT3{0, 0, 1.0f}, DirectX::XMFLOAT2{ 1.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//5
        { DirectX::XMFLOAT3{-side,side,side}, DirectX::XMFLOAT3{0, 0, 1.0f}, DirectX::XMFLOAT2{ 0.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//6
        { DirectX::XMFLOAT3{ side,side,side}, DirectX::XMFLOAT3{0, 0, 1.0f}, DirectX::XMFLOAT2{ 1.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//7

        //// left
        { DirectX::XMFLOAT3{  -side,-side,-side }, DirectX::XMFLOAT3{-1.0f, 0.0f, 0.0f}, DirectX::XMFLOAT2{ 1.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//8
        { DirectX::XMFLOAT3{-side,side,-side}, DirectX::XMFLOAT3{-1.0f, 0.0f, 0.0f}, DirectX::XMFLOAT2{ 1.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//9
        { DirectX::XMFLOAT3{  -side,-side,side }, DirectX::XMFLOAT3{-1.0f, 0.0f, 0.0f}, DirectX::XMFLOAT2{ 0.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//10
        { DirectX::XMFLOAT3{ -side,side,side}, DirectX::XMFLOAT3{-1.0f, 0.0f, 0.0f}, DirectX::XMFLOAT2{ 0.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//11


        //// right
        { DirectX::XMFLOAT3{side,-side,-side}, DirectX::XMFLOAT3{1.0f, 0.0f, 0.0f}, DirectX::XMFLOAT2{ 1.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//12
        { DirectX::XMFLOAT3{side,side,-side}, DirectX::XMFLOAT3{1.0f, 0.0f, 0.0f}, DirectX::XMFLOAT2{ 1.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//13
        { DirectX::XMFLOAT3{side,-side,side}, DirectX::XMFLOAT3{1.0f, 0.0, 0.0f}, DirectX::XMFLOAT2{ 0.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//14
        { DirectX::XMFLOAT3{side,side,side }, DirectX::XMFLOAT3{1.0f, 0.0f, 0.0f}, DirectX::XMFLOAT2{ 0.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//15

        //// bottom
        { DirectX::XMFLOAT3{-side,-side,-side  }, DirectX::XMFLOAT3{0.0f, -1.0f, 0.0f}, DirectX::XMFLOAT2{ 0.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//16
        { DirectX::XMFLOAT3{ side,-side,-side}, DirectX::XMFLOAT3{0.0f, -1.0f, 0.0f}, DirectX::XMFLOAT2{ 1.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//17
        { DirectX::XMFLOAT3{ -side,-side,side }, DirectX::XMFLOAT3{0.0f, -1.0, 0.0f}, DirectX::XMFLOAT2{ 0.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//18
        { DirectX::XMFLOAT3{ side,-side,side}, DirectX::XMFLOAT3{0.0f, -1.0f, 0.0f}, DirectX::XMFLOAT2{ 1.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//19

        //// top
        { DirectX::XMFLOAT3{ -side,side,-side }, DirectX::XMFLOAT3{0.0f, 1.0f, 0.0f}, DirectX::XMFLOAT2{ 0.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//20
        { DirectX::XMFLOAT3{ side,side,-side}, DirectX::XMFLOAT3{0.0f, 1.0f, 0.0f}, DirectX::XMFLOAT2{ 1.0f, 1.0f}, {0, 1.0f, 0, 1.0f}},//21
        { DirectX::XMFLOAT3{-side,side,side}, DirectX::XMFLOAT3{0.0f, 1.0, 0.0f}, DirectX::XMFLOAT2{ 0.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//22
        { DirectX::XMFLOAT3{ side,side,side}, DirectX::XMFLOAT3{0.0f, 1.0f, 0.0f}, DirectX::XMFLOAT2{ 1.0f, 0.0f}, {0, 1.0f, 0, 1.0f}},//23
    };
   

    // create the indices using an int array
    int indices[] =
    {
                 0,2, 1,    2,3,1,
                4,5, 7,    4,7,6,
                8,10, 9,  10,11,9,
                12,13,15, 12,15,14,
                16,17,18, 18,17,19,
                20,23,21, 20,22,23
    };
    num_indicies = sizeof(indices) / sizeof(int);

    veriticies = std::make_shared<ConstantBuffer<Cube::VERTEX>>(gfx, BufferType::VertexBuffer, vertices, sizeof(vertices) / sizeof(Cube::VERTEX));
    bindables.push_back(veriticies);

    indicies = std::make_shared<ConstantBuffer<int>>(gfx, BufferType::IndexBuffer, indices, sizeof(indices) / sizeof(int));
    bindables.push_back(indicies);
    Transformations def = {};
    tranforms = std::make_shared<ConstantBuffer<Transformations>>(gfx, BufferType::TransformBuffer, &def, sizeof(def) / sizeof(Transformations));
    bindables.push_back(tranforms);
    
    //sampler
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = gfx.GetDevicePtr()->CreateSamplerState(&samplerDesc, &sampler);
    if (FAILED(hr))
    {
        throw("ddd");
    }



}
void Cube::UpdateMatrices(Graphics& gfx, DirectX::XMFLOAT4X4 projection)
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




void Cube::SetPosition(DirectX::XMFLOAT3 pos_in)
{
    position = pos_in;
}

void Cube::SetRot(DirectX::XMFLOAT3 angles)
{
    angle = angles;
}


void Cube::ConnectShaderResources(Graphics& gfx)
{
    if (texture) {
        if (shaderResourseView == nullptr) {
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
            shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;
            gfx.GetDevicePtr()->CreateShaderResourceView(texture, &shaderResourceViewDesc, &shaderResourseView);
        }

        gfx.GetContextPtr()->PSSetSamplers(0, 1, sampler.GetAddressOf());
        gfx.GetContextPtr()->PSSetShaderResources(0, 1, shaderResourseView.GetAddressOf());
    }
}

void  Cube::GetTexture(ID3D11Texture2D* texture_in)
{
    texture = texture_in;
}