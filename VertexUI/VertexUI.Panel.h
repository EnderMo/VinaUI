#pragma once
/*
*
* VertexUI PanelDrawingInterface
*
* Vertillusion Studio 2021
*
*/
#pragma comment(lib,"Msimg32.lib")
#include "framework.h"
#include "VertexUI.Colors.h"
#include <Shlwapi.h>
#include <ShlObj.h>
#include <commoncontrols.h>
#include <algorithm>
#include <time.h>
#include <assert.h>
#include <vector>
#include <functional>
#include <map>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib") 
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <iostream>
#pragma comment(lib, "d2d1.lib") 
#pragma comment(lib, "dwrite.lib") 
ID2D1Factory* m_pD2DFactory = NULL;
IDWriteFactory* pDWriteFactory = NULL;

IWICImagingFactory* m_ImageFactory = NULL;

ID2D1DCRenderTarget* m_pDCRT = NULL;
double gScale = 1;
double GetScreenScale() {
	HWND hd = GetDesktopWindow();
	HDC hdc = GetDC(hd);
	int zoom = GetDeviceCaps(hdc, LOGPIXELSY);
	ReleaseDC(hd, hdc);

	double dpi = 1;
	switch (zoom) {
	case 96:
		dpi = 1;
		std::cout << "100%" << std::endl;
		break;
	case 120:
		dpi = 1.25;
		std::cout << "125%" << std::endl;
		break;
	case 144:
		dpi = 1.5;
		std::cout << "150%" << std::endl;
		break;
	case 168:
		dpi = 1.75;
		std::cout << "150%" << std::endl;
		break;
	case 192:
		dpi = 2;
		std::cout << "200%" << std::endl;
		break;
	case 216:
		dpi = 2.25;
		std::cout << "200%" << std::endl;
		break;
	case 240:
		dpi = 2.5;
		std::cout << "200%" << std::endl;
		break;
	default:
		std::cout << "error" << std::endl;
		break;
	}
	return dpi;
}
void InitGlobalD2D()
{
	ID2D1Factory* pD2DFactory = NULL;
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pD2DFactory
	);
	m_pD2DFactory = pD2DFactory;
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);

	m_pD2DFactory->CreateDCRenderTarget(&props, &m_pDCRT);

	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory));

	CoInitialize(NULL);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(m_ImageFactory), (LPVOID*)&m_ImageFactory);

}
void D2DInitDCRT(ID2D1DCRenderTarget** dc)
{
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);

	m_pD2DFactory->CreateDCRenderTarget(&props, dc);
}
int GlobalAnimationCount = 0;
bool GPURenderCompatibility = true;
#pragma warning(disable:4996)
#define FullDesign

#ifdef FullDesign
#include "VertexUI.ClickArea.h"
#endif
#include <shellapi.h>
#include "VertexUI.Control.hpp"
#define VERTEXUI_FLAGS_ANIMATION 0

#define VERTEXUI_FLAGS_NOANIMATION 1

#define VERTEXUI_FLAGS_MEMDRAW 2

#define VERTEXUI_FLAGS_CACHE 1

#define VERTEXUI_DEVMODE

#define RCWID (rc.right -rc.left)

#define InitCtl(ctlname,x,y,cx,cy,fun,txt) {ctlname,x,y,cx,cy,fun,txt}



namespace VertexUI
{
	typedef void (DRAWPANEL)(HWND, HDC);
	typedef void (D2DDRAWPANEL)(HWND, ID2D1DCRenderTarget*);
	typedef void (D2DHWNDDRAWPANEL)(HWND, ID2D1HwndRenderTarget*);
	typedef void (SDRAWPANEL)(HDC);
	typedef class VERTEXUICTL {
	public:
		const wchar_t* CtlName;
		int x;
		int y;
		int sizex;
		int sizey;
		int (*runfun)() = 0;
		const wchar_t* Text;
	}VUICTL;


	VertexUIColorInfo NCIX{ VERTEXUICOLOR_LAVENDER,VERTEXUICOLOR_WHITEDRAW,VERTEXUICOLOR_BLOOMLAVENDER,0,0,18,8 };
	VertexUIColorInfo NCIW{ VERTEXUICOLOR_LAVENDER,VERTEXUICOLOR_WHITEDRAW,VERTEXUICOLOR_BLOOMLAVENDER,0 };
	VertexUIColorInfo CNCIX{ VERTEXUICOLOR_LAVENDER,VERTEXUICOLOR_WHITE,VERTEXUICOLOR_BLOOMLAVENDER,0 };
	class PanelHandleFun
	{
	public:
		HDC hdc;
		HDC hmdc;
		RECT rc;
		HBITMAP hpbp;
		HBITMAP hmbp;
		int x;
		int y;
		int cx;
		int cy;
		DRAWPANEL* exDraw;
		COLORREF c1;
		COLORREF c2;
		HWND h;
	};
	typedef struct PanelDCCache
	{
		HDC inhdc;//兼容VertexUI内建
		HDC hdc;
		HDC hMemDC;
	};
	int ScrollDepth;
	int ScrollUpdate = 0;
	const wchar_t* ScrollSingleID = L"vui!NULL";
	int ScrollSinglePage;
	int ScrollAreaOffset;
	int ScrollArea = 2000;
	int dystate = 1;
	bool AlwaysRedraw = true;
	const wchar_t* ListPanel = L"ListPanel";
	const wchar_t* ViewPanel = L"ViewPanel";
	const wchar_t* PanelID = L"Init";
	const wchar_t* PrevPanelID = L"Init";
	const wchar_t* ButtonText = L"Button";
	int g_hoverstate = 0;

	VertexUIPos SharedPos;
	namespace Panel
	{
		//CreatePanel only passed in parameters below:(HWND,HDC).

