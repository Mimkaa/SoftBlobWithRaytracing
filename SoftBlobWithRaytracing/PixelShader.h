#pragma once

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const std::string filename)
		:
		name{filename}
	{
		auto wide = ToWideString(name);
		auto lpcwstr = wide.c_str();
		D3DX11CompileFromFile(lpcwstr, 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PSBlob, 0, 0);
		GetDevice(gfx) -> CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pixelShader);
		
	}
	ID3DBlob* GetBlob()
	{
		return PSBlob;
	}
	/*~PixelShader()
	{
		pixelShader->Release();
	}*/
	void Bind(Graphics& gfx)
	{
	
		GetContext(gfx)->PSSetShader(pixelShader, 0, 0);
	}
private:
	ID3DBlob* PSBlob;
	ID3D11PixelShader* pixelShader;
	std::string name;
};