#pragma once
#include <vector>
#include <memory>
#include "Globals.h"
#include "Drawable.h"
#include "Graphics.h"
#include "BIndablesHeader.h"

class Cube : public Drawable
{
public:
	struct VERTEX { FLOAT X, Y, Z; float color[4]; };

    Cube(Graphics& gfx);


   

    void SetPosition(DirectX::XMFLOAT3 pos_in);

    void SetRot(DirectX::XMFLOAT3 angles);

    void UpdateMatrices(Graphics& gfx, DirectX::XMFLOAT4X4 projection);

   
private:
    std::shared_ptr<ConstantBuffer<Transformations>> tranforms;
    std::shared_ptr<ConstantBuffer<int>> indicies;
    std::shared_ptr<ConstantBuffer<VERTEX>> veriticies;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 position;

};
