#pragma once
#include "Graphics.h"
class Bindable
{
public:
	virtual void Bind(Graphics& gfx) = 0;
protected:
	ID3D11DeviceContext* GetContext(Graphics& gfx);
	ID3D11Device* GetDevice(Graphics& gfx);
};



