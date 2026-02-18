#pragma once

/*VertexUI Window Presets*/
///#include "framework.h"
#include <map>
#include <math.h>
#include <crtdbg.h>
#include "VertexUI.min.h"
#include "../LightFrame.WindowAnimation.h"
#define win_max(a,b)            (((a) > (b)) ? (a) : (b))
#define win_min(a,b)            (((a) < (b)) ? (a) : (b))
void CenterWindow(HWND hWnd)
{
	int scrWidth, scrHeight;
	RECT rect;
	//获得屏幕尺寸
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//取得窗口尺寸
	GetWindowRect(hWnd, &rect);
	//重新设置rect里的值  
	long width = rect.right - rect.left;
	long height = rect.bottom - rect.top;
	rect.left = (scrWidth - width) / 2;
	rect.top = (scrHeight - height) / 2;

	//移动窗口到指定的位置
	SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, width, height, SWP_NOSIZE | SWP_NOZORDER);
}

namespace VertexUI
{
	namespace Window
	{
		namespace SimpleShadow
		{
			class iDropShadow
			{
			public:
				iDropShadow(void);
				virtual ~iDropShadow(void);

				void Create(HDC hdc, int x1, int y1, int cx1, int cy1, int r);

				bool SetSize(int NewSize = 0);
				bool SetSharpness(unsigned int NewSharpness = 5);
				bool SetDarkness(unsigned int NewDarkness = 200);
				bool SetPosition(int NewXOffset = 5, int NewYOffset = 5);
				bool SetColor(COLORREF NewColor = 0);

				void Update(HDC hdc, int x, int y, int cx, int cy)
				{

					int nShadWndWid = cx+ gnSize * 2;
					int nShadWndHei = cy+ gnSize * 2;

					BITMAPINFO bmi;

					ZeroMemory(&bmi, sizeof(BITMAPINFO));
					bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					bmi.bmiHeader.biWidth = nShadWndWid;
					bmi.bmiHeader.biHeight = nShadWndHei;
					bmi.bmiHeader.biPlanes = 1;
					bmi.bmiHeader.biBitCount = 32;
					bmi.bmiHeader.biCompression = BI_RGB;
					bmi.bmiHeader.biSizeImage = nShadWndWid * nShadWndHei * 4;

					BYTE* pvBits;

					HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pvBits, NULL, 0);

					ZeroMemory(pvBits, bmi.bmiHeader.biSizeImage);
					MakeShadow((UINT32*)pvBits, x, y, cx, cy);


					POINT ptSrc = { 0, 0 };
					SIZE WndSize = { nShadWndWid, nShadWndHei };
					BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

					HDC hMemDC = CreateCompatibleDC(NULL);
					HBITMAP hOriBmp = (HBITMAP)SelectObject(hMemDC, hbitmap);

					AlphaBlend(hdc, x + gnxOffset-gnSize, y + gnyOffset-gnSize, cx+gnSize*2, cy+gnSize*2,hMemDC,  0, 0, cx+gnSize*2, cy+gnSize*2,blendPixelFunction);

					SelectObject(hMemDC, hOriBmp);
					DeleteObject(hbitmap);
					DeleteDC(hMemDC);

				}
			protected:




				unsigned char gnDarkness;
				unsigned char gnSharpness;
				signed char gnSize;
				signed char gnxOffset;
				signed char gnyOffset;

				COLORREF gColor;

				int r;

				void MakeShadow(UINT* pShadBits, int x, int y, int cx, int cy);
				inline DWORD PreMultiply(COLORREF cl, unsigned char nAlpha)
				{
					return (GetRValue(cl) * (DWORD)nAlpha / 255) << 16 |
						(GetGValue(cl) * (DWORD)nAlpha / 255) << 8 |
						(GetBValue(cl) * (DWORD)nAlpha / 255);
				}
				void Show(HDC hdc, int x, int y, int cx, int cy);


			
			};
			iDropShadow::iDropShadow()
				:
				gnDarkness(150)
				, gnSharpness(5)
				, gnSize(50)
				, gnxOffset(5)
				, gnyOffset(5)
				, gColor(RGB(0, 0, 0))
			{
			}

			iDropShadow::~iDropShadow()
			{
			}


			void iDropShadow::Create(HDC hdc, int x1, int y1, int cx1, int cy1, int r)
			{
				this->r = r * gScale;
				Show(hdc, (x1+1) * gScale, (y1+1) * gScale, (cx1-2) * gScale, (cy1-2) * gScale);
			}



