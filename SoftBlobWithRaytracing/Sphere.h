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


private:
    std::shared_ptr<ConstantBuffer<Transformations>> tranforms;
    std::shared_ptr<ConstantBuffer<int>> indicies;
    std::shared_ptr<ConstantBuffer<VERTEX>> veriticies;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 position;
    ID3D11Texture2D* texture = nullptr;
    Win::ComPtr< ID3D11SamplerState> sampler;
    Win::ComPtr<ID3D11ShaderResourceView> shaderResourseView;

};
