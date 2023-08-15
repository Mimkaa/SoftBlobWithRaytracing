#pragma once
#include "Bindable.h"
class Graphics;


class Topology : public Bindable
{
public:
	Topology(D3D11_PRIMITIVE_TOPOLOGY topology_in = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void Bind(Graphics& gfx) override;
private:
	D3D11_PRIMITIVE_TOPOLOGY topology;
};