		//It convert (x,y,sizex,sizey) into RECT {x,y,x+sizex,y+sizey}.
		void RectTypeConvert(RECT& rc, int x, int y, int sizex, int sizey)
		{
			rc = { x,y,x + sizex,y + sizey };
		}
		RECT RectTypeConvert(int x, int y, int sizex, int sizey)
		{
			RECT rc = { x,y,x + sizex,y + sizey };
			return rc;
		}
		void Refresh(HWND hWnd)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			InvalidateRect(hWnd, &rc, 0);
		}
		template <class T> void SafeRelease(T** ppT)
		{
			if (*ppT)
			{
				(*ppT)->Release();
				*ppT = NULL;
			}
		}
		unsigned long RGBToHex(unsigned long OriClr)
		{
			return GetRValue(OriClr) << 16 | GetGValue(OriClr) << 8 | GetBValue(OriClr);
		}
		template<class T>
		void D2DDrawRoundRect(T* m_pDCRT, float x, float y, float cx, float cy, unsigned long ClrFill, float radius, float alpha = 1, float border = 0, unsigned long borderColor = 0, float borderAlphaSpecial = 0, bool OnlyBorder = false)
		{
			int hr = 0;
			ID2D1SolidColorBrush* testBrush = NULL;
			hr = m_pDCRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(ClrFill), alpha)),
				&testBrush
			);
			ID2D1SolidColorBrush* testBrushOut = NULL;
			float ba = alpha;
			if (borderAlphaSpecial != 0)ba = borderAlphaSpecial;
			hr = m_pDCRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(borderColor), ba)),
				&testBrushOut
			);
			if (SUCCEEDED(hr))
			{
				D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
					D2D1::RectF(x, y, cx + x, cy + y),
					radius,
					radius
				);
				m_pDCRT->DrawRoundedRectangle(roundedRect, testBrushOut, border);
				if (OnlyBorder == false)
				{
					m_pDCRT->FillRoundedRectangle(roundedRect, testBrush);
				}
				SafeRelease(&testBrush);
				SafeRelease(&testBrushOut);
			}
		}
		template<class T>
		void D2DDrawRoundRect2(T m_pDCRT, float x, float y, float cx, float cy, unsigned long ClrFill, float radius, float alpha = 1, float border = 0, unsigned long borderColor = 0, float borderAlphaSpecial = 0, bool OnlyBorder = false)
		{
			int hr = 0;
			ID2D1SolidColorBrush* testBrush = NULL;
			hr = m_pDCRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(ClrFill), alpha)),
				&testBrush
			);
			ID2D1SolidColorBrush* testBrushOut = NULL;
			float ba = alpha;
			if (borderAlphaSpecial != 0)ba = borderAlphaSpecial;
			hr = m_pDCRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(borderColor), ba)),
				&testBrushOut
			);
			if (SUCCEEDED(hr))
			{
				D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
					D2D1::RectF(x, y, cx + x, cy + y),
					radius,
					radius
				);
				m_pDCRT->DrawRoundedRectangle(roundedRect, testBrushOut, border);
				if (OnlyBorder == false)
				{
					m_pDCRT->FillRoundedRectangle(roundedRect, testBrush);
				}
				SafeRelease(&testBrush);
				SafeRelease(&testBrushOut);
			}
		}
		template<class T>
		void D2DDrawSolidRect(T* m_pDCRT, float x, float y, float cx, float cy, unsigned long ClrFill, float alpha = 1)
		{
			ID2D1SolidColorBrush* testBrush = NULL;
			int hr = 0;

			hr = m_pDCRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(ClrFill), alpha)),
				&testBrush
			);
			if (SUCCEEDED(hr))
			{
				D2D1_RECT_F Rect = D2D1::Rect(
					x, y, cx + x, cy + y
				);
				m_pDCRT->FillRectangle(Rect, testBrush);
				//testBrush->Release();
				//*testBrush = NULL;
				SafeRelease(&testBrush);
			}
			else
			{
				MessageBox(0, 0, 0, 0);
			}
		}
		void CompD2DGdi(HWND hWnd, HDC hdc, D2DDRAWPANEL* DrawFun)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			m_pDCRT->BindDC(hdc, &rc);
			m_pDCRT->BeginDraw();
			m_pDCRT->SetTransform(D2D1::Matrix3x2F::Identity());
			DrawFun(hWnd, m_pDCRT);
			m_pDCRT->EndDraw();

		}
		template<class T>
		void CompGdiD2D(HWND hWnd, T* hrt, std::function<DRAWPANEL> dwf)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top
			);


			ID2D1GdiInteropRenderTarget* m_pGDIRT;
			hrt->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (void**)&m_pGDIRT);

			HDC hDC = NULL;
			m_pGDIRT->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);


			dwf(hWnd, hDC);
			m_pGDIRT->ReleaseDC(NULL);
			SafeRelease(&m_pGDIRT);
		}
		template<class T>
		void D2DDrawText(T pRenderTarget, const wchar_t* Text, int x, int y, int cx, int cy, float Size = 18, unsigned long ClrFill = VERTEXUICOLOR_WHITE, const wchar_t* font = L"Segoe UI", float alpha = 1, DWRITE_FONT_WEIGHT wid = DWRITE_FONT_WEIGHT_NORMAL)
		{
			IDWriteTextFormat* pTextFormat = NULL;
			ID2D1SolidColorBrush* testBrush = NULL;
			pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(ClrFill), alpha)),
				&testBrush
			);
			//create text format
			pDWriteFactory->CreateTextFormat(
				font,
				NULL,
				wid,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				Size,
				L"",
				&pTextFormat
			);

			D2D1_RECT_F layoutRect = D2D1::RectF(x, y, x + cx, y + cy);

			//draw text
			pRenderTarget->DrawText(
				Text,
				wcslen(Text),
				pTextFormat,
				layoutRect,
				testBrush
			);
			SafeRelease(&pTextFormat);
			SafeRelease(&testBrush);
		}
		template<class T>
		void D2DDrawText2(T pRenderTarget, const wchar_t* Text, float x, float y, float cx, float cy, float Size = 18, unsigned long ClrFill = VERTEXUICOLOR_WHITE, const wchar_t* font = L"Segoe UI", float alpha = 1, bool center = false)
		{
			IDWriteTextFormat* pTextFormat = NULL;
			ID2D1SolidColorBrush* testBrush = NULL;
			pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(ClrFill), alpha)),
				&testBrush
			);
			//create text format
			pDWriteFactory->CreateTextFormat(
				font,
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				Size,
				L"",
				&pTextFormat
			);
			if (center == true)pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			D2D1_RECT_F layoutRect = D2D1::RectF(x, y, x + cx, y + cy);

			//draw text
			pRenderTarget->DrawText(
				Text,
				wcslen(Text),
				pTextFormat,
				layoutRect,
				testBrush
			);
			SafeRelease(&pTextFormat);
			SafeRelease(&testBrush);
		}
		void CompD2DDrawRoundRect(HWND hWnd, HDC hdc, int x, int y, int cx, int cy, unsigned long ClrFill, int radius, float alpha = 1, int border = 0, unsigned long borderColor = 0, float borderAlphaSpecial = 0, bool OnlyBorder = false)
		{
			// Create a DC render target.



			RECT rc;



			HRESULT hr;

			// Get the dimensions of the client drawing area.
			GetClientRect(hWnd, &rc);


			// Create the DC render target.

			hr = m_pDCRT->BindDC(hdc, &rc);

			m_pDCRT->BeginDraw();

			m_pDCRT->SetTransform(D2D1::Matrix3x2F::Identity());

			//m_pDCRT->Clear(D2D1::ColorF(D2D1::ColorF(1,1,1,1)));
			D2DDrawRoundRect(m_pDCRT, x, y, cx, cy, ClrFill, radius, alpha, border, borderColor, borderAlphaSpecial, OnlyBorder);

			hr = m_pDCRT->EndDraw();

			//SafeRelease(&m_pDCRT);


		}
		ID2D1HwndRenderTarget* pRT1;
		std::unordered_map<HWND, ID2D1HwndRenderTarget*> pRts;
		void CreateD2DPanel(HWND hWnd, D2DHWNDDRAWPANEL* dwf) {
			RECT rc;
			GetClientRect(hWnd, &rc);
			ID2D1HwndRenderTarget* pRT = NULL;

			// Create a Direct2D render target          
			if (pRT == NULL && pRts[hWnd]==nullptr)
			{
				D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
				rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
				m_pD2DFactory->CreateHwndRenderTarget(
					rtProps,
					D2D1::HwndRenderTargetProperties(
						hWnd,
						D2D1::SizeU(
							rc.right - rc.left,
							rc.bottom - rc.top)
					),
					&pRT
				);
				pRts[hWnd] = pRT;
			}
			pRts[hWnd]->BeginDraw();

			dwf(hWnd, pRts[hWnd]);

			HRESULT hr = pRts[hWnd]->EndDraw();
			if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)SafeRelease(&pRts[hWnd]);
			//SafeRelease(&pRT);
		}
		//GDI
		//DrawRect
		void DrawRect(HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl)
		{
			RECT mRc;
			RectTypeConvert(mRc, x, y, sizex, sizey);
			HBRUSH hb = CreateSolidBrush(cl);
			HBRUSH bhb = (HBRUSH)SelectObject(hdc, hb);
			SelectObject(hdc, hb);
			FillRect(hdc, &mRc, hb);
			SelectObject(hdc, bhb);
			DeleteObject(hb);
		}
		void DrawRect(HWND h, HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl)
		{
			DrawRect(hdc, x, y, sizex, sizey, cl);
		}
#define CreateRect(hWnd,hdc, x, y,sizex,sizey,cl) DrawRect(hWnd,hdc, x, y,sizex,sizey,cl)
		void DrawHollowRect(HDC hdc, int x, int y, int cx, int cy, int p, COLORREF cl)
		{
			HPEN hPen = ::CreatePen(PS_SOLID, p, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hdc, OBJ_PEN);
			HBRUSH hOldBrush = (HBRUSH)::GetCurrentObject(hdc, OBJ_BRUSH);

			SelectObject(hdc, hPen);
			SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
			Rectangle(hdc, x, y, x + cx, y + cy);


			SelectObject(hdc, hOldBrush);
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);
		}
#define CreateHollowRect(hdc, x, y, cx, cy,  p, cl) DrawHollowRect(hdc, x, y, cx, cy,  p, cl)
		void DrawLine(HDC hdc,/*POINT* pt*/int x, int y, int x1, int y1, int p, COLORREF cl)
		{
			HPEN hPen = ::CreatePen(PS_SOLID, p, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hdc, OBJ_PEN);

			SelectObject(hdc, hPen);

			MoveToEx(hdc, x, y, 0);
			LineTo(hdc, x1, y1);


			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);
		}
		void CreateRectArea(HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl)
		{
			RECT mRc;
			RectTypeConvert(mRc, x, y, sizex, sizey);
			HBRUSH hb = CreateSolidBrush(cl);
			HBRUSH bhb = (HBRUSH)SelectObject(hdc, hb);
			SelectObject(hdc, hb);
			FillRect(hdc, &mRc, hb);
			SelectObject(hdc, bhb);
			DeleteObject(hb);
		}

		//Fill the whole window
		void CreateFillArea(HWND h, HDC hdc, COLORREF cl)
		{
			RECT Rc;
			GetClientRect(h, &Rc);
			DrawRect(hdc, Rc.left, Rc.top, Rc.right, Rc.bottom, cl);
		}

		//Text
		void TextPreDraw(HDC hdc, int x, int y, int sizex, int sizey, const wchar_t* txt, COLORREF cl)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, sizex, sizey);
			LOGFONT lf;
			HFONT hFont = 0;
			SetTextColor(hdc, cl);
			SetBkMode(hdc, TRANSPARENT);
			if (hFont == 0)
			{
				memset(&lf, 0, sizeof(LOGFONT));
				lf.lfHeight = -18;
				wcscpy_s(lf.lfFaceName, L"Segoe UI");
				hFont = CreateFontIndirect(&lf);  // create the font
			}
			HFONT old = (HFONT)SelectObject(hdc, hFont);
			DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			DeleteObject(hFont);
			SelectObject(hdc, old);
		}

		//Flags: 0.ToLeft 1.Center 
		void TextPreDrawEx(HDC hdc, int x, int y, int sizex, int sizey, const wchar_t* txt, int sz, int flag, COLORREF cl)
		{
			if (txt == nullptr)return;
			RECT rc;
			RectTypeConvert(rc, x, y, sizex, sizey);
			LOGFONT lf;
			HFONT hFont = 0;
			SetTextColor(hdc, cl);
			SetBkMode(hdc, TRANSPARENT);
			if (hFont == 0)
			{
				memset(&lf, 0, sizeof(LOGFONT));
				lf.lfHeight = -sz;
				//lf.lfWeight = 10;
				wcscpy_s(lf.lfFaceName, L"Segoe UI");
				hFont = CreateFontIndirect(&lf);  // create the font
			}
			HFONT old = (HFONT)SelectObject(hdc, hFont);
			if (flag == 0)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_VCENTER);
			}
			if (flag == 1)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			if (flag == 2)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE);
			}
			if (flag == 3)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_CENTER | DT_VCENTER);
			}
			if (flag == 4)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, 0);
			}
			DeleteObject(hFont);
			SelectObject(hdc, old);
		}
		void _TextPreDrawEx(HDC hdc, int x, int y, int sizex, int sizey, const wchar_t* txt, int sz, const wchar_t* Font, int flag, COLORREF cl)
		{
			if (txt == nullptr)return;
			RECT rc;
			RectTypeConvert(rc, x, y, sizex, sizey);
			LOGFONT lf;
			HFONT hFont = 0;
			SetTextColor(hdc, cl);
			SetBkMode(hdc, TRANSPARENT);
			if (hFont == 0)
			{
				memset(&lf, 0, sizeof(LOGFONT));
				lf.lfHeight = -sz;
				if (flag == 5)
				{
					lf.lfStrikeOut = TRUE;
					flag = 0;
				}
				//lf.lfQuality = ANTIALIASED_QUALITY;
				wcscpy_s(lf.lfFaceName, Font);
				hFont = CreateFontIndirect(&lf);  // create the font
			}
			HFONT old = (HFONT)SelectObject(hdc, hFont);
			if (flag == 0)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_VCENTER);
			}
			if (flag == 1)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			if (flag == 2)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE);
			}
			if (flag == 3)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_CENTER | DT_VCENTER);
			}
			if (flag == 4)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_VCENTER);
			}
			DeleteObject(hFont);
			SelectObject(hdc, old);
		}
		int TextDrawOuter(HDC hDrawingDC, int x, int y, int cx, int cy, int sz, int mode, const wchar_t* txt, const wchar_t* font = L"Segoe UI", int outsz = 1, COLORREF inC = 0, COLORREF OutC = VERTEXUICOLOR_WHITE)
		{
			::BeginPath(hDrawingDC);
			_TextPreDrawEx(hDrawingDC, x, y, cx, cy, txt, sz, font, mode, 0);
			::EndPath(hDrawingDC);
			HPEN hp = CreatePen(PS_SOLID, outsz, OutC);
			HPEN hOld = (HPEN)SelectObject(hDrawingDC, hp);
			HBRUSH hb = CreateSolidBrush(inC);
			HBRUSH HbR = (HBRUSH)SelectObject(hDrawingDC, hb);
			::StrokeAndFillPath(hDrawingDC);
			SelectObject(hDrawingDC, hOld);
			SelectObject(hDrawingDC, HbR);
			DeleteObject(hb);
			DeleteObject(hp);
			return 0;
		}
		//Only straight Line
		void PanelDrawSTLine(HDC pDC, int X0, int Y0, int X1, int Y1, COLORREF clrLine)
		{
			if (Y0 > Y1) { int Temp = Y0; Y0 = Y1; Y1 = Temp; Temp = X0; X0 = X1; X1 = Temp; }SetPixel(pDC, X0, Y0, clrLine);
			int XDir, DeltaX = X1 - X0; if (DeltaX >= 0) { XDir = 1; }
			else { XDir = -1; DeltaX = 0 - DeltaX; }int DeltaY = Y1 - Y0;
			if (DeltaY == 0) { while (DeltaX-- != 0) { X0 += XDir; SetPixel(pDC, X0, Y0, clrLine); }return; }
			if (DeltaX == 0) { do { Y0++; SetPixel(pDC, X0, Y0, clrLine); } while (--DeltaY != 0); return; }
			if (DeltaX == DeltaY) { do { X0 += XDir; Y0++; SetPixel(pDC, X0, Y0, clrLine); } while (--DeltaY != 0); return; }SetPixel(pDC, X1, Y1, clrLine);
		}
