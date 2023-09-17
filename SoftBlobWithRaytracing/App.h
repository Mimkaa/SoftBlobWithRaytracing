#pragma once
#include <string>
#include "Graphics.h"
#include "Globals.h"
#include "BIndablesHeader.h"
#include "Sphere.h"
#include "Cube.h"
#include "Plain.h"
#include "PlainCube.h"

class App
{
	
public:
	App(HWND hWnd, int ScreenWidth, int ScreeHeight);
	

	void Update();

	void DrawCubeTexturtes();

	void Draw();

	void ClearScreen();

private:
	DirectX::XMFLOAT4X4 storedProjection;
	DirectX::XMFLOAT4X4 storedView;
	Graphics gfx;
	Cube cube;
	Plain canvas;
	PlainCube canvasCube;
	Sphere sphere;
	float angle;
	
	
};