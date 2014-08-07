#pragma once

#include "resource.h"
ID2D1Factory * pD2DFactory = NULL;
ID2D1HwndRenderTarget* pD2DHRT = NULL;

IWICImagingFactory* pWICImagingFactory = NULL;
IWICBitmapDecoder* pWICBitmapDecoder = NULL;
IWICFormatConverter* pWICFormatConverter = NULL;
IWICBitmapFrameDecode* pWICBitmapFrameDecode = NULL;

ID2D1Bitmap* pD2DBitMap = NULL;

HRESULT hr;

template<class Interface>
inline void SafeRelease(
	Interface **
	);

void Setup(HWND);
void ReleaseThemAll();
void setSourcePosition(int, int, int,
	float&, float&, float&, float&);
void setDestPosition(int, int, int,
	float&, float&, float&, float&);
void drawTileOnScreenBlock(int, int, int, int, int, int);