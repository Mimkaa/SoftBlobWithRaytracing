#include "VertexShader.h"
#include "Graphics.h"
#include <string>
#include "HelperFunctions.h"

VertexShader::VertexShader(Graphics& gfx, const std::string filename)
	:
	name{ filename }
{
	ID3DBlob* error;
	auto wide = ToWideString(name);
	auto lpcwstr = wide.c_str();
	D3DX11CompileFromFile(lpcwstr, 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VSBlob, 0, 0);
	GetDevice(gfx)->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &vertexShader);
	HRESULT hr = GetDevice(gfx)->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(hr)) {
		throw("you are a loser in VS");
	}

}
ID3DBlob* VertexShader::GetBlob()
{
	return VSBlob.Get();
}
/*~VertexShader()
{
	vertexShader->Release();
	VSBlob->Release();
}*/
void VertexShader::Bind(Graphics& gfx)
{

	GetContext(gfx)->VSSetShader(vertexShader.Get(), 0, 0);
}
