#pragma once
#include <vector>
#include <memory>
using namespace DirectX;
class Cube
{
public:
	struct VERTEX { FLOAT X, Y, Z; float color[4]; };
    struct Transformations { XMFLOAT4X4 model; XMFLOAT4X4 projection; };

	Cube(Graphics& gfx)
	{
		bindables.push_back(std::make_shared<Topology>());

		auto vs = VertexShader(gfx, "VSShader.hlsl");

		bindables.push_back(std::make_shared<VertexShader>(vs));
		bindables.push_back(std::make_shared<PixelShader>(gfx, "PSShader.hlsl"));
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		
		bindables.push_back(std::make_shared<InputLayout>(gfx, ied, sizeof(ied)/sizeof(ied[0]), vs.GetBlob()));

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
        num_indicies = sizeof(indices)/sizeof(int);

        veriticies = std::make_shared<ConstantBuffer<VERTEX>>(gfx, BufferType::VertexBuffer, vertices, sizeof(vertices) / sizeof(VERTEX));
        bindables.push_back(veriticies);

        indicies = std::make_shared<ConstantBuffer<int>>(gfx, BufferType::IndexBuffer, indices, sizeof(indices) / sizeof(int));
        bindables.push_back(indicies);

        Cube::Transformations def = {};
        tranforms = std::make_shared<ConstantBuffer<Cube::Transformations>>(gfx, BufferType::TransformBuffer, &def, sizeof(def) / sizeof(Transformations));
        bindables.push_back(tranforms);

        
        

	}    
    void UpdateMatrix(Graphics& gfx, XMFLOAT4X4& model, XMFLOAT4X4& proj)
    {
        Transformations trans{ model, proj };
        tranforms->Update(gfx, &trans, sizeof(trans)/sizeof(Transformations));
    }

    void Bind(Graphics& gfx)
    {
        for (auto& b : bindables)
        {
            b->Bind(gfx);
        }
    }
    void Draw(Graphics& gfx)
    {
        gfx.GetContextPtr()->DrawIndexed(num_indicies, 0, 0);
    }
private:
	std::vector<std::shared_ptr<Bindable>> bindables;
    std::shared_ptr<ConstantBuffer<Transformations>> tranforms;
    std::shared_ptr<ConstantBuffer<int>> indicies;
    std::shared_ptr<ConstantBuffer<VERTEX>> veriticies;
    int num_indicies;

};
