#pragma once
#include <vector>
#include <memory>
#include "Globals.h"
#include "Drawable.h"
#include "Graphics.h"
#include "BIndablesHeader.h"

class Sphere : public Drawable
{
public:
    struct VERTEX { DirectX::XMFLOAT3 pos; DirectX::XMFLOAT3 norm; DirectX::XMFLOAT2 tex; float color[4]; };

    Sphere(Graphics& gfx);




    void SetPosition(DirectX::XMFLOAT3 pos_in);

    void SetRot(DirectX::XMFLOAT3 angles);

    void UpdateMatrices(Graphics& gfx, DirectX::XMFLOAT4X4 projection, DirectX::XMFLOAT4X4 view);

    void GetTexture(ID3D11Texture2D* texture_in);

    void ConnectShaderResources(Graphics& gfx);

    void CreateCubeTexture(Graphics& gfx);

    void ConnectCubeTextureToPSShader(Graphics& gfx);
  
    ID3D11RenderTargetView** GetCubeTexPlanes();
    ~Sphere()
    {
        for (int i = 0; i < 6; i++)
        {
            if (cubeTexture[i]!= nullptr) cubeTexture[i]->Release();
            if (StagingCubeTexture[i] != nullptr) StagingCubeTexture[i]->Release();
            if (cubeTextureRenderView[i] != nullptr)
            {
                cubeTextureRenderView[i]->Release();
       
            }
          
        }
    }
    void CopyCubeTextureToStaging(Graphics& gfx);
    void WriteCubeTexture(Graphics& gfx);

private:
    std::shared_ptr<ConstantBuffer<Transformations>> tranforms;
    std::shared_ptr<ConstantBuffer<int>> indicies;
    std::shared_ptr<ConstantBuffer<VERTEX>> veriticies;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 position;
    ID3D11Texture2D* texture = nullptr;
    Win::ComPtr< ID3D11SamplerState> sampler;
    Win::ComPtr< ID3D11SamplerState> samplerCube;
    Win::ComPtr<ID3D11ShaderResourceView> shaderResourseView;
    ID3D11Texture2D* cubeTexture[6];
    ID3D11Texture2D* StagingCubeTexture[6];
    ID3D11Texture2D* cubeTextureMade = NULL;
    ID3D11RenderTargetView* cubeTextureRenderView[6];
    Win::ComPtr<ID3D11ShaderResourceView> shaderResourseViewCube;
};