#define SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
		HBITMAP ExtractFileIcon(LPCTSTR pszPath, UINT nWidth, UINT nHeight)
		{
			HBITMAP hBitmap = NULL;
			if ((NULL != pszPath) && (nWidth > 0.0) && (nHeight > 0.0))
			{
				IShellItemImageFactory* psif = NULL;
				SIZE size = { nWidth, nHeight };
				HRESULT hr = ::SHCreateItemFromParsingName(pszPath, NULL, IID_PPV_ARGS(&psif));
				if (SUCCEEDED(hr) && (NULL != psif))
				{
					psif->GetImage(size, SIIGBF_ICONONLY, &hBitmap);
					//OutputDebugString(L"114514");//测试完成
				}
				else
				{
					hBitmap = 0;
				}
				//psif->Release();
				SAFE_RELEASE(psif);
			}

			return hBitmap;
		}
		void DisplayIco(HDC hdc, RECT rc, HBITMAP hb)
		{
			HDC hMemDC = CreateCompatibleDC(hdc);               // 创建一个兼容内存DC
			HGDIOBJ hOldObj = SelectObject(hMemDC, hb);   // 将g_bitmap选入到这个内存DC

			//BitBlt(hdc, x, y, ICON_WIDTH, ICON_HEIGHT, hMemDC, 0, 0, SRCCOPY);

			BLENDFUNCTION ftn = { 0 };
			ftn.BlendOp = AC_SRC_OVER;                           // 目前只能设置这个值
			ftn.AlphaFormat = AC_SRC_ALPHA;                      // 也只能设置这个值
			ftn.BlendFlags = 0;                                  // 必须为0
			ftn.SourceConstantAlpha = 255;                       // 指定源图片的alpha

			// 调用这个函数来进行Alpha混合
			AlphaBlend(hdc, rc.left + 2, rc.top + 2, 32, 32, hMemDC, 0, 0, 32, 32, ftn);

			SelectObject(hMemDC, hOldObj);
			DeleteObject(hMemDC);
			DeleteObject(hOldObj);
			DeleteObject(hb);
		}
		void DisplayIcoEx(HDC hdc, RECT rc, HBITMAP hb)
		{
			HDC hMemDC = CreateCompatibleDC(hdc);               // 创建一个兼容内存DC
			HGDIOBJ hOldObj = SelectObject(hMemDC, hb);   // 将g_bitmap选入到这个内存DC

			//BitBlt(hdc, x, y, ICON_WIDTH, ICON_HEIGHT, hMemDC, 0, 0, SRCCOPY);

			BLENDFUNCTION ftn = { 0 };
			ftn.BlendOp = AC_SRC_OVER;                           // 目前只能设置这个值
			ftn.AlphaFormat = AC_SRC_ALPHA;                      // 也只能设置这个值
			ftn.BlendFlags = 0;                                  // 必须为0
			ftn.SourceConstantAlpha = 255;                       // 指定源图片的alpha

			// 调用这个函数来进行Alpha混合
			AlphaBlend(hdc, rc.left + 2, rc.top + 2, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, RCWID, rc.bottom - rc.top, ftn);

			SelectObject(hMemDC, hOldObj);
			DeleteObject(hMemDC);
			DeleteObject(hOldObj);
			DeleteObject(hb);
		}
		//Frame
		HICON GetFileIcon(const wchar_t* strFilePath, BOOL bLarge)
		{
			SHFILEINFO SHFI;  ZeroMemory(&SHFI, sizeof(SHFI));  DWORD_PTR ret = ::SHGetFileInfo(strFilePath, 0, &SHFI, sizeof(SHFI),
				SHGFI_ICON | (bLarge ? SHGFI_LARGEICON : SHGFI_SMALLICON));  if (ret != 0) { return SHFI.hIcon; }  return NULL;
		}

		HICON GetFileIcon2(const wchar_t* pszPath)
		{
			SHFILEINFO sfi;
			if (!SHGetFileInfo(pszPath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX)) return NULL;

			// 获取大号图像列表
			IImageList* piml;

			if (FAILED(SHGetImageList(SHIL_EXTRALARGE, IID_PPV_ARGS(&piml)))) return NULL;

			// 提取图标
			HICON hico;
			piml->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hico);

			// 清理资源
			piml->Release();

			return hico;

		}
		void DisplayIcon(HDC hdc, const wchar_t* path, int x, int y, int sz)
		{
			//正式代码
			if (path == std::wstring(L"NULL"))return;
			if (path == std::wstring(L""))return;
			if (path == std::wstring(L" "))return;
			HICON hIcon = GetFileIcon(path, 1);
			if (hIcon != NULL)
				DrawIconEx(hdc, x, y, hIcon, sz, sz, 0, NULL, DI_NORMAL);
			DestroyIcon(hIcon);
		}
		template <typename T>
		void D2DDisplayIcon(T hdc, const wchar_t* path, int x, int y, int sz)
		{
			ID2D1Bitmap* pBitmap = NULL;
			if (std::wstring(path) == L"NULL" || std::wstring(path) == L"" || std::wstring(path) == L" ")
				return;

			HICON hIcon = GetFileIcon(path, 1);
			if (!hIcon) return;

			ICONINFO iconInfo;
			GetIconInfo(hIcon, &iconInfo);

			// 获取图标的位图信息
			BITMAP bm;
			GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bm);

			// 创建一个 D2D 位图
			D2D1_SIZE_U size = D2D1::SizeU(bm.bmWidth, bm.bmHeight);
			D2D1_BITMAP_PROPERTIES bitmapProps = D2D1::BitmapProperties(
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

			BYTE* pPixels = new BYTE[bm.bmWidthBytes * bm.bmHeight];
			GetBitmapBits(iconInfo.hbmColor, bm.bmWidthBytes * bm.bmHeight, pPixels);

			ID2D1Bitmap* d2dBitmap = nullptr;
			hdc->CreateBitmap(size, pPixels, bm.bmWidthBytes, bitmapProps, &d2dBitmap);



			// 绘制 D2D 位图
			hdc->DrawBitmap(d2dBitmap, D2D1::RectF(x, y, x + sz, y + sz));

			// 结束绘制
			HRESULT hr = hdc->EndDraw();
			if (FAILED(hr))
			{
				OutputDebugString(L"Failed to render icon.");
			}

			// 清理
			delete[] pPixels;
			d2dBitmap->Release();
			DeleteObject(iconInfo.hbmColor);
			DeleteObject(iconInfo.hbmMask);
		}
		ID2D1Bitmap* CreateIconBitmap(ID2D1RenderTarget* pRenderTarget, const wchar_t* path, int sz)
		{
			if (!pRenderTarget) return nullptr;

			// 获取HICON
			HICON hIcon = nullptr;
			SHFILEINFO shfi = { 0 };
			if (SHGetFileInfo(path, 0, &shfi, sizeof(shfi), SHGFI_ICON | SHGFI_LARGEICON)) {
				hIcon = shfi.hIcon;
			}

			if (!hIcon) return nullptr;

			// 创建32位DIB
			HDC hdcScreen = GetDC(nullptr);
			HDC hdcMem = CreateCompatibleDC(hdcScreen);

			BITMAPINFO bmi = { 0 };
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = sz;
			bmi.bmiHeader.biHeight = -sz;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;

			BYTE* pBits = nullptr;
			HBITMAP hBitmap = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, (void**)&pBits, nullptr, 0);

			HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);
			DrawIconEx(hdcMem, 0, 0, hIcon, sz, sz, 0, nullptr, DI_NORMAL);
			SelectObject(hdcMem, hOld);

			// 创建D2D位图
			D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
			);

			ID2D1Bitmap* pD2DBitmap = nullptr;
			HRESULT hr = pRenderTarget->CreateBitmap(
				D2D1::SizeU(sz, sz),
				pBits,
				sz * 4,
				&props,
				&pD2DBitmap
			);

			// 清理
			DeleteObject(hBitmap);
			DeleteDC(hdcMem);
			ReleaseDC(nullptr, hdcScreen);

			return pD2DBitmap;
		}
		int gByteWidth = 0;
		int gD2DBmWid = 0;
		int gD2DBmHei = 0;
		BYTE* D2DGetIconByte(const wchar_t* path, int sz)
		{

			if (std::wstring(path) == L"NULL" || std::wstring(path) == L"" || std::wstring(path) == L" ")
				return nullptr;

			HICON hIcon = GetFileIcon2(path);
			if (!hIcon) return nullptr;

			ICONINFO iconInfo;
			GetIconInfo(hIcon, &iconInfo);

			// 获取图标的位图信息
			BITMAP bm;
			GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bm);


			BYTE* pPixels = new BYTE[(bm.bmWidthBytes + 16) * (bm.bmHeight + 16)];

			GetBitmapBits(iconInfo.hbmColor, (bm.bmWidthBytes) * (bm.bmHeight), pPixels);
			gByteWidth = bm.bmWidthBytes;
			gD2DBmWid = bm.bmWidth;
			gD2DBmHei = bm.bmHeight;
			/*
			// 绘制 D2D 位图
			hdc->DrawBitmap(d2dBitmap, D2D1::RectF(x, y, x + sz, y + sz));
			//delete[] pPixels;
			//d2dBitmap->Release();
			*/
			DeleteObject(iconInfo.hbmColor);
			DeleteObject(iconInfo.hbmMask);
			return pPixels;
		}
		template <typename T>
		void D2DDrawBitmapFrompBm(T hrt, ID2D1Bitmap* pBm, int x, int y, int sz)
		{
			if (!hrt || !pBm) return;

			ID2D1RenderTarget* pRenderTarget = nullptr;

			// 根据不同的类型获取渲染目标
			if constexpr (std::is_same_v<T, ID2D1HwndRenderTarget*> ||
				std::is_same_v<T, ID2D1RenderTarget*>) {
				pRenderTarget = static_cast<ID2D1RenderTarget*>(hrt);
			}
			else if constexpr (std::is_same_v<T, ID2D1DeviceContext*>) {
				pRenderTarget = static_cast<ID2D1DeviceContext*>(hrt);
			}
			else {
				// 对于其他类型，假设可以直接转换为ID2D1RenderTarget
				pRenderTarget = static_cast<ID2D1RenderTarget*>(hrt);
			}

			if (!pRenderTarget) return;

			// 创建目标矩形
			D2D1_RECT_F destRect = D2D1::RectF(
				static_cast<FLOAT>(x),
				static_cast<FLOAT>(y),
				static_cast<FLOAT>(x + sz),
				static_cast<FLOAT>(y + sz)
			);

			// 创建源矩形（整个位图）
			D2D1_SIZE_U bitmapSize = pBm->GetPixelSize();
			D2D1_RECT_F sourceRect = D2D1::RectF(0, 0,
				static_cast<FLOAT>(bitmapSize.width),
				static_cast<FLOAT>(bitmapSize.height));

			// 设置插值模式以获得更好的缩放效果
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

			// 绘制位图
			pRenderTarget->DrawBitmap(
				pBm,
				destRect,
				1.0f, // opacity
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				sourceRect
			);
		}

		template <typename T>
		void D2DDrawBitmapFromByte(T hdc, BYTE* pPixels, int x, int y, int sz)
		{
			int bp = gByteWidth;

			// 创建一个 D2D 位图
			D2D1_SIZE_U size = D2D1::SizeU(gD2DBmWid, gD2DBmHei);
			D2D1_BITMAP_PROPERTIES bitmapProps = D2D1::BitmapProperties(
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));


			ID2D1Bitmap* d2dBitmap = nullptr;
			hdc->CreateBitmap(size, pPixels, bp, bitmapProps, &d2dBitmap);



			// 绘制 D2D 位图

			hdc->DrawBitmap(d2dBitmap, D2D1::RectF(x, y, x + sz, y + sz), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, gD2DBmWid, gD2DBmHei));

			// 结束绘制

			// 清理
			d2dBitmap->Release();
		}
		void CreatehIcon(HDC hdc, HICON hIcon, int x, int y, int sz)
		{
			if (hIcon != NULL)
				DrawIconEx(hdc, x, y, hIcon, sz, sz, 0, NULL, DI_NORMAL);
		}
		void PanelDrawOutFrame(HWND h, HDC hdc, COLORREF cl)
		{
			RECT rc;
			GetClientRect(h, &rc);
			CreateHollowRect(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 1, cl);
			/*
		PanelDrawSTLine(hdc, 0, 0, rc.right, 0, cl);
		PanelDrawSTLine(hdc, 0, 0, 0, rc.bottom, cl);
		PanelDrawSTLine(hdc, 0, rc.bottom - 1, rc.right - rc.left, rc.bottom - 1, cl);
		PanelDrawSTLine(hdc, rc.right - 1, 0, rc.right - 1, rc.bottom - 1, cl);
		*/
		}
		void PanelDrawCloseBtn(HWND h, HDC hdc, int x, int y, int x1, int y1, int inframepos, COLORREF cl)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, x1, y1);
			HPEN hpen;
			hpen = CreatePen(PS_SOLID, 1, cl);
			HPEN prhp = (HPEN)SelectObject(hdc, hpen);
			SelectObject(hdc, hpen);
			MoveToEx(hdc, rc.left + inframepos + 1, rc.top + inframepos + 1, NULL);
			LineTo(hdc, rc.right - inframepos, rc.bottom - inframepos);

			MoveToEx(hdc, rc.left + inframepos + 1, rc.bottom - inframepos - 1, NULL);
			LineTo(hdc, rc.right - inframepos, rc.top + inframepos);
			DeleteObject(hpen);
			SelectObject(hdc, prhp);
		}
		void PanelDrawDisplayBtn(HWND h, HDC hdc, int x, int y, int x1, int y1, int inframepos, int mdpos, COLORREF cl)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, x1, y1);
			HPEN hpen;
			hpen = CreatePen(PS_SOLID, 1, cl);
			HPEN prhp = (HPEN)SelectObject(hdc, hpen);
			SelectObject(hdc, hpen);
			MoveToEx(hdc, rc.left + inframepos + 1, rc.top + inframepos + 1 + mdpos, NULL);
			LineTo(hdc, rc.left + (x1 / 2) - inframepos, rc.top + (y1 / 2) - inframepos + mdpos);
			MoveToEx(hdc, rc.right - (x1 / 2.5) - 1, rc.top + inframepos + 1 + mdpos, NULL);
			LineTo(hdc, rc.right - (x1 / 2) - inframepos - 1, rc.bottom - (y1 / 2) - inframepos + mdpos);
			//MoveToEx(hdc, rc.left + inframepos + 1, rc.bottom - inframepos - 1, NULL);
			//LineTo(hdc, rc.right - inframepos, rc.top + inframepos);
			DeleteObject(hpen);
			SelectObject(hdc, prhp);
		}
		void PanelDrawDisplayBtn2(HWND h, HDC hdc, int x, int y, int x1, int y1, int inframepos, int mdpos, COLORREF cl)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, x1, y1);
			HPEN hpen;
			hpen = CreatePen(PS_SOLID, 1, cl);
			HPEN prhp = (HPEN)SelectObject(hdc, hpen);
			SelectObject(hdc, hpen);
			MoveToEx(hdc, rc.left + inframepos + 1, rc.top + inframepos + 1 + mdpos, NULL);
			LineTo(hdc, rc.left + (x1 / 2) - inframepos, rc.top + (y1 / 2) - inframepos + mdpos);
			MoveToEx(hdc, rc.right - (x1 / 2) - 0, rc.top + inframepos + 1 + mdpos, NULL);
			LineTo(hdc, rc.right - (x1 / 2) - inframepos - 1, rc.bottom - (y1 / 2) - inframepos + mdpos);
			//MoveToEx(hdc, rc.left + inframepos + 1, rc.bottom - inframepos - 1, NULL);
			//LineTo(hdc, rc.right - inframepos, rc.top + inframepos);
			DeleteObject(hpen);
			SelectObject(hdc, prhp);
		}
		//Button
		void CreateSimpleButton(HWND h, HDC hdc, int x, int y, int sizex, int sizey, const wchar_t* s)
		{
			CreateRect(h, hdc, x, y, sizex, sizey, VERTEXUI_GREENSEA);
			TextPreDraw(hdc, x, y, sizex, sizey, s, VERTEXUI_WHITE);
		}

		//Custom Color Button
		void CreateSimpleButtonEx(HWND h, HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl, const wchar_t* s, int ctsz = 0, COLORREF tClr = VERTEXUICOLOR_WHITE)
		{
			int tsz = 0;
			CreateRect(h, hdc, x, y, sizex, sizey, cl);
			if (sizey > 23)
			{
				tsz = 18;
			}
			else
			{
				tsz = 16;
			}
			if (ctsz != 0)tsz = ctsz;
			TextPreDrawEx(hdc, x, y, sizex, sizey, s, tsz, 1, tClr);
		}
		void CreateAlphaMask(HDC hdc, int x, int y, int sizex, int sizey, int a, COLORREF cl);
		void DrawLinearGradient(HDC pDC, int x, int y, int cx, int cy, COLORREF clr1, COLORREF clr2, int angle);
		void CreateTransparentButton(HWND h, HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl, const wchar_t* s, int a, int ctsz = 0)
		{
			int tsz = 0;
			CreateAlphaMask(hdc, x, y, sizex, sizey, a, cl);
			if (sizey > 23)
			{
				tsz = 18;
			}
			else
			{
				tsz = 16;
			}
			if (ctsz != 0)tsz = ctsz;
			TextPreDrawEx(hdc, x, y, sizex, sizey, s, tsz, 1, VERTEXUI_WHITE);
		}
		void CreateGradientButton(HWND h, HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl, COLORREF clTo, int deg, const wchar_t* s, int ctsz = 0, COLORREF tClr = VERTEXUICOLOR_WHITE)
		{
			int tsz = 0;
			DrawLinearGradient(hdc, x, y, sizex, sizey, cl, clTo, deg);
			if (sizey > 23)
			{
				tsz = 18;
			}
			else
			{
				tsz = 16;
			}
			if (ctsz != 0)tsz = ctsz;
			TextPreDrawEx(hdc, x, y, sizex, sizey, s, tsz, 1, tClr);
		}
		void DrawRoundRect(HDC hDrawingDC, int x, int y, int cx, int cy, int cornersize, COLORREF cl)
		{
			HPEN hGreenPen = ::CreatePen(PS_SOLID, 2, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hDrawingDC, OBJ_PEN);
			HBRUSH hOldBrush = (HBRUSH)CreateSolidBrush(cl);

			::SelectObject(hDrawingDC, hGreenPen);
			::SelectObject(hDrawingDC, hOldBrush);
			::RoundRect(hDrawingDC, x, y, x + cx, y + cy, cornersize, cornersize);


			::SelectObject(hDrawingDC, hOldBrush);
			DeleteObject(hOldBrush);
			::SelectObject(hDrawingDC, hOldPen);
			::DeleteObject(hGreenPen);
		}
		void DrawHollowRoundRect(HDC hDrawingDC, int x, int y, int cx, int cy, int cornersize, int pensize, COLORREF cl)
		{
			HPEN hPen = ::CreatePen(PS_SOLID, pensize, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hDrawingDC, OBJ_PEN);
			HBRUSH hOldBrush = (HBRUSH)::GetCurrentObject(hDrawingDC, OBJ_BRUSH);

			SelectObject(hDrawingDC, hPen);
			SelectObject(hDrawingDC, (HBRUSH)GetStockObject(NULL_BRUSH));
			RoundRect(hDrawingDC, x, y, x + cx, y + cy, cornersize, cornersize);


			SelectObject(hDrawingDC, hOldBrush);
			DeleteObject(hOldBrush);
			SelectObject(hDrawingDC, hOldPen);
			DeleteObject(hPen);
		}
		void CreateRoundButton(HDC hdc, int x, int y, int cx, int cy, int cornersize, const wchar_t* txt, COLORREF cl)
		{
			DrawRoundRect(hdc, x, y, cx, cy, cornersize, cl);
			TextPreDraw(hdc, x, y, cx, cy, txt, VERTEXUICOLOR_WHITE);
		}
		void CreateRoundButtonEx(HDC hdc, int x, int y, int cx, int cy, int cornersize, const wchar_t* txt, int txtsz, COLORREF cl, COLORREF tcl = VERTEXUICOLOR_WHITE)
		{
			DrawRoundRect(hdc, x, y, cx, cy, cornersize, cl);
			TextPreDrawEx(hdc, x, y, cx, cy, txt, txtsz, 1, tcl);
		}
		void DrawPloygon(HDC hDrawingDC, const POINT* pt, int ang, COLORREF cl)
		{
			HPEN hGreenPen = ::CreatePen(PS_SOLID, 2, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hDrawingDC, OBJ_PEN);
			HBRUSH hOldBrush = (HBRUSH)CreateSolidBrush(cl);
			SelectObject(hDrawingDC, hGreenPen);
			SelectObject(hDrawingDC, hOldBrush);
			Polygon(hDrawingDC, pt, ang);
			SelectObject(hDrawingDC, hOldBrush);
			DeleteObject(hOldBrush);
			SelectObject(hDrawingDC, hOldPen);
			DeleteObject(hGreenPen);
		}

		void CreateSTab(HDC hdc, int x, int y, bool isopen, int scale = 1)
		{
			if (isopen == false)
			{
				DrawRoundRect(hdc, x * scale, y * scale, 50 * scale, 20 * scale, 15 * scale, VERTEXUICOLOR_MIDNIGHTPLUS);
				DrawRoundRect(hdc, x * scale, y * scale, 20 * scale, 20 * scale, 15 * scale, VERTEXUICOLOR_GREENSEA);
			}
			if (isopen == true)
			{
				DrawRoundRect(hdc, x * scale, y * scale, 50 * scale, 20 * scale, 15 * scale, VERTEXUICOLOR_MIDNIGHTPLUS);
				DrawRoundRect(hdc, (x + 30) * scale, y * scale, 20 * scale, 20 * scale, 15 * scale, VERTEXUICOLOR_GREENSEA);
			}
		}
		void CreateSimpleHollowButton(HDC hdc, int x, int y, int sizex, int sizey, int p, const wchar_t* s, COLORREF cl, COLORREF tcl = VERTEXUICOLOR_WHITE, int tsize = 18)
		{
			if (sizex < 0)sizex = 0;
			CreateHollowRect(hdc, x, y, sizex, sizey, p, cl);
			if (tsize < 0)tsize = 0;
			TextPreDrawEx(hdc, x, y, sizex, sizey, s, tsize, 1, tcl);
		}
		void CreateCtl(HWND hWnd, HDC hdc, VUICTL vctl)
		{
			if (vctl.CtlName == L"Button")
			{
				CreateSimpleButton(hWnd, hdc, vctl.x, vctl.y, vctl.sizex, vctl.sizey, vctl.Text);
			}
		}
		HBITMAP hwndToHbitmap(HWND hwnd, HDC hDC)
		{

			//HDC hDC = GetDC(hwnd);


			RECT rect;

			GetClientRect(hwnd, &rect);


			HDC hDCMem = CreateCompatibleDC(hDC);


			HBITMAP hBitMap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);


			HBITMAP hOldMap = (HBITMAP)SelectObject(hDCMem, hBitMap);


			BitBlt(hDCMem, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);
			hBitMap = (HBITMAP)SelectObject(hDCMem, hOldMap);
			DeleteDC(hDCMem);
			return hBitMap;
		}
		HBITMAP DCToHbitmap(HDC hDC, int cx, int cy)
		{

			//HDC hDC = GetDC(hwnd);




			HDC hDCMem = CreateCompatibleDC(hDC);


			HBITMAP hBitMap = CreateCompatibleBitmap(hDC, cx, cy);


			HBITMAP hOldMap = (HBITMAP)SelectObject(hDCMem, hBitMap);


			BitBlt(hDCMem, 0, 0, cx, cy, hDC, 0, 0, SRCCOPY);
			hBitMap = (HBITMAP)SelectObject(hDCMem, hOldMap);
			DeleteDC(hDCMem);
			return hBitMap;
		}
		//Create a Drawing Panel.
		template<class draw>
		void _CreatePanel(HWND h, HDC hdc, draw DrawFun)
		{
			HDC         hMemDC;
			HBITMAP     hBmpMem;
			HBITMAP     hPreBmp;
			RECT rc;
			GetClientRect(h, &rc);
			hMemDC = CreateCompatibleDC(hdc);

			hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

			hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
			//On hMemDC.
			DrawFun(h, hMemDC);

			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);

			SelectObject(hMemDC, hPreBmp);

			DeleteObject(hBmpMem);

			DeleteDC(hMemDC);


		}
		void CreatePanelEx(HWND h, HDC hdc, DRAWPANEL DrawFun, int x = 0, int y = 0, int xOf = 0, int yOf = 0)
		{
			HDC         hMemDC;
			HBITMAP     hBmpMem;
			HBITMAP     hPreBmp;
			RECT rc;
			GetClientRect(h, &rc);
			if (xOf == 0)xOf = rc.right - rc.left;
			hMemDC = CreateCompatibleDC(hdc);

			hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, yOf);

			hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
			//On hMemDC.
			DrawFun(h, hMemDC);

			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, x, y, SRCCOPY);


			SelectObject(hMemDC, hPreBmp);


			DeleteObject(hBmpMem);

			DeleteDC(hMemDC);
		}
		void CreateAreaPanel(HWND h, HDC hdc, DRAWPANEL DrawFun, int x, int y, int cx, int cy, int xOf = 0, int yOf = 0, HDC CachedDC = 0)
		{
			if (0 == CachedDC)
			{
				HDC         hMemDC;
				HBITMAP     hBmpMem;
				HBITMAP     hPreBmp;
				RECT rc;
				GetClientRect(h, &rc);
				hMemDC = CreateCompatibleDC(hdc);

				hBmpMem = CreateCompatibleBitmap(hdc, cx, cy);

				hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
				//On hMemDC.
				DrawFun(h, hMemDC);

				BitBlt(hdc, x, y, cx, cy, hMemDC, xOf, yOf, SRCCOPY);


				SelectObject(hMemDC, hPreBmp);


				DeleteObject(hBmpMem);

				DeleteDC(hMemDC);
			}
			else 	BitBlt(hdc, x, y, cx, cy, CachedDC, xOf, yOf, SRCCOPY);
		}
		//
		void CreatePanel(HWND h, HDC hdc, std::function<DRAWPANEL> DrawFun)
		{
			HDC         hMemDC;
			HBITMAP     hBmpMem;
			HBITMAP     hPreBmp;
			RECT rc;
			GetClientRect(h, &rc);
			hMemDC = CreateCompatibleDC(hdc);

			hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

			hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
			//On hMemDC.
			DrawFun(h, hMemDC);

			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);


			SelectObject(hMemDC, hPreBmp);


			DeleteObject(hBmpMem);

			DeleteDC(hMemDC);
		}

		//


		void SetPanelID(const wchar_t* id)
		{
			PrevPanelID = PanelID;
			PanelID = id;
		}
		void SwitchPanel(const wchar_t* ID)
		{
			RECT rc;
			GetClientRect(GhWnd, &rc);
			SetPanelID(ID);
			InvalidateRect(GhWnd, &rc, 1);
		}
		void RefreshDisplay(HWND h)
		{
			RECT rc;
			GetClientRect(h, &rc);
			InvalidateRect(h, &rc, 0);
		}
		void RefreshWindow(HWND h)
		{
			RECT rc;
			GetClientRect(h, &rc);
			dystate = 1;
			InvalidateRect(h, &rc, 0);
		}
		void RefreshWindowAndDC(HWND h)
		{
			RECT rc;
			GetClientRect(h, &rc);
			dystate = 3;
			InvalidateRect(h, &rc, 0);
		}
		void RefreshCache(HWND h)
		{
			RECT rc;
			GetClientRect(h, &rc);
			dystate = 2;
			InvalidateRect(h, &rc, 0);
		}
		void RefreshAreaFromCache(HWND h, int x, int y, int cx, int cy)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, cx, cy);
			InvalidateRect(h, &rc, 0);
			SendMessage(h, WM_PAINT, 0, 0);
		}
	}
	//--
	class VertexUIControllerObject
	{
	public:
		void SetPanelID(const wchar_t* id)
		{
			this->PrevPanelID = this->PanelID;
			this->PanelID = id;
		}
		void SwitchPanel(const wchar_t* ID)
		{
			RECT rc;
			GetClientRect(this->hWnd, &rc);
			SetPanelID(ID);
			InvalidateRect(this->hWnd, &rc, 1);
		}
		void SetOrigin(HWND hWnd)
		{
			this->hWnd = hWnd;
		}
		int IsPanel(const wchar_t* p)
		{
			if (this->PanelID == p)return 1;
			else return 0;
		}
		const wchar_t* gTxt = L"qwq";
	private:
		HWND hWnd;
		const wchar_t* PanelID;
		const wchar_t* PrevPanelID;
	};

}


