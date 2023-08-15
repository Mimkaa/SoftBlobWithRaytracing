#include "InputLayout.h"
#include "Graphics.h"


InputLayout::InputLayout(Graphics& gfx, D3D11_INPUT_ELEMENT_DESC* desc_in, int nElements, ID3DBlob* VSBlob)
{
	GetDevice(gfx)->CreateInputLayout(desc_in, nElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pLayout);

}

void InputLayout::Bind(Graphics & gfx)
{
	GetContext(gfx)->IASetInputLayout(pLayout);
	auto bb = GetDevice(gfx) == nullptr;
}
