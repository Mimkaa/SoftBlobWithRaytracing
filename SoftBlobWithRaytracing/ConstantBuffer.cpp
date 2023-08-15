#include "ConstantBuffer.h"
#include "Graphics.h"
void SetVertexBuffer(const std::tuple<ID3D11DeviceContext*, int, int, ID3D11Buffer*, UINT, UINT>& data)
{
    std::get<0>(data)->IASetVertexBuffers(std::get<1>(data), std::get<2>(data), &std::get<3>(data), &std::get<4>(data), &std::get<5>(data));
}

void SetIndexBuffer(const std::tuple <ID3D11DeviceContext*, ID3D11Buffer*, DXGI_FORMAT>& data)
{
    std::get<0>(data)->IASetIndexBuffer(std::get<1>(data), std::get<2>(data), 0);
}

void SetTransformBuffer(const std::tuple < ID3D11DeviceContext*, int, int, ID3D11Buffer*>& data)
{
    std::get<0>(data)->VSSetConstantBuffers(std::get<1>(data), std::get<2>(data), &std::get<3>(data));
}





