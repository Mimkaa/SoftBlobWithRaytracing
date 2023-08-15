#pragma once
#include "Bindable.h"
class Graphics;

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, D3D11_INPUT_ELEMENT_DESC* desc_in, int nElements, ID3DBlob* VSBlob);
	
	void Bind(Graphics& gfx) override;
	
private:
	ID3D11InputLayout* pLayout;
};