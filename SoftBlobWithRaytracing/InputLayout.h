#pragma once


class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, D3D11_INPUT_ELEMENT_DESC* desc_in, int nElements, ID3DBlob* VSBlob)
	{
		GetDevice(gfx)->CreateInputLayout(desc_in, nElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pLayout);
		
	}

	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->IASetInputLayout(pLayout);
		auto bb = GetDevice(gfx) == nullptr;
	}
	
private:
	ID3D11InputLayout* pLayout;
};