			//算法部分
			void iDropShadow::MakeShadow(UINT* pShadBits, int x, int y, int cx, int cy)
			{
				// The shadow algorithm:
				// Apply modified (with blur effect) morphologic dilation to make the blurred border
				HRGN hWndRgn = CreateRoundRectRgn(-1,-1, cx+2, cy+2,r,r);


				// Deterwin_mine the Start and end point of each horizontal scan line
				SIZE szParent = { cx,cy };
				SIZE szShadow = { szParent.cx + 2 * gnSize, szParent.cy + 2 * gnSize };
				// Extra 2 lines (set to be empty) in ptAnchors are used in dilation
				int nAnchors = win_max(szParent.cy, szShadow.cy);	// # of anchor points pares
				int(*ptAnchors)[2] = new int[nAnchors + 2][2];
				int(*ptAnchorsOri)[2] = new int[szParent.cy][2];	// anchor points, will not modify during erosion
				ptAnchors[0][0] = szParent.cx;
				ptAnchors[0][1] = 0;
				ptAnchors[nAnchors + 1][0] = szParent.cx;
				ptAnchors[nAnchors + 1][1] = 0;

				if (gnSize > 0)
				{
					// Put the parent window anchors at the center
					for (int i = 0; i < gnSize; i++)
					{
						ptAnchors[i + 1][0] = szParent.cx;
						ptAnchors[i + 1][1] = 0;
						ptAnchors[szShadow.cy - i][0] = szParent.cx;
						ptAnchors[szShadow.cy - i][1] = 0;
					}
					ptAnchors += gnSize;
				}
				for (int i = 0; i < szParent.cy; i++)
				{
					// find start point
					int j;
					for (j = 0; j < szParent.cx; j++)
					{
						if (PtInRegion(hWndRgn, j, i))
						{
							ptAnchors[i + 1][0] = j + gnSize;
							ptAnchorsOri[i][0] = j;
							break;
						}
					}

					if (j >= szParent.cx)	// Start point not found
					{
						ptAnchors[i + 1][0] = szParent.cx;
						ptAnchorsOri[i][1] = 0;
						ptAnchors[i + 1][0] = szParent.cx;
						ptAnchorsOri[i][1] = 0;
					}
					else
					{
						// find end point
						for (j = szParent.cx - 1; j >= ptAnchors[i + 1][0]; j--)
						{
							if (PtInRegion(hWndRgn, j, i))
							{
								ptAnchors[i + 1][1] = j + 1 + gnSize;
								ptAnchorsOri[i][1] = j + 1;
								break;
							}
						}
					}
					// 		if(0 != ptAnchorsOri[i][1])
					// 			_RPTF2(_CRT_WARN, "%d %d\n", ptAnchorsOri[i][0], ptAnchorsOri[i][1]);
				}

				if (gnSize > 0)
					ptAnchors -= gnSize;	// Restore pos of ptAnchors for erosion
				int(*ptAnchorsTmp)[2] = new int[nAnchors + 2][2];	// Store the result of erosion
				// First and last line should be empty
				ptAnchorsTmp[0][0] = szParent.cx;
				ptAnchorsTmp[0][1] = 0;
				ptAnchorsTmp[nAnchors + 1][0] = szParent.cx;
				ptAnchorsTmp[nAnchors + 1][1] = 0;
				int nEroTimes = 0;
				// morphologic erosion
				for (int i = 0; i < gnSharpness - gnSize; i++)
				{
					nEroTimes++;
					//ptAnchorsTmp[1][0] = szParent.cx;
					//ptAnchorsTmp[1][1] = 0;
					//ptAnchorsTmp[szParent.cy + 1][0] = szParent.cx;
					//ptAnchorsTmp[szParent.cy + 1][1] = 0;
					for (int j = 1; j < nAnchors + 1; j++)
					{
						ptAnchorsTmp[j][0] = win_max(ptAnchors[j - 1][0], win_max(ptAnchors[j][0], ptAnchors[j + 1][0])) + 1;
						ptAnchorsTmp[j][1] = win_min(ptAnchors[j - 1][1], win_min(ptAnchors[j][1], ptAnchors[j + 1][1])) - 1;
					}
					// Exchange ptAnchors and ptAnchorsTmp;
					int(*ptAnchorsXange)[2] = ptAnchorsTmp;
					ptAnchorsTmp = ptAnchors;
					ptAnchors = ptAnchorsXange;
				}

				// morphologic dilation
				ptAnchors += (gnSize < 0 ? -gnSize : 0) + 1;	// now coordinates in ptAnchors are same as in shadow window
				// Generate the kernel
				int nKernelSize = gnSize > gnSharpness ? gnSize : gnSharpness;
				int nCenterSize = gnSize > gnSharpness ? (gnSize - gnSharpness) : 0;
				UINT32* pKernel = new UINT32[(2 * nKernelSize + 1) * (2 * nKernelSize + 1)];
				UINT32* pKernelIter = pKernel;
				for (int i = 0; i <= 2 * nKernelSize; i++)
				{
					for (int j = 0; j <= 2 * nKernelSize; j++)
					{
						double dLength = sqrt((i - nKernelSize) * (i - nKernelSize) + (j - nKernelSize) * (double)(j - nKernelSize));
						if (dLength < nCenterSize)
							*pKernelIter = gnDarkness << 24 | PreMultiply(gColor, gnDarkness);
						else if (dLength <= nKernelSize)
						{
							UINT32 nFactor = ((UINT32)((1 - (dLength - nCenterSize) / (gnSharpness + 1)) * gnDarkness));
							*pKernelIter = nFactor << 24 | PreMultiply(gColor, nFactor);
						}
						else
							*pKernelIter = 0;
						//TRACE("%d ", *pKernelIter >> 24);
						pKernelIter++;
					}
					//TRACE("\n");
				}
				// Generate blurred border
				for (int i = nKernelSize; i < szShadow.cy - nKernelSize; i++)
				{
					int j;
					if (ptAnchors[i][0] < ptAnchors[i][1])
					{

						// Start of line
						for (j = ptAnchors[i][0];
							j < win_min(win_max(ptAnchors[i - 1][0], ptAnchors[i + 1][0]) + 1, ptAnchors[i][1]);
							j++)
						{
							for (int k = 0; k <= 2 * nKernelSize; k++)
							{
								UINT32* pPixel = pShadBits +
									(szShadow.cy - i - 1 + nKernelSize - k) * szShadow.cx + j - nKernelSize;
								UINT32* pKernelPixel = pKernel + k * (2 * nKernelSize + 1);
								for (int l = 0; l <= 2 * nKernelSize; l++)
								{
									if (*pPixel < *pKernelPixel)
										*pPixel = *pKernelPixel;
									pPixel++;
									pKernelPixel++;
								}
							}
						}	// for() start of line

						// End of line
						for (j = win_max(j, win_min(ptAnchors[i - 1][1], ptAnchors[i + 1][1]) - 1);
							j < ptAnchors[i][1];
							j++)
						{
							for (int k = 0; k <= 2 * nKernelSize; k++)
							{
								UINT32* pPixel = pShadBits +
									(szShadow.cy - i - 1 + nKernelSize - k) * szShadow.cx + j - nKernelSize;
								UINT32* pKernelPixel = pKernel + k * (2 * nKernelSize + 1);
								for (int l = 0; l <= 2 * nKernelSize; l++)
								{
									if (*pPixel < *pKernelPixel)
										*pPixel = *pKernelPixel;
									pPixel++;
									pKernelPixel++;
								}
							}
						}	// for() end of line

					}
				}	// for() Generate blurred border

				// Erase unwanted parts and complement missing
				UINT32 clCenter = gnDarkness << 24 | PreMultiply(gColor, gnDarkness);
				for (int i = win_min(nKernelSize, win_max(gnSize - gnyOffset, 0));
					i < win_max(szShadow.cy - nKernelSize, win_min(szParent.cy + gnSize - gnyOffset, szParent.cy + 2 * gnSize));
					i++)
				{
					UINT32* pLine = pShadBits + (szShadow.cy - i - 1) * szShadow.cx;
					if (i - gnSize + gnyOffset < 0 || i - gnSize + gnyOffset >= szParent.cy)	// Line is not covered by parent window
					{
						for (int j = ptAnchors[i][0]; j < ptAnchors[i][1]; j++)
						{
							*(pLine + j) = clCenter;
						}
					}
					else
					{
						for (int j = ptAnchors[i][0];
							j < win_min(ptAnchorsOri[i - gnSize + gnyOffset][0] + gnSize - gnxOffset, ptAnchors[i][1]);
							j++)
							*(pLine + j) = clCenter;
						for (int j = win_max(ptAnchorsOri[i - gnSize + gnyOffset][0] + gnSize - gnxOffset, 0);
							j < win_min(ptAnchorsOri[i - gnSize + gnyOffset][1] + gnSize - gnxOffset, szShadow.cx);
							j++)
							*(pLine + j) = 0;
						for (int j = win_max(ptAnchorsOri[i - gnSize + gnyOffset][1] + gnSize - gnxOffset, ptAnchors[i][0]);
							j < ptAnchors[i][1];
							j++)
							*(pLine + j) = clCenter;
					}
				}

				// Delete used resources
				delete[](ptAnchors - (gnSize < 0 ? -gnSize : 0) - 1);
				delete[] ptAnchorsTmp;
				delete[] ptAnchorsOri;
				delete[] pKernel;
				DeleteObject(hWndRgn);
			}

			bool iDropShadow::SetSize(int NewSize)
			{
				if (NewSize > 40 || NewSize < -20)
					return false;

				gnSize = (signed char)NewSize*gScale;
				return true;
			}

			bool iDropShadow::SetSharpness(unsigned int NewSharpness)
			{
				if (NewSharpness > 80)
					return false;

				gnSharpness = (unsigned char)NewSharpness * gScale;
				return true;
			}

			bool iDropShadow::SetDarkness(unsigned int NewDarkness)
			{
				if (NewDarkness > 255)
					return false;

				gnDarkness = (unsigned char)NewDarkness;
				return true;
			}

			bool iDropShadow::SetPosition(int NewXOffset, int NewYOffset)
			{
				if (NewXOffset > 20 || NewXOffset < -20 ||
					NewYOffset > 20 || NewYOffset < -20)
					return false;

				gnxOffset = (signed char)NewXOffset * gScale;
				gnyOffset = (signed char)NewYOffset * gScale;
				return true;
			}

			bool iDropShadow::SetColor(COLORREF NewColor)
			{
				gColor = NewColor;
				return true;
			}

			void iDropShadow::Show(HDC hdc, int x1, int y1, int cx1, int cy1)
			{


					Update(hdc, x1, y1, cx1, cy1);

			}
		}
		namespace OuterShadow
		{
			HWND m_hWnd;
			const wchar_t* ShadowClassName = L"LightFrame.UI.BackDropWnd.Shadow.Class";
			class DropShadow;
			std::unordered_map<HWND, DropShadow>Shadowmap;
			class DropShadow
			{
			public:
				DropShadow(void);
			public:
				virtual ~DropShadow(void);

			protected:
				static HINSTANCE s_hInstance;
				static std::map<HWND, DropShadow*> s_Shadowmap;
				HRGN nRgn = (HRGN)NULL;
				HWND ghWnd;

				WNDPROC gOriParentProc;

				enum SS
				{
					SS_ENABLED = 1,
					SS_VISABLE = 1 << 1,
					SS_PARENTVISIBLE = 1 << 2,
				};
				BYTE gStatus;

				unsigned char gnDarkness;
				unsigned char gnSharpness;
				signed char gnSize;
				signed char gnxOffset;
				signed char gnyOffset;
				LPARAM gWndSize;
				bool gbUpdate;

				COLORREF gColor;

			public:
				static bool Initialize(HINSTANCE hInstance)
				{

					s_hInstance = hInstance;
					WNDCLASSEX wcex = { 0 };


					wcex.cbSize = sizeof(WNDCLASSEX);
					wcex.style = CS_HREDRAW | CS_VREDRAW;
					wcex.lpfnWndProc = SWndProc;
					wcex.cbClsExtra = 0;
					wcex.cbWndExtra = 0;
					wcex.hInstance = hInstance;
					wcex.hIcon = NULL;
					wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
					wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
					wcex.lpszMenuName = NULL;
					wcex.lpszClassName = ShadowClassName;
					wcex.hIconSm = NULL;
					RegisterClassEx(&wcex);

					return true;
				}
				void Create(HWND hParentWnd);

