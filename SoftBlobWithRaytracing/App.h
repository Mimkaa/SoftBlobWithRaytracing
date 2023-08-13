#pragma once
#include <string>

#include "Cube.h"

class App
{
	
public:
	App(HWND hWnd, int ScreenWidth, int ScreeHeight)
		:
		gfx{ hWnd, ScreenWidth, ScreeHeight },
		cube{gfx},
		cube1{gfx}
	{
		using namespace DirectX;

		XMMATRIX projection = XMMatrixPerspectiveFovLH(1.5f, 4 / 3, 0.1f, 1000);
		XMStoreFloat4x4(&storedProjection, XMMatrixTranspose(projection));
		angle = 0.0f;
	}

	void Update()
	{
		cube.UpdateMatrices(gfx, storedProjection);
		cube.SetPosition({ 0.0f, 0.0f, 5.0f });
		cube.SetRot({ angle, angle, 0.0f });
		angle += 0.001;

		cube1.UpdateMatrices(gfx, storedProjection);
		cube1.SetPosition({ 0.0f, 0.0f, 2.0f });
		cube1.SetRot({ -angle, -angle, 0.0f });
		angle += 0.001;
	}

	void Draw()
	{
		// render a frame
		const float colorBack[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		gfx.ClearScreen(colorBack);


		cube.Bind(gfx);
		cube.Draw(gfx);
		gfx.Present();

		cube1.Bind(gfx);
		cube1.Draw(gfx);


		gfx.Present();
	}

private:
	DirectX::XMFLOAT4X4 storedProjection;
	Graphics gfx;
	Cube cube;
	Cube cube1;
	float angle;
	
	
};