namespace VertexUI::Panel
{
	void CreateAlphaMaskFromDC(HDC hdc, int x, int y, int cx, int cy, int alpha, HDC cl);
	void AreaBlur(HDC hdc, VertexUIPos vp, int a, int b, int th);

	void ContrastPolygon(HWND hWnd, HDC hdc, VertexUIPos vp, const POINT* pt, int ang)
	{
		int s = 1;



		HDC         hMemDC3;
		HBITMAP     hBmpMem3;
		HBITMAP     hPreBmp3;

		hMemDC3 = CreateCompatibleDC(hdc);

		hBmpMem3 = CreateCompatibleBitmap(hdc, vp.cx * s, vp.cy * s);

		hPreBmp3 = (HBITMAP)SelectObject(hMemDC3, hBmpMem3);


		//On hMemDC.
		// //DC1不可用
		//先在DC3画黑白图
		//将白色去掉,画到带有背景的DC2
		CreateRect(0, hMemDC3, 0, 0, vp.cx * s, vp.cy * s, RGB(0, 0, 0));

		//HERE
		DrawPloygon(hMemDC3, pt, ang, RGB(255, 255, 255));

		TransparentBlt(hdc, 0, 0, vp.cx * s, vp.cy * s, hMemDC3, 0, 0, vp.cx * s, vp.cy * s, RGB(255, 255, 255));
		//CreateAlphaMaskFromDC(hdc, 0, 0, vp.cx * s, vp.cy * s, ldce.Alpha, hMemDC2);



		SelectObject(hMemDC3, hPreBmp3);


		DeleteObject(hBmpMem3);

		DeleteDC(hMemDC3);


		//CreateRoundButtonEx(hdc, 8, 8, (vp.cx - 8) * s, (vp.cy - 8) * s, csz * s, txt, vci.TxtSize * s, vci.BkColor, vci.TxtColor);
	}
}
#define _Delay(_x) Sleep(_x);
namespace VertexUI::Click
{
	extern int KeepDownMsg;
}
namespace VertexUI::Panel
{
	void XSleep(UINT Delay_ms)
	{
		DWORD dwTick = GetTickCount64() + Delay_ms;
		while (GetTickCount64() < dwTick)
		{
			MSG msg;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Sleep(0);
		}

	}
	double CalcEaseOutCurve(double t, double b, double c, double d) {
		return c * ((t = t / d - 1) * t * t + 1) + b;
	}
	double CalcEaseInCurve(double t, double b, double c, double d) {
		return c * (t /= d) * t * t + b;
	}
	double CalcOutCurve4(double t, double b, double  c, double d) {
		return -c * ((t = t / d - 1) * t * t * t - 1) + b;
	}
	double CalcEaseInOutCurve(double t, double b, double c, double d) {
		t += 10;
		if ((t /= d / 2) < 1) return c / 2 * t * t * t + b;
		return c / 2 * ((t -= 2) * t * t + 2) + b;

	}
	double CalcBounceCurve(double t, double b, double c, double d)
	{
		t /= d;

		if (t < (1.0 / 2.75))
		{
			return c * (7.5625 * t * t) + b;
		}
		else if (t < (2.0 / 2.75))
		{
			t -= (1.5 / 2.75);
			return c * (7.5625 * (t * t) + 0.75) + b;
		}
		else if (t < (2.5 / 2.75))
		{
			t -= (2.25 / 2.75);
			return c * (7.5625 * (t * t) + 0.9375) + b;
		}
		else
		{
			t -= (2.625 / 2.75);
			return c * (7.5625 * (t * t) + 0.984375) + b;
		}
	}
#undef max

