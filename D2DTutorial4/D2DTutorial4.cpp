// D2DTutorial4.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <d2d1.h>
#include <wincodec.h>
#include <wincodecsdk.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "Windowscodecs")
#include "D2DTutorial4.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
	)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

void setDestPosition(int blockX, int blockY, int blockSize,
	float& x1, float& y1, float& x2, float& y2) {
	x1 = static_cast<float>(blockX*blockSize);
	y1 = static_cast<float>(blockY*blockSize);

	x2 = x1 + blockSize;
	y2 = y1 + blockSize;
}

void setSourcePosition(int blockX, int blockY, int blockSize,
	float& x1, float& y1, float& x2, float& y2) {
	x1 = static_cast<float>(blockX*blockSize + 1);
	y1 = static_cast<float>(blockY*blockSize + 1);

	x2 = x1 + blockSize - 3;
	y2 = y1 + blockSize - 3;
}

void drawTileOnScreenBlock(int tileX, int tileY, int tileSize, int screenX, int screenY, int screenSize) {
	float srcX1, srcY1, srcX2, srcY2, destX1, destY1, destX2, destY2;
	setSourcePosition(tileX, tileY, tileSize,
		srcX1, srcY1, srcX2, srcY2);
	setDestPosition(screenX, screenY, screenSize,
		destX1, destY1, destX2, destY2);
	pD2DHRT->DrawBitmap(pD2DBitMap,
		D2D1::RectF(destX1, destY1, destX2, destY2),
		1.0,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(srcX1, srcY1, srcX2, srcY2));
}

void SetUp(HWND hWnd) {
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	RECT rc;
	GetClientRect(hWnd, &rc);

	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
		hWnd,
		D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top)
		),
		&pD2DHRT
		);

	if (SUCCEEDED(hr)) {
		hr = CoCreateInstance(CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory),
			(void**)(&pWICImagingFactory)
			);

		hr = pWICImagingFactory->CreateDecoderFromFilename(
			L"D:\\images\\tiles_spritesheet.png",
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pWICBitmapDecoder);

		hr = pWICImagingFactory->CreateFormatConverter(&pWICFormatConverter);

		hr = pWICBitmapDecoder->GetFrame(0, &pWICBitmapFrameDecode);

		hr = pWICFormatConverter->Initialize(
			pWICBitmapFrameDecode,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL, 0.f,
			WICBitmapPaletteTypeMedianCut
			);

		hr = pD2DHRT->CreateBitmapFromWicBitmap(
			pWICFormatConverter,
			NULL,
			&pD2DBitMap
			);
	}
}

void DrawMe(HWND hWnd) {
	SetUp(hWnd);

	int blockSize{ 72 };
	int realBlockSize{ 70 };

	pD2DHRT->BeginDraw();

	pD2DHRT->SetTransform(D2D1::Matrix3x2F::Identity());
	pD2DHRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// Flat tiles
	for (int i = 0; i != 10; i++) {
		drawTileOnScreenBlock(7, 8, blockSize, i, 8, realBlockSize);
	}
	// End left part of the land
	drawTileOnScreenBlock(8, 8, blockSize, 10, 8, realBlockSize);
	// Wave
	drawTileOnScreenBlock(6, 9, blockSize, 9, 9, realBlockSize);
	drawTileOnScreenBlock(6, 9, blockSize, 10, 9, realBlockSize);
	// Exit sign
	drawTileOnScreenBlock(4, 5, blockSize, 1, 7, realBlockSize);

	hr = pD2DHRT->EndDraw();

	ReleaseThemAll();

}

void ReleaseThemAll() {
	SafeRelease(&pD2DBitMap);
	SafeRelease(&pWICBitmapFrameDecode);
	SafeRelease(&pWICBitmapDecoder);
	SafeRelease(&pWICFormatConverter);
	SafeRelease(&pWICImagingFactory);
	SafeRelease(&pD2DHRT);
	SafeRelease(&pD2DFactory);
}

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_D2DTUTORIAL4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D2DTUTORIAL4));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2DTUTORIAL4));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_D2DTUTORIAL4);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   SetMenu(hWnd, NULL);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DISPLAYCHANGE:
		InvalidateRect(hWnd, NULL, FALSE);
	case WM_PAINT:
		DrawMe(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
