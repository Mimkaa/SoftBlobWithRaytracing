#include "Bindable.h"
#include "Graphics.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx)
{
	return gfx.GetContextPtr();
}
ID3D11Device* Bindable::GetDevice(Graphics& gfx)
{
	return gfx.GetDevicePtr();
}