				bool SetSize(int NewSize = 0);
				bool SetSharpness(unsigned int NewSharpness = 5);
				bool SetDarkness(unsigned int NewDarkness = 200);
				bool SetPosition(int NewXOffset = 5, int NewYOffset = 5);
				bool SetColor(COLORREF NewColor = 0);
				void SetSpecRgn(HRGN hRgn)
				{
					this->nRgn = hRgn;

				}
				void Update(HWND hParent)
				{
					RECT WndRect;
					GetWindowRect(hParent, &WndRect);
					int nShadWndWid = WndRect.right - WndRect.left + gnSize * 2;
					int nShadWndHei = WndRect.bottom - WndRect.top + gnSize * 2;

					BITMAPINFO bmi;

					ZeroMemory(&bmi, sizeof(BITMAPINFO));
					bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					bmi.bmiHeader.biWidth = nShadWndWid;
					bmi.bmiHeader.biHeight = nShadWndHei;
					bmi.bmiHeader.biPlanes = 1;
					bmi.bmiHeader.biBitCount = 32;
					bmi.bmiHeader.biCompression = BI_RGB;
					bmi.bmiHeader.biSizeImage = nShadWndWid * nShadWndHei * 4;

					BYTE* pvBits;
					HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pvBits, NULL, 0);

					ZeroMemory(pvBits, bmi.bmiHeader.biSizeImage);
					MakeShadow((UINT32*)pvBits, hParent, &WndRect);

					HDC hMemDC = CreateCompatibleDC(NULL);
					HBITMAP hOriBmp = (HBITMAP)SelectObject(hMemDC, hbitmap);

					POINT ptDst = { WndRect.left + gnxOffset - gnSize, WndRect.top + gnyOffset - gnSize };
					POINT ptSrc = { 0, 0 };
					SIZE WndSize = { nShadWndWid, nShadWndHei };
					BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

					MoveWindow(ghWnd, ptDst.x, ptDst.y, nShadWndWid, nShadWndHei, FALSE);

					BOOL bRet = UpdateLayeredWindow(ghWnd, NULL, &ptDst, &WndSize, hMemDC,
						&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

					SelectObject(hMemDC, hOriBmp);
					DeleteObject(hbitmap);
					DeleteDC(hMemDC);

				}
			protected:

				void MakeShadow(UINT* pShadBits, HWND hParent, RECT* rcParent);
				inline DWORD PreMultiply(COLORREF cl, unsigned char nAlpha)
				{
					return (GetRValue(cl) * (DWORD)nAlpha / 255) << 16 |
						(GetGValue(cl) * (DWORD)nAlpha / 255) << 8 |
						(GetBValue(cl) * (DWORD)nAlpha / 255);
				}
				void Show(HWND hParent);

				static std::unordered_map<HWND, WNDPROC> originalWndProc;
				static std::unordered_map<HWND, HWND> ShadowWnd;
				static LRESULT CALLBACK SWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

					if (message == 0xff5)
					{
						DestroyWindow(hWnd);
					}

					return DefWindowProc(hWnd, message, wParam, lParam);
				}

				static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

					if (message == WM_MOVE)
					{
						// 仅移动位置
						if (ShadowWnd.find(hWnd) != ShadowWnd.end()) {
							RECT rc;
							GetWindowRect(hWnd, &rc);
							RECT crc;
							GetClientRect(hWnd, &crc);
							RECT src;
							GetClientRect(ShadowWnd[hWnd], &src);
							int n = src.right - crc.right;
							SetWindowPos(ShadowWnd[hWnd], 0, rc.left - n / 2, rc.top - n / 2, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
						}
						// 必须调用原始过程，否则父窗口可能不知道自己被移动了
						return CallWindowProc(originalWndProc[hWnd], hWnd, message, wParam, lParam);
					}
					if (message == WM_SIZE)
					{
						// 修复：当大小改变时，必须调用 Update 来重绘阴影位图，而不仅仅是 SetWindowPos
						if (s_Shadowmap.find(hWnd) != s_Shadowmap.end()) {
							s_Shadowmap[hWnd]->Update(hWnd);
						}
						// 必须调用原始过程，否则父窗口不会进行布局
						return CallWindowProc(originalWndProc[hWnd], hWnd, message, wParam, lParam);
					}
					if (message == WM_DESTROY || message == WM_CLOSE || message == WM_QUIT)
					{
						if (ShadowWnd.find(hWnd) != ShadowWnd.end()) {
							ShowWindow(ShadowWnd[hWnd], SW_HIDE);
							PostMessage(ShadowWnd[hWnd], 0xff5, 0, 0);
						}

						// 修复：在窗口销毁时，必须还原窗口过程，否则后续消息或重复创建会导致 Crash
						if (message == WM_DESTROY && originalWndProc.find(hWnd) != originalWndProc.end())
						{
							WNDPROC oldProc = originalWndProc[hWnd];
							SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)oldProc);

							// 清理 Map，防止野指针
							originalWndProc.erase(hWnd);
							ShadowWnd.erase(hWnd);
							s_Shadowmap.erase(hWnd);

							return CallWindowProc(oldProc, hWnd, message, wParam, lParam);
						}
					}

