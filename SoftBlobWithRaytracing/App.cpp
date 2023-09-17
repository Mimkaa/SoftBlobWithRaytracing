#include "App.h"


App::App(HWND hWnd, int ScreenWidth, int ScreeHeight)
	:
	gfx{ hWnd, ScreenWidth, ScreeHeight },
	cube{ gfx },
	canvas{gfx},
	sphere{gfx},
	canvasCube{gfx}
{
	using namespace DirectX;
	
	XMMATRIX projection = XMMatrixPerspectiveFovLH(1.5f, 1.3, 0.1f, 10);
	XMStoreFloat4x4(&storedProjection, XMMatrixTranspose(projection));

	auto forwardVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	float pitch = 0.4f;
	float yaw = 0.3f;
	float roll = 0.0f;

	auto direction = DirectX::XMVector3Transform(forwardVector, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	auto eyePosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	auto traget = eyePosition + direction;

	XMMATRIX view = XMMatrixLookAtLH(eyePosition, traget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMStoreFloat4x4(&storedView, XMMatrixTranspose(view));

	sphere.CreateCubeTexture(gfx);

	angle = 0.0f;
}

void App::Update()
{
	canvasCube.UpdateMatrices(gfx, storedProjection, storedView);
	canvasCube.SetPosition({ 0.0f, 0.0f, 2.0f });

	cube.UpdateMatrices(gfx, storedProjection, storedView);
	cube.SetPosition({ 2.0f, 0.0f, 4.0f });
	cube.SetRot({ -angle, -angle, 0.0f });

	canvas.UpdateMatrices(gfx, storedProjection, storedView);
	canvas.SetPosition({ 0.0f, 0.0f, 2.0f });

	sphere.UpdateMatrices(gfx, storedProjection, storedView);
	sphere.SetPosition({ -2.0f, 0.0f, 4.0f });
	sphere.SetRot({ -angle, -angle, 0.0f });

	angle += 0.001;

	
}

void App::ClearScreen()
{
	// render a frame
	const float colorBack[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	gfx.ClearScreen(colorBack);

}

void App::Draw()
{
	

	canvasCube.Bind(gfx);
	canvasCube.UpdateTexture(gfx);
	//canvasCube.Draw(gfx);

	cube.Bind(gfx);
	cube.GetTexture(canvasCube.GiveTexture());
	cube.ConnectShaderResources(gfx);
	cube.Draw(gfx);

	

	canvas.Bind(gfx);
	canvas.UpdateTexture(gfx);
	//canvas.Draw(gfx);

	

	sphere.Bind(gfx);
	sphere.GetTexture(canvas.GiveTexture());
	sphere.ConnectShaderResources(gfx);
	sphere.Draw(gfx);

	gfx.Present();
}

void App::DrawCubeTexturtes()
{
	// set one of the textures which belong to the sphere as a render target (1)
	using namespace DirectX;
	const float colorBack[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	auto forwardVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	auto eyePosition = DirectX::XMVectorSet(-2.0f, 0.0f, 4.0f, 0.0f);
	auto eyePositionOrigin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float pitch = 0.0f;
	float yaw = 1.5708f;
	float roll = 0.0f;

	// draw sides around
	auto mainBuff = gfx.GetMainRenderTargerView();
	auto mainBuffBackUp = gfx.GetMainRenderTargerView();

	for (int i = 0; i < 6; i++)
	{
		const float colorBackTrans[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		auto firtsSide = sphere.GetCubeTexPlanes();
		gfx.GetContextPtr()->ClearRenderTargetView(firtsSide[i], colorBackTrans);
	}

	for (int i = 0; i < 1; i++)
	{
		int j = i + 1;
		float yaww = j * yaw;
		auto direction = DirectX::XMVector3Transform(forwardVector, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaww, roll));
		auto traget = eyePosition + direction;

		XMMATRIX view = XMMatrixLookAtLH(eyePosition, traget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		XMStoreFloat4x4(&storedView, XMMatrixTranspose(view));

		// set one of the textures which belong to the sphere as a render target
		auto firtsSide = sphere.GetCubeTexPlanes();
		//mainBuff = firtsSide[i];
		gfx.GetContextPtr()->OMSetRenderTargets(1, &firtsSide[i], NULL); 
		Draw();
	}
	
	//auto directionT = DirectX::XMVector3Transform(forwardVector, DirectX::XMMatrixRotationRollPitchYaw(1.5708f *3 , 0.0f, 0.0f));
	//auto tragetT = eyePosition + directionT;

	//XMMATRIX viewT = XMMatrixLookAtLH(eyePosition, tragetT, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	//XMStoreFloat4x4(&storedView, XMMatrixTranspose(viewT));

	//// set one of the textures which belong to the sphere as a render target
	//auto firtsSideT = sphere.GetCubeTexPlanes();
	//gfx.GetContextPtr()->OMSetRenderTargets(1, &firtsSideT[4], NULL);
	//Draw();

	//auto directionD = DirectX::XMVector3Transform(forwardVector, DirectX::XMMatrixRotationRollPitchYaw(1.5708f*3, 0.0f, 0.0f));
	//auto tragetD = eyePosition + directionD;

	//XMMATRIX viewD = XMMatrixLookAtLH(eyePosition, tragetD, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	//XMStoreFloat4x4(&storedView, XMMatrixTranspose(viewD));

	//// set one of the textures which belong to the sphere as a render target
	//auto firtsSideD = sphere.GetCubeTexPlanes();
	//gfx.GetContextPtr()->OMSetRenderTargets(1, &firtsSideD[5], NULL);
	//Draw();

	auto directionOr = DirectX::XMVector3Transform(forwardVector, DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f));
	auto tragetOr = eyePositionOrigin + directionOr;

	XMMATRIX viewOr = XMMatrixLookAtLH(eyePositionOrigin, tragetOr, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMStoreFloat4x4(&storedView, XMMatrixTranspose(viewOr));

	sphere.CopyCubeTextureToStaging(gfx);

	sphere.WriteCubeTexture(gfx);

	gfx.GetContextPtr()->OMSetRenderTargets(1, &mainBuff, NULL);
	
	
	//gfx.GetContextPtr()->OMSetRenderTargets(1, &mainBuff, NULL);
	/*auto Mrtv = gfx.GetMainRenderTargerView();

	gfx.GetContextPtr()->OMSetRenderTargets(1, &Mrtv, NULL);*/
	

}