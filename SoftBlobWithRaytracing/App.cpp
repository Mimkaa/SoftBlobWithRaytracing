#include "App.h"


App::App(HWND hWnd, int ScreenWidth, int ScreeHeight)
	:
	gfx{ hWnd, ScreenWidth, ScreeHeight },
	cube{ gfx },
	cube1{ gfx },
	canvas{gfx},
	sphere{gfx}
{
	using namespace DirectX;
	
	XMMATRIX projection = XMMatrixPerspectiveFovLH(1.5f, 1.3, 0.1f, 10);
	XMStoreFloat4x4(&storedProjection, XMMatrixTranspose(projection));
	angle = 0.0f;
}

void App::Update()
{
	cube.UpdateMatrices(gfx, storedProjection);
	cube.SetPosition({ 0.0f, 0.0f, 5.0f });
	cube.SetRot({ angle, angle, 0.0f });

	canvas.UpdateMatrices(gfx, storedProjection);
	canvas.SetPosition({ 0.0f, 0.0f, 2.0f });

	cube1.UpdateMatrices(gfx, storedProjection);
	cube1.SetPosition({ 1.0f, 0.0f, 5.5f });
	cube1.SetRot({ -angle, -angle, 0.0f });

	sphere.UpdateMatrices(gfx, storedProjection);
	sphere.SetPosition({ 3.5f, 0.0f, 5.5f });
	//sphere.SetRot({ -angle, -angle, 0.0f });

	angle += 0.001;

	
}

void App::Draw()
{
	// render a frame
	const float colorBack[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	gfx.ClearScreen(colorBack);


	/*cube.Bind(gfx);
	cube.Draw(gfx);

	

	cube1.Bind(gfx);
	cube1.Draw(gfx);*/

	canvas.Bind(gfx);
	canvas.UpdateTexture(gfx);
	canvas.Draw(gfx);


	sphere.Bind(gfx);
	sphere.GetTexture(canvas.GiveTexture());
	sphere.ConnectShaderResources(gfx);
	sphere.Draw(gfx);

	gfx.Present();
}