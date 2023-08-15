#pragma once
#include <string>
#include "Graphics.h"
#include "Globals.h"
#include "BIndablesHeader.h"
#include "Cube.h"

class App
{
	
public:
	App(HWND hWnd, int ScreenWidth, int ScreeHeight);
	

	void Update();

	void Draw();

private:
	DirectX::XMFLOAT4X4 storedProjection;
	Graphics gfx;
	Cube cube;
	Cube cube1;
	float angle;
	
	
};