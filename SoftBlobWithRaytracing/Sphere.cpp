#include "Sphere.h"
#include "SpherePrimitive.h"

Sphere::Sphere(Graphics& gfx)
{
    auto model = SpherePrimitive::Make<VERTEX>();
    using namespace DirectX;
    // position and rotation
    XMVECTOR zeros = XMVectorZero();
    XMStoreFloat3(&angle, zeros);
    XMStoreFloat3(&position, zeros);

    bindables.push_back(std::make_shared<Drawer>());

    bindables.push_back(std::make_shared<Topology>());

    auto vs = VertexShader(gfx, "VSNOTTXShader.hlsl");

    bindables.push_back(std::make_shared<VertexShader>(vs));
    bindables.push_back(std::make_shared<PixelShader>(gfx, "PSShader.hlsl"));
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
        
    };

    bindables.push_back(std::make_shared<InputLayout>(gfx, ied, sizeof(ied) / sizeof(ied[0]), vs.GetBlob()));

    

    num_indicies = model.indices.size();

    veriticies = std::make_shared<ConstantBuffer<VERTEX>>(gfx, BufferType::VertexBuffer, model.vertices.data(), model.vertices.size());
    bindables.push_back(veriticies);

    indicies = std::make_shared<ConstantBuffer<int>>(gfx, BufferType::IndexBuffer, model.indices.data(), model.indices.size());
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
    D3D11_SAMPLER_DESC samplerDesc2 = {};
    samplerDesc2.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc2.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc2.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc2.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc2.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc2.MinLOD = 0;
    samplerDesc2.MaxLOD = D3D11_FLOAT32_MAX;

    gfx.GetDevicePtr()->CreateSamplerState(&samplerDesc2, &samplerCube);
 

   

}

void Sphere::ConnectShaderResources(Graphics& gfx)
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
        gfx.GetContextPtr()->PSSetShaderResources(1, 1, shaderResourseViewCube.GetAddressOf());
        gfx.GetContextPtr()->PSSetSamplers(1, 1, samplerCube.GetAddressOf());
    }
}

void  Sphere::GetTexture(ID3D11Texture2D* texture_in)
{
    texture = texture_in;
}

void Sphere::UpdateMatrices(Graphics& gfx, DirectX::XMFLOAT4X4 projection, DirectX::XMFLOAT4X4 view)
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




void Sphere::SetPosition(DirectX::XMFLOAT3 pos_in)
{
    position = pos_in;
}

void Sphere::SetRot(DirectX::XMFLOAT3 angles)
{
    angle = angles;
}

void  Sphere::CreateCubeTexture(Graphics& gfx)
{
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = 800;
    textureDesc.Height = 800;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT ;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE ;
    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    textureDesc.MiscFlags = 0;

    // Create the texture
    for (int i = 0; i < 6; i++)
    {
        HRESULT hr = gfx.GetDevicePtr()->CreateTexture2D(&textureDesc, NULL, &cubeTexture[i]);
        if (FAILED(hr))
        {
            throw("");
        }
    }
    // staging textures

    D3D11_TEXTURE2D_DESC stagingDesc = {};
    stagingDesc.Width = 800;
    stagingDesc.Height = 800;
    stagingDesc.MipLevels = 1;
    stagingDesc.ArraySize = 1;
    stagingDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    stagingDesc.SampleDesc.Count = 1;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    stagingDesc.BindFlags = 0; // No bindings required for staging texture
    stagingDesc.MiscFlags = 0;
    for (int i = 0; i < 6; i++)
    {
        HRESULT hr = gfx.GetDevicePtr()->CreateTexture2D(&stagingDesc, NULL, &StagingCubeTexture[i]);
        if (FAILED(hr))
        {
            throw("");
        }
    }

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    // create rtv
    for (int i = 0; i < 6; i++)
    {
        HRESULT hr = gfx.GetDevicePtr()->CreateRenderTargetView(cubeTexture[i], &renderTargetViewDesc, &cubeTextureRenderView[i]);
        if (FAILED(hr))
        {
            throw("");
        }
    }

    //Description of each face
    D3D11_TEXTURE2D_DESC texDesc = {};


    //The Shader Resource view description
    D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc = {};

    texDesc.Width = 800;
    texDesc.Height = 800;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 6;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.CPUAccessFlags = 0;
    texDesc.SampleDesc.Count = 1;
    //texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    SMViewDesc.Format = texDesc.Format;
    SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    SMViewDesc.TextureCube.MipLevels = texDesc.MipLevels;
    SMViewDesc.TextureCube.MostDetailedMip = 0;

    HRESULT hr = gfx.GetDevicePtr()->CreateTexture2D(&texDesc, NULL, &cubeTextureMade);
    if (FAILED(hr))
    {
        throw("");
    }
    
    HRESULT hhr = gfx.GetDevicePtr()->CreateShaderResourceView(cubeTextureMade, &SMViewDesc, &shaderResourseViewCube);
    if (FAILED(hhr))
    {
        throw("");
    }
}

void Sphere::WriteCubeTexture(Graphics & gfx)
{
    for (int i = 0; i < 6; i++)
    {

        for (UINT mipLevel = 0; mipLevel < 1; ++mipLevel)
        {
            D3D11_MAPPED_SUBRESOURCE mappedTex2D;
            // prolem is here
            HRESULT hr = (gfx.GetContextPtr()->Map(StagingCubeTexture[i], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));
           
            gfx.GetContextPtr()->UpdateSubresource(cubeTextureMade,
                D3D11CalcSubresource(mipLevel, i, 1),
                0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

            gfx.GetContextPtr()->Unmap(StagingCubeTexture[i], mipLevel);
        }



    }
}

void Sphere::CopyCubeTextureToStaging(Graphics& gfx)
{
    for (int i = 0; i < 6; i++)
    {
        gfx.GetContextPtr()->CopyResource(StagingCubeTexture[i], cubeTexture[i]);
    }
}

ID3D11RenderTargetView** Sphere::GetCubeTexPlanes()
{
    return cubeTextureRenderView;
}

void Sphere::ConnectCubeTextureToPSShader(Graphics& gfx)
{
    gfx.GetContextPtr()->PSSetShaderResources(0, 1, shaderResourseViewCube.GetAddressOf());
}