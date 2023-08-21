#pragma once
#include "Bindable.h"
#include <string>

class Graphics;

namespace Win = Microsoft::WRL;

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::string filename);
	
	ID3DBlob* GetBlob();
	
	void Bind(Graphics& gfx);
private:
	Win::ComPtr<ID3DBlob> VSBlob;
	Win::ComPtr<ID3D11VertexShader> vertexShader;
	std::string name;
};