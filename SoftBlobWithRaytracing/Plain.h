#pragma once
#include <vector>
#include <memory>
#include "Globals.h"
#include "Drawable.h"
#include "Graphics.h"
#include "BIndablesHeader.h"


namespace Win = Microsoft::WRL;

class Plain : public Drawable
{
public:
    struct VERTEX { FLOAT X, Y, Z; float color[4]; FLOAT TX, TY; };
    struct COMPSHADERVARS { DirectX::XMFLOAT4 vars; DirectX::XMFLOAT4 lifeTimeVars; };
    struct FLUIDSIMCELL { DirectX::XMFLOAT2 vel; DirectX::XMFLOAT2 dencity; };

    Plain(Graphics& gfx);

    void InitializeComputeShaderStuff(Graphics& gfx, float width, float height);

    void UpdateTexture(Graphics& gfx);

    void BindAdditional(Graphics& gfx) override;

    void SetPosition(DirectX::XMFLOAT3 pos_in);

    void SetRot(DirectX::XMFLOAT3 angles);

    void UpdateMatrices(Graphics& gfx, DirectX::XMFLOAT4X4 projection);

    float GetMeanOfDensity(Graphics& gfx);

    ID3D11Texture2D* GiveTexture();
 

private:
    std::shared_ptr<ConstantBuffer<Transformations>> tranforms;
    std::shared_ptr<ConstantBuffer<int>> indicies;
    std::shared_ptr<ConstantBuffer<VERTEX>> veriticies;
    
    // texture stuff
    Win::ComPtr<ID3D11Texture2D> texture;
    Win::ComPtr<ID3D11RenderTargetView> renderTangetView;
    Win::ComPtr<ID3D11ShaderResourceView> shaderResourseView;
    Win::ComPtr<ID3D11UnorderedAccessView> uav;
    // buffer stuff
    Win::ComPtr<ID3D11Buffer> CSBuffer;
    Win::ComPtr< ID3D11ShaderResourceView> CSBufferResource;

    // compute Shaders
    Win::ComPtr<ID3D11ComputeShader> DiffuceShader;
    Win::ComPtr<ID3D11ComputeShader> AdvectShader;
    Win::ComPtr<ID3D11ComputeShader> ClearDivergenceShader;
    Win::ComPtr<ID3D11ComputeShader> FinalFluidShader;
    Win::ComPtr<ID3D11ComputeShader> SwapBufferShader;

    Win::ComPtr<ID3D11Buffer> CSFluidBuffer;
    Win::ComPtr<ID3D11UnorderedAccessView> fluidUav;
    Win::ComPtr<ID3D11Buffer> CSFluidBufferPrev;
    Win::ComPtr<ID3D11UnorderedAccessView> fluidUavPrev;

  


    Win::ComPtr< ID3D11SamplerState> sampler;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 position;
    COMPSHADERVARS compShaderVars;
    
 
    

};