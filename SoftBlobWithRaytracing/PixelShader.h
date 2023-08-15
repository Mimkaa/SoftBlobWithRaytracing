#pragma once
#include "Bindable.h"
#include <string>
class Graphics;

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
	ID3DBlob* PSBlob;
	ID3D11PixelShader* pixelShader;
	std::string name;
};