#pragma once
#include "Bindable.h"
#include <string>

class Graphics;

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::string filename);
	
	ID3DBlob* GetBlob();
	
	void Bind(Graphics& gfx);
private:
	ID3DBlob* VSBlob;
	ID3D11VertexShader* vertexShader;
	std::string name;
};