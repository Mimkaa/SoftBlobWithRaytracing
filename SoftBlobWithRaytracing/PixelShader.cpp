#include "PixelShader.h"
#include "Graphics.h"
#include <string>
#include "HelperFunctions.h"

PixelShader::PixelShader(Graphics& gfx, const std::string filename)
	:
	name{ filename }
{
	auto wide = ToWideString(name);
	auto lpcwstr = wide.c_str();
	D3DX11CompileFromFile(lpcwstr, 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PSBlob, 0, 0);
	HRESULT hr = GetDevice(gfx)->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pixelShader);
	if (FAILED(hr)) {
		throw("you are a loser in PS");
	}
}
ID3DBlob* PixelShader::GetBlob()
{
	return PSBlob.Get();
}
/*~PixelShader()
{
	pixelShader->Release();
}*/
void PixelShader::Bind(Graphics& gfx)
{

	GetContext(gfx)->PSSetShader(pixelShader.Get(), 0, 0);
}

