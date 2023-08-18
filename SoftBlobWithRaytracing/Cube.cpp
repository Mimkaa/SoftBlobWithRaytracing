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
    bindables.push_back(std::make_shared<PixelShader>(gfx, "PSShader.hlsl"));
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    bindables.push_back(std::make_shared<InputLayout>(gfx, ied, sizeof(ied) / sizeof(ied[0]), vs.GetBlob()));

    VERTEX vertices[] =
    {
        { -1.0f, 1.0f, -1.0f, {0, 0, 1.0f , 1.0f}, },    // vertex 0
        { 1.0f, 1.0f, -1.0f, {0, 1.0f, 0, 1.0f}, },     // vertex 1
        { -1.0f, -1.0f, -1.0f, {1.0f , 0, 0, 1.0f}, },   // 2
        { 1.0f, -1.0f, -1.0f, {0, 1.0f, 1.0f, 1.0f}, },  // 3
        { -1.0f, 1.0f, 1.0f, {0, 0, 1.0f, 1.0f}, },     // ...
        { 1.0f, 1.0f, 1.0f, {1.0f, 0, 0, 1.0f}, },
        { -1.0f, -1.0f, 1.0f, {0, 1.0f, 0, 1.0f}, },
        { 1.0f, -1.0f, 1.0f, {0, 1.0f, 1.0f, 1.0f} },
    };


    // create the indices using an int array
    int indices[] =
    {
        0, 1, 2,    // side 1
        2, 1, 3,
        4, 0, 6,    // side 2
        6, 0, 2,
        7, 5, 6,    // side 3
        6, 5, 4,
        3, 1, 7,    // side 4
        7, 1, 5,
        4, 5, 0,    // side 5
        0, 5, 1,
        3, 7, 2,    // side 6
        2, 7, 6,
    };
    num_indicies = sizeof(indices) / sizeof(int);

    veriticies = std::make_shared<ConstantBuffer<Cube::VERTEX>>(gfx, BufferType::VertexBuffer, vertices, sizeof(vertices) / sizeof(Cube::VERTEX));
    bindables.push_back(veriticies);

    indicies = std::make_shared<ConstantBuffer<int>>(gfx, BufferType::IndexBuffer, indices, sizeof(indices) / sizeof(int));
    bindables.push_back(indicies);
    Transformations def = {};
    tranforms = std::make_shared<ConstantBuffer<Transformations>>(gfx, BufferType::TransformBuffer, &def, sizeof(def) / sizeof(Transformations));
    bindables.push_back(tranforms);
    




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
