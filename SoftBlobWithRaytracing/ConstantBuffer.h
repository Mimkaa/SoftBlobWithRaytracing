#pragma once
#include "Bindable.h"
#include <tuple>



enum BufferType
{
    VertexBuffer,
    IndexBuffer,
    TransformBuffer,
};



void SetVertexBuffer(const std::tuple<ID3D11DeviceContext*, int, int, ID3D11Buffer*, UINT, UINT>& data);


void SetIndexBuffer(const std::tuple <ID3D11DeviceContext*, ID3D11Buffer*, DXGI_FORMAT>& data);


void SetTransformBuffer(const std::tuple < ID3D11DeviceContext*, int, int, ID3D11Buffer*>& data);




template <typename... Args>
struct FunctionPointerHolder {
    using FunctionType = void (*)(Args...);
    FunctionType functionPointer;

    // Constructor to set the function pointer
    constexpr FunctionPointerHolder(FunctionType ptr) : functionPointer(ptr) {}

    // Call operator to invoke the function pointer
    constexpr void operator()(Args... args) const {
        functionPointer(args...);
    }
};

template<BufferType> struct Map;

template<> struct Map<TransformBuffer>
{
    static constexpr D3D11_BIND_FLAG type = D3D11_BIND_CONSTANT_BUFFER;

    static constexpr FunctionPointerHolder<const std::tuple < ID3D11DeviceContext*, int, int, ID3D11Buffer*>&> Bind =
        &SetTransformBuffer;

    // Function to return the parameters
    static constexpr std::tuple<ID3D11DeviceContext*, int, int, ID3D11Buffer*> params(
        ID3D11DeviceContext* context, int slot, int number, ID3D11Buffer* buffPtr) {
        return std::make_tuple(context, slot, number, buffPtr);
    }



};

template<> struct Map<IndexBuffer>
{
    static constexpr D3D11_BIND_FLAG type = D3D11_BIND_INDEX_BUFFER;
    static constexpr FunctionPointerHolder<const std::tuple <ID3D11DeviceContext*, ID3D11Buffer*, DXGI_FORMAT>&> Bind =
        &SetIndexBuffer;

    // Function to return the parameters
    static constexpr std::tuple<ID3D11DeviceContext*, ID3D11Buffer*, DXGI_FORMAT> params(
        ID3D11DeviceContext* context, ID3D11Buffer* indBuff, DXGI_FORMAT format) {
        return std::make_tuple(context, indBuff, format);
    }

};
template<> struct Map<VertexBuffer>
{
    static constexpr FunctionPointerHolder<const std::tuple<ID3D11DeviceContext*, int, int, ID3D11Buffer*, UINT, UINT>&> Bind =
        &SetVertexBuffer;

    // Function to return the parameters
    static constexpr std::tuple<ID3D11DeviceContext*, int, int, ID3D11Buffer*, UINT, UINT> params(
        ID3D11DeviceContext* context, int startSlot, int numBuffers, ID3D11Buffer* buffer, UINT stride, UINT offset) {
        return std::make_tuple(context, startSlot, numBuffers, buffer, stride, offset);
    }
};

class Graphics;

template <class T>
class ConstantBuffer :public Bindable
{
public:
    ConstantBuffer(Graphics& gfx, BufferType bufType, const T* consts, int size, int slot = 0)
        :
        slot{ slot },
        bufType{ bufType }
    {
        D3D11_BIND_FLAG type;
        switch (bufType)
        {
        case BufferType::VertexBuffer:
        {
            type = D3D11_BIND_VERTEX_BUFFER;
            break;
        }
        case BufferType::TransformBuffer:
        {
            type = D3D11_BIND_CONSTANT_BUFFER;
            break;
        }
        case BufferType::IndexBuffer:
        {
            type = D3D11_BIND_INDEX_BUFFER;
            break;
        }
        }

        D3D11_BUFFER_DESC cbDesc;
        cbDesc.ByteWidth = sizeof(T) * size;
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags = type;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbDesc.MiscFlags = 0;
        cbDesc.StructureByteStride = 0;



        HRESULT hr = GetDevice(gfx)->CreateBuffer(&cbDesc, NULL, &pTranformBuffer);
        if (FAILED(hr)) {
            throw("you are a loser in buffer");
        }

        // copy the indices in the buffer
        D3D11_MAPPED_SUBRESOURCE msi;
        GetContext(gfx)->Map(pTranformBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msi);    // map the buffer
        memcpy(msi.pData, consts, sizeof(T) * size);                 // copy the data
        GetContext(gfx)->Unmap(pTranformBuffer, NULL);                                      // unmap the buffer



    }
 

    void Bind(Graphics& gfx) override
    {
        switch (bufType)
        {
        case BufferType::VertexBuffer:
        {
            auto params = Map<VertexBuffer>::params(GetContext(gfx), slot, 1, pTranformBuffer, sizeof(T), 0);
            Map< BufferType::VertexBuffer>::Bind(params);
            break;
        }
        case BufferType::TransformBuffer:
        {
            auto params = Map<TransformBuffer>::params(GetContext(gfx), slot, 1, pTranformBuffer);
            Map< BufferType::TransformBuffer>::Bind(params);
            break;
        }
        case BufferType::IndexBuffer:
        {
            auto params = Map<IndexBuffer>::params(GetContext(gfx), pTranformBuffer, DXGI_FORMAT_R32_UINT);
            Map< BufferType::IndexBuffer>::Bind(params);
            break;
        }

        }



    }

    void Update(Graphics& gfx, const T* trans, int size)
    {
        D3D11_MAPPED_SUBRESOURCE ms;
        GetContext(gfx)->Map(pTranformBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
        memcpy(ms.pData, trans, sizeof(T) * size);                 // copy the data
        GetContext(gfx)->Unmap(pTranformBuffer, NULL);                                      // unmap the buffer
    }

    ~ConstantBuffer()
    {
        pTranformBuffer->Release();
        
    }

private:
    ID3D11Buffer* pTranformBuffer;
    int slot;
    BufferType bufType;

};