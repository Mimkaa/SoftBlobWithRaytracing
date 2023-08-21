#pragma once
#include "Bindable.h"
#include <string>
class Graphics;

namespace Win = Microsoft::WRL;

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const std::string filename);
	
	ID3DBlob* GetBlob();

	/*~PixelShader()
	{
		pixelShader->Release();
	}*/
	void Bind(Graphics& gfx);
	
private:
	Win::ComPtr < ID3DBlob> PSBlob;
	Win::ComPtr<ID3D11PixelShader> pixelShader;
	std::string name;
};