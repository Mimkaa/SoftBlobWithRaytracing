// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include "DirectXMath.h"
#include "DirectXPackedVector.h"
#include <string>
#include "HelperFunctions.h"
#include "Graphics.h"
#include "BIndablesHeader.h"
#include "Drawable.h"



using namespace DirectX;

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")

XMMATRIX GetModelTansformation(float angle);

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nShowCmd)
{
    // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)COLOR_HIGHLIGHT;
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
        L"WindowClass1",    // name of the window class
        L"Our First Windowed Program",   // title of the window
        WS_OVERLAPPEDWINDOW,    // window style
        300,    // x-position of the window
        300,    // y-position of the window
        SCREEN_WIDTH, SCREEN_HEIGHT,   // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        hInstance,    // application handle
        NULL);    // used with multiple windows, NULL

    // display the window on the screen
    ShowWindow(hWnd, nShowCmd);
    
    // this struct holds Windows event messages
    MSG msg;

    float angle = 0.0f;
    Graphics gfx{hWnd, SCREEN_WIDTH, SCREEN_HEIGHT};

    Cube cube{ gfx };
    // wait for the next message in the queue, store the result in 'msg'
    while (TRUE)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);
        }
        // send transformations on the GPU side

        XMMATRIX translate = XMMatrixTranslation(0.0f, 0.0f, 4.5f);
        XMMATRIX model = XMMatrixMultiply(GetModelTansformation(angle), translate);
        XMFLOAT4X4 storedTranModel;
        XMFLOAT4X4 storedProjection;
        // projection 
        //////////////
        XMMATRIX projection = XMMatrixPerspectiveFovLH(1.5f, 4 / 3, 0.1f, 1000);

        XMStoreFloat4x4(&storedTranModel, XMMatrixTranspose(model));
        XMStoreFloat4x4(&storedProjection, XMMatrixTranspose(projection));

        cube.UpdateMatrix(gfx, storedTranModel, storedProjection);
        // render a frame
        const float colorBack[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        gfx.ClearScreen(colorBack);
        cube.Bind(gfx);
        cube.Draw(gfx);
        gfx.Present();
        //RenderFrame();

        if (msg.message == WM_QUIT)
        {
            break;
        }
        angle += 0.001f;
    }
    

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;


}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch (message)
    {
        // this message is read when the window is closed
    case WM_DESTROY:
    {
        // close the application entirely
        PostQuitMessage(0);// this will send a message WM_QUIT, which has an integer value of '0'.
        return 0;
    } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc(hWnd, message, wParam, lParam);
}


XMMATRIX GetModelTansformation(float angle)
{
    using namespace DirectX;
    XMMATRIX rotX = XMMatrixRotationX(angle);
    XMMATRIX rotY = XMMatrixRotationY(angle);
    XMMATRIX rotZ = XMMatrixRotationZ(angle);
    return XMMatrixMultiply(rotX, rotY);
}


    

