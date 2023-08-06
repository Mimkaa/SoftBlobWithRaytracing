#pragma once


class Topology : public Bindable
{
public:
	Topology(D3D11_PRIMITIVE_TOPOLOGY topology_in= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		:
		topology{ topology_in }
	{}
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->IASetPrimitiveTopology(topology);
	}
private:
	D3D11_PRIMITIVE_TOPOLOGY topology;
};