	double log_base(double a, double b) {
		return std::log(a) / std::log(b);
	}
	double EaseInCore(double normalizedTime)
	{
		double num = std::max(0.0, (double)1);
		double num2 = 1.1;
		if (std::max(num2, 1.001))
		{
			num2 = 1.001;
		}
		double num3 = std::pow(num2, num);
		double num4 = 1.0 - num2;
		double num5 = (1.0 - num3) / num4 + num3 * 0.5;
		double num6 = normalizedTime * num5;
		double d = log_base(-num6 * (1.0 - num2) + 1.0, num2);
		double num7 = floor(d);
		double y = num7 + 1.0;
		double num8 = (1.0 - std::pow(num2, num7)) / (num4 * num5);
		double num9 = (1.0 - std::pow(num2, y)) / (num4 * num5);
		double num10 = (num8 + num9) * 0.5;
		double num11 = normalizedTime - num10;
		double num12 = num10 - num8;
		double num13 = std::pow(1.0 / num2, num - num7);
		return -num13 / (num12 * num12) * (num11 - num12) * (num11 + num12);
	}
	void CtlMoveInAnimation(HWND hWnd, RECT rc, int& animated, int MoveNum = 0)
	{
		for (int t = 1; t < 40; t++)
		{
			animated = CalcEaseOutCurve(t, 0, 40, 40) - MoveNum; InvalidateRect(hWnd, &rc, 0); XSleep(10);
		}
	}


