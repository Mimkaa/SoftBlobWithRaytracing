#pragma once
class Drawer : public Bindable
{
public:
	void Draw(Graphics& gfx, int nIndices)
	{
		GetContext(gfx)->DrawIndexed(nIndices, 0, 0);
	}
	
	void Bind(Graphics& gfx) override
	{

	}
};