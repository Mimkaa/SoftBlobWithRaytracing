#include "Topology.h"
#include "Graphics.h"


Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY topology_in)
	:
	topology{ topology_in }
{}
void Topology::Bind(Graphics& gfx)
{
	GetContext(gfx)->IASetPrimitiveTopology(topology);
}