	HDC inhdc;
	HDC GlobalSharedDC;
	HBITMAP inbmp;
	HBITMAP CopyBitmap(HBITMAP hbm) {
		HDC hdcSrc = CreateCompatibleDC(NULL);
		HDC hdcDst = CreateCompatibleDC(NULL);
		HBITMAP hbmOld, hbmOld2, hbmNew;
		BITMAP bm;
		GetObject(hbm, sizeof(bm), &bm);
		hbmOld = (HBITMAP)SelectObject(hdcSrc, hbm);
		hbmNew = CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes,
			bm.bmBitsPixel,
			NULL);
		hbmOld2 = (HBITMAP)SelectObject(hdcDst, hbmNew);
		BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);
		SelectObject(hdcSrc, hbmOld);
		SelectObject(hdcDst, hbmOld2);
		DeleteDC(hdcSrc);
		DeleteDC(hdcDst);
		return hbmNew;
	}
	void CreateButton(HWND hWnd, HDC hdc, int x, int y, int sizex, int sizey, int cid, const wchar_t* CTLTXT)
	{
		//VERTEXUICTL btn;
		//btn.Text = L"114514";
		CreateSimpleButton(hWnd, hdc, x, y, sizex, sizey, CTLTXT);
	}


	void DrawGradient(HDC pDC, RECT rRect, COLORREF crFrom, COLORREF crTo)
	{

		int iHeight = rRect.bottom - rRect.top;

		int iWidth = rRect.right - 1;

		//

		int iR = GetRValue(crFrom);

		int iG = GetGValue(crFrom);

		int iB = GetBValue(crFrom);

		//

		int idR = (256 * (GetRValue(crTo) - iR)) / iWidth;

		int idG = (256 * (GetGValue(crTo) - iG)) / iWidth;

		int idB = (256 * (GetBValue(crTo) - iB)) / iWidth;



		iR *= 256;

		iG *= 256;

		iB *= 256;

		// ->

		for (int i = rRect.left; i <= iWidth; i++, iR += idR, iG += idG, iB += idB)
		{
			RECT rcx;
			RectTypeConvert(rcx, i, rRect.top, 1, iHeight);
			HBRUSH hbr = CreateSolidBrush(RGB(iR / 256, iG / 256, iB / 256));
			HBRUSH hOld = (HBRUSH)SelectObject(pDC, hbr);
			FillRect(pDC, &rcx, hbr);
			DeleteObject(hbr);
			SelectObject(pDC, hOld);
		}

	}
	COLORREF GetGradientValue(COLORREF crFrom, COLORREF crTo, int iWidth, int stage)
	{


		//

		int iR = GetRValue(crFrom);

		int iG = GetGValue(crFrom);

		int iB = GetBValue(crFrom);

		//

		int idR = (256 * (GetRValue(crTo) - iR)) / iWidth;

		int idG = (256 * (GetGValue(crTo) - iG)) / iWidth;

		int idB = (256 * (GetBValue(crTo) - iB)) / iWidth;



		iR *= 256;

		iG *= 256;

		iB *= 256;

		// ->

		for (int i = 0; i <= stage; i++, iR += idR, iG += idG, iB += idB)
		{
			if (i == stage)return(RGB(iR / 256, iG / 256, iB / 256));
		}

	}
	void DrawGradientEx(HDC pDC, int x, int y, int cx, int cy, COLORREF crFrom, COLORREF crTo)
	{

		int iHeight = cy;

		int iWidth = cx;

		//

		int iR = GetRValue(crFrom);

		int iG = GetGValue(crFrom);

		int iB = GetBValue(crFrom);

		//

		int idR = (256 * (GetRValue(crTo) - iR)) / iWidth;

		int idG = (256 * (GetGValue(crTo) - iG)) / iWidth;

		int idB = (256 * (GetBValue(crTo) - iB)) / iWidth;



		iR *= 256;

		iG *= 256;

		iB *= 256;

		// ->

		for (int i = x; i <= x + iWidth; i++, iR += idR, iG += idG, iB += idB)
		{
			RECT rcx;
			RectTypeConvert(rcx, i, y, 1, iHeight);
			HBRUSH hbr = CreateSolidBrush(RGB(iR / 256, iG / 256, iB / 256));
			HBRUSH hOld = (HBRUSH)SelectObject(pDC, hbr);
			FillRect(pDC, &rcx, hbr);
			DeleteObject(hbr);
			SelectObject(pDC, hOld);
		}

	}
	void DrawGradientToBottomEx(HDC pDC, int x, int y, int cx, int cy, COLORREF crFrom, COLORREF crTo)
	{

		int iHeight = cy;

		int iWidth = cx;
		//
		int iR = GetRValue(crFrom);

		int iG = GetGValue(crFrom);

		int iB = GetBValue(crFrom);
		//
		int idR = (256 * (GetRValue(crTo) - iR)) / iHeight;

		int idG = (256 * (GetGValue(crTo) - iG)) / iHeight;

		int idB = (256 * (GetBValue(crTo) - iB)) / iHeight;



		iR *= 256;

		iG *= 256;

		iB *= 256;

		// ->

		for (int i = y; i <= y + iHeight; i++, iR += idR, iG += idG, iB += idB)
		{
			RECT rcx;
			RectTypeConvert(rcx, x, i, iWidth, 1);
			HBRUSH hbr = CreateSolidBrush(RGB(iR / 256, iG / 256, iB / 256));
			HBRUSH hOld = (HBRUSH)SelectObject(pDC, hbr);
			FillRect(pDC, &rcx, hbr);
			DeleteObject(hbr);
			SelectObject(pDC, hOld);
		}

	}
	void DrawLinearGradient(HDC pDC, int x, int y, int cx, int cy, COLORREF clr1, COLORREF clr2, int angle)
	{
		int width = cx;
		int height = cy;
		int r1 = GetRValue(clr1);
		int r2 = GetRValue(clr2);
		int g1 = GetGValue(clr1);
		int g2 = GetGValue(clr2);
		int b1 = GetBValue(clr1);
		int b2 = GetBValue(clr2);

		int rSpan = r2 - r1;
		int gSpan = g2 - g1;
		int bSpan = b2 - b1;

		int len = 0;
		int len_clr = 0;
		if (angle == 0)len_clr = len = height;
		else if (angle == 90)len_clr = len = width;
		else len_clr = len = width + height / tan((double)angle * 3.14 / 180);

		HDC dcMem = CreateCompatibleDC(pDC);

		HBITMAP bmp = CreateCompatibleBitmap(pDC, cx, cy);

		HBITMAP hPreBmp = (HBITMAP)SelectObject(dcMem, bmp);
		//开始内存绘制
		for (int i = 0; i < len; i++)
		{
			COLORREF clr = RGB(r1 + (i * rSpan) / len_clr, g1 + (i * gSpan) / len_clr, b1 + (i * bSpan) / len_clr);
			HPEN pen = CreatePen(PS_SOLID, 2, clr);
			SelectObject(dcMem, pen);
			if (angle == 0)
			{
				MoveToEx(dcMem, 0, i, 0);
				LineTo(dcMem, width, i);
			}
			else
			{
				SelectObject(dcMem, pen);
				MoveToEx(dcMem, i, 0, 0);
				int y = (int)i * tan((double)angle * 3.14 / 180);
				LineTo(dcMem, 0, y);
			}
			DeleteObject(pen);
		}
		BitBlt(pDC, x, y, cx, cy, dcMem, 0, 0, SRCCOPY);
		SelectObject(dcMem, hPreBmp);


		DeleteObject(bmp);

		DeleteDC(dcMem);

	}
	void DrawAlphaShadowToRightSample(HDC hdc, int x, int y, int ShadowAlphaFactor, int cy, COLORREF cl)
	{
		for (int i = 0; i <= 10; i++)
		{
			CreateAlphaMask(hdc, x + i, y, 1, cy, (10 - i) * ShadowAlphaFactor, VERTEXUICOLOR_DARKEN);
		}
	}
	void DrawAlphaShadowToBottomSample(HDC hdc, int x, int y, int ShadowAlphaFactor, int cx, COLORREF cl, int Expand = 0)
	{
		for (int i = 0; i <= 10 + Expand; i++)
		{
			CreateAlphaMask(hdc, x, y + i, cx, 1, (10 + Expand - i) * ShadowAlphaFactor, cl);
		}
	}
	void DrawAlphaShadowToTopSample(HDC hdc, int x, int y, int ShadowAlphaFactor, int cx, COLORREF cl, int b)
	{
		for (int i = 0; i <= b; i++)
		{
			CreateAlphaMask(hdc, x, y - i - 1, cx, 1, (b - i) * ShadowAlphaFactor, cl);
		}
	}
	void _RegisterScrollBarPanel(const wchar_t* id, int p = -1)
	{
		ScrollSingleID = id;
		ScrollSinglePage = p;
	}
#define MointorValue MonitorValue
	void CreateAlphaMask(HDC hdc, int x, int y, int cx, int cy, int alpha, COLORREF cl)
	{
		int ocx, ocy;
		ocx = cx; ocy = cy;
		if (cx < 0)cx = abs(cx);
		if (cy < 0)cy = abs(cy);
		if (ocx < 0)x -= cx;
		if (ocy < 0)y -= cy;
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hBmpMem = CreateCompatibleBitmap(hdc, cx, cy);//获取整个屏幕,之后绘制.
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);

		CreateRectArea(hMemDC, 0, 0, cx, cy, cl);
		//CreateRect(hWnd, hMemDC, ((rc.right - rc.left)/2) - (800 / 2), rc.bottom - 230, 800, 100, RGB(20, 20, 20));
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 1;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha = alpha;

		AlphaBlend(hdc, x, y, cx, cy, hMemDC, 0, 0, cx, cy, bf);
		SelectObject(hMemDC, hPreBmp);
		DeleteObject(hBmpMem);
		DeleteDC(hMemDC);//透明度混合
	}
	void CreateAlphaMaskFromDC(HDC hdc, int x, int y, int cx, int cy, int alpha, HDC cl)
	{
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hBmpMem = CreateCompatibleBitmap(hdc, cx, cy);//获取整个屏幕,之后绘制.
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);

		//CreateRect(hWnd, hMemDC, ((rc.right - rc.left)/2) - (800 / 2), rc.bottom - 230, 800, 100, RGB(20, 20, 20));
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 1;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha = alpha;
		BitBlt(hMemDC, 0, 0, cx, cy, cl, 0, 0, SRCCOPY);
		AlphaBlend(hdc, x, y, cx, cy, hMemDC, 0, 0, cx, cy, bf);
		SelectObject(hMemDC, hPreBmp);
		DeleteObject(hBmpMem);
		DeleteDC(hMemDC);//透明度混合
	}
	HBITMAP CreateGDIBitmap(int nWid, int nHei, void** ppBits)
	{
		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = nWid;
		bmi.bmiHeader.biHeight = -nHei;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = 0;

		HDC hdc = GetDC(NULL);
		HBITMAP hBmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, ppBits, 0, 0);
		ReleaseDC(NULL, hdc);
		return hBmp;
	}
	int in_test()
	{
		return 0;
	}
	typedef class VERTEXUILISTPANEL
	{
		typedef int RUNFUN;
	public:
		std::vector<const wchar_t*> listtxt = { 128,(L"") };
		std::vector<std::function<void()>> listclick = { 128,(in_test) };
		void Add(const wchar_t* txt, std::function<void()> fun)
		{
			id++;
			listtxt[id] = txt;
			listclick[id] = fun;
		}
		COLORREF PanelBkColor = VERTEXUICOLOR_DARKNIGHT;
	protected:
		int id = 0;
	}tagVertexUIList;
	typedef void (DRAWLISTPANEL)(HWND, HDC, VERTEXUILISTPANEL);
	bool ListPanelBlured = false;
