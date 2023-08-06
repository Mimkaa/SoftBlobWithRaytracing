#pragma once

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) = 0;
protected:
	ID3D11DeviceContext* GetContext(Graphics& gfx)
	{
		return gfx.GetContextPtr();
	}
	ID3D11Device* GetDevice(Graphics& gfx)
	{
		return gfx.GetDevicePtr();
	}
};



