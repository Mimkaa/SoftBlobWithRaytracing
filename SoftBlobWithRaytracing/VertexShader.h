#pragma once

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::string filename)
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
			throw("you are a loser");
		}
		
	}
	ID3DBlob* GetBlob()
	{
		return VSBlob;
	}
	/*~VertexShader()
	{
		vertexShader->Release();
		VSBlob->Release();
	}*/
	void Bind(Graphics& gfx)
	{
		
		GetContext(gfx)->VSSetShader(vertexShader, 0, 0);
	}
private:
	ID3DBlob* VSBlob;
	ID3D11VertexShader* vertexShader;
	std::string name;
};