					if (originalWndProc.find(hWnd) != originalWndProc.end())
						return CallWindowProc(originalWndProc[hWnd], hWnd, message, wParam, lParam);

					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			};
			std::unordered_map<HWND, WNDPROC> DropShadow::originalWndProc;

			HINSTANCE DropShadow::s_hInstance = (HINSTANCE)INVALID_HANDLE_VALUE;
			std::map<HWND, DropShadow*> DropShadow::s_Shadowmap;
			std::unordered_map<HWND, HWND> DropShadow::ShadowWnd;
			DropShadow::DropShadow()
				: ghWnd((HWND)INVALID_HANDLE_VALUE)
				, gOriParentProc(NULL)
				, gnDarkness(150)
				, gnSharpness(5)
				, gnSize(50)
				, gnxOffset(5)
				, gnyOffset(5)
				, gColor(RGB(0, 0, 0))
				, gWndSize(0)
				, gbUpdate(false)
			{
			}

			DropShadow::~DropShadow()
			{
			}


			void DropShadow::Create(HWND hParentWnd)
			{
				ghWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, ShadowClassName, NULL, WS_POPUPWINDOW, CW_USEDEFAULT, 0, 0, 0, hParentWnd, NULL, s_hInstance, NULL);
				gStatus = SS_ENABLED;
				m_hWnd = ghWnd;

				// 修复：注册实例到 Map，以便 WndProc 中 WM_SIZE 可以调用 Update
				s_Shadowmap[hParentWnd] = this;
				ShadowWnd[hParentWnd] = ghWnd;

				Show(hParentWnd);

				// 修复：防止重复子类化导致无限递归或指针覆盖
				if (originalWndProc.find(hParentWnd) == originalWndProc.end())
				{
					LONG_PTR lp = SetWindowLongPtr(hParentWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
					originalWndProc[hParentWnd] = (WNDPROC)lp;
				}
			}



			//算法部分
			void DropShadow::MakeShadow(UINT* pShadBits, HWND hWnd, RECT* rcParent)
			{
				// The shadow algorithm:
				// Apply modified (with blur effect) morphologic dilation to make the blurred border
				HRGN hWndRgn = CreateRectRgn(0, 0, rcParent->right - rcParent->left, rcParent->bottom - rcParent->top);
				if (this->nRgn == NULL)GetWindowRgn(hWnd, hWndRgn);
				else hWndRgn = this->nRgn;

				// Deterwin_mine the Start and end point of each horizontal scan line
				SIZE szParent = { rcParent->right - rcParent->left, rcParent->bottom - rcParent->top };
				SIZE szShadow = { szParent.cx + 2 * gnSize, szParent.cy + 2 * gnSize };
				// Extra 2 lines (set to be empty) in ptAnchors are used in dilation
				int nAnchors = win_max(szParent.cy, szShadow.cy);	// # of anchor points pares
				int(*ptAnchors)[2] = new int[nAnchors + 2][2];
				int(*ptAnchorsOri)[2] = new int[szParent.cy][2];	// anchor points, will not modify during erosion
				ptAnchors[0][0] = szParent.cx;
				ptAnchors[0][1] = 0;
				ptAnchors[nAnchors + 1][0] = szParent.cx;
				ptAnchors[nAnchors + 1][1] = 0;
				if (gnSize > 0)
				{
					// Put the parent window anchors at the center
					for (int i = 0; i < gnSize; i++)
					{
						ptAnchors[i + 1][0] = szParent.cx;
						ptAnchors[i + 1][1] = 0;
						ptAnchors[szShadow.cy - i][0] = szParent.cx;
						ptAnchors[szShadow.cy - i][1] = 0;
					}
					ptAnchors += gnSize;
				}
				for (int i = 0; i < szParent.cy; i++)
				{
					// find start point
					int j;
					for (j = 0; j < szParent.cx; j++)
					{
						if (PtInRegion(hWndRgn, j, i))
						{
							ptAnchors[i + 1][0] = j + gnSize;
							ptAnchorsOri[i][0] = j;
							break;
						}
					}

					if (j >= szParent.cx)	// Start point not found
					{
						ptAnchors[i + 1][0] = szParent.cx;
						ptAnchorsOri[i][1] = 0;
						ptAnchors[i + 1][0] = szParent.cx;
						ptAnchorsOri[i][1] = 0;
					}
					else
					{
						// find end point
						for (j = szParent.cx - 1; j >= ptAnchors[i + 1][0]; j--)
						{
							if (PtInRegion(hWndRgn, j, i))
							{
								ptAnchors[i + 1][1] = j + 1 + gnSize;
								ptAnchorsOri[i][1] = j + 1;
								break;
							}
						}
					}
					// 		if(0 != ptAnchorsOri[i][1])
					// 			_RPTF2(_CRT_WARN, "%d %d\n", ptAnchorsOri[i][0], ptAnchorsOri[i][1]);
				}

				if (gnSize > 0)
					ptAnchors -= gnSize;	// Restore pos of ptAnchors for erosion
				int(*ptAnchorsTmp)[2] = new int[nAnchors + 2][2];	// Store the result of erosion
				// First and last line should be empty
				ptAnchorsTmp[0][0] = szParent.cx;
				ptAnchorsTmp[0][1] = 0;
				ptAnchorsTmp[nAnchors + 1][0] = szParent.cx;
				ptAnchorsTmp[nAnchors + 1][1] = 0;
				int nEroTimes = 0;
				// morphologic erosion
				for (int i = 0; i < gnSharpness - gnSize; i++)
				{
					nEroTimes++;
					//ptAnchorsTmp[1][0] = szParent.cx;
					//ptAnchorsTmp[1][1] = 0;
					//ptAnchorsTmp[szParent.cy + 1][0] = szParent.cx;
					//ptAnchorsTmp[szParent.cy + 1][1] = 0;
					for (int j = 1; j < nAnchors + 1; j++)
					{
						ptAnchorsTmp[j][0] = win_max(ptAnchors[j - 1][0], win_max(ptAnchors[j][0], ptAnchors[j + 1][0])) + 1;
						ptAnchorsTmp[j][1] = win_min(ptAnchors[j - 1][1], win_min(ptAnchors[j][1], ptAnchors[j + 1][1])) - 1;
					}
					// Exchange ptAnchors and ptAnchorsTmp;
					int(*ptAnchorsXange)[2] = ptAnchorsTmp;
					ptAnchorsTmp = ptAnchors;
					ptAnchors = ptAnchorsXange;
				}

				// morphologic dilation
				ptAnchors += (gnSize < 0 ? -gnSize : 0) + 1;	// now coordinates in ptAnchors are same as in shadow window
				// Generate the kernel
				int nKernelSize = gnSize > gnSharpness ? gnSize : gnSharpness;
				int nCenterSize = gnSize > gnSharpness ? (gnSize - gnSharpness) : 0;
				UINT32* pKernel = new UINT32[(2 * nKernelSize + 1) * (2 * nKernelSize + 1)];
				UINT32* pKernelIter = pKernel;
				for (int i = 0; i <= 2 * nKernelSize; i++)
				{
					for (int j = 0; j <= 2 * nKernelSize; j++)
					{
						double dLength = sqrt((i - nKernelSize) * (i - nKernelSize) + (j - nKernelSize) * (double)(j - nKernelSize));
						if (dLength < nCenterSize)
							*pKernelIter = gnDarkness << 24 | PreMultiply(gColor, gnDarkness);
						else if (dLength <= nKernelSize)
						{
							UINT32 nFactor = ((UINT32)((1 - (dLength - nCenterSize) / (gnSharpness + 1)) * gnDarkness));
							*pKernelIter = nFactor << 24 | PreMultiply(gColor, nFactor);
						}
						else
							*pKernelIter = 0;
						//TRACE("%d ", *pKernelIter >> 24);
						pKernelIter++;
					}
					//TRACE("\n");
				}
				// Generate blurred border
				for (int i = nKernelSize; i < szShadow.cy - nKernelSize; i++)
				{
					int j;
					if (ptAnchors[i][0] < ptAnchors[i][1])
					{

						// Start of line
						for (j = ptAnchors[i][0];
							j < win_min(win_max(ptAnchors[i - 1][0], ptAnchors[i + 1][0]) + 1, ptAnchors[i][1]);
							j++)
						{
							for (int k = 0; k <= 2 * nKernelSize; k++)
							{
								UINT32* pPixel = pShadBits +
									(szShadow.cy - i - 1 + nKernelSize - k) * szShadow.cx + j - nKernelSize;
								UINT32* pKernelPixel = pKernel + k * (2 * nKernelSize + 1);
								for (int l = 0; l <= 2 * nKernelSize; l++)
								{
									if (*pPixel < *pKernelPixel)
										*pPixel = *pKernelPixel;
									pPixel++;
									pKernelPixel++;
								}
							}
						}	// for() start of line

						// End of line
						for (j = win_max(j, win_min(ptAnchors[i - 1][1], ptAnchors[i + 1][1]) - 1);
							j < ptAnchors[i][1];
							j++)
						{
							for (int k = 0; k <= 2 * nKernelSize; k++)
							{
								UINT32* pPixel = pShadBits +
									(szShadow.cy - i - 1 + nKernelSize - k) * szShadow.cx + j - nKernelSize;
								UINT32* pKernelPixel = pKernel + k * (2 * nKernelSize + 1);
								for (int l = 0; l <= 2 * nKernelSize; l++)
								{
									if (*pPixel < *pKernelPixel)
										*pPixel = *pKernelPixel;
									pPixel++;
									pKernelPixel++;
								}
							}
						}	// for() end of line

					}
				}	// for() Generate blurred border

				// Erase unwanted parts and complement missing
				UINT32 clCenter = gnDarkness << 24 | PreMultiply(gColor, gnDarkness);
				for (int i = win_min(nKernelSize, win_max(gnSize - gnyOffset, 0));
					i < win_max(szShadow.cy - nKernelSize, win_min(szParent.cy + gnSize - gnyOffset, szParent.cy + 2 * gnSize));
					i++)
				{
					UINT32* pLine = pShadBits + (szShadow.cy - i - 1) * szShadow.cx;
					if (i - gnSize + gnyOffset < 0 || i - gnSize + gnyOffset >= szParent.cy)	// Line is not covered by parent window
					{
						for (int j = ptAnchors[i][0]; j < ptAnchors[i][1]; j++)
						{
							*(pLine + j) = clCenter;
						}
					}
					else
					{
						for (int j = ptAnchors[i][0];
							j < win_min(ptAnchorsOri[i - gnSize + gnyOffset][0] + gnSize - gnxOffset, ptAnchors[i][1]);
							j++)
							*(pLine + j) = clCenter;
						for (int j = win_max(ptAnchorsOri[i - gnSize + gnyOffset][0] + gnSize - gnxOffset, 0);
							j < win_min(ptAnchorsOri[i - gnSize + gnyOffset][1] + gnSize - gnxOffset, szShadow.cx);
							j++)
							*(pLine + j) = 0;
						for (int j = win_max(ptAnchorsOri[i - gnSize + gnyOffset][1] + gnSize - gnxOffset, ptAnchors[i][0]);
							j < ptAnchors[i][1];
							j++)
							*(pLine + j) = clCenter;
					}
				}

				// Delete used resources
				delete[](ptAnchors - (gnSize < 0 ? -gnSize : 0) - 1);
				delete[] ptAnchorsTmp;
				delete[] ptAnchorsOri;
				delete[] pKernel;
				DeleteObject(hWndRgn);
			}

			bool DropShadow::SetSize(int NewSize)
			{
				if (NewSize > 40 || NewSize < -20)
					return false;

				gnSize = (signed char)NewSize;
				if (SS_VISABLE & gStatus)
					Update(GetParent(ghWnd));
				return true;
			}

			bool DropShadow::SetSharpness(unsigned int NewSharpness)
			{
				if (NewSharpness > 80)
					return false;

				gnSharpness = (unsigned char)NewSharpness;
				if (SS_VISABLE & gStatus)
					Update(GetParent(ghWnd));
				return true;
			}

			bool DropShadow::SetDarkness(unsigned int NewDarkness)
			{
				if (NewDarkness > 255)
					return false;

				gnDarkness = (unsigned char)NewDarkness;
				if (SS_VISABLE & gStatus)
					Update(GetParent(ghWnd));
				return true;
			}

			bool DropShadow::SetPosition(int NewXOffset, int NewYOffset)
			{
				if (NewXOffset > 20 || NewXOffset < -20 ||
					NewYOffset > 20 || NewYOffset < -20)
					return false;

				gnxOffset = (signed char)NewXOffset;
				gnyOffset = (signed char)NewYOffset;
				if (SS_VISABLE & gStatus)
					Update(GetParent(ghWnd));
				return true;
			}

			bool DropShadow::SetColor(COLORREF NewColor)
			{
				gColor = NewColor;
				if (SS_VISABLE & gStatus)
					Update(GetParent(ghWnd));
				return true;
			}

			void DropShadow::Show(HWND hParentWnd)
			{
				gStatus &= SS_ENABLED;

				if ((gStatus & SS_ENABLED))
				{
					LONG lParentStyle = GetWindowLong(hParentWnd, GWL_STYLE);

					if (WS_VISIBLE & lParentStyle)
					{
						gStatus |= SS_PARENTVISIBLE;

						if (!((WS_MAXIMIZE | WS_MINIMIZE) & lParentStyle))
							gStatus |= SS_VISABLE;
					}
				}

				if (gStatus & SS_VISABLE)
				{
					ShowWindow(ghWnd, SW_SHOWNA);
					Update(hParentWnd);
				}
				else
					ShowWindow(ghWnd, SW_HIDE);

			}
		}
		/*
		namespace Slider
		{

			HWND hSliderWnd = 0;
			WNDPROC hSliderProc = 0;
			HINSTANCE hSliderInst = 0;
			int barVal = 285;
			int ReturnValue;
			int _PrevReturnValue=50;
			using namespace::VertexUI::Click;

			int hSliderMode = 0;

			int hSliderIsClosed = 0;
			const wchar_t* SliderReturnValue;


			int gVal;
			class TestControl : public VertexUIControl
			{
			public:
				void Set(int x, int y, int cx, int cy, int type)
				{
					this->x = x;
					this->y = y;
					this->cx = cx;
					this->cy = cy;
					this->type = type;
				}
				virtual void CreateCtl(HWND hWnd, HDC hdc)
				{
					unsigned long color = VERTEXUICOLOR_LAVENDER;
					VertexUIColorInfo ci = { VERTEXUICOLOR_MIDNIGHTPLUS };
					if (mode == 1)
					{
						color = VERTEXUICOLOR_BLOOMLAVENDER;
						DrawRoundRect(hdc, (int)(x + value * 3 - (cy * 1.5) / 2), int(y - cy / 3.5), int(cy * 1.5)-4, int(cy * 1.5)-4, cy * 2, VERTEXUICOLOR_LAVENDER);
						AreaBlur(hdc, { x-30,y-20,cx+60,cy+40 }, 3, 2, 0);

						wchar_t ValueChar[20];
						_itow(value, ValueChar, 10);
						TextPreDraw(hdc, x + value * 3 - (cy * 3) / 2, y - 35, cy * 3, 16, ValueChar, VERTEXUICOLOR_WHITE);
					}
					vcdb.RoundButton(hWnd, hdc, { int(x-cy/1.5),y,int(cx+(cy/1.5)*2),cy }, 12, L"", ci);
					vcdb.RoundButton(hWnd, hdc, { int(x-cy/1.5),y,int(value*3+(cy/1.5)*2),cy }, 12, L"", {color});
					vcdb.RoundButton(hWnd, hdc, { (int)(x+value*3-(cy*1.5)/2),int(y-cy/3.5),int(cy*1.5),int(cy*1.5) }, cy*2, L"", { VERTEXUICOLOR_BLOOMLAVENDER });
				}
				virtual int AddEvent()
				{
					value += gVal;
					if (value > 100)value = 100;
					if (value < 0)value = 0;
					if (GetPtInfo(hWnd, x, y-10, cx+1, cy+20, RECT{ 0 }) == 1)
					{
						if (ClickMsg == 1)
						{
							ClickMsg = 0;
						}
						if (KeepDownMsg == 1)
						{
							POINT pt;
							GetCursorPos(&pt);
							ScreenToClient(hWnd, &pt);
							value = (pt.x - 40)/3;
							RefreshWindow(hWnd);
						}
						if (hState == 0)
						{
							hState = 1;
							mode = 1;
							RefreshWindow(hWnd);
						}

						return 0;
					}
					mode = 0;
					return -1;
				}
				virtual void CreateInheritedCtl(HWND hWnd, HDC hdc, TestControl* vuic)
				{
					this->hWnd = hWnd;
					this->CreateCtl(hWnd, hdc);
				}
				int value;
			protected:
				HWND hWnd;
				int x, y, cx, cy;
				int type;
				int mode = 0;
			};

			VertexUIPanel Panel1;

			INT_PTR CALLBACK CreateManifestWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				static TestControl* Ctl = new TestControl;
				RECT rc;

				GetClientRect(hWnd, &rc);
				UNREFERENCED_PARAMETER(lParam);
				switch (uMsg)
				{
				case WM_CREATE:
				{
					//Alpha = 248; AlphaSet;
					OutputDebugString(L"Mask Created");
					HRGN hRgn = CreateRoundRectRgn(0, 0, rc.right - rc.left + 1, rc.bottom - rc.top + 1, 4, 4);
					SetWindowRgn(hWnd, hRgn, TRUE);
					Ctl->value = ReturnValue;
					return TRUE;
				}
				case WM_PAINT:
				{
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hWnd, &ps);

					//InvalidateRect(h, &rc, 1);
					CreatePanel(hWnd, hdc, [](HWND hWnd, HDC hdc)->void {

#pragma region BaseDraw
					RECT rc;

					GetClientRect(hWnd, &rc);
					CreateFillArea(hWnd, hdc, VERTEXUICOLOR_MIDNIGHT);
					PanelDrawCloseBtn(hWnd, hdc, rc.right - 34, 6, 25, 25, 4, RGB(187, 192, 201));


#pragma endregion BaseDraw

					Panel1.Set(hWnd, hdc);


					VertexUIColorInfo vci;
					vci.BkColor = VERTEXUICOLOR_LAVENDER;
					vci.Corner = 12;
					vci.TxtColor = VERTEXUICOLOR_WHITEDRAW;
					vci.HoverColor = VERTEXUICOLOR_BLOOMLAVENDER;
					VertexUIControl Confirm;
					Confirm.CreateCtl(L"Button2", rc.right /2-60, rc.bottom - 60, 120, 40, [hWnd] {		DestroyWindow(hWnd);
					PostQuitMessage(0); return 0; }, L"确定", vci);
					Panel1.Add(Confirm);

					

					Ctl->Set(40, 90, 300, 20, 0);
					Panel1.Add(Ctl);

					ReturnValue = Ctl->value;


					});

					EndPaint(hWnd, &ps);
					break;
				}
				case WM_LBUTTONDOWN:
				{
					int val = 0;
					ClickAreaPtInfo(hWnd, 0, 0, rc.right - 35, 40, lParam, val);
					if (val == 1)
					{
						SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
					}
					else
					{
						SendKeepDownEvent(hWnd, wParam, lParam);
					}
					break;
				}
				case WM_LBUTTONUP:
				{
					SendClickEvent(hWnd, wParam, lParam);
					break;
				}
				case WM_MOUSEMOVE:
				{
					if (GetPtInfo(hWnd, rc.right - 40, 6, 35, 35))
					{
						if (ClickMsg == 1)
						{

							ClickMsg=0;
							ReturnValue = _PrevReturnValue;
							DestroyWindow(hWnd);
							PostQuitMessage(0);
							return 0;
						}
					}
					add_event(Panel1);
					if (hState == 1)
					{
						hState = 0;
						InvalidateRect(hWnd, &rc, 0);
						return 0;
					}
					//hSliderWindowAreaEvent(hWnd, lParam);
					break;
				}
				case WM_MOUSEWHEEL:
				{
					short stat = HIWORD(wParam);
					if (stat > 0)
					{
						gVal+=2;
					}
					if (stat < 0)
					{
						gVal -=2;
					}
					SendMessage(hWnd, WM_MOUSEMOVE, wParam, lParam);
					RefreshDisplay(hWnd);
					gVal = 0;
					break;
				}
				case WM_ERASEBKGND:
				{
					break;
				}
				case WM_SIZE:
				{

					InvalidateRect(hWnd, &rc, 0);
					break;
				}
				case WM_COMMAND:
				{
					switch (LOWORD(wParam))
					{
					}
					break;
				}
				}
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			//
			const wchar_t* SliderszWindowClass = L"LightFrame.Slider.Class";            // 主窗口类名

			ATOM SliderBoxRegisterClass(HINSTANCE hInstance)
			{
				WNDCLASSEXW wcex;

				wcex.cbSize = sizeof(WNDCLASSEX);

				wcex.style = CS_HREDRAW | CS_VREDRAW;
				wcex.lpfnWndProc = (WNDPROC)CreateManifestWindowProc;
				wcex.cbClsExtra = 0;
				wcex.cbWndExtra = 0;
				wcex.hInstance = hInstance;
				wcex.hIcon = LoadIcon(hInstance, NULL);
				wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
				wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
				//wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(47, 52, 61));
				wcex.lpszMenuName = 0;
				wcex.lpszClassName = SliderszWindowClass;
				wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

				return RegisterClassExW(&wcex);
			}
			int CreateSliderBox(
				HWND hWnd,int val, HWND mh = 0, int hmode = 0      // handle 
			)
			{
				_PrevReturnValue = val;
				ReturnValue = val;
				EnableWindow(hWnd, FALSE);
				RECT rc;
				GetWindowRect(hWnd, &rc);
				RECT crc;
				GetClientRect(hWnd, &crc);
				SliderBoxRegisterClass(hSliderInst);
				hSliderWnd = CreateWindow(SliderszWindowClass, L"LightFrame_hSlider_legacy", WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,//TipBox
					rc.left, rc.top + 1, 380, 230, mh, 0, hSliderInst, NULL);
				SendMessage(hSliderWnd, WM_CREATE, 0, 0);
				if (mh == 0)CenterWindow(hSliderWnd);
				hSliderMode = hmode;
				SetWindowPos(hSliderWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				LONG_PTR Style = ::GetWindowLongPtr(hSliderWnd, GWL_STYLE);
				Style = Style & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX;
				::SetWindowLongPtr(hSliderWnd, GWL_STYLE, Style);
				OuterShadow::DropShadow Shadow;
				Shadow.Initialize(0);
				Shadow.SetSharpness(60);
				Shadow.SetSize(25);
				Shadow.SetDarkness(160);
				Shadow.SetPosition(0, 0);
				Shadow.Create(hSliderWnd);
				MSG msg;
				while (GetMessage(&msg, NULL, 0, 0)) {

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				EnableWindow(hWnd, TRUE);
				SetForegroundWindow(hWnd);
				//MoveInMenuNew(hMaskWnd);
				return 0;
			}
		}
		namespace TimeSelector
		{
			unsigned long gtime;
			short gVal = 0;
			HWND hTimeSelectorWnd = 0;
			HINSTANCE hTimeSelectorInst = 0;
			class TimeBlock : public VertexUIControl
			{
			public:
				void Set(int x, int y, int cx, int cy, int type)
				{
					this->x = x;
					this->y = y;
					this->cx = cx;
					this->cy = cy;
					this->type = type;
				}
				virtual void CreateCtl(HWND hWnd, HDC hdc)
				{

					if (value >= 60)value = 0;
					if (value < 0)value = 59;
					wchar_t tiStr[16];
					wsprintf(tiStr, L"%02d", value);
					VertexUIColorInfo ci = { VERTEXUICOLOR_MIDNIGHTPLUS,VERTEXUICOLOR_GREENSEA };
					ci.TxtSize = 30;
					vcdb.RoundButton(hWnd, hdc, { x,y,cx,cy }, 12, tiStr, ci);
					const wchar_t* Str = L"Unknown";
					if (type == 0)Str = L"秒";
					if (type == 1)Str = L"分钟";
					if (type == 2)Str = L"小时";
					TextPreDrawEx(hdc, x, y + cy - 40, cx, 40, Str, 14, 1, VERTEXUICOLOR_WHITE);
				}
				virtual int AddEvent()
				{
					if (GetPtInfo(hWnd, x, y, cx, cy, RECT{ 0 }) == 1)
					{
						value += gVal;
					}
					return -1;
				}
				virtual void CreateInheritedCtl(HWND hWnd, HDC hdc, TimeBlock* vuic)
				{
					this->hWnd = hWnd;
					CreateCtl(hWnd, hdc);
				}
				int value;
			protected:
				HWND hWnd;
				int x, y, cx, cy;
				int type;
			};
			VertexUIPanel Panel;
			int TimeSelectorValue;
			int barVal = 285;
			int ReturnValue;
			using namespace::VertexUI::Click;


			int hTimeSelectorMode = 0;

			int hTimeSelectorIsClosed = 0;
			const wchar_t* TimeSelectorReturnValue;
			int hTimeSelectorWindowAreaEvent(HWND hWnd, LPARAM lParam)
			{

				int val = 0;

				int msg = 0;

				UINT pindex = 0;
				RECT winrc;
				GetClientRect(hWnd, &winrc);
				RECT trc;
				GetWindowRect(hWnd, &trc);
				RECT rc = {};
				add_event(Panel);
				else
				{
				if (hState == 1)
				{
					hState = 0;
					InvalidateRect(hWnd, &winrc, 0);
					return 0;
				}
				}



				return 0;
			}
			TimeBlock* hrs = new TimeBlock;
			TimeBlock* win_min = new TimeBlock;
			TimeBlock* sec = new TimeBlock;
			INT_PTR CALLBACK CreateManifestWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				wchar_t txt[128];
				RECT rc;
				GetClientRect(hWnd, &rc);
				UNREFERENCED_PARAMETER(lParam);

				switch (uMsg)
				{
				case WM_CREATE:
				{
					//Alpha = 248; AlphaSet;
					OutputDebugString(L"Mask Created");
					HRGN hRgn = CreateRoundRectRgn(0, 0, rc.right - rc.left + 1, rc.bottom - rc.top + 1, 4, 4);
					SetWindowRgn(hWnd, hRgn, TRUE);
					return TRUE;
				}
				case WM_PAINT:
				{
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hWnd, &ps);
					HDC         hMemDC;
					HBITMAP     hBmpMem;
					HBITMAP     hPreBmp;
					ReturnValue = barVal / 3;
					//InvalidateRect(h, &rc, 1);
					hMemDC = CreateCompatibleDC(hdc);

					hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

					hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
					//On hMemDC.
					//-------------------------------------------
					CreateFillArea(hWnd, hMemDC, VERTEXUICOLOR_MIDNIGHT);
					//PanelDrawCloseBtn(hWnd, hMemDC, rc.right - 34, 6, 25, 25, 4, RGB(187, 192, 201));
					Panel.Set(hWnd, hMemDC);
					hrs->Set(31, 30, 100, 140, 2);
					win_min->Set(31 + (100 + 20) * 1, 30, 100, 140, 1);
					sec->Set(31 + (100 + 20) * 2, 30, 100, 140, 0);
					Panel.Add(hrs);
					Panel.Add(win_min);
					Panel.Add(sec);
					VertexUIControl ok;
					VertexUIColorInfo ci;
					ci.BkColor = VERTEXUICOLOR_MIDNIGHTPLUS;
					ci.HoverColor = VERTEXUICOLOR_BLOOMLAVENDER;
					ci.TxtSize = 16;
					ok.CreateCtl(L"Button2", 271, rc.bottom - 50, 100, 40, [hWnd] {
						int val;
						val = hrs->value * 60 * 60 + win_min->value * 60 + sec->value;
						gtime = val;
						FadeOutAnimation(hWnd);
						DestroyWindow(hWnd);
						PostQuitMessage(0);
						}, L"确定", ci);
					Panel.Add(ok);
					//-------------------------------------------
					BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);

					SelectObject(hMemDC, hPreBmp);


					DeleteObject(hBmpMem);

					DeleteDC(hMemDC);
					EndPaint(hWnd, &ps);
					break;
				}
				case WM_LBUTTONDOWN:
				{
					int val = 0;
					ClickAreaPtInfo(hWnd, 0, 0, rc.right - 35, 40, lParam, val);
					if (val == 1)
					{
						SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
					}
					if (GetAreaPtInfo(hWnd, 40, 65, 302, 50, RECT{ 0 }, lParam))
					{
						SendKeepDownEvent(hWnd, wParam, lParam);
					}
					break;
				}
				case WM_LBUTTONUP:
				{
					SendClickEvent(hWnd, wParam, lParam);
					break;
				}
				case WM_MOUSEMOVE:
				{
					hTimeSelectorWindowAreaEvent(hWnd, lParam);
					break;
				}
				case WM_MOUSEWHEEL:
				{
					short stat = HIWORD(wParam);
					if (stat > 0)
					{
						gVal = 1;
					}
					if (stat < 0)
					{
						gVal = -1;
					}
					SendMessage(hWnd, WM_MOUSEMOVE, wParam, lParam);
					RefreshDisplay(hWnd);
					gVal = 0;
					break;
				}
				case WM_ERASEBKGND:
				{
					break;
				}
				case WM_SIZE:
				{

					InvalidateRect(hWnd, &rc, 0);
					break;
				}
				case WM_COMMAND:
				{
					switch (LOWORD(wParam))
					{
					}
					break;
				}
				//--消息处理不知道放这里吗?
				}
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			//
			const wchar_t* TimeSelectorszWindowClass = L"LightFrame.TimeSelector.Class";            // 主窗口类名

			ATOM TimeSelectorBoxRegisterClass(HINSTANCE hInstance)
			{
				WNDCLASSEXW wcex;

				wcex.cbSize = sizeof(WNDCLASSEX);

				wcex.style = CS_HREDRAW | CS_VREDRAW;
				wcex.lpfnWndProc = (WNDPROC)CreateManifestWindowProc;
				wcex.cbClsExtra = 0;
				wcex.cbWndExtra = 0;
				wcex.hInstance = hInstance;
				wcex.hIcon = LoadIcon(hInstance, NULL);
				wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
				wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
				//wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(47, 52, 61));
				wcex.lpszMenuName = 0;
				wcex.lpszClassName = TimeSelectorszWindowClass;
				wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

				return RegisterClassExW(&wcex);
			}
			int CreateTimeSelectorBox(
				unsigned long& time, HWND hWnd = 0, HWND mh = 0, int hmode = 0      // handle 
			)
			{
				ClickMsg = 0;
				time = gtime;
				EnableWindow(hWnd, FALSE);
				RECT rc;
				GetWindowRect(hWnd, &rc);
				RECT crc;
				GetClientRect(hWnd, &crc);
				TimeSelectorBoxRegisterClass(hTimeSelectorInst);
				hTimeSelectorWnd = CreateWindow(TimeSelectorszWindowClass, L"LightFrame_hTimeSelector_legacy", WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,//TipBox
					rc.left, rc.top + 1, 400, 230, mh, 0, hTimeSelectorInst, NULL);
				CenterWindow(hTimeSelectorWnd);
				SendMessage(hTimeSelectorWnd, WM_CREATE, 0, 0);
				hTimeSelectorMode = hmode;
				SetWindowPos(hTimeSelectorWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				LONG_PTR Style = ::GetWindowLongPtr(hTimeSelectorWnd, GWL_STYLE);
				Style = Style & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX;
				::SetWindowLongPtr(hTimeSelectorWnd, GWL_STYLE, Style);

				OuterShadow::DropShadow Shadow;
				Shadow.Initialize(0);
				Shadow.SetSharpness(60);
				Shadow.SetSize(25);
				Shadow.SetDarkness(160);
				Shadow.SetPosition(0, 0);
				Shadow.Create(hTimeSelectorWnd);
				if (hWnd != 0)
				{
					MSG msg;
					while (GetMessage(&msg, NULL, 0, 0)) {

						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				time = gtime;
				EnableWindow(hWnd, TRUE);
				SetForegroundWindow(hWnd);
				//MoveInMenuNew(hMaskWnd);
				return 0;
			}
		}
		namespace iContainer
		{
			enum eTrackMode
			{
				Auto,
				Always,
				MoveOver,
				Never
			};
			class ICONTAINER
			{
			public:
				int iHeight = 0;
				int iWidth = 0;
				int xPosmove = 0;
				wchar_t* TopTxt = L"";
				std::vector<const wchar_t*> listtxt = { 128,(L"") };
				std::vector<std::function<void()>> listclick = { 128,(in_test) };
				COLORREF PanelBkColor = VERTEXUICOLOR_MIDNIGHT;
				int TrackMode = Auto;
				int TrackDistance = 60;
				void AddOption(int id, const wchar_t* name, std::function<void()>ev)
				{
					this->listtxt[id] = name;
					this->listclick[id] = ev;
				}
			private:

			};
			ICONTAINER ic;
			int Exist = 0;

			HWND hiContainerWnd = 0;
			WNDPROC hiContainerProc = 0;
			HINSTANCE hiContainerInst = 0;

			using namespace::VertexUI::Click;
			VERTEXUICTL VUIW_iContainer_OKButton;


			int hiContainerMode = 0;
			typedef struct taghiContainerWW
			{// 
				HWND hWnd11w;          // handle 

				LPCTSTR lpText11w;     // text 

				LPCTSTR lpCaption11w;  //  title 

				UINT uType11w;         // style 

			} iContainer_PARAM1w;
			int hiContainerIsClosed = 0;
			const wchar_t* iContainerReturnValue;
			int hiContainerWindowAreaEvent(HWND hWnd, LPARAM lParam)
			{

				int val = 0;

				int msg = 0;

				UINT pindex = 0;
				RECT winrc;
				GetClientRect(hWnd, &winrc);
				RECT trc;
				GetWindowRect(hWnd, &trc);
				RECT rc = {};

				for (int i = 1; i <= 128; i++)
				{
					if (ic.listtxt[i] == L"")
					{
						break;
					}
					int y = i * 36;
					if (ic.TopTxt == L"")y -= 32;

					if ((GetAreaPtInfo(hWnd, 6, y, winrc.right - winrc.left - 10, 30, rc, lParam)) == 1)
					{
						if (ClickMsg == 1)
						{
							ClickMsg = 0;
							ic.listclick[i]();
							DestroyWindow(hWnd);
						}
						if (hState == 0)
						{
							HDC hdc = GetDC(hWnd);
							if (ic.listtxt[2] != std::wstring(L""))vcdb.RoundButton(hWnd, hdc, { 6, y + 2, (rc.right - rc.left) ,36 }, 4, L"", { VERTEXUICOLOR_BLOOMLAVENDER });
							TextPreDrawEx(hdc, 12, y + 2, rc.right - rc.left, 34, ic.listtxt[i], 17, 0, VERTEXUICOLOR_WHITE);
							DeleteObject(hdc);
							ReleaseDC(hWnd, hdc);
							DeleteDC(hdc);
							hState = 1;
						}
						return 0;
					}
				}
				if (hState == 1)
				{
					hState = 0;
					InvalidateRect(hWnd, &winrc, 0);
					return 0;
				}



				return 0;
			}
			clock_t time;
			LRESULT CALLBACK CreateManifestWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{

				RECT rc;

				GetClientRect(hWnd, &rc);
				UNREFERENCED_PARAMETER(lParam);
				static BOOL fActive;
				static int inarea = 1;
				switch (uMsg)
				{

				case WM_CREATE:
				{
					Alpha = 0; AlphaSet;
					OutputDebugString(L"Mask Created");
					HRGN hRgn = CreateRoundRectRgn(0, 0, rc.right - rc.left + 1, rc.bottom - rc.top + 1, 3, 3);
					SetWindowRgn(hWnd, hRgn, TRUE);
					if (ic.TrackMode == Always || ic.TrackMode == MoveOver)
					{
						SetTimer(hWnd, 1, 10, 0);
						if (ic.TrackMode == MoveOver)
						{
							FadeInAnimation(hWnd);
							time = clock();
						}
					}
					SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_ALPHA | LWA_COLORKEY);
					return TRUE;
				}
				case WM_PAINT:
				{
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hWnd, &ps);
					HDC         hMemDC;
					HBITMAP     hBmpMem;
					HBITMAP     hPreBmp;
					//InvalidateRect(h, &rc, 1);
					hMemDC = CreateCompatibleDC(hdc);

					hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

					hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
					//On hMemDC.
					CreateFillArea(hWnd, hMemDC, VERTEXUICOLOR_MIDNIGHT);

					for (int i = 1; i <= 128; i++)
					{
						if (ic.listtxt[i] == L"")
						{
							break;
						}
						int y = i * 36;
						if (ic.TopTxt == L"")y -= 32;
						else
						{
							TextPreDrawEx(hMemDC, 12, 2, rc.right - rc.left, 34, ic.TopTxt, 17, 0, VuiDarkChangeToMedium(VERTEXUICOLOR_MIDNIGHTPLUS, 80));
						}
						if (ic.listtxt[2] != std::wstring(L""))vcdb.RoundButton(hWnd, hMemDC, { 6, y + 2, (rc.right - rc.left) - 9,36 }, 4, L"", { VERTEXUICOLOR_MIDNIGHTPLUS });
						TextPreDrawEx(hMemDC, 12, y + 2, rc.right - rc.left, 34, ic.listtxt[i], 17, 0, VERTEXUICOLOR_WHITE);

						//CreateRect(hWnd, hMemDC, 5, y, (rc.right - rc.left) - 10, 1, VERTEXUICOLOR_DARKNIGHT);
					}

					PanelDrawOutFrame(hWnd, hMemDC, VERTEXUICOLOR_DARKEN);

					BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);

					SelectObject(hMemDC, hPreBmp);

					DeleteObject(hBmpMem);

					DeleteDC(hMemDC);
					EndPaint(hWnd, &ps);



					break;
				}
				case WM_LBUTTONDOWN:
				{
					//int val = 0;
					//ClickAreaPtInfo(hWnd, 0, 0, rc.right - 35, 40, lParam, val);
					//if (val == 1)
					//{
					//	SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
					//}
					break;
				}
				case WM_LBUTTONUP:
				{
					SendClickEvent(hWnd, wParam, lParam);
					break;
				}
				case WM_MOUSEMOVE:
				{
					TRACKMOUSEEVENT tme;
					tme.cbSize = sizeof(tme);
					tme.dwFlags = TME_LEAVE;
					tme.dwHoverTime = 1;
					tme.hwndTrack = hWnd;
					TrackMouseEvent(&tme);
					hiContainerWindowAreaEvent(hWnd, lParam);
					KillTimer(hWnd, 1);
					if (inarea == 0)
					{
						SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_ALPHA | LWA_COLORKEY);
						inarea = 1;
					}
					break;
				}
				case WM_TIMER:
				{
					switch (wParam)
					{
					case 1:
					{
						inarea = 0;
						POINT pt;
						wchar_t txt[80];
						GetCursorPos(&pt);
						wsprintf(txt, L"x=%d;y=%d\n", pt.x, pt.y);
						OutputDebugString(txt);
						RECT rcx;
						GetWindowRect(hWnd, &rcx);
						INT_PTR alphaPtr = 255;
						int x, y = 0;
						if (ic.TrackMode == Always || ic.TrackMode == Auto)
						{
							if (pt.x < rcx.left || pt.x > rcx.right)
							{
								if (pt.x < rcx.left)
								{
									x = ic.TrackDistance - (rcx.left - pt.x);
								}
								if (pt.x > rcx.left)
								{
									x = rcx.right - pt.x + ic.TrackDistance;
								}
								alphaPtr = x * (255 / ic.TrackDistance);
							}
							else
							{
								if (pt.y < rcx.top || pt.y > rcx.bottom)
								{
									if (pt.y < rcx.top)
									{
										y = ic.TrackDistance - (rcx.top - pt.y);
									}
									if (pt.y > rcx.top)
									{
										y = rcx.bottom - pt.y + ic.TrackDistance;
									}
									alphaPtr = y * (255 / ic.TrackDistance);
								}
							}
							SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), alphaPtr, LWA_ALPHA | LWA_COLORKEY);
							if (alphaPtr < 5)
							{
								DestroyWindow(hWnd);
							}
						}
						if (ic.TrackMode == MoveOver)
						{
							RECT rcxx = rcx;
							rcxx.left -= ic.TrackDistance; rcxx.top -= ic.TrackDistance; rcxx.right += ic.TrackDistance; rcxx.bottom += ic.TrackDistance;
							if (!PtInRect(&rcxx, pt))
							{
								if (time >= 1000)
								{
									FadeOutAnimation(hWnd);
									DestroyWindow(hWnd);
									time = 0;
								}
							}
						}
					}
					}
					break;
				}
				case WM_MOUSELEAVE:
				{
					SetTimer(hWnd, 1, 10, 0);
					break;
				}
				case WM_ACTIVATE:
				{
					RECT rc;

					fActive = LOWORD(wParam);

					if (fActive == WA_ACTIVE)
					{
						break;
					}
					else
					{
						KillTimer(hWnd, 1);
						if (ic.TrackMode != Never)FadeOutAnimation(hWnd);
						DestroyWindow(hWnd);
					}
					break;
				}
				case WM_ERASEBKGND:
				{
					break;
				}
				case WM_DESTROY:
				{
					break;
				}
				case WM_SIZE:
				{

					InvalidateRect(hWnd, &rc, 0);
					break;
				}
				case WM_COMMAND:
				{
					switch (LOWORD(wParam))
					{
					}
					break;
				}
				//--消息处理不知道放这里吗?
				}
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			//
			const wchar_t* iContainerszWindowClass = L"LightFrame.iContainerctl";            // 主窗口类名

			ATOM iContainerBoxRegisterClass(HINSTANCE hInstance)
			{
				WNDCLASSEXW wcex;

				wcex.cbSize = sizeof(WNDCLASSEX);

				wcex.style = CS_HREDRAW | CS_VREDRAW;
				wcex.lpfnWndProc = (WNDPROC)CreateManifestWindowProc;
				wcex.cbClsExtra = 0;
				wcex.cbWndExtra = 0;
				wcex.hInstance = hInstance;
				wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(NULL));
				wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
				wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
				//wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(47, 52, 61));
				wcex.lpszMenuName = 0;
				wcex.lpszClassName = iContainerszWindowClass;
				wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(NULL));

				return RegisterClassExW(&wcex);
			}
			int CreateiContainerBox(
				HWND mh, ICONTAINER icr, HWND hWnd = 0, int hmode = 0      //OwnerData | Owner
			)
			{

				ic = {};
				ic = icr;
				if (hWnd != 0)
				{
					EnableWindow(hWnd, FALSE);
				}
				if (ic.listtxt[2] == std::wstring(L""))ic.iHeight = 45;
				if (ic.iHeight <= 0)ic.iHeight = 160;
				if (ic.iWidth <= 0)ic.iWidth = 128;
				POINT pt;
				GetCursorPos(&pt);
				iContainerBoxRegisterClass(hiContainerInst);
				hiContainerWnd = CreateWindowExW(0L, iContainerszWindowClass, L"LightFrame_hiContainer_legacy", WS_POPUP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,//TipBox
					pt.x + 2 + ic.xPosmove, pt.y, ic.iWidth, ic.iHeight, mh, 0, hiContainerInst, NULL);
				SendMessage(hiContainerWnd, WM_CREATE, 0, 0);
				hiContainerMode = hmode;
				SetWindowPos(hiContainerWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				LONG_PTR Style = ::GetWindowLongPtr(hiContainerWnd, GWL_STYLE);
				Style = Style & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX;
				::SetWindowLongPtr(hiContainerWnd, GWL_STYLE, Style);
				::SetWindowLong(hiContainerWnd, GWL_EXSTYLE, GetWindowLong(hiContainerWnd, GWL_EXSTYLE)
					& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
				::SetWindowPos(hiContainerWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

				if (ic.TrackMode == 3 || ic.TrackMode == 4)
				{
					OuterShadow::DropShadow Shadow;
					Shadow.Initialize(0);
					Shadow.SetSharpness(60);
					Shadow.SetSize(25);
					Shadow.SetDarkness(128);
					Shadow.SetPosition(0, 0);
					Shadow.Create(hiContainerWnd);
				}
				if (hWnd != 0)
				{
					MSG msg;
					while (GetMessage(&msg, NULL, 0, 0)) {

						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					EnableWindow(hWnd, TRUE);
					SetForegroundWindow(hWnd);
				}

				//MoveInMenuNew(hMaskWnd);
				return 0;
			}
			
		}*/
	}
}