#define _SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;
#include "../GaussBlurFilter.hpp"

	HBITMAP GaussianBlurFunc(HDC hDC, int x, int y, int iWidth, int iHeight)
	{
		static HBITMAP hBitMap;
		DeleteObject(hBitMap);

		HDC hMemDC = CreateCompatibleDC(hDC);
		hBitMap = CreateCompatibleBitmap(hDC, iWidth, iHeight); // 创建与设备描述表兼容的位图
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBitMap);
		BitBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, 0, 0, SRCCOPY);
		int size = iWidth * iHeight * 5;
		unsigned char* pBits = (unsigned char*)malloc(size);  //在堆上申请

		int nBytes = GetBitmapBits(hBitMap, size, pBits);

		//GaussianBlur(pBits, iWidth, iHeight, 4,40); //单线程高斯模糊 不明白为啥要开

		unsigned char* nBits = (unsigned char*)malloc(size);  //在堆上申请
		CGaussBlurFilter<double> _filter;
		_filter.SetSigma(3.5); // 设置高斯半径
		_filter.SetMultiThreads(true, 4);
		_filter.Filter(pBits, nBits, iWidth, iHeight, 32);
		SetBitmapBits(hBitMap, size, nBits);
		SelectObject(hMemDC, hPreBmp);
		DeleteObject(hMemDC);

		_SAFE_DELETE(pBits);
		_SAFE_DELETE(nBits);
		//delete[]pBits;
		return hBitMap;
	}
	HBITMAP _GaussianBlurFunc(HDC hDC, int x, int y, int iWidth, int iHeight, double s = 3.5, bool IsLowQuality = false, int lqNum = 8, bool lqButhq = false, bool MultiThread = true)
	{
		if (IsLowQuality == false)lqNum = 1;
		iWidth /= lqNum; iHeight /= lqNum;
		static HBITMAP hBitMap;
		DeleteObject(hBitMap);
		HDC hMemDC = CreateCompatibleDC(hDC);
		hBitMap = CreateCompatibleBitmap(hDC, iWidth, iHeight); // 创建与设备描述表兼容的位图
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBitMap);
		if (IsLowQuality == true) {
			if (lqButhq == true)
			{
				::SetStretchBltMode(hMemDC, HALFTONE);
				::SetBrushOrgEx(hMemDC, 0, 0, NULL);
			}StretchBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, iWidth * lqNum, iHeight * lqNum, SRCCOPY);
		}
		else BitBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, SRCCOPY);
		int size = iWidth * iHeight * 4;
		clock_t time = clock();
		unsigned char* pBits = (unsigned char*)malloc(size);  //在堆上申请

		int nBytes = GetBitmapBits(hBitMap, size, pBits);


		unsigned char* nBits = (unsigned char*)malloc(size);  //在堆上申请
		CGaussBlurFilter<double> _filter;
		_filter.SetSigma(s); // 设置高斯半径
		if (MultiThread != false)_filter.SetMultiThreads(true, 4);
		_filter.Filter(pBits, nBits, iWidth, iHeight, 32);
		SetBitmapBits(hBitMap, size, nBits);


		clock_t etime = clock();
		int ntime = etime - time;
		//SetBitmapBits(hBitMap, size, pBits);

		wchar_t timec[20];
		_itow(ntime, timec, 10);
		OutputDebugString(L"Blur Time:");
		OutputDebugString(timec);
		OutputDebugString(L"\n");
		//SelectObject(hdcMem, hOld);
		SelectObject(hMemDC, hPreBmp);
		DeleteObject(hMemDC);

		_SAFE_DELETE(pBits);
		_SAFE_DELETE(nBits);
		//delete[]pBits;
		return hBitMap;
	}
	//防止Bmp冲突（（
	HBITMAP _GaussianBlurFunc2(HDC hDC, int x, int y, int iWidth, int iHeight, int s = 3.5, bool IsLowQuality = false, int lqNum = 8, bool lqButhq = false, bool MultiThread = true)
	{
		if (IsLowQuality == false)lqNum = 1;
		iWidth /= lqNum; iHeight /= lqNum;
		static HBITMAP hBitMap;
		DeleteObject(hBitMap);
		HDC hMemDC = CreateCompatibleDC(hDC);
		hBitMap = CreateCompatibleBitmap(hDC, iWidth, iHeight); // 创建与设备描述表兼容的位图
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBitMap);
		if (IsLowQuality == true) {
			if (lqButhq == true)
			{
				::SetStretchBltMode(hMemDC, HALFTONE);
				::SetBrushOrgEx(hMemDC, 0, 0, NULL);
			}StretchBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, iWidth * lqNum, iHeight * lqNum, SRCCOPY);
		}
		else BitBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, SRCCOPY);
		int size = iWidth * iHeight * 4;
		clock_t time = clock();
		unsigned char* pBits = (unsigned char*)malloc(size);  //在堆上申请

		int nBytes = GetBitmapBits(hBitMap, size, pBits);


		unsigned char* nBits = (unsigned char*)malloc(size);  //在堆上申请
		CGaussBlurFilter<double> _filter;
		_filter.SetSigma(s); // 设置高斯半径
		if (MultiThread != false)_filter.SetMultiThreads(true, 4);
		_filter.Filter(pBits, nBits, iWidth, iHeight, 32);
		SetBitmapBits(hBitMap, size, nBits);


		clock_t etime = clock();
		int ntime = etime - time;
		//SetBitmapBits(hBitMap, size, pBits);

		wchar_t timec[20];
		_itow(ntime, timec, 10);
		OutputDebugString(L"Blur Time:");
		OutputDebugString(timec);
		OutputDebugString(L"\n");
		//SelectObject(hdcMem, hOld);
		SelectObject(hMemDC, hPreBmp);
		DeleteObject(hMemDC);

		_SAFE_DELETE(pBits);
		_SAFE_DELETE(nBits);
		//delete[]pBits;
		return hBitMap;
	}
	void _GaussianBitsBlurFunc(UINT* pb, int x, int y, int iWidth, int iHeight, int s = 3.5, bool MultiThread = true)
	{

		int size = iWidth * iHeight * 4;
		clock_t time = clock();



		UINT* nBits = (UINT*)malloc(size);  //在堆上申请
		CGaussBlurFilter<double> _filter;
		_filter.SetSigma(s); // 设置高斯半径
		if (MultiThread != false)_filter.SetMultiThreads(true, 4);
		_filter.Filter(pb, nBits, iWidth, iHeight, 32);


		clock_t etime = clock();
		int ntime = etime - time;
		//SetBitmapBits(hBitMap, size, pBits);

		wchar_t timec[20];
		_itow(ntime, timec, 10);
		OutputDebugString(L"Blur Time:");
		OutputDebugString(timec);
		OutputDebugString(L"\n");
		//SelectObject(hdcMem, hOld);
		pb = nBits;
		_SAFE_DELETE(nBits);
		//delete[]pBits;
		return;
	}
	HBITMAP _GaussianBlurFunc_Old(HDC hDC, int x, int y, int iWidth, int iHeight, int s = 3.5, bool IsLowQuality = false, int lqNum = 8)
	{
		if (IsLowQuality == false)lqNum = 1;
		iWidth /= lqNum; iHeight /= lqNum;
		HBITMAP hBitMap;
		HDC hMemDC = CreateCompatibleDC(hDC);
		hBitMap = CreateCompatibleBitmap(hDC, iWidth, iHeight); // 创建与设备描述表兼容的位图
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBitMap);
		if (IsLowQuality == true)StretchBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, iWidth * lqNum, iHeight * lqNum, SRCCOPY);
		else BitBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, SRCCOPY);
		int size = iWidth * iHeight * 4;
		clock_t time = clock();
		unsigned char* pBits = (unsigned char*)malloc(size);  //在堆上申请

		int nBytes = GetBitmapBits(hBitMap, size, pBits);


		unsigned char* nBits = (unsigned char*)malloc(size);  //在堆上申请
		CGaussBlurFilter<double> _filter;
		_filter.SetSigma(s); // 设置高斯半径
		_filter.SetMultiThreads(true, 4);
		_filter.Filter(pBits, nBits, iWidth, iHeight, 32);
		SetBitmapBits(hBitMap, size, nBits);


		clock_t etime = clock();
		int ntime = etime - time;
		//SetBitmapBits(hBitMap, size, pBits);

		wchar_t timec[20];
		_itow(ntime, timec, 10);
		OutputDebugString(timec);
		//SelectObject(hdcMem, hOld);
		SelectObject(hMemDC, hPreBmp);
		DeleteObject(hMemDC);

		_SAFE_DELETE(pBits);
		_SAFE_DELETE(nBits);
		//delete[]pBits;
		return hBitMap;
	}

	void AreaBlur(HDC hdc, VertexUIPos vp, int  a, int b, int th)
	{

		static HBITMAP FinalBlur;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC = CreateCompatibleDC(hdc);
		FinalBlur = _GaussianBlurFunc2(hdc, vp.x, vp.y, vp.cx, vp.cy, a, true, b, true, th);
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
		::SetStretchBltMode(hdc, HALFTONE);
		::SetBrushOrgEx(hdc, 0, 0, NULL);
		StretchBlt(hdc, vp.x, vp.y, vp.cx, vp.cy, hMemDC, 0, 0, vp.cx / b, vp.cy / b, SRCCOPY);
		SelectObject(hMemDC, hPreBmp);
		DeleteDC(hMemDC);
		DeleteObject(FinalBlur);
	}
	void AreaBlur3(HDC hdc, VertexUIPos vp, double a, int b, int th)
	{

		static HBITMAP FinalBlur;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC = CreateCompatibleDC(hdc);
		FinalBlur = _GaussianBlurFunc(hdc, vp.x, vp.y, vp.cx, vp.cy, a, true, b, true, th);
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
		::SetStretchBltMode(hdc, HALFTONE);
		::SetBrushOrgEx(hdc, 0, 0, NULL);
		StretchBlt(hdc, vp.x, vp.y, vp.cx, vp.cy, hMemDC, 0, 0, vp.cx / b, vp.cy / b, SRCCOPY);
		SelectObject(hMemDC, hPreBmp);
		DeleteDC(hMemDC);
		DeleteObject(FinalBlur);
	}
	void AreaBlurBmp(HDC hdc, HBITMAP& Back, VertexUIPos vp, int a, int b, int th)
	{
		Back = _GaussianBlurFunc2(hdc, vp.x, vp.y, vp.cx, vp.cy, a, true, b, true, th);
	}
	void AreaBlur2(HDC hdc, HDC oDC, VertexUIPos vp, int a, int b, int th, int Exp)
	{

		static HBITMAP FinalBlur;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC = CreateCompatibleDC(hdc);
		FinalBlur = _GaussianBlurFunc2(oDC, vp.x, vp.y, vp.cx * Exp, vp.cy * Exp, a, true, b + Exp, true, th);
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
		::SetStretchBltMode(hdc, HALFTONE);
		::SetBrushOrgEx(hdc, 0, 0, NULL);
		StretchBlt(hdc, vp.x * Exp, vp.y * Exp, vp.cx * Exp, vp.cy * Exp, hMemDC, 0, 0, vp.cx / (b + Exp), vp.cy / (b + Exp), SRCCOPY);
		SelectObject(hMemDC, hPreBmp);
		DeleteDC(hMemDC);
		DeleteObject(FinalBlur);
	}
	void ResumeDCFromBitmap(HDC hdc, VertexUIPos vp, HBITMAP prebmp, int xExt = 0, int yExt = 0)
	{
		HDC hMemDC1 = CreateCompatibleDC(hdc);
		HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, prebmp);
		BitBlt(hdc, vp.x, vp.y, vp.cx, vp.cy, hMemDC1, xExt, yExt, SRCCOPY);
		SelectObject(hMemDC1, prebmp);
		DeleteDC(hMemDC1);
	}
}
#ifdef VERTEXUI_DEVMODE
using namespace::VertexUI;
using namespace::VertexUI::Panel;

//Other


#endif
