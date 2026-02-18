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
#include <imm.h>
#pragma comment(lib, "imm32.lib")
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
		void D2DDrawCircleArc(T* m_pDCRT, float x, float y, float rad, unsigned long arcClr, float animated, float strokeWidth = 2.0f, float alpha = 1.0f)
		{
			int hr = 0;
			ID2D1SolidColorBrush* arcBrush = NULL;
			hr = m_pDCRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(arcClr), alpha)),
				&arcBrush
			);

			if (SUCCEEDED(hr))
			{
				// 创建笔刷样式
				ID2D1StrokeStyle* strokeStyle = NULL;
				D2D1_STROKE_STYLE_PROPERTIES strokeProperties = D2D1::StrokeStyleProperties(
					D2D1_CAP_STYLE_ROUND,
					D2D1_CAP_STYLE_ROUND,
					D2D1_CAP_STYLE_ROUND
				);

				hr = m_pD2DFactory->CreateStrokeStyle(strokeProperties, NULL, 0, &strokeStyle);

				if (SUCCEEDED(hr))
				{
					float sweepAngle = 0;
					float startAngle = -3.141592653f / 2.0f; // 从顶部开始

					// 处理动画逻辑：0-100正向显示，100-200反向消失
					if (animated <= 100)
					{
						// 0-100: 正向显示
						sweepAngle = (animated / 100.0f) * 2.0f * 3.141592653f;
					}
					else if (animated <= 200)
					{
						// 100-200: 反向消失（从完整圆开始，逐渐减少）
						float progress = (animated - 100.0f) / 100.0f; // 0-1
						sweepAngle = (1.0f - progress) * 2.0f * 3.141592653f;
						startAngle = startAngle + progress * 2.0f * 3.141592653f; // 起始点移动
					}
					else
					{
						// 超过200时不显示
						SafeRelease(&strokeStyle);
						SafeRelease(&arcBrush);
						return;
					}

					// 如果角度太小，不绘制
					if (sweepAngle < 0.01f)
					{
						SafeRelease(&strokeStyle);
						SafeRelease(&arcBrush);
						return;
					}

					// 创建路径几何
					ID2D1PathGeometry* pathGeometry = NULL;
					hr = m_pD2DFactory->CreatePathGeometry(&pathGeometry);

					if (SUCCEEDED(hr))
					{
						ID2D1GeometrySink* sink = NULL;
						hr = pathGeometry->Open(&sink);

						if (SUCCEEDED(hr))
						{
							// 起始点
							D2D1_POINT_2F startPoint;
							startPoint.x = x + rad * cos(startAngle);
							startPoint.y = y + rad * sin(startAngle);

							sink->BeginFigure(startPoint, D2D1_FIGURE_BEGIN_FILLED);

							D2D1_ARC_SEGMENT arcSegment;
							arcSegment.point.x = x + rad * cos(startAngle + sweepAngle);
							arcSegment.point.y = y + rad * sin(startAngle + sweepAngle);
							arcSegment.size.width = rad;
							arcSegment.size.height = rad;
							arcSegment.rotationAngle = 0.0f;
							arcSegment.sweepDirection = D2D1_SWEEP_DIRECTION_CLOCKWISE;
							arcSegment.arcSize = (sweepAngle > 3.141592653f) ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL;

							sink->AddArc(&arcSegment);
							sink->EndFigure(D2D1_FIGURE_END_OPEN);
							sink->Close();

							m_pDCRT->DrawGeometry(pathGeometry, arcBrush, strokeWidth, strokeStyle);

							SafeRelease(&sink);
						}

						SafeRelease(&pathGeometry);
					}

					SafeRelease(&strokeStyle);
				}

				SafeRelease(&arcBrush);
			}
		}

		HRESULT D2DRenderCurrentScaledMipmap(ID2D1HwndRenderTarget* hrt, float x, float y, float cx, float cy, float scale, float opacity)
		{
			if (!hrt) return E_INVALIDARG;

			// 确保 scale 是缩小比例 (0 < scale < 1)
			if (scale > 1.0f) scale = 1.0f / scale;
			if (scale <= 0.0f) scale = 0.1f; // 防止除零或负数

			// 限制 opacity 在 [0, 1] 范围内
			if (opacity < 0.0f) opacity = 0.0f;
			if (opacity > 1.0f) opacity = 1.0f;

			HRESULT hr = S_OK;

			// 定义源区域
			D2D1_SIZE_U pixelSize = hrt->GetPixelSize();
			D2D1_RECT_U srcRect = D2D1::RectU(
				static_cast<UINT32>(x),
				static_cast<UINT32>(y),
				static_cast<UINT32>(x + cx),
				static_cast<UINT32>(y + cy)
			);

			if (srcRect.right > pixelSize.width) srcRect.right = pixelSize.width;
			if (srcRect.bottom > pixelSize.height) srcRect.bottom = pixelSize.height;
			if (srcRect.left >= srcRect.right || srcRect.top >= srcRect.bottom) return S_OK;

			float scaledWidth = cx * scale;
			float scaledHeight = cy * scale;

		
			static ID2D1Bitmap* pScreenCapture = NULL;
			static ID2D1BitmapRenderTarget* pSmallRT = NULL;

		
			bool needRecreate = false;

			if (pScreenCapture) {
				D2D1_SIZE_F size = pScreenCapture->GetSize();
				// 截取区域大小改变则重建
				if (static_cast<UINT32>(size.width) != (srcRect.right - srcRect.left) ||
					static_cast<UINT32>(size.height) != (srcRect.bottom - srcRect.top)) {
					needRecreate = true;
				}
			}
			else {
				needRecreate = true;
			}

			if (needRecreate) {
				SafeRelease(&pScreenCapture);
				SafeRelease(&pSmallRT);

				D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(hrt->GetPixelFormat());
				D2D1_SIZE_U captureSize = D2D1::SizeU(srcRect.right - srcRect.left, srcRect.bottom - srcRect.top);
				hr = hrt->CreateBitmap(captureSize, props, &pScreenCapture);
			}

			// 确保 pSmallRT 也被正确创建/重建
			if (SUCCEEDED(hr) && !pSmallRT) {
				// 注意：这里 pSmallRT 的大小应该根据 scaledWidth/Height 动态调整
				// 为了简单起见，如果 scale 变动频繁，这里会导致频繁重建。
				// 假设 scale 相对固定。
				D2D1_SIZE_F smallSize = D2D1::SizeF(scaledWidth, scaledHeight);
				hr = hrt->CreateCompatibleRenderTarget(smallSize, &pSmallRT);
			}
			else if (pSmallRT) {
				// 检查 SmallRT 尺寸是否匹配当前的缩放需求
				D2D1_SIZE_F currentSize = pSmallRT->GetSize();
				if (std::abs(currentSize.width - scaledWidth) > 1.0f || std::abs(currentSize.height - scaledHeight) > 1.0f) {
					SafeRelease(&pSmallRT);
					D2D1_SIZE_F smallSize = D2D1::SizeF(scaledWidth, scaledHeight);
					hr = hrt->CreateCompatibleRenderTarget(smallSize, &pSmallRT);
				}
			}

		

			if (SUCCEEDED(hr) && pScreenCapture)
			{
	
				hr = pScreenCapture->CopyFromRenderTarget(&D2D1::Point2U(0, 0), hrt, &srcRect);
			}

			// 缩小
			if (SUCCEEDED(hr) && pSmallRT)
			{
				pSmallRT->BeginDraw();
				pSmallRT->Clear(D2D1::ColorF(0, 0.0f));

				pSmallRT->DrawBitmap(
					pScreenCapture,
					D2D1::RectF(0, 0, scaledWidth, scaledHeight),
					opacity,
					D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					NULL
				);

				hr = pSmallRT->EndDraw();
			}

			// 获取缩小后的 Bitmap
			ID2D1Bitmap* pSmallBitmap = NULL;
			if (SUCCEEDED(hr) && pSmallRT)
			{
				hr = pSmallRT->GetBitmap(&pSmallBitmap);
			}

			// 放大
			if (SUCCEEDED(hr) && pSmallBitmap)
			{
				hrt->DrawBitmap(
					pSmallBitmap,
					D2D1::RectF(x, y, x + cx/gScale, y + cy/gScale),
					opacity,
					D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					NULL
				);
			}

			// 清理资源
			// pSmallBitmap 是每次 GetBitmap 产生的，必须释放
			SafeRelease(&pSmallBitmap);

		
			if (FAILED(hr)) {
				SafeRelease(&pScreenCapture);
				SafeRelease(&pSmallRT);
			}

			return hr;
		}

		template <class T>
		void D2DCreateQuickBlur(T* h, float x, float y, float cx, float cy, float rad)
		{
			for (int i = 0; i < rad; i++)
			{
				int n = 3;
				if (gScale < 1.25)n = 2;
				float o = 0.9f;
				if (i > 8) { n = 4; o = 0.8; }
				if (i > 14) { n = 5; o = 0.8; }
				if (i > 18) { n = 8; o = 0.8; }
				if (i < 3) { n = 3; o = 0.5; }
				if (i < 2) { n = 3; o = 0.3; }
				D2DRenderCurrentScaledMipmap(h, x, y, cx, cy, n, o);
			}
		}
		template <class T>
		void D2DCreateQuickBlur2(T* h, float x, float y, float cx, float cy, float rad)
		{
			for (int i = 0; i < rad; i++)
			{
				int n = 4;
				if (gScale < 1.25)n = 2;
				float o = 0.9f;
				if (i > 8) { n = 4; o = 0.9; }
				if (i < 3) { n = 3; o = 0.8; }
				if (i < 2) { n = 3; o = 0.3; }
				D2DRenderCurrentScaledMipmap(h, x, y, cx, cy, n, o);
			}
		}
		template <class T>
		void D2DCreateQuickHeavyBlur(T* h, float x, float y, float cx, float cy, float rad)
		{
			for (int i = 0; i < rad; i++)
			{
				int n = 3;
				if (gScale < 1.25)n = 2;
				float o = 0.9f;
				if (i > 5) { n = 4; o = 0.8; }
				if (i > 8) { n = 8; o = 0.8; }
				if (i > 12) { n = 16; o = 0.8; }
				if (i > 16) { n = 32; o = 0.8; }
				if (i < 3) { n = 3; o = 0.5; }
				if (i < 2) { n = 3; o = 0.3; }
				D2DRenderCurrentScaledMipmap(h, x, y, cx, cy, n, o);
			}
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
		void D2DDrawDashedRoundRect(T* m_pDCRT, float x, float y, float cx, float cy, unsigned long ClrFill, float radius, float alpha = 1, float border = 0, unsigned long borderColor = 0, float borderAlphaSpecial = 0, bool OnlyBorder = false)
		{
			int hr = 0;
			ID2D1SolidColorBrush* testBrush = NULL;
			hr = m_pDCRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(ClrFill), alpha)),
				&testBrush
			);

			ID2D1SolidColorBrush* testBrushOut = NULL;
			float ba = (borderAlphaSpecial != 0) ? borderAlphaSpecial : alpha;
			hr = m_pDCRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(borderColor), ba)),
				&testBrushOut
			);

			if (SUCCEEDED(hr))
			{
				// 1. 获取 ID2D1Factory 以创建 StrokeStyle
				ID2D1Factory* pFactory = NULL;
				m_pDCRT->GetFactory(&pFactory);

				// 2. 定义虚线样式
				ID2D1StrokeStyle* pStrokeStyle = NULL;
				if (pFactory)
				{
					pFactory->CreateStrokeStyle(
						D2D1::StrokeStyleProperties(
							D2D1_CAP_STYLE_FLAT,    // 线端点样式
							D2D1_CAP_STYLE_FLAT,    // 线端点样式
							D2D1_CAP_STYLE_ROUND,   // 结合点样式
							D2D1_LINE_JOIN_MITER,
							5.0f,
							D2D1_DASH_STYLE_DASH,   // 这里设置为虚线 (Dash)
							0.0f
						),
						NULL, 0,
						&pStrokeStyle
					);
				}

				D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
					D2D1::RectF(x, y, cx + x, cy + y),
					radius,
					radius
				);

				// 3. 在绘制时传入 pStrokeStyle
				if (border > 0)
				{
					m_pDCRT->DrawRoundedRectangle(roundedRect, testBrushOut, border, pStrokeStyle);
				}

				if (OnlyBorder == false)
				{
					m_pDCRT->FillRoundedRectangle(roundedRect, testBrush);
				}

				// 释放资源
				SafeRelease(&testBrush);
				SafeRelease(&testBrushOut);
				SafeRelease(&pStrokeStyle);
				SafeRelease(&pFactory); // GetFactory 增加引用计数，视具体实现可能需要释放
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
		typedef void (D2DHWNDDRAWLAYERPANEL)(HWND, ID2D1HwndRenderTarget*, int x, int y, int cx, int cy);
		template <class T>
		void D2DDrawInClippedRoundRect(HWND hWnd, T pRT, float x, float y, float cx, float cy, float rr, std::function<D2DHWNDDRAWLAYERPANEL> dwf)
		{
			HRESULT hr = S_OK;

			// Create a layer.
			ID2D1Layer* pLayer = NULL;
			hr = pRT->CreateLayer(NULL, &pLayer);
			D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
				D2D1::RectF(x, y, cx + x, cy + y),
				rr, rr
			);
			ID2D1RoundedRectangleGeometry* pathGeo;
			m_pD2DFactory->CreateRoundedRectangleGeometry(roundedRect, &pathGeo);
			if (SUCCEEDED(hr))
			{
				pRT->PushLayer(
					D2D1::LayerParameters(D2D1::InfiniteRect(), pathGeo, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE),
					pLayer
				);

				dwf(hWnd, pRT, x, y, cx, cy);
				pRT->PopLayer();

			}
			SafeRelease(&pathGeo);
			SafeRelease(&pLayer);

			return;
		}
		void D2DDrawDynamicLightArea(ID2D1HwndRenderTarget* pRT, float x, float y, float cx, float cy, float rr, unsigned long backgroundClr, unsigned long lightClr, float bgAlpha, float lightAlpha)
		{
			// 1. 获取鼠标位置
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(pRT->GetHwnd(), &pt);
			pt.x /= gScale; pt.y /= gScale;

			// --- 增强版颜色转换辅助函数 ---
			// 指定颜色和特定的 alpha 值
			auto GetD2DColor = [](unsigned long clr, float alphaOverride) -> D2D1_COLOR_F {
				return D2D1::ColorF(
					GetRValue(clr) / 255.0f,
					GetGValue(clr) / 255.0f,
					GetBValue(clr) / 255.0f,
					alphaOverride // 直接使用外部传入的独立 Alpha
				);
				};

			// 2. 定义内部绘制逻辑
			auto drawLightEffect = [&](HWND hWnd, ID2D1HwndRenderTarget* rt, float _x, float _y, float _cx, float _cy) {

				// --- A. 绘制背景层 ---
				ID2D1SolidColorBrush* pBackBrush = nullptr;
				rt->CreateSolidColorBrush(GetD2DColor(backgroundClr, bgAlpha), &pBackBrush);
				rt->FillRectangle(D2D1::RectF(_x, _y, _x + _cx, _y + _cy), pBackBrush);

				// --- B. 绘制光源层 ---
				ID2D1GradientStopCollection* pGradientStops = nullptr;
				D2D1_GRADIENT_STOP stops[2];

				// 中心点：使用光源颜色和光源透明度
				stops[0].color = GetD2DColor(lightClr, lightAlpha);
				stops[0].position = 0.0f;

				// 边缘点：颜色设为光源色或背景色均可，但 Alpha 必须设为 0 (或极低) 
				// 这样光源才会“熄灭”并露出下方的背景层
				stops[1].color = GetD2DColor(lightClr, 0.0f);
				stops[1].position = 1.0f;

				rt->CreateGradientStopCollection(stops, 2, &pGradientStops);

				ID2D1RadialGradientBrush* pRadialBrush = nullptr;
				float lightRadius = _cx * 0.8f; // 可根据需要调整光源范围

				rt->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(
						D2D1::Point2F((float)pt.x, (float)pt.y),
						D2D1::Point2F(0, 0),
						lightRadius, lightRadius),
					pGradientStops,
					&pRadialBrush
				);

				// 填充光源（此时它会叠加在背景层之上）
				rt->FillRectangle(D2D1::RectF(_x, _y, _x + _cx, _y + _cy), pRadialBrush);

				// 释放资源
				SafeRelease(&pRadialBrush);
				SafeRelease(&pGradientStops);
				SafeRelease(&pBackBrush);
				};

			// 4. 调用裁剪模板
			D2DDrawInClippedRoundRect<ID2D1HwndRenderTarget*>(pRT->GetHwnd(), pRT, x, y, cx, cy, rr, drawLightEffect);
		}
		template<class T>
		void D2DDrawText(T pRenderTarget, const wchar_t* Text, float x, float y, int cx, int cy, float Size = 18, unsigned long ClrFill = VERTEXUICOLOR_WHITE, const wchar_t* font = L"Segoe UI", float alpha = 1, DWRITE_FONT_WEIGHT wid = DWRITE_FONT_WEIGHT_NORMAL)
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
		template<class T>
		void D2DDrawText3(T pRenderTarget, const wchar_t* Text, float x, float y, float cx, float cy, float Size = 18, unsigned long ClrFill = VERTEXUICOLOR_WHITE, const wchar_t* font = L"Segoe UI", float alpha = 1, bool center = false)
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
				DWRITE_FONT_WEIGHT_SEMI_BOLD,
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

		// 辅助函数：查找所有 :...: 模式的匹配
		std::vector<std::pair<size_t, size_t>> FindIconPatterns(const std::wstring& text) {
			std::vector<std::pair<size_t, size_t>> matches;
			size_t pos = 0;
			while (pos < text.length()) {
				size_t start = text.find(L':', pos);
				if (start == std::wstring::npos) break;

				size_t end = text.find(L':', start + 1);
				if (end == std::wstring::npos) break;

				// 检查是否是有效的模式，中间应该有内容
				if (end > start + 1) {
					matches.push_back({ start, end });
				}
				pos = end + 1;
			}
			return matches;
		}

		template<class T>
		void D2DDrawText4(T pRenderTarget, const wchar_t* Text, float x, float y, float cx, float cy,
			float Size = 18, unsigned long ClrFill = VERTEXUICOLOR_WHITE,
			const wchar_t* font = L"Segoe UI", float alpha = 1, bool center = false,
			const wchar_t* iconFont = L"Font Awesome 6 Free Solid") // 添加图标字体参数
		{
			if (!Text) return;
			if (cx < 5 || cy == 5)return;

			std::wstring fullText = Text;
			auto iconMatches = FindIconPatterns(fullText);

			// 创建默认字体格式
			IDWriteTextFormat* pDefaultFormat = NULL;
			pDWriteFactory->CreateTextFormat(
				font,
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				Size,
				L"",
				&pDefaultFormat
			);

			// 创建图标字体格式
			IDWriteTextFormat* pIconFormat = NULL;
			pDWriteFactory->CreateTextFormat(
				iconFont,
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				Size,
				L"",
				&pIconFormat
			);

			if (center) pDefaultFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			if (center) pIconFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

			// 创建画刷
			ID2D1SolidColorBrush* pBrush = NULL;
			pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF(RGBToHex(ClrFill), alpha)),
				&pBrush
			);

			D2D1_RECT_F layoutRect = D2D1::RectF(x, y, x + cx, y + cy);

			// 如果没有图标匹配，直接绘制整个文本
			if (iconMatches.empty()) {
				pRenderTarget->DrawText(
					Text,
					wcslen(Text),
					pDefaultFormat,
					layoutRect,
					pBrush
				);
			}
			else {
				// 使用 IDWriteTextLayout 进行复杂文本渲染
				IDWriteTextLayout* pTextLayout = NULL;
				pDWriteFactory->CreateTextLayout(
					fullText.c_str(),
					fullText.length(),
					pDefaultFormat,
					cx,  // max width
					cy,   // max height
					&pTextLayout
				);

				// 为每个图标范围设置不同的字体
				for (const auto& match : iconMatches) {
					DWRITE_TEXT_RANGE range;
					range.startPosition = static_cast<UINT32>(match.first);
					range.length = static_cast<UINT32>(match.second - match.first + 1); // 包含两个冒号
					pTextLayout->SetFontFamilyName(iconFont, range);
				}

				pRenderTarget->DrawTextLayout(
					D2D1::Point2F(x, y),
					pTextLayout,
					pBrush
				);

				SafeRelease(&pTextLayout);
			}

			SafeRelease(&pDefaultFormat);
			SafeRelease(&pIconFormat);
			SafeRelease(&pBrush);
		}

		template<class T>
		void D2DDrawText4_1(T pRenderTarget, const wchar_t* Text, float x, float y, float cx, float cy,
			float Size = 18, unsigned long ClrFill = VERTEXUICOLOR_WHITE,
			const wchar_t* font = L"Segoe UI", float alpha = 1, bool center = false,
			const wchar_t* iconFont = L"Font Awesome 6 Free Solid")
		{
			if (!Text || !pRenderTarget) return;
			if (cx < 5 || cy < 5) return;

			std::wstring fullText = Text;
			auto iconMatches = FindIconPatterns(fullText);

			// 1. 创建半粗体格式，样式改为 NORMAL (不再使用 OBLIQUE)
			IDWriteTextFormat* pDefaultFormat = NULL;
			pDWriteFactory->CreateTextFormat(
				font,
				NULL,
				DWRITE_FONT_WEIGHT_SEMI_BOLD,
				DWRITE_FONT_STYLE_NORMAL, // 这里改回正常，后面用矩阵倾斜
				DWRITE_FONT_STRETCH_NORMAL,
				Size,
				L"",
				&pDefaultFormat
			);

			// 2. 创建图标字体格式
			IDWriteTextFormat* pIconFormat = NULL;
			pDWriteFactory->CreateTextFormat(
				iconFont,
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				Size,
				L"",
				&pIconFormat
			);

			if (center) {
				pDefaultFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				pIconFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			}

			// 3. 创建画刷
			ID2D1SolidColorBrush* pBrush = NULL;
			pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(RGBToHex(ClrFill), alpha),
				&pBrush
			);

			// --- 矩阵变换核心部分 ---
			D2D1_MATRIX_3X2_F oldMatrix;
			pRenderTarget->GetTransform(&oldMatrix);

			// 计算倾斜中心：如果是左对齐，中心在 (x, y + Size/2)
			// 如果是居中对齐，中心在 (x + cx/2, y + cy/2)
			D2D1_POINT_2F skewCenter = center ?
				D2D1::Point2F(x + cx / 2.0f, y + cy / 2.0f) :
				D2D1::Point2F(x, y + Size / 2.0f);

			// -12.0f 是倾斜角度，中文建议在 -10.0f 到 -15.0f 之间
			D2D1_MATRIX_3X2_F skewMatrix = D2D1::Matrix3x2F::Skew(-10.0f, 0.0f, skewCenter);

			// 应用变换
			pRenderTarget->SetTransform(skewMatrix * oldMatrix);
			// -----------------------

			D2D1_RECT_F layoutRect = D2D1::RectF(x, y, x + cx, y + cy);

			if (iconMatches.empty()) {
				pRenderTarget->DrawText(
					Text,
					(UINT32)wcslen(Text),
					pDefaultFormat,
					layoutRect,
					pBrush
				);
			}
			else {
				IDWriteTextLayout* pTextLayout = NULL;
				pDWriteFactory->CreateTextLayout(
					fullText.c_str(),
					(UINT32)fullText.length(),
					pDefaultFormat,
					cx,
					cy,
					&pTextLayout
				);

				for (const auto& match : iconMatches) {
					DWRITE_TEXT_RANGE range;
					range.startPosition = static_cast<UINT32>(match.first);
					range.length = static_cast<UINT32>(match.second - match.first + 1);
					pTextLayout->SetFontFamilyName(iconFont, range);
				}

				pRenderTarget->DrawTextLayout(
					D2D1::Point2F(x, y),
					pTextLayout,
					pBrush
				);

				SafeRelease(&pTextLayout);
			}

			// 恢复原始变换，防止影响后续绘制
			pRenderTarget->SetTransform(oldMatrix);

			SafeRelease(&pDefaultFormat);
			SafeRelease(&pIconFormat);
			SafeRelease(&pBrush);
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
		void CreateD2DPanel(HWND hWnd, std::function<void(HWND, ID2D1HwndRenderTarget*)> dwf) {
			RECT rc;
			GetClientRect(hWnd, &rc);
			ID2D1HwndRenderTarget* pRT = NULL;

			// Create a Direct2D render target          
			if (pRT == NULL && pRts[hWnd] == nullptr)
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
		template <class T>
		HRESULT LoadBitmapFromFile(
			T* pRenderTarget,
			IWICImagingFactory* pIWICFactory,
			const wchar_t* uri,
			int x, int y, int cx, int cy
		)
		{
			ID2D1Bitmap* bmp;
			IWICBitmapDecoder* pDecoder = NULL;
			IWICBitmapFrameDecode* pSource = NULL;
			IWICStream* pStream = NULL;
			IWICFormatConverter* pConverter = NULL;
			IWICBitmapScaler* pScaler = NULL;

			HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
				uri,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&pDecoder
			);
			if (SUCCEEDED(hr))
			{
				// Create the initial frame.
				hr = pDecoder->GetFrame(0, &pSource);
			}
			if (SUCCEEDED(hr))
			{

				// Convert the image format to 32bppPBGRA
				// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
				hr = pIWICFactory->CreateFormatConverter(&pConverter);

			}


			if (SUCCEEDED(hr))
			{
				hr = pConverter->Initialize(
					pSource,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
				if (SUCCEEDED(hr))
				{

					// Create a Direct2D bitmap from the WIC bitmap.
					hr = pRenderTarget->CreateBitmapFromWicBitmap(
						pConverter,
						NULL,
						&bmp
					);
					pRenderTarget->DrawBitmap(bmp, D2D1::RectF(x, y, x + cx, y + cy));
				}


				SafeRelease(&pDecoder);
				SafeRelease(&pSource);
				SafeRelease(&pStream);
				SafeRelease(&pConverter);
				SafeRelease(&pScaler);
				SafeRelease(&bmp);
				return hr;
			}
		}
		template <class T>
		void D2DDrawQuickShadow(
			T hdc,
			float x,
			float y,
			float cx,
			float cy,
			float cornerRadius,
			float offsetX = 0.0f,      // x偏移
			float offsetY = 0.0f,      // y偏移
			float blurRadius = 10.0f,  // 模糊半径
			int blurQuality = 10,      // 模糊质量（层数）
			float spreadRadius = 0.0f, // 扩展半径（类似box-shadow的spread参数）
			float opacity = 0.15f,     // 整体透明度
			DWORD shadowColor = RGB(0, 0, 0)  // 阴影颜色
		)
		{
			for (int i = 0; i < blurQuality; i++) {
				float progress = (float)i / blurQuality;  // 0到1的进度
				float currentBlur = blurRadius * progress; // 当前层的模糊偏移

				// 透明度：越外层越透明
				float alpha = opacity * (1.0f - progress);

				// 关键修正：
				// 所有层都以同一个中心点绘制，只改变大小和圆角，不移动位置
				// 但加上 offset 来整体偏移整个阴影
				float drawX = x + offsetX;
				float drawY = y + offsetY;

				// 尺寸调整：模糊和扩展共同作用
				float widthAdjust = currentBlur + spreadRadius;
				float heightAdjust = currentBlur + spreadRadius;

				D2DDrawRoundRect(
					hdc,
					drawX - widthAdjust,   // 左上角x
					drawY - heightAdjust + 3,  // 左上角y（保留+3偏移）
					cx + widthAdjust * 2,  // 宽度
					cy + heightAdjust * 2, // 高度
					shadowColor,
					cornerRadius + currentBlur,  // 圆角随模糊增加
					alpha
				);
			}
		}

		template <class T>
		void D2DDrawRoundRectWithTexture(T hrt, float x, float y, float cx, float cy, unsigned long clr, float rad)
		{
			D2DDrawRoundRect(hrt, x, y, cx, cy, VuiFadeColor(clr, 10), rad);
			D2DDrawRoundRect(hrt, x, y, cx, cy-1, VuiFadeColor(clr, 5), rad);
			D2DDrawRoundRect(hrt, x, y, cx, cy-2, clr, rad);
		}

		template <class T>
		void D2DDrawRoundRectWithTexture2(T hrt, float x, float y, float cx, float cy, unsigned long clr, float rad,float a,float bdsize)
		{
			D2DDrawRoundRect(hrt, x, y, cx, cy, VuiFadeColor(clr, 10), rad,a,bdsize, VuiFadeColor(clr, 15));
			D2DDrawRoundRect(hrt, x, y, cx, cy - 1, VuiFadeColor(clr, 5), rad,a);
			D2DDrawRoundRect(hrt, x, y, cx, cy - 2, clr, rad,a);
		}

		ID2D1Bitmap* LoadIconToD2DBitmap(ID2D1HwndRenderTarget* hrt, UINT iconID, int destWidth = 0, int destHeight = 0) {
			HRESULT hr = S_OK;

			HICON hIcon = (HICON)LoadImage(
				GetModuleHandle(NULL),
				MAKEINTRESOURCE(iconID),
				IMAGE_ICON,
				256,  
				256, 
				LR_SHARED
			);

			if (!hIcon) return nullptr;

			if (!m_ImageFactory) {
				hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_ImageFactory));
				if (FAILED(hr)) return nullptr;
			}

			IWICBitmap* pWICBitmap = nullptr;
			IWICFormatConverter* pConverter = nullptr;
			ID2D1Bitmap* pD2DBitmap = nullptr;

			hr = m_ImageFactory->CreateBitmapFromHICON(hIcon, &pWICBitmap);

			if (SUCCEEDED(hr)) {
				hr = m_ImageFactory->CreateFormatConverter(&pConverter);
			}

			if (SUCCEEDED(hr)) {
		
				hr = pConverter->Initialize(
					pWICBitmap,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}

			if (SUCCEEDED(hr)) {
				// 3. 创建位图属性，确保 DPI 一致
				// 有时默认 DPI 较低也会导致模糊，我们可以手动设置为渲染目标的 DPI
				FLOAT dpiX, dpiY;
				hrt->GetDpi(&dpiX, &dpiY);
				D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

				hr = hrt->CreateBitmapFromWicBitmap(pConverter, &props, &pD2DBitmap);
			}

			if (pConverter) pConverter->Release();
			if (pWICBitmap) pWICBitmap->Release();

			return pD2DBitmap;
		}
		template<class T>
		void D2DDrawNoiseRect(
			T* m_pDCRT,
			float x, float y, float cx, float cy,
			unsigned long ClrFill,
			unsigned int seed,
			float density = 0.5f,
			float layerOpacity = 1.0f // 新增：图层透明度参数 (0.0~1.0)
		)
		{
		
			UINT32 width = static_cast<UINT32>(cx);
			UINT32 height = static_cast<UINT32>(cy);
			if (width <= 0 || height <= 0) return;
			std::vector<UINT32> pixels(width * height);

			BYTE r_base = GetRValue(ClrFill);
			BYTE g_base = GetGValue(ClrFill);
			BYTE b_base = GetBValue(ClrFill);

			// 3. 随机数生成器
			std::mt19937 gen(seed);
			std::uniform_real_distribution<float> dist(0.0f, 1.0f);
			// 亮度抖动范围：0.9 到 1.1 (即 10% 的幅度)
			std::uniform_real_distribution<float> lightDist(0.8f, 1.25f);

			for (UINT32 i = 0; i < width * height; ++i) {
				if (dist(gen) < density) {
					// 计算随机亮度因子
					float factor = lightDist(gen);
					BYTE r = static_cast<BYTE>(min(255.0f, r_base * factor));
					BYTE g = static_cast<BYTE>(min(255.0f, g_base * factor));
					BYTE b = static_cast<BYTE>(min(255.0f, b_base * factor));

					// 内存布局：0xAARRGGBB (对应小端序 B8G8R8A8)
					pixels[i] = (0xFF << 24) | (r << 16) | (g << 8) | b;
				}
				else {
					pixels[i] = 0x00000000;
				}
			}

			// 4. 创建 D2D 位图
			ID2D1Bitmap* pBitmap = NULL;
			D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
			);

			HRESULT hr = m_pDCRT->CreateBitmap(
				D2D1::SizeU(width, height),
				pixels.data(),
				width * sizeof(UINT32),
				&props,
				&pBitmap
			);

			if (SUCCEEDED(hr)) {
				D2D1_RECT_F destRect = D2D1::RectF(x, y, x + cx, y + cy);
				// 在此处指定图层透明度 layerOpacity
				m_pDCRT->DrawBitmap(pBitmap, destRect, layerOpacity);
				pBitmap->Release(); // 建议使用原生 Release 或你的 SafeRelease
			}
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

		HICON GetFileIcon3(const wchar_t* pszPath)
		{
			SHFILEINFO sfi;
			if (!SHGetFileInfo(pszPath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX)) return NULL;

			// 获取大号图像列表
			IImageList* piml;

			if (FAILED(SHGetImageList(SHIL_JUMBO, IID_PPV_ARGS(&piml)))) return NULL;

			// 提取图标
			HICON hico;
			piml->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hico);

			// 清理资源
			piml->Release();

			return hico;

		}
		HICON GetExtIcon2(const wchar_t* pszPath)
		{
			SHFILEINFO sfi;
			if (!SHGetFileInfo(pszPath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES)) return NULL;

			// 获取大号图像列表
			IImageList* piml;

			if (FAILED(SHGetImageList(SHIL_JUMBO, IID_PPV_ARGS(&piml)))) return NULL;

			// 提取图标
			HICON hico;
			piml->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &hico);

			// 清理资源
			piml->Release();

			return hico;

		}

		HICON GetExtIcon3(const wchar_t* pszPath)
		{
			SHFILEINFO sfi;
			if (!SHGetFileInfo(pszPath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES)) return NULL;

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
		template <typename T>
		ID2D1Bitmap* D2DCreateIconBitmap(T* pRenderTarget, const wchar_t* path, int sz)
		{
			if (!pRenderTarget) return nullptr;

			// 获取HICON

			HICON hIcon;
			if (sz > 128)
			{
				hIcon = GetFileIcon3(path);
			}
			else  hIcon = GetFileIcon2(path);
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
			DestroyIcon(hIcon);
			DeleteDC(hdcMem);
			ReleaseDC(nullptr, hdcScreen);

			return pD2DBitmap;
		}
		template <typename T>
		ID2D1Bitmap* D2DCreateExtIconBitmap(T* pRenderTarget, const wchar_t* path, int sz)
		{
			if (!pRenderTarget) return nullptr;

			// 获取HICON

			HICON hIcon;
			if (sz > 128)
			{
				hIcon = GetExtIcon3(path);
			}
			else  hIcon = GetExtIcon2(path);
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
			DestroyIcon(hIcon);
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
		void D2DDrawBitmapFrompBm(T hrt, ID2D1Bitmap* pBm, float x, float y, float sz)
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
		void D2DDrawBitmapFrompBm2(T hrt, ID2D1Bitmap* pBm, float x, float y, float sz, float opacity = 1.0f)
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
				opacity, // <--- 关键修改：使用新的 opacity 参数
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
		template <typename T>
		void D2DDrawBitmapFromByte2(T hdc, BYTE* pPixels, int x, int y, int w, int h, int sc)
		{
			int bp = gByteWidth;

			// 创建一个 D2D 位图
			D2D1_SIZE_U size = D2D1::SizeU(w / sc, h / sc);
			D2D1_BITMAP_PROPERTIES bitmapProps = D2D1::BitmapProperties(
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));


			ID2D1Bitmap* d2dBitmap = nullptr;
			hdc->CreateBitmap(size, pPixels, bp, bitmapProps, &d2dBitmap);



			// 绘制 D2D 位图

			hdc->DrawBitmap(d2dBitmap, D2D1::RectF(x, y, x + w, y + h), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, w / sc, h / sc));

			// 结束绘制

			// 清理
			d2dBitmap->Release();
		}
		template <typename T>
		void D2DDisplayIcon(T hdc, const wchar_t* path, int x, int y, int sz)
		{
			ID2D1Bitmap* BMP = D2DCreateIconBitmap(hdc, path, sz);
			D2DDrawBitmapFrompBm(hdc, BMP, x, y, sz);
			SafeRelease(&BMP);
		}
		static std::unordered_map<std::wstring, ID2D1Bitmap*> g_icon_cache;

		// 从缓存中获取或创建图标并显示
		// 修改全局缓存结构：为每个渲染目标维护独立的缓存
		static std::unordered_map<ID2D1RenderTarget*,
			std::unordered_map<std::wstring, ID2D1Bitmap*>> g_per_context_icon_cache;

		template <typename T>
		void D2DDisplayIconFromCache(T hdc, const wchar_t* path, float x, float y, float sz,float opacity=1.0)
		{
			if (!hdc) return;

		
			ID2D1RenderTarget* pRenderTarget = nullptr;
			if constexpr (std::is_same_v<T, ID2D1HwndRenderTarget*>) {
				pRenderTarget = static_cast<ID2D1HwndRenderTarget*>(hdc);
			}
			else if constexpr (std::is_same_v<T, ID2D1DeviceContext*>) {
				pRenderTarget = static_cast<ID2D1DeviceContext*>(hdc);
			}
			else if constexpr (std::is_same_v<T, ID2D1RenderTarget*>) {
				pRenderTarget = static_cast<ID2D1RenderTarget*>(hdc);
			}
			else {
				return;
			}

			if (!pRenderTarget) return;

			std::wstring path_key(path);

			//缓存查找
			auto context_it = g_per_context_icon_cache.find(pRenderTarget);
			ID2D1Bitmap* BMP = nullptr;

			if (context_it != g_per_context_icon_cache.end()) {
				auto& icon_map = context_it->second;
				auto it = icon_map.find(path_key);
				if (it != icon_map.end()) {
					BMP = it->second;
				}
			}

			if (!BMP) {
				
				BMP = D2DCreateIconBitmap(hdc, path, sz);
				if (BMP) {
					
					g_per_context_icon_cache[pRenderTarget][path_key] = BMP;
				}
			}

			if (BMP) {
				if (opacity >= 1.0)
				{
					D2DDrawBitmapFrompBm(hdc, BMP, x, y, sz);
				}
				else D2DDrawBitmapFrompBm2(hdc, BMP, x, y, sz,opacity);
			}
		}

		template <typename T>
		void D2DDisplayExtIconFromCache(T hdc, const wchar_t* path, int x, int y, int sz)
		{

			std::wstring path_key(path);


			auto it = g_icon_cache.find(path_key);
			ID2D1Bitmap* BMP = nullptr;

			if (it != g_icon_cache.end()) {

				BMP = it->second;

			}
			else {

				BMP = D2DCreateExtIconBitmap(hdc, path, sz);

				if (BMP) {
					g_icon_cache[path_key] = BMP;
				}
			}


			if (BMP) {
				D2DDrawBitmapFrompBm(hdc, BMP, x, y, sz);
			}

		}

		void D2DClearIconCache() {
			for (auto& pair : g_icon_cache) {
				SafeRelease(&pair.second); 
			}
			g_icon_cache.clear(); 
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
		}
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
	double CalcBezierCurve(double t, double b, double c, double d,
		double p1x, double p1y, double p2x, double p2y) {

		if (d <= 0) return c;
		if (t <= 0) return b;
		if (t >= d) return c;


		double normalizedT = t / d;

		double uMin = 0.0;
		double uMax = 1.0;
		double u = 0.5;
		const int iterations = 20;

		for (int i = 0; i < iterations; ++i) {
			u = (uMin + uMax) / 2.0;

			double oneMinusU = 1.0 - u;
			double uSq = u * u;
			double oneMinusUSq = oneMinusU * oneMinusU;

			double x = 3.0 * oneMinusUSq * u * p1x +
				3.0 * oneMinusU * uSq * p2x +
				uSq * u;

			if (x < normalizedT) {
				uMin = u;
			}
			else {
				uMax = u;
			}
		}


		double oneMinusU = 1.0 - u;
		double uSq = u * u;
		double oneMinusUSq = oneMinusU * oneMinusU;

		double y = 3.0 * oneMinusUSq * u * p1y +
			3.0 * oneMinusU * uSq * p2y +
			uSq * u;

		return b + y * (c - b);
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
}