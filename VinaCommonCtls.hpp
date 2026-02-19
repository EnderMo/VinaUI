#pragma once
#include <variant>
#include <random>
#include <filesystem>
#include "VinaInternalFunctionMapping.hpp"
#include "VinaWindow.hpp"
#include <thread>
using namespace VertexUI::Click;
typedef ID2D1DCRenderTarget* DCRT;
typedef ID2D1HwndRenderTarget* HRT;

struct VertexUICtlColor
{
	unsigned long color = VERTEXUICOLOR_MIDNIGHT;
	unsigned long borderColor = VERTEXUICOLOR_MIDNIGHTPLUS;
	unsigned long innerColor = VERTEXUICOLOR_WHITEDRAW;
	unsigned long hoverColor = VERTEXUICOLOR_MIDNIGHT;
	unsigned long hoverBorderColor = VERTEXUICOLOR_MIDNIGHTPLUS;
	unsigned long hoverInnerColor = VERTEXUICOLOR_WHITEDRAW;
	unsigned long activeColor = VERTEXUICOLOR_SEA;
	unsigned long activeBorderColor = VERTEXUICOLOR_MIDNIGHTPLUS;
	unsigned long activeInnerColor = VERTEXUICOLOR_WHITEDRAW;

};
class VinaButton : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, const wchar_t* txt, std::function<void()>events = [] {}, unsigned long clr = VERTEXUICOLOR_MIDNIGHT, int TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE)
	{
		this->func = events;
		this->Clr = clr;
		this->txtsz = TxtSize;
		this->txtClr = TxtColor;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
	}
	void Set(int x, int y, int cx, int cy)
	{
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		if (Isvalid == false)return;
		RECT rc;
		GetClientRect(hWnd, &rc);
		if (this->IsPushed == true)
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap < 10)
			{
				if (flag == 1)
					ap++;
			}
			if (ap >= 10)
			{
				flag = 0;
				GlobalAnimationCount--;
			}

			float num;
			num = CalcBounceCurve(ap, 0, 0.5, 10);
			/*
			CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
				VertexUI::Window::SimpleShadow::iDropShadow Shadow;
				Shadow.SetSharpness(15);
				Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
				Shadow.SetSize(5 + num * 10);
				Shadow.SetDarkness(100 - (10 - ap) * 5);
				Shadow.SetPosition(0, 0);
				Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, 8);
				});
				*/
			unsigned long nClr;
			int nR, nG, nB;
			nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
			nClr = RGB(nR, nG, nB);
			unsigned long bClr = VERTEXUICOLOR_MIDNIGHTPLUS;
			if (this->cy == this->cx)bClr = VuiDarkenColor(nClr,50);
			D2DDrawRoundRect(hdc, x + num, y + num, cx - num * 2, cy - num * 2, VuiFadeColor(nClr, 10), rdsize, 1, 1.0f, bClr);
			D2DDrawRoundRect(hdc, x + num, y + num, cx - num * 2, cy - 2 - num * 2, nClr, rdsize);

			D2DDrawText2(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz - num, txtClr, L"Segoe UI", 1, true);
		}
		else if (this->IsHoverd == true)
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap < 10)
			{
				if (flag == 1)
					ap++;
			}
			if (ap >= 10)
			{
				flag = 0;
				GlobalAnimationCount--;
			}

			float num;
			num = CalcEaseOutCurve(ap, 0, 0.5, 10);
			/*
			CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
				VertexUI::Window::SimpleShadow::iDropShadow Shadow;
				Shadow.SetSharpness(15);
				Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
				Shadow.SetSize(5 + num * 10);
				Shadow.SetDarkness(100 - (10 - ap) * 5);
				Shadow.SetPosition(0, 0);
				Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, 8);
				});
				*/
			unsigned long nClr;
			int nR, nG, nB;
			nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
			nClr = RGB(nR, nG, nB);
			unsigned long bClr = VERTEXUICOLOR_MIDNIGHTPLUS;
			if (this->cy == this->cx)bClr = VuiDarkenColor(nClr, 50);
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, VuiFadeColor(nClr, 10), rdsize, 1, 1.0f+num, bClr);
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy - 2 + num * 2, nClr, rdsize);

			D2DDrawText2(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
		}
		else
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap > 0)
			{
				if (flag == 1)
					ap--;
			}
			if (ap == 0)
			{
				flag = 0;
				GlobalAnimationCount--;
			}
			float num;
			num = CalcEaseOutCurve(ap, 0, (float)(0.5), 10);
			if (ap != 0)
			{
				/*
				CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
					VertexUI::Window::SimpleShadow::iDropShadow Shadow;
					Shadow.SetSharpness(15);
					Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
					Shadow.SetSize(5 + num * 10);
					Shadow.SetDarkness(100 - (10 - ap) * 5);
					Shadow.SetPosition(0, 0);
					Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, 8);
					});
					*/
			}
			unsigned long nClr = Clr;
			if (ap != 0)
			{
				int nR, nG, nB;
				nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
				nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
				nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
				nClr = RGB(nR, nG, nB);
			}
			unsigned long bClr = VERTEXUICOLOR_MIDNIGHTPLUS;
			if (this->cy == this->cx)bClr = VuiDarkenColor(nClr, 50);
			
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, VuiFadeColor(nClr, 10), rdsize, 1, 1.0f, bClr);
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy - 1 + num * 2, VuiFadeColor(nClr, 5), rdsize);
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy - 2 + num * 2, nClr, rdsize);
			D2DDrawText2(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
		}
	}
	void SetValidity(bool v)
	{
		Isvalid = v;
	}
	virtual int OnMouseUp()
	{
		ap = 0;
		Refresh(hWnd);
		func();
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{
		ap = 0;
		this->IsPushed = true;
		Refresh(hWnd);
		return 0;
	}
	std::wstring GetText()
	{
		return this->txt;
	}
	std::function<void()> GetOnClick()
	{
		return this->func;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{
		if (Isvalid == false)return 0;
		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {

			this->IsHoverd = true;
			Refresh(hWnd);
		}
		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaButton> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}
	void SetBorderRadius(float px)
	{
		this->rdsize = px;
	}
	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:
	bool Isvalid = true;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	float rdsize = 8;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};
class VinaSlider : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, int Value = -1, unsigned long clr = VERTEXUICOLOR_DARKEN, const wchar_t* placeholder = L"", std::function<void()>events = [] {})
	{
		this->func = events;
		this->txt = placeholder;
		this->x = x - 10;
		this->y = y - 10;
		this->cx = cx + 20;
		this->cy = cy + 20;
		if (Value != -1)this->Value = Value;
		this->Clr = clr;
	}

	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		RECT crc;

		double progress = ((double)(cx - 20) * Value) / 100.0;
		D2DDrawRoundRect(hdc, x + 10, y + 10, cx - 20, cy - 20, VuiFadeColor(Clr, 60), 8, 0.2f, 2, VuiFadeColor(Clr, 100), 0.6f);
		D2DDrawRoundRect(hdc, x + 10, y + 10, progress, cy - 20, VuiFadeColor(Clr, 10), 8);
		D2DDrawText2(hdc, this->txt.c_str(), x + 10, y + 12, cx - 20, cy - 20, 12, VuiFadeColor(Clr, 120), L"Segoe UI", 0.75f, true);
	}

	virtual int OnMouseUp()
	{

		this->OnEdit = false;
		Refresh(hWnd);
		return 0;
	}
	virtual int OnMouseDown()
	{
		this->OnEdit = true;
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);
		double p = (static_cast<double>(pt.x) - static_cast<double>((this->x + 10) * gScale)) / static_cast<double>((cx - 20) * gScale);
		this->Value = GetMaxValue(GetMinValue(p * 100, 0), 100);
		func();
		Refresh(hWnd);
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{

		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();
		if (eventtype == vinaEvent::mouseOver) {
			if (this->OnEdit == false)return 0;
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);
			double p = (static_cast<double>(pt.x) - static_cast<double>((this->x + 10) * gScale)) / static_cast<double>((cx - 20) * gScale);
			this->Value = GetMaxValue(GetMinValue(p * 100, 0), 100);
			func();
			Refresh(hWnd);
		}

		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaSlider> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}
	int GetValue()
	{
		return Value;
	}
	void SetValue(int val)
	{
		this->Value = val;
	}
	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:
	bool OnEdit = false;
	int Value = 0;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};
class VinaOptionCtl : public VertexUIControl {
public:
	
	std::function<void()> onMouseEnterFunc = nullptr;

	void Set(int x, int y, int cx, int cy, const wchar_t* txt, std::function<void()>events = [] {}, unsigned long clr = VERTEXUICOLOR_MIDNIGHT, float TxtSize = 15, int activeStat = 1, unsigned long TxtColor = VERTEXUICOLOR_WHITE)
	{
		this->func = events;
		this->Clr = clr;
		this->txtsz = TxtSize;
		this->txtClr = TxtColor;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
		this->active = activeStat;
	}
	void SetEmphasize(bool b)
	{
		this->IsEmphasized = b;
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		if (Isvalid == false)return;
		RECT rc;
		GetClientRect(hWnd, &rc);
		if (this->IsPushed == true && (this->active >= 1))
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap < 10)
			{
				if (flag == 1)
					ap++;
			}
			if (ap >= 10)
			{
				flag = 0;
				GlobalAnimationCount--;
			}

			float num;
			num = CalcBounceCurve(ap, 0, 0.5, 10);
			/*
			CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
				VertexUI::Window::SimpleShadow::iDropShadow Shadow;
				Shadow.SetSharpness(15);
				Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
				Shadow.SetSize(5 + num * 10);
				Shadow.SetDarkness(100 - (10 - ap) * 5);
				Shadow.SetPosition(0, 0);
				Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, 8);
				});
				*/
			unsigned long nClr;
			int nR, nG, nB;
			nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
			nClr = RGB(nR, nG, nB);
			if (this->cx < 60)
			{
				if (this->active == 2)D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 6, 1, 1.0f, VERTEXUICOLOR_SEA);
				else if (this->active == 1)D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 6, 1, 1.0f, nClr);

				if (this->active == 0)
				{
					if (this->IsEmphasized == true) D2DDrawText4_1(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, VuiFadeColor(txtClr, 40), L"Segoe UI", 1, true);
					else D2DDrawText4(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, VuiFadeColor(txtClr, 40), L"Segoe UI", 1, true);
				}

				if (this->IsEmphasized == true) D2DDrawText4_1(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
				else D2DDrawText4(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
			}
			else
			{
				D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 6, 1, 1.0f, nClr);
				if (this->IsEmphasized == true) D2DDrawText4_1(hdc, txt.c_str(), x + 20, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, txtClr, L"Segoe UI", 1, false);
				else D2DDrawText4(hdc, txt.c_str(), x + 20, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, txtClr, L"Segoe UI", 1, false);
			}
		}
		else if (this->IsHoverd == true && (this->active >= 1))
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap < 10)
			{
				if (flag == 1)
					ap++;
			}
			if (ap >= 10)
			{
				flag = 0;
				GlobalAnimationCount--;
			}

			float num;
			num = CalcEaseOutCurve(ap, 0, 0.5, 10);
			/*
			CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
				VertexUI::Window::SimpleShadow::iDropShadow Shadow;
				Shadow.SetSharpness(15);
				Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
				Shadow.SetSize(5 + num * 10);
				Shadow.SetDarkness(100 - (10 - ap) * 5);
				Shadow.SetPosition(0, 0);
				Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, 8);
				});
				*/
			unsigned long nClr;
			int nR, nG, nB;
			nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
			nClr = RGB(nR, nG, nB);
			if (this->cx < 60)
			{
				if (this->active == 2)D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 6, 1, 1.0f, VERTEXUICOLOR_SEA);
				else if (this->active == 1)D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 6, 1, 1.0f, nClr);

				if (this->active == 0)
				{
					if (this->IsEmphasized == true) D2DDrawText4_1(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, VuiFadeColor(txtClr, 40), L"Segoe UI", 1, true);
					else D2DDrawText4(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, VuiFadeColor(txtClr, 40), L"Segoe UI", 1, true);
				}

				if (this->IsEmphasized == true) D2DDrawText4_1(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
				else D2DDrawText4(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
			}
			else
			{
				D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, VuiFadeColor(nClr, 10), 6, 1, 1.0f, nClr);
				D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy - 1 + num * 2, VuiFadeColor(nClr, 5), 6, 1, 1.0f, nClr);
				D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy - 2 + num * 2, nClr, 6, 1, 1.0f, nClr);

				DrawAnimatedText(hdc, (this->cx < 60 ? x : x + 20), (float)(y + cy / 2 - txtsz / 1.5) + (this->cx < 60 ? 3 : 0), cx, cy, txtsz, txtClr, (this->cx < 60));

			}
		}
		else
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap > 0)
			{
				if (flag == 1)
					ap--;
			}
			if (ap == 0)
			{
				flag = 0;
				GlobalAnimationCount--;
			}
			float num;
			num = CalcEaseOutCurve(ap, 0, (float)(0.5), 10);
			if (ap != 0)
			{
				/*
				CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
					VertexUI::Window::SimpleShadow::iDropShadow Shadow;
					Shadow.SetSharpness(15);
					Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
					Shadow.SetSize(5 + num * 10);
					Shadow.SetDarkness(100 - (10 - ap) * 5);
					Shadow.SetPosition(0, 0);
					Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, 8);
					});
					*/
			}
			unsigned long nClr = Clr;
			if (ap != 0)
			{
				int nR, nG, nB;
				nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
				nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
				nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
				nClr = RGB(nR, nG, nB);
			}
			if (this->active == 0) {
				D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, VuiFadeColor(nClr, 30), 6, 1, 1.0f, nClr);
				D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy - 1 + num * 2, VuiFadeColor(nClr, 25), 6);
				D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy - 2 + num * 2, VuiFadeColor(nClr, 20), 6);

				if (this->cx >= 100)
				{
					if (this->IsEmphasized == true) D2DDrawText4_1(hdc, txt.c_str(), x + 20, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, VuiFadeColor(txtClr, 80), L"Segoe UI", 1, false);
					else D2DDrawText4(hdc, txt.c_str(), x + 20, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, VuiFadeColor(txtClr, 80), L"Segoe UI", 1, false);
				}
				else
				{
					if (this->IsEmphasized == true) D2DDrawText4_1(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, VuiFadeColor(txtClr, 80), L"Segoe UI", 1, true);
					else D2DDrawText4(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, VuiFadeColor(txtClr, 80), L"Segoe UI", 1, true);
				}
			}
			else
			{
				if (this->cx < 60)
				{
					if (this->active == 2)D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 6, 1, 1.0f, VERTEXUICOLOR_SEA);
					else if (this->active == 1)D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 6, 1, 1.0f, nClr);

					if (this->IsEmphasized == true) D2DDrawText4_1(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
					else D2DDrawText4(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5) + 3, cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
				}
				else
				{
					D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 6, 1, 1.0f, nClr);
					DrawAnimatedText(hdc, (this->cx < 60 ? x : x + 20), (float)(y + cy / 2 - txtsz / 1.5) + (this->cx < 60 ? 3 : 0), cx, cy, txtsz, txtClr, (this->cx < 60));
				}
			}
		}
	}
	void SetValidity(bool v)
	{
		Isvalid = v;
	}
	void SetText(const wchar_t* txt, int direction = 0)
	{
		if (this->txt == txt) return;

		VinaWindow* parent = vinaWinMap[this->hWnd];
		if (parent) {
			// 1. 停止旧动画
			if (textAnimId != -1) {
				parent->StopAnimation(textAnimId);
			}

			// 2. 准备状态
			this->oldTxt = this->txt;
			this->txt = txt;
			this->animDirection = direction;
			this->textAnimProgress = 0.0; // 必须明确设置为0

			// 3. 启动/确保动画系统运行 (WM_TIMER 12)
			parent->StartAnimationSystem();

			// 4. 创建动画
			// 注意：AnimateVariable 内部会修改 variable 的值，不需要在此处手动加 Refresh，
			// 因为 AnimationManager::UpdateAnimations 内部会调用 Refresh(task.targetHwnd)
			textAnimId = parent->AnimateVariableWithBezier<double>(
				this->hWnd,
				this->textAnimProgress,
				0.0,
				1.0,
				0.4,
				0.11, 0.57, 0.65, 1.0,
				[this]() {
					this->textAnimId = -1;
					this->textAnimProgress = 1.0; // 确保结束时完全到位
				}
			);
		}
		else {
			this->txt = txt;
			this->textAnimProgress = 1.0;
			Refresh(this->hWnd);
		}
	}
	virtual int OnMouseUp()
	{
		ap = 0;
		Refresh(hWnd);
		if (this->active >= 1)func();
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{
		ap = 0;
		this->IsPushed = true;
		Refresh(hWnd);
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{
		if (Isvalid == false)return 0;
		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {
			
			if (!this->IsHoverd) { 
				if (this->onMouseEnterFunc) this->onMouseEnterFunc();
			}
			this->IsHoverd = true;
			Refresh(hWnd);
		}
		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaOptionCtl> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	// 辅助函数：处理内部文字动画渲染
// 辅助函数：处理内部文字动画渲染
	void DrawAnimatedText(HRT hdc, int x, int y, int cx, int cy, float txtsz, unsigned long clr, bool center) {
		if (textAnimProgress >= 1.0) {
			// 无动画状态，直接绘制原文字（不使用裁剪，降低功耗）
			if (this->IsEmphasized) D2DDrawText4_1(hdc, txt.c_str(), (float)x, (float)y, (float)cx, (float)cy, txtsz, clr, L"Segoe UI", 1.0f, center);
			else D2DDrawText4(hdc, txt.c_str(), (float)x, (float)y, (float)cx, (float)cy, txtsz, clr, L"Segoe UI", 1.0f, center);
			return;
		}

		// 仅在动画播放时进入裁剪绘制逻辑
		D2DDrawInClippedRoundRect(this->hWnd, hdc, (float)x, (float)y, (float)cx, (float)cy-20, 0,
			[this, txtsz, clr, center](HWND hWnd, HRT pRT, float x, float y, float cx, float cy)
			{
				float moveDist = txtsz * 1.5f; // 增加滚动间距使过渡更自然
				float oldY, newY;

				// 取消透明度变换，始终为 1.0f
				float constAlpha = 1.0f;

				if (animDirection == 0) { // 旧上新下
					oldY = (float)y + (moveDist * (float)textAnimProgress);
					newY = (float)y - (moveDist * (float)(1.0 - textAnimProgress));
				}
				else { // 旧下新上
					oldY = (float)y - (moveDist * (float)textAnimProgress);
					newY = (float)y + (moveDist * (float)(1.0 - textAnimProgress));
				}

				// 绘制旧文字 (滚动出裁剪区)
				if (this->IsEmphasized) D2DDrawText4_1(pRT, oldTxt.c_str(), (float)x, oldY, (float)cx, (float)cy, txtsz, clr, L"Segoe UI", constAlpha, center);
				else D2DDrawText4(pRT, oldTxt.c_str(), (float)x, oldY, (float)cx, (float)cy, txtsz, clr, L"Segoe UI", constAlpha, center);

				// 绘制新文字 (滚动进裁剪区)
				if (this->IsEmphasized) D2DDrawText4_1(pRT, txt.c_str(), (float)x, newY, (float)cx, (float)cy, txtsz, clr, L"Segoe UI", constAlpha, center);
				else D2DDrawText4(pRT, txt.c_str(), (float)x, newY, (float)cx, (float)cy, txtsz, clr, L"Segoe UI", constAlpha, center);
			});
	}
	void SetOnMouseEnter(std::function<void()> hoverEvent)
	{
		this->onMouseEnterFunc = hoverEvent;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}

	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:
	std::wstring oldTxt;         // 存储切换前的文字
	double textAnimProgress = 1.0; // 动画进度 (0.0 -> 1.0)
	int textAnimId = -1;         // 动画ID
	int animDirection = 0;       // 0: 旧上新下, 1: 旧下新上
	bool IsEmphasized = false;
	bool Isvalid = true;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	int active = 1;
	std::function<void()>func;
	std::wstring text;
};
class VinaCompatibleButton : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, const wchar_t* txt, std::function<void()>events = [] {}, unsigned long clr = VERTEXUICOLOR_MIDNIGHT, float TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE)
	{
		this->func = events;
		this->Clr = clr;
		this->txtsz = TxtSize;
		this->txtClr = TxtColor;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
	}

	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		if (Isvalid == false)return;
		RECT rc;
		GetClientRect(hWnd, &rc);
		if (this->IsHoverd == true)
		{
			

			D2DDrawRoundRect(hdc, x, y, cx, cy, VuiFadeColor(Clr, 20) , 8, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);

			D2DDrawText2(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
		}
		else
		{

			D2DDrawRoundRect(hdc, x , y , cx , cy , Clr, 8, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);
			D2DDrawText2(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, txtClr, L"Segoe UI", 1, true);
		}
	}
	void SetValidity(bool v)
	{
		Isvalid = v;
	}
	virtual int OnMouseUp()
	{
		Refresh(hWnd);
		func();
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{
		this->IsPushed = true;
		Refresh(hWnd);
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{
		if (Isvalid == false)return 0;
		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {

			//this->IsHoverd = true;
			//Refresh(hWnd);
		}
		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaCompatibleButton> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}

	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:
	bool Isvalid = true;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};
class VinaHSLGroup : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, std::function<void()> events = [] {}, unsigned long clr = RGB(40, 40, 40), int TxtSize = 12, unsigned long TxtColor = VERTEXUICOLOR_WHITE)
	{
		this->func = events;
		this->TxtSize = TxtSize;
		this->TxtColor = TxtColor;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;


		int previewWidth = 80; 
		int sliderStartX = x + previewWidth + 10; 
		int sliderWidth = cx - previewWidth - 40; 

		int sliderHeight = GetMaxValue((cy - 60) / 3, 40); 
		int startY = y + 30;

		hSliderX = sliderStartX;
		hSliderY = startY;
		hSliderCX = sliderWidth;
		hSliderCY = sliderHeight;

		sSliderX = sliderStartX;
		sSliderY = startY + sliderHeight + 10;
		sSliderCX = sliderWidth;
		sSliderCY = sliderHeight;

		lSliderX = sliderStartX;
		lSliderY = startY + 2 * (sliderHeight + 10);
		lSliderCX = sliderWidth;
		lSliderCY = sliderHeight;
	}

	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		if (Isvalid == false) return;
		RECT rc;
		GetClientRect(hWnd, &rc);

	
		D2DDrawRoundRect(hdc, (float)x, (float)y, (float)cx, (float)cy + 5, VERTEXUICOLOR_MIDNIGHT, 8);

		D2DDrawText2(hdc, L"HSL", x, (float)(y + 5), cx, 20, TxtSize + 2, TxtColor, L"Segoe UI", 1, true);

		DrawColorPreview(hdc);

		DrawHSlider(hdc);

		DrawSSlider(hdc);

		DrawLSlider(hdc);
	}

	void DrawColorPreview(HRT hdc)
	{
		// 绘制方形颜色预览
		int previewX = x + 20;
		int previewY = y + 30;
		int previewSize = 60; 

		D2DDrawRoundRect(hdc, (float)previewX, (float)previewY, (float)previewSize, (float)previewSize, currentColor, 4, 1, 1.0f, RGB(255, 255, 255));


		wchar_t hslText[64];
		swprintf_s(hslText, L"H:%.0f\nS:%.0f\nL:%.0f", hValue, sValue, lValue);

		D2DDrawText2(hdc, hslText, previewX, (float)(previewY + previewSize / 2 - 20), previewSize, previewSize, TxtSize - 2, RGB(255, 255, 255), L"Segoe UI", 1, true);
	}

	void DrawHSlider(HRT hdc)
	{
		
	//	D2DDrawText2(hdc, L"H:", hSliderX - 25+10, (float)(hSliderY + hSliderCY / 2 - TxtSize / 1.5), 20, hSliderCY, TxtSize, TxtColor, L"Segoe UI", 1, true);

		
		DrawHueGradient(hdc, hSliderX + 10, hSliderY, hSliderCX, hSliderCY);


		double hProgress = ((double)hValue / 360.0) * hSliderCX;
		D2DDrawRoundRect(hdc, (float)(hSliderX + hProgress - 2) + 10, (float)hSliderY-1, 4, (float)hSliderCY+2, currentColor, 2, 1, 3.0f, RGB(255, 255, 255), 0.8f);
	}

	void DrawSSlider(HRT hdc)
	{
		//D2DDrawText2(hdc, L"S:", sSliderX - 25+10, (float)(sSliderY + sSliderCY / 2 - TxtSize / 1.5), 20, sSliderCY, TxtSize, TxtColor, L"Segoe UI", 1, true);

		
		DrawSaturationGradient(hdc, sSliderX + 10, sSliderY, sSliderCX, sSliderCY);

		
		double sProgress = ((double)sValue / 100.0) * sSliderCX;
		D2DDrawRoundRect(hdc, (float)(sSliderX + 10 + sProgress - 2), (float)sSliderY-1, 4, (float)sSliderCY+2, currentColor, 2, 1, 3.0f, RGB(255, 255, 255), 0.8f);
	}

	void DrawLSlider(HRT hdc)
	{
		
		//D2DDrawText2(hdc, L"L:", lSliderX - 25+10, (float)(lSliderY + lSliderCY / 2 - TxtSize / 1.5), 20, lSliderCY, TxtSize, TxtColor, L"Segoe UI", 1, true);

		
		DrawLightnessGradient(hdc, lSliderX + 10, lSliderY, lSliderCX, lSliderCY);

		
		double lProgress = ((double)lValue / 100.0) * lSliderCX;
		D2DDrawRoundRect(hdc, (float)(lSliderX + 10 + lProgress - 2), (float)lSliderY-1, 4, (float)lSliderCY+2, currentColor, 2, 1, 3.0f, RGB(255, 255, 255), 0.8f);
	}

	void DrawHueGradient(HRT hdc, int x, int y, int cx, int cy)
	{
		
		for (int i = 0; i < cx; i++)
		{
			double hue = (double)i / (double)cx * 360.0;
			unsigned long color = HSLtoRGB(hue, 1.0, 0.5);
			D2DDrawSolidRect(hdc, (float)(x + i), (float)y, 1, (float)cy, color);
		}
		
		unsigned long firstColor = HSLtoRGB(0, 1.0, 0.5);
		unsigned long lastColor = HSLtoRGB(359, 1.0, 0.5); 
		
		D2DDrawRoundRect(hdc, (float)x - 3, (float)y, 4.0f, (float)cy, firstColor, 3, 1);
		
		D2DDrawRoundRect(hdc, (float)(x + cx - 3) + 2, (float)y, 4.0f, (float)cy, lastColor, 3, 1);
	}

	void DrawSaturationGradient(HRT hdc, int x, int y, int cx, int cy)
	{
		
		for (int i = 0; i < cx; i++)
		{
			double sat = (double)i / (double)cx;
			unsigned long color = HSLtoRGB(hValue, sat, 0.5); 
			D2DDrawSolidRect(hdc, (float)(x + i), (float)y, 1, (float)cy, color);
		}
		
		unsigned long firstColor = HSLtoRGB(hValue, 0.0, 0.5); 
		unsigned long lastColor = HSLtoRGB(hValue, 1.0, 0.5); 
		
		D2DDrawRoundRect(hdc, (float)x - 3, (float)y, 4.0f, (float)cy, firstColor, 3, 1);
	
		D2DDrawRoundRect(hdc, (float)(x + cx - 3) + 2, (float)y, 4.0f, (float)cy, lastColor, 3, 1);
	}

	void DrawLightnessGradient(HRT hdc, int x, int y, int cx, int cy)
	{
	
		for (int i = 0; i < cx; i++)
		{
			double light = (double)i / (double)cx;
			unsigned long color = HSLtoRGB(hValue, sValue / 100.0, light);
			D2DDrawSolidRect(hdc, (float)(x + i), (float)y, 1, (float)cy, color);
		}

		unsigned long firstColor = HSLtoRGB(hValue, sValue / 100.0, 0.0); 
		unsigned long lastColor = HSLtoRGB(hValue, sValue / 100.0, 1.0); 
		
		D2DDrawRoundRect(hdc, (float)x - 3, (float)y, 4.0f, (float)cy, firstColor, 3, 1);
	
		D2DDrawRoundRect(hdc, (float)(x + cx - 3) + 2, (float)y, 4.0f, (float)cy, lastColor, 3, 1);
	}

	virtual int OnMouseUp()
	{
		Refresh(hWnd);
		func();
		return 0;
	}

	virtual int OnMouseDown()
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		
		int scaledX = (int)(pt.x / gScale);
		int scaledY = (int)(pt.y / gScale);

	
		if (scaledX >= hSliderX && scaledX <= hSliderX + hSliderCX &&
			scaledY >= hSliderY && scaledY <= hSliderY + hSliderCY)
		{
			hValue = GetMaxValue(GetMinValue(((scaledX - hSliderX) * 360) / hSliderCX, 0), 360);
			currentColor = HSLtoRGB(hValue, sValue / 100.0, lValue / 100.0);
		}
		else if (scaledX >= sSliderX && scaledX <= sSliderX + sSliderCX &&
			scaledY >= sSliderY && scaledY <= sSliderY + sSliderCY)
		{
			sValue = GetMaxValue(GetMinValue(((scaledX - sSliderX) * 100) / sSliderCX, 0), 100);
			currentColor = HSLtoRGB(hValue, sValue / 100.0, lValue / 100.0);
		}
		else if (scaledX >= lSliderX && scaledX <= lSliderX + lSliderCX &&
			scaledY >= lSliderY && scaledY <= lSliderY + lSliderCY)
		{
			lValue = GetMaxValue(GetMinValue(((scaledX - lSliderX) * 100) / lSliderCX, 0), 100);
			currentColor = HSLtoRGB(hValue, sValue / 100.0, lValue / 100.0);
		}

		Refresh(hWnd);
		return 0;
	}

	virtual int OnMouseMove()
	{
	
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);

			int scaledX = (int)(pt.x / gScale);
			int scaledY = (int)(pt.y / gScale);

		
			if (scaledX >= hSliderX && scaledX <= hSliderX + hSliderCX &&
				scaledY >= hSliderY && scaledY <= hSliderY + hSliderCY)
			{
				hValue = GetMaxValue(GetMinValue(((scaledX - hSliderX) * 360) / hSliderCX, 0), 360);
				currentColor = HSLtoRGB(hValue, sValue / 100.0, lValue / 100.0);
			}
			else if (scaledX >= sSliderX && scaledX <= sSliderX + sSliderCX &&
				scaledY >= sSliderY && scaledY <= sSliderY + sSliderCY)
			{
				sValue = GetMaxValue(GetMinValue(((scaledX - sSliderX) * 100) / sSliderCX, 0), 100);
				currentColor = HSLtoRGB(hValue, sValue / 100.0, lValue / 100.0);
			}
			else if (scaledX >= lSliderX && scaledX <= lSliderX + lSliderCX &&
				scaledY >= lSliderY && scaledY <= lSliderY + lSliderCY)
			{
				lValue = GetMaxValue(GetMinValue(((scaledX - lSliderX) * 100) / lSliderCX, 0), 100);
				currentColor = HSLtoRGB(hValue, sValue / 100.0, lValue / 100.0);
			}

			Refresh(hWnd);
		}
		return 0;
	}

	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{
		if (Isvalid == false) return 0;
		if (eventtype == vinaEvent::mouseUp) this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown) this->OnMouseDown();
		if (eventtype == vinaEvent::mouseOver) {
			this->IsHoverd = true;
			this->OnMouseMove();
			Refresh(hWnd);
		}
		return 0;
	}

	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaHSLGroup> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}

	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x, y, cx, cy };
		return _;
	}

	void SetValidity(bool v)
	{
		Isvalid = v;
	}

	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}

	void SetColor(unsigned long rgb)
	{
		int r = GetRValue(rgb);
		int g = GetGValue(rgb);
		int b = GetBValue(rgb);
		RGBtoHSL(r, g, b, hValue, sValue, lValue);
		currentColor = rgb;
		Refresh(hWnd);
	}

	unsigned long GetColor()
	{
		return currentColor;
	}

	std::wstring _event = L"";
	int id = -1;

protected:
	bool Isvalid = true;
	HWND hWnd;
	std::function<void()> func;


	double hValue = 0;  // 0-360
	double sValue = 50; // 0-100
	double lValue = 50; // 0-100
	unsigned long currentColor = RGB(128, 128, 128);


	int hSliderX, hSliderY, hSliderCX, hSliderCY;
	int sSliderX, sSliderY, sSliderCX, sSliderCY;
	int lSliderX, lSliderY, lSliderCX, lSliderCY;

	
	int TxtSize;
	unsigned long TxtColor;


	int x, y, cx, cy;
};

class VinaText : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, const wchar_t* txt, int TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE, std::function<void()>events = [] {})
	{
		this->func = events;
		this->txtsz = TxtSize;
		this->Clr = TxtColor;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		if (GetPtInfo(hWnd, x, y, cx, cy))
		{

			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap < 10)
			{
				if (flag == 1)
					ap++;
			}
			if (ap >= 10)
			{
				flag = 0;
				GlobalAnimationCount--;
			}

			float num;
			num = CalcEaseOutCurve(ap, 0, 0.5, 10);
			unsigned long nClr;
			int nR, nG, nB;
			int fact = 1;
			if (VuiColorAverage(Clr) > 128)fact = -2;
			nR = GetMaxValue(GetRValue(Clr) + num * 20 * fact, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20 * fact, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20 * fact, 255);
			nClr = RGB(nR, nG, nB);
			D2DDrawText(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Segoe UI", 1);
		}
		else
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap > 0)
			{
				if (flag == 1)
					ap--;
			}
			if (ap == 0)
			{
				flag = 0;
				GlobalAnimationCount--;
			}
			float num;
			num = CalcEaseOutCurve(ap, 0, (float)(0.5), 10);
			unsigned long nClr = Clr;
			if (ap != 0)
			{
				int nR, nG, nB;
				int fact = 1;
				if (VuiColorAverage(Clr) > 128)fact = -2;
				nR = GetMaxValue(GetRValue(Clr) + num * 20 * fact, 255);
				nG = GetMaxValue(GetGValue(Clr) + num * 20 * fact, 255);
				nB = GetMaxValue(GetBValue(Clr) + num * 20 * fact, 255);
				nClr = RGB(nR, nG, nB);
			}
			D2DDrawText(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Segoe UI", 1);
		}
	}
	virtual int OnMouseUp()
	{
		this->func();
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{



		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{

		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {

			this->IsHoverd = true;
			Refresh(hWnd);
		}
		return 0;
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaText> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	std::wstring txt;
	std::wstring c;
	std::wstring _event;
	unsigned long Clr;

	int id = -1;
protected:
	int x, y, cx, cy;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};
class VinaSideTab : public VertexUIControl {
public:
	std::wstring CvtFont(std::wstring from)
	{
		if (from == std::wstring(L"test-right"))return std::wstring(L"\uf178");
		if (from == std::wstring(L"test-right-upd"))return std::wstring(L"\uf178");
		if (from == std::wstring(L"test-right-dld"))return std::wstring(L"\uf178");
		if (from == std::wstring(L"win-close"))return std::wstring(L"\uf00d");
		if (from == std::wstring(L"win-max"))return std::wstring(L"\uf065");
		if (from == std::wstring(L"win-winmax"))return std::wstring(L"\uf424");
		if (from == std::wstring(L"win-winrestore"))return std::wstring(L"\uf422");
		if (from == std::wstring(L"win-min"))return std::wstring(L"\uf068");
		if (from == std::wstring(L"win-restore"))return std::wstring(L"\uf066");
		if (from == std::wstring(L"test-left"))return std::wstring(L"\uf177");
		if (from == std::wstring(L"test-more"))return std::wstring(L"\uf141");
		if (from == std::wstring(L"test-more2"))return std::wstring(L"\uf52c");
		if (from == std::wstring(L"test-more3"))return std::wstring(L"\uf142");
		if (from == std::wstring(L"test-home"))return std::wstring(L"\uf80a");
		if (from == std::wstring(L"test-info"))return std::wstring(L"\uf05a");
		if (from == std::wstring(L"test-comp"))return std::wstring(L"\uf5fd");
		if (from == std::wstring(L"test-set"))return std::wstring(L"\u2699");
		if (from == std::wstring(L"test-heart"))return std::wstring(L"\uf004");
		if (from == std::wstring(L"test-icecream"))return std::wstring(L"\uf810");
		if (from == std::wstring(L"test-test1"))return std::wstring(L"\u2195");
		return L" ";
	}
	void Set(int x, int y, int cx, const wchar_t* txt2, const wchar_t* txt, int TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE, std::function<void()>events = [] {})
	{
		this->func = events;
		this->txtsz = TxtSize;
		this->Clr = TxtColor;
		this->text2 = txt2;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = TxtSize;
	}
	void Activate(bool v = true)
	{
		this->IsActivate = v;
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		bool isHover = this->IsHoverd;
		if (isHover)
		{
			if (flag == 0) { GlobalAnimationCount++; flag = 1; }
			if (ap < 10) { if (flag == 1) ap++; }
			if (ap >= 10) { flag = 0; GlobalAnimationCount--; }
		}
		else
		{
			if (flag == 0) { GlobalAnimationCount++; flag = 1; }
			if (ap > 0) { if (flag == 1) ap--; }
			if (ap == 0) { flag = 0; GlobalAnimationCount--; }
		}

		float num = CalcEaseOutCurve(ap, 0, 0.5f, 10);
		unsigned long nClr;
		int fact = (VuiColorAverage(Clr) > 128) ? -2 : 1;
		int nR = GetMaxValue(GetRValue(Clr) + (int)(num * 20 * fact), 255);
		int nG = GetMaxValue(GetGValue(Clr) + (int)(num * 20 * fact), 255);
		int nB = GetMaxValue(GetBValue(Clr) + (int)(num * 20 * fact), 255);
		nClr = RGB(nR, nG, nB);

		std::wstring newStr = this->CvtFont(txt);

		if (this->IsActivate == true) {
			D2DDrawRoundRect(hdc, x - 5, y - 8.5f + num * 2, cx, txtsz + 16, VERTEXUICOLOR_WHITE, 12, 0.1f);
			if (VuiColorAverage(VERTEXUICOLOR_DARKNIGHT) < 60) {
				D2DDrawRoundRect(hdc, x - 5, y - 10, cx, txtsz + 16, VERTEXUICOLOR_MIDNIGHTPLUS, 12, 1, 1, VERTEXUICOLOR_WHITE, 0.1f);
			}
			else {
				D2DDrawRoundRect(hdc, x - 5, y - 10, cx, txtsz + 16, VERTEXUICOLOR_DARKNIGHT, 12, 1, 1, VERTEXUICOLOR_WHITE, 0.1f);
			}
		}

		float textBaseY = (float)(y + cy / 2 - txtsz / 1.5f);

		float iconX;
		bool showText = (cx >= 90);

		if (showText) {
			iconX = (float)(x + 3);

			D2DDrawText(hdc, text2.c_str(), x + 35, textBaseY + 2.5f, cx, cy, txtsz - 6, VERTEXUICOLOR_WHITE, L"Segoe UI", 1);
		}
		//宽度较窄，文字不显示
		else {

			iconX = (float)(x + cx / 2 - txtsz / 1.4);
		}


		D2DDrawText(hdc, newStr.c_str(), iconX, textBaseY + 1.5f, cx, cy, txtsz - 2, VuiDarkenColor(nClr, 30), L"Font Awesome 6 Free Solid", 1);
		D2DDrawText(hdc, newStr.c_str(), iconX, textBaseY, cx, cy, txtsz - 2, nClr, L"Font Awesome 6 Free Solid", 1);
	}
	virtual int OnMouseUp()
	{
		this->func();
		this->IsActivate = true;
		Refresh(hWnd);
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{



		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{

		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {

			this->IsHoverd = true;
			Refresh(hWnd);
		}
		return 0;
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaSideTab> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	std::wstring GetText() {
		return this->text2;
	}
	std::wstring txt;
	std::wstring c;
	std::wstring _event;
	unsigned long Clr;

	int id = -1;
protected:
	int x, y, cx, cy;
	HWND hWnd;
	std::wstring text2;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	bool IsActivate = false;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};
class VinaCard : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, const wchar_t* title, const wchar_t* sub, std::function<void(HRT hdc, float x, float y, float cx, float cy)> drawContent = [](HRT hdc, float x, float y, float cx, float cy) {},
		std::function<void()> func = [] {}, unsigned long bgColor = VERTEXUICOLOR_MIDNIGHT, int cornerRadius = 8, unsigned long titleColor = VERTEXUICOLOR_WHITE, int titleSize = 18)
	{
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
		this->title = title;
		this->sub = sub;
		this->drawContent = drawContent;
		this->bgColor = bgColor;
		this->cornerRadius = cornerRadius;
		this->titleColor = titleColor;
		this->titleSize = titleSize;
		this->func = func;
	}

	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		if (Isvalid == false) return;

		RECT rc;
		GetClientRect(hWnd, &rc);


		if (IsHoverd && !isHovering && hoverProgress < 1.0) {

			isHovering = true;
			VinaWindow* parent = vinaWinMap[this->hWnd];
			if (parent) {
				if (hoverAnimationId != -1) {
					parent->StopAnimation(hoverAnimationId);
					hoverAnimationId = -1;
				}
				hoverAnimationId = parent->AnimateVariableWithBezier(
					hWnd, hoverProgress, hoverProgress, 1.0, 0.25, 0.25, 0.1, 0.25, 1.0
				);
			}
		}
		else if (!IsHoverd && isHovering && hoverProgress > 0.0) {

			isHovering = false;
			VinaWindow* parent = vinaWinMap[this->hWnd];
			if (parent) {
				if (hoverAnimationId != -1) {
					parent->StopAnimation(hoverAnimationId);
					hoverAnimationId = -1;
				}
				hoverAnimationId = parent->AnimateVariableWithBezier(
					hWnd, hoverProgress, hoverProgress, 0.0, 0.15, 0.42, 0.0, 0.58, 1.0
				);
			}
		}

		float overlayPercentage = 0.35f + (0.02f * (hoverProgress));
		D2DDrawQuickShadow(hdc, (float)x, (float)y, (float)cx, (float)cy, 8, 0, 3, 16, 10, 0, 0.01f, VERTEXUICOLOR_DARKEN);
		D2DDrawRoundRect(hdc, (float)x - 1, (float)y - 1, (float)cx + 2, (float)cy + 2, VERTEXUICOLOR_DARKEN, (float)cornerRadius + 1, 0, 1, VERTEXUICOLOR_WHITE, 0.1f, true);
		D2DDrawInClippedRoundRect(hWnd, hdc, (float)x, (float)y, (float)cx, (float)cy, (float)cornerRadius,
			[&](HWND hWnd, HRT pRT, float x, float y, float cx, float cy) -> void {
				D2DDrawRoundRect(pRT, x, y, cx, cy, VERTEXUICOLOR_MIDNIGHT, 0, 1, 0.8f, VERTEXUICOLOR_MIDNIGHTPLUS);

				if (drawContent) {
					drawContent(pRT, x, y, cx, cy);
				}

				float overlayHeight = cy * overlayPercentage;

				float maxOffset = 5.0f;
				float offset = maxOffset * (hoverProgress);
				float overlayY = (y + cy - overlayHeight) - offset;

				if (IsHoverd == true)
				{

					CompGdiD2D(hWnd, hdc, [x, overlayY, cx, overlayHeight](HWND hWnd, HDC hdc) {
						//AreaBlur(hdc, { (int)(x * gScale), (int)(overlayY * gScale), int(cx * gScale), int((overlayHeight + 20) * gScale) }, 2, 3, 0);
						});
					D2DDrawRoundRect(pRT, x - 1, overlayY, cx + 2, overlayHeight + 20, VuiFadeColor2(VERTEXUICOLOR_WHITE, 40), 0, 0.6f, 0.8f, VERTEXUICOLOR_WHITE);
				}
				else D2DDrawRoundRect(pRT, x, overlayY, cx, overlayHeight + 20, VuiFadeColor(VERTEXUICOLOR_MIDNIGHTPLUS, -10), 0, 1, 0.8f, VERTEXUICOLOR_WHITE, 0.3f);

				D2DDrawText3(hdc, title.c_str(), x + 15, overlayY + 10, cx, overlayHeight, titleSize, titleColor, L"Segoe UI", 1, 0);
				D2DDrawText(hdc, sub.c_str(), x + 15, overlayY + 35, cx, overlayHeight, titleSize - 4, VuiFadeColor(titleColor, 80), L"Segoe UI", 1);
				return;
			}
		);

	}

	virtual int OnMouseUp()
	{
		IsHoverd = false;
		Refresh(hWnd);
		if (func) func();
		return 0;
	}

	virtual int OnMouseDown()
	{
		this->IsPushed = true;
		Refresh(hWnd);
		return 0;
	}

	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{
		if (Isvalid == false) return 0;

		if (eventtype == vinaEvent::mouseUp) this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown) this->OnMouseDown();
		if (eventtype == vinaEvent::mouseOver)
		{
			this->IsHoverd = true;
			if (!isHovering)Refresh(hWnd);
		}
		// 只处理事件，不处理动画逻辑，动画在CreateCtl中处理
		return 0;
	}

	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaCard> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}

	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x, y, cx, cy };
		return _;
	}

	void SetValidity(bool v)
	{
		Isvalid = v;
	}

	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}

	std::wstring title;

	std::wstring sub;
	unsigned long bgColor;
	unsigned long titleColor;
	int cornerRadius;
	int titleSize;
	std::function<void(HRT hdc, float x, float y, float cx, float cy)> drawContent;
	int id = -1;

protected:
	bool Isvalid = true;
	std::function<void()>func;
	HWND hWnd;
	double hoverProgress = 0.0;
	int hoverAnimationId = -1;
	std::wstring _event = L"";
	bool isHovering = false;  // 用于跟踪内部悬停状态
};

class VinaEdit : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, const wchar_t* placeholder = L"",
		std::function<void(std::wstring)> onEnterEvents = [](std::wstring) {},
		unsigned long clr = VERTEXUICOLOR_MIDNIGHT,
		unsigned long txtClr = VERTEXUICOLOR_WHITE,
		int txtSize = 15)
	{
		this->x = (float)x;
		this->y = (float)y;
		this->cx = (float)cx;
		this->cy = (float)cy;
		this->placeholder = placeholder ? placeholder : L"";
		this->func = onEnterEvents;
		this->Clr = clr;
		this->txtColor = txtClr;
		this->textSize = (float)txtSize;
		this->text = L"";
		this->caretPos = 0;
		this->selStart = 0;
		this->selEnd = 0;
		this->scrollOffset = 0;
		this->IsMouseDown = false;
		this->isForceShowCaret = false;
		this->caretOpacity = 0.0f; // 初始透明度
		this->caretAnimId = -1;    // 无活跃动画
	}

	// 启动/循环光标动画
// 修改 VinaEdit 类中的 StartCaretAnimation
	void StartCaretAnimation(float from, float to) {
		if (!this->IsFocused) return;

		auto it = vinaWinMap.find(this->hWnd);
		if (it != vinaWinMap.end()) {
			if (this->caretAnimId != -1) {
				it->second->StopAnimation(this->caretAnimId);
				this->caretAnimId = -1;
			}

			this->caretAnimId = it->second->AnimateVariableWithBezier(this->hWnd, this->caretOpacity, from, to, 0.6f, 0.0f, 1.11f, 0.0f, .96f, [this, from, to]() {
				//检查ID
				if (this->IsFocused) {
					this->StartCaretAnimation(to, from);
				}
				});
		}
	}
	//停止动画并隐藏光标
	void StopCaretAnimation() {
		this->IsFocused = false;
		auto it = vinaWinMap.find(this->hWnd);
		if (it != vinaWinMap.end() && this->caretAnimId != -1) {
			it->second->StopAnimation(this->caretAnimId);
		}
		this->caretAnimId = -1; // 重置ID
		this->caretOpacity = 0.0f;
	}

	void Set2(int x, int y, int cx, int cy,unsigned long Clr1,unsigned long Clr2)
	{
		this->x = (float)x;
		this->y = (float)y;
		this->cx = (float)cx;
		this->cy = (float)cy;
		this->Clr = Clr1;
		this->txtColor = Clr2;
	}
	void UpdateImePosition() {
		HIMC hIMC = ImmGetContext(this->hWnd);
		if (hIMC) {
			
			float cursorOffset = MeasureStringWidth(text.substr(0, caretPos), (int)textSize);
			float padding = 10.0f;

			COMPOSITIONFORM cf;
			
			cf.dwStyle = CFS_POINT;
			cf.ptCurrentPos.x = (long)((this->x + padding - this->scrollOffset + cursorOffset) * gScale);
			cf.ptCurrentPos.y = (long)((this->y + (this->cy / 2.0f)) * gScale);

	
			ImmSetCompositionWindow(hIMC, &cf);

			CANDIDATEFORM caf;
			caf.dwIndex = 0;
			caf.dwStyle = CFS_CANDIDATEPOS;
			caf.ptCurrentPos = cf.ptCurrentPos;
			ImmSetCandidateWindow(hIMC, &caf);

			ImmReleaseContext(this->hWnd, hIMC);
		}
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		if (this->IsFocused == false && this->IsMouseDown == false)
		{
			if (this->UnfocusFlag(this->text) == -1) return;
		}
		this->hWnd = hWnd;

		unsigned long bgClr = Clr;
		D2DDrawRoundRect(hdc, x, y, cx, cy, bgClr, 8, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);

		int safeCaret = Clamp(caretPos, 0, (int)text.length());
		float cursorOffset = MeasureStringWidth(text.substr(0, safeCaret), (int)textSize);
		float totalTextWidth = MeasureStringWidth(text, (int)textSize);

		float padding = 10.0f;
		float contentWidth = cx - (padding * 2);

		if (cursorOffset - scrollOffset > contentWidth) {
			scrollOffset = cursorOffset - contentWidth;
		}
		else if (cursorOffset - scrollOffset < 0) {
			scrollOffset = cursorOffset;
		}
		if (totalTextWidth < contentWidth) scrollOffset = 0;

		D2DDrawInClippedRoundRect(hWnd, hdc, x + 2, y + 2, cx - 4, cy - 4, 4,
			[&](HWND hWnd, HRT pRT, float clipX, float clipY, float clipCx, float clipCy) {

				float drawX = x + padding - scrollOffset;
				float drawY = y + (cy - textSize) / 2.0f;

				ID2D1SolidColorBrush* pTextBrush = NULL;
				IDWriteTextFormat* pFormat = NULL;

				pRT->CreateSolidColorBrush(D2D1::ColorF(RGBToHex(txtColor), 1.0f), &pTextBrush);
				pDWriteFactory->CreateTextFormat(L"Segoe UI", NULL, DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, textSize, L"", &pFormat);

				if (pTextBrush && pFormat) {
					// 绘制选区
					if (HasSelection()) {
						int start = std::min(selStart, selEnd);
						int end = std::max(selStart, selEnd);
						float sX = MeasureStringWidth(text.substr(0, start), (int)textSize);
						float eX = MeasureStringWidth(text.substr(0, end), (int)textSize);

						ID2D1SolidColorBrush* pSelBrush = NULL;
						pRT->CreateSolidColorBrush(D2D1::ColorF(0x0078D7, 0.4f), &pSelBrush);
						if (pSelBrush) {
							pRT->FillRectangle(D2D1::RectF(drawX + sX, y + 4, drawX + eX, y + cy - 4), pSelBrush);
							pSelBrush->Release();
						}
					}

					// 绘制占位符
					if (text.empty()) {
						ID2D1SolidColorBrush* pPHBrush = NULL;
						pRT->CreateSolidColorBrush(D2D1::ColorF(RGBToHex(txtColor), 0.5f), &pPHBrush);
						if (pPHBrush) {
							pRT->DrawText(placeholder.c_str(), (UINT32)placeholder.length(), pFormat,
								D2D1::RectF(x + padding, drawY, x + cx, y + cy), pPHBrush);
							pPHBrush->Release();
						}
					}
					else {
						pRT->DrawText(text.c_str(), (UINT32)text.length(), pFormat,
							D2D1::RectF(drawX, drawY, drawX + totalTextWidth + 100, y + cy), pTextBrush);
					}

					// 绘制光标
					if (this->IsFocused) {
						float caretX = drawX + cursorOffset;
						D2DDrawSolidRect(pRT, caretX, y+cy/2-8 , 1.2f, 18, txtColor, caretOpacity);
						isForceShowCaret = false;
					}
				}

				if (pTextBrush) pTextBrush->Release();
				if (pFormat) pFormat->Release();
			});
	}
	std::function<int(std::wstring& a)>UnfocusFlag = [](std::wstring& a) { return 0; };
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{
		if (eventtype == vinaEvent::mouseDown) {
			StopCaretAnimation();
			IsMouseDown = true;
			caretPos = GetIndexFromX((float)pt.x / gScale);
			selStart = caretPos;
			selEnd = caretPos;

	
			isForceShowCaret = true;
			caretOpacity = 1.0f;
			
			Refresh(hWnd);
		}
		if (eventtype == vinaEvent::mouseOver && IsMouseDown) {
		
			int selStartBackup = selStart;
			caretPos = GetIndexFromX((float)pt.x / gScale);
			selEnd = caretPos;

			Refresh(hWnd);
		}


		if (eventtype == vinaEvent::mouseUp) {
			if (!IsFocused) {
				IsFocused = true;
				StartCaretAnimation(0.0f, 1.0f); 
			}
			IsMouseDown = false;
		}
				if (eventtype == vinaEvent::mouseUnfocus) {
			StopCaretAnimation();
			Refresh(hWnd);
		}
		return 0;
	}


	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype, unsigned long keyid)
	{
		if (eventtype == vinaEvent::ime_char)
		{
			wchar_t ch = (wchar_t)keyid;
			if (ch >= 32) {
				UpdateImePosition();
				if (HasSelection()) RemoveSelection();
				text.insert(caretPos, 1, ch);
				caretPos++;
				selStart = selEnd = caretPos;
				isForceShowCaret = true;
				caretOpacity = 1.0f; 

				Refresh(hWnd);
			}
			return 0;
		}

		if (eventtype == vinaEvent::keyDown)
		{
			bool shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
			bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;

			if (keyid == VK_LEFT) {
				if (caretPos > 0) caretPos--;
				if (!shift) selStart = selEnd = caretPos; else selEnd = caretPos;
			}
			else if (keyid == VK_RIGHT) {
				if (caretPos < (int)text.length()) caretPos++;
				if (!shift) selStart = selEnd = caretPos; else selEnd = caretPos;
			}
			else if (keyid == VK_HOME) {
				caretPos = 0;
				if (!shift) selStart = selEnd = caretPos; else selEnd = caretPos;
			}
			else if (keyid == VK_END) {
				caretPos = (int)text.length();
				if (!shift) selStart = selEnd = caretPos; else selEnd = caretPos;
			}
			else if (keyid == VK_BACK) {
				if (HasSelection()) RemoveSelection();
				else if (caretPos > 0) {
					text.erase(caretPos - 1, 1);
					caretPos--;
				}
				selStart = selEnd = caretPos;
			}
			else if (keyid == VK_DELETE) {
				if (HasSelection()) RemoveSelection();
				else if (caretPos < (int)text.length()) text.erase(caretPos, 1);
				selStart = selEnd = caretPos;
			}
			else if (keyid == VK_RETURN) {
				if (func) func(text);
			}
			else if (ctrl && keyid == 'A') {
				selStart = 0;
				selEnd = (int)text.length();
				caretPos = selEnd;
			}
			else if (ctrl && keyid == 'C') CopyToClipboard();
			else if (ctrl && keyid == 'V') PasteFromClipboard();
			else if (ctrl && keyid == 'X') { CopyToClipboard(); RemoveSelection(); }

			isForceShowCaret = true;
			caretOpacity = 1.0f; 
			Refresh(hWnd);
		}
		return 0;
	}

	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaEdit> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}

	virtual VertexUIPos GetCurrentRect() { return { (int)x, (int)y, (int)cx, (int)cy }; }
	std::wstring GetText() const { return text; }
	void SetText(std::wstring t) { text = t; caretPos = (int)t.length(); selStart = selEnd = caretPos; }
	float cx = 0;

private:
	std::wstring text;
	float x, y, cy = 0;
	unsigned long Clr;
	HWND hWnd;
	std::wstring placeholder;
	std::function<void(std::wstring)> func;
	unsigned long txtColor;
	float textSize;

	bool IsMouseDown = false;
	int caretPos = 0;
	int selStart = 0;
	int selEnd = 0;
	float scrollOffset = 0;
	bool isForceShowCaret = false;

	float caretOpacity;     // 被 Animator 驱动的透明度
	int caretAnimId;        // 当前动画 ID

	int Clamp(int val, int minVal, int maxVal) {
		if (val < minVal) return minVal;
		if (val > maxVal) return maxVal;
		return val;
	}

	bool HasSelection() const { return selStart != selEnd; }

	void RemoveSelection() {
		if (!HasSelection()) return;
		int start = std::min(selStart, selEnd);
		int len = abs(selEnd - selStart);
		start = Clamp(start, 0, (int)text.length());
		len = Clamp(len, 0, (int)text.length() - start);
		text.erase(start, len);
		caretPos = start;
		selStart = selEnd = caretPos;
	}

	void CopyToClipboard() {
		if (!HasSelection()) return;
		if (!OpenClipboard(hWnd)) return;
		EmptyClipboard();
		int start = std::min(selStart, selEnd);
		int len = abs(selEnd - selStart);
		std::wstring sub = text.substr(Clamp(start, 0, (int)text.length()), Clamp(len, 0, (int)text.length()));
		HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, (sub.length() + 1) * sizeof(wchar_t));
		if (hGlob) {
			void* ptr = GlobalLock(hGlob);
			if (ptr) {
				memcpy(ptr, sub.c_str(), (sub.length() + 1) * sizeof(wchar_t));
				GlobalUnlock(hGlob);
				SetClipboardData(CF_UNICODETEXT, hGlob);
			}
		}
		CloseClipboard();
	}

	void PasteFromClipboard() {
		if (!OpenClipboard(hWnd)) return;
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		if (hData) {
			wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
			if (pszText) {
				std::wstring pasteText(pszText);
				if (HasSelection()) RemoveSelection();
				text.insert(Clamp(caretPos, 0, (int)text.length()), pasteText);
				caretPos += (int)pasteText.length();
				selStart = selEnd = caretPos;
				GlobalUnlock(hData);
			}
		}
		CloseClipboard();
	}

	float MeasureStringWidth(const std::wstring& str, int fontSize) {
		if (str.empty()) return 0.0f;
		IDWriteTextLayout* pTextLayout = NULL;
		IDWriteTextFormat* pTextFormat = NULL;
		float width = 0.0f;

		HRESULT hr = pDWriteFactory->CreateTextFormat(L"Segoe UI", NULL, DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, (float)fontSize, L"", &pTextFormat);

		if (SUCCEEDED(hr)) {
			hr = pDWriteFactory->CreateTextLayout(str.c_str(), (UINT32)str.length(), pTextFormat, 10000.0f, 1000.0f, &pTextLayout);
			if (SUCCEEDED(hr)) {
				DWRITE_TEXT_METRICS metrics;
				pTextLayout->GetMetrics(&metrics);
				width = metrics.width;
			}
		}
		if (pTextLayout) pTextLayout->Release();
		if (pTextFormat) pTextFormat->Release();
		return width;
	}

	int GetIndexFromX(float mouseX) {
		float padding = 10.0f;
		float relativeX = mouseX - (this->x + padding - this->scrollOffset);
		if (relativeX <= 0) return 0;
		for (int i = 1; i <= (int)text.length(); i++) {
			float w = MeasureStringWidth(text.substr(0, i), (int)textSize);
			if (w > relativeX) {
				float prevW = MeasureStringWidth(text.substr(0, i - 1), (int)textSize);
				return (relativeX - prevW < w - relativeX) ? i - 1 : i;
			}
		}
		return (int)text.length();
	}

	wchar_t GetCharFromKey(unsigned long vk, bool shift) {
		if (vk >= 'A' && vk <= 'Z') return shift ? (wchar_t)vk : (wchar_t)(vk + 32);
		if (vk >= '0' && vk <= '9') {
			if (!shift) return (wchar_t)vk;
			const wchar_t* sym = L")!@#$%^&*(";
			return sym[vk - '0'];
		}
		if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9) return (wchar_t)('0' + (vk - VK_NUMPAD0));
		switch (vk) {
		case VK_SPACE: return L' ';
		case VK_OEM_MINUS:  return shift ? L'_' : L'-';
		case VK_OEM_PLUS:   return shift ? L'+' : L'=';
		case VK_OEM_1:      return shift ? L':' : L';';
		case VK_OEM_2:      return shift ? L'?' : L'/';
		case VK_OEM_3:      return shift ? L'~' : L'`';
		case VK_OEM_4:      return shift ? L'{' : L'[';
		case VK_OEM_5:      return shift ? L'|' : L'\\';
		case VK_OEM_6:      return shift ? L'}' : L']';
		case VK_OEM_7:      return shift ? L'\"' : L'\'';
		case VK_OEM_COMMA:  return shift ? L'<' : L',';
		case VK_OEM_PERIOD: return shift ? L'>' : L'.';
		}
		return 0;
	}

	unsigned int RGBToHex(unsigned long clr) {
		return ((clr & 0xFF) << 16) | (clr & 0xFF00) | ((clr & 0xFF0000) >> 16);
	}
};
class VinaNotice : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, const wchar_t* txt, unsigned long clr = VERTEXUICOLOR_SEA, int TxtSize = 14, unsigned long TxtColor = VERTEXUICOLOR_WHITE, std::function<void()>events = [] {})
	{
		this->func = events;
		this->Clr = clr;
		this->txtsz = TxtSize;
		this->txtClr = TxtColor;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
	}

	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		if (this->IsPushed == true)
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap < 10)
			{
				if (flag == 1)
					ap++;
			}
			if (ap >= 10)
			{
				flag = 0;
				GlobalAnimationCount--;
			}

			float num;
			num = CalcBounceCurve(ap, 0, 0.5, 10);
			/*
			CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
				VertexUI::Window::SimpleShadow::iDropShadow Shadow;
				Shadow.SetSharpness(15);
				Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
				Shadow.SetSize(5 + num * 10);
				Shadow.SetDarkness(100 - (10 - ap) * 5);
				Shadow.SetPosition(0, 0);
				Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, 8);
				});
				*/
			unsigned long nClr;
			int nR, nG, nB;
			nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
			nClr = RGB(nR, nG, nB);
			D2DDrawRoundRect(hdc, x + num, y + num, cx - num * 2, cy - num * 2, Clr, 8, 1, 1.0f, VuiFadeColor(Clr, 40));

			D2DDrawText2(hdc, txt.c_str(), x + 10, (float)(y + cy / 2 - txtsz * 1.2), cx, cy, txtsz, txtClr, L"Segoe UI", 1, false);
		}
		else if (this->IsHoverd == true)
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap < 10)
			{
				if (flag == 1)
					ap++;
			}
			if (ap >= 10)
			{
				flag = 0;
				GlobalAnimationCount--;
			}

			float num;
			num = CalcEaseOutCurve(ap, 0, 0.5, 10);
			/*
			CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
				VertexUI::Window::SimpleShadow::iDropShadow Shadow;
				Shadow.SetSharpness(15);
				Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
				Shadow.SetSize(5 + num * 10);
				Shadow.SetDarkness(100 - (10 - ap) * 5);
				Shadow.SetPosition(0, 0);
				Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, 8);
				});
				*/
			unsigned long nClr;
			int nR, nG, nB;
			nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
			nClr = RGB(nR, nG, nB);
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, Clr, 8, 1, 1.0f + num, VuiFadeColor(Clr, 40));

			D2DDrawText2(hdc, txt.c_str(), x + 10, (float)(y + cy / 2 - txtsz * 1.2), cx, cy, txtsz, txtClr, L"Segoe UI", 1, false);
		}
		else
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap > 0)
			{
				if (flag == 1)
					ap--;
			}
			if (ap == 0)
			{
				flag = 0;
				GlobalAnimationCount--;
			}
			float num;
			num = CalcEaseOutCurve(ap, 0, (float)(0.5), 10);
			if (ap != 0)
			{
				/*
				CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
					VertexUI::Window::SimpleShadow::iDropShadow Shadow;
					Shadow.SetSharpness(15);
					Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
					Shadow.SetSize(5 + num * 10);
					Shadow.SetDarkness(100 - (10 - ap) * 5);
					Shadow.SetPosition(0, 0);
					Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, 8);
					});
					*/
			}
			unsigned long nClr = Clr;
			if (ap != 0)
			{
				int nR, nG, nB;
				nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
				nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
				nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
				nClr = RGB(nR, nG, nB);
			}
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, Clr, 8, 1, 1.0f + num, VuiFadeColor(Clr, 40));

			D2DDrawText2(hdc, txt.c_str(), x + 10, (float)(y + cy/2-txtsz*1.2), cx, cy, txtsz, txtClr, L"Segoe UI", 1, false);
		}
	}

	virtual int OnMouseUp()
	{
		ap = 0;
		Refresh(hWnd);
		func();
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{
		ap = 0;
		this->IsPushed = true;
		Refresh(hWnd);
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{

		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {

			this->IsHoverd = true;
			Refresh(hWnd);
		}
		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaNotice > vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}

	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:

	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};

class VinaFAIcon : public VertexUIControl {
public:
	std::wstring CvtFont(std::wstring from)
	{
		if (from == std::wstring(L"test-right"))return std::wstring(L"\uf178");
		if (from == std::wstring(L"test-right-upd"))return std::wstring(L"\uf178");
		if (from == std::wstring(L"test-right-dld"))return std::wstring(L"\uf178");
		if (from == std::wstring(L"win-close"))return std::wstring(L"\uf00d");
		if (from == std::wstring(L"win-max"))return std::wstring(L"\uf065");
		if (from == std::wstring(L"win-winmax"))return std::wstring(L"\uf424");
		if (from == std::wstring(L"win-winrestore"))return std::wstring(L"\uf422");
		if (from == std::wstring(L"win-min"))return std::wstring(L"\uf068");
		if (from == std::wstring(L"win-restore"))return std::wstring(L"\uf066");
		if (from == std::wstring(L"test-complete"))return std::wstring(L"\uf00C");
		if (from == std::wstring(L"test-left"))return std::wstring(L"\uf177");
		if (from == std::wstring(L"test-more"))return std::wstring(L"\uf141");
		if (from == std::wstring(L"test-more2"))return std::wstring(L"\uf52c");
		if (from == std::wstring(L"test-more3"))return std::wstring(L"\uf142");
		if (from == std::wstring(L"win-fold"))return std::wstring(L"\uf077");
		if (from == std::wstring(L"win-extend"))return std::wstring(L"\u2304");
		if (from == std::wstring(L"test-refresh"))return std::wstring(L"\uf0E2");
		if (from == std::wstring(L"test-plus"))return std::wstring(L"\uf067");
	}
	void Set(int x, int y, const wchar_t* txt, int TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE, std::function<void()>events = [] {})
	{
		this->func = events;
		this->txtsz = TxtSize;
		this->Clr = TxtColor;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = TxtSize;
		this->cy = TxtSize;
	}
	void Set(int x, int y, int cx, const wchar_t* txt, int TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE, std::function<void()>events = [] {})
	{
		this->func = events;
		this->txtsz = TxtSize;
		this->Clr = TxtColor;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = TxtSize;
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		if (GetPtInfo(hWnd, x, y, cx, cy))
		{

			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap < 10)
			{
				if (flag == 1)
					ap++;
			}
			if (ap >= 10)
			{
				flag = 0;
				GlobalAnimationCount--;
			}

			float num;
			num = CalcEaseOutCurve(ap, 0, 0.5, 10);
			unsigned long nClr;
			int nR, nG, nB;
			int fact = 1;
			if (VuiColorAverage(Clr) > 128)fact = -2;
			nR = GetMaxValue(GetRValue(Clr) + num * 20 * fact, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20 * fact, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20 * fact, 255);
			nClr = RGB(nR, nG, nB);
			std::wstring newStr = this->CvtFont(txt.c_str());
			if (txt == std::wstring(L"test-left"))
			{
				D2DDrawText(hdc, newStr.c_str(), x - num * 3, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
			}
			else if (txt == std::wstring(L"test-right"))
			{
				D2DDrawText(hdc, newStr.c_str(), x + num * 3, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
			}
			else if (txt == std::wstring(L"test-right-upd"))
			{
				D2DDrawText3(hdc, L"现在更新", x - 5 + num * 3, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz - 4, nClr, L"Segoe UI", 1);
				D2DDrawText(hdc, newStr.c_str(), x + cx - txtsz + num * 3, (float)(y + 1 + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
			}
			else if (txt == std::wstring(L"test-right-dld"))
			{
				D2DDrawText3(hdc, L"下载", x - 5 + num * 3, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz - 4, nClr, L"Segoe UI", 1);
				D2DDrawText(hdc, newStr.c_str(), x + cx - txtsz + num * 3, (float)(y + 1 + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
			}
			else D2DDrawText(hdc, newStr.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
		}
		else
		{
			if (flag == 0)
			{
				GlobalAnimationCount++;
				flag = 1;
			}
			if (ap > 0)
			{
				if (flag == 1)
					ap--;
			}
			if (ap == 0)
			{
				flag = 0;
				GlobalAnimationCount--;
			}
			float num;
			num = CalcEaseOutCurve(ap, 0, (float)(0.5), 10);
			unsigned long nClr = Clr;
			if (ap != 0)
			{
				int nR, nG, nB;
				int fact = 1;
				if (VuiColorAverage(Clr) > 128)fact = -2;
				nR = GetMaxValue(GetRValue(Clr) + num * 20 * fact, 255);
				nG = GetMaxValue(GetGValue(Clr) + num * 20 * fact, 255);
				nB = GetMaxValue(GetBValue(Clr) + num * 20 * fact, 255);
				nClr = RGB(nR, nG, nB);
			}
			std::wstring newStr = this->CvtFont(txt.c_str());
			if (txt == std::wstring(L"test-left"))
			{
				D2DDrawText(hdc, newStr.c_str(), x - num * 3, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
			}
			else if (txt == std::wstring(L"test-right"))
			{
				D2DDrawText(hdc, newStr.c_str(), x + num * 3, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
			}
			else if (txt == std::wstring(L"test-right-upd"))
			{
				D2DDrawText3(hdc, L"现在更新", x - 5 + num * 3, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz - 4, nClr, L"Segoe UI", 1);
				D2DDrawText(hdc, newStr.c_str(), x + cx - txtsz + num * 3, (float)(y + 1 + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
			}
			else if (txt == std::wstring(L"test-right-dld"))
			{
				D2DDrawText3(hdc, L"下载", x - 5 + num * 3, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz - 4, nClr, L"Segoe UI", 1);
				D2DDrawText(hdc, newStr.c_str(), x + cx - txtsz + num * 3, (float)(y + 1 + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
			}
			else D2DDrawText(hdc, newStr.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);

		}
	}
	virtual int OnMouseUp()
	{
		this->func();
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{



		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{

		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {

			this->IsHoverd = true;
			Refresh(hWnd);
		}
		return 0;
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaFAIcon> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	std::wstring txt;
	std::wstring c;
	std::wstring _event;
	unsigned long Clr;

	int id = -1;
protected:
	int x, y, cx, cy;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};
class VinaSwitch : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, VertexUICtlColor ci, std::function<void()>events = [] {})
	{
		this->func = events;
		this->Clr = ci.color;
		this->borderClr = ci.borderColor;
		this->innerClr = ci.innerColor;
		this->aBorderClr = ci.activeBorderColor;
		this->aClr = ci.activeColor;
		this->aInnerClr = ci.activeInnerColor;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
	}


	unsigned long InterpolateColor(unsigned long color1, unsigned long color2, double ratio) {
		int r1 = GetRValue(color1), g1 = GetGValue(color1), b1 = GetBValue(color1);
		int r2 = GetRValue(color2), g2 = GetGValue(color2), b2 = GetBValue(color2);
		int r = (int)(r1 + (r2 - r1) * ratio);
		int g = (int)(g1 + (g2 - g1) * ratio);
		int b = (int)(b1 + (b2 - b1) * ratio);
		return RGB(r, g, b);
	}

	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		
		if (IsHoverd && !isHovering && hoverProgress < 1.0) {
			isHovering = true;
			VinaWindow* parent = vinaWinMap[this->hWnd];
			if (parent) {
				if (hoverAnimationId != -1) {
					parent->StopAnimation(hoverAnimationId);
					hoverAnimationId = -1;
				}
				hoverAnimationId = parent->AnimateVariableWithBezier<double>(
					hWnd, hoverProgress, hoverProgress, 1.0, 0.25, 0.25, 0.1, 0.25, 1.0
				);
			}
		}
		else if (!IsHoverd && isHovering && hoverProgress > 0.0) {
			isHovering = false;
			VinaWindow* parent = vinaWinMap[this->hWnd];
			if (parent) {
				if (hoverAnimationId != -1) {
					parent->StopAnimation(hoverAnimationId);
					hoverAnimationId = -1;
				}
				hoverAnimationId = parent->AnimateVariableWithBezier<double>(
					hWnd, hoverProgress, hoverProgress, 0.0, 0.15, 0.42, 0.0, 0.58, 1.0
				);
			}
		}

	
		unsigned long baseBg = InterpolateColor(Clr, aClr, switchProgress);
		unsigned long currentBorder = InterpolateColor(borderClr, aBorderClr, switchProgress);
		unsigned long currentInner = InterpolateColor(innerClr, aInnerClr, switchProgress);


		double lightIntensity = hoverProgress * 0.2;
		unsigned long finalBgClr = InterpolateColor(baseBg, RGB(255, 255, 255), lightIntensity);

		int dist = cy * 0.15;
		int innerBlockSize = cy - dist * 2;


		double startX = (double)x + dist;
		double endX = (double)x + cx - innerBlockSize - dist;
		float currentBallX = (float)(startX + (endX - startX) * switchProgress);

		D2DDrawRoundRect(hdc, x, y, cx, cy, finalBgClr, cy / 2, 1, 1.0f, currentBorder);
		D2DDrawRoundRect(hdc, currentBallX, (float)y + dist, (float)innerBlockSize, (float)innerBlockSize, currentInner, cy / 2, 1);
	}

	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{
		if (eventtype == vinaEvent::mouseUp) {
			preValue = value;
			value = !value;

			VinaWindow* parent = vinaWinMap[this->hWnd];
			if (parent) {
				if (switchAnimationId != -1) {
					parent->StopAnimation(switchAnimationId);
				}
				double target = value ? 1.0 : 0.0;
				switchAnimationId = parent->AnimateVariableWithBezier<double>(
					hWnd, switchProgress, switchProgress, target, 0.2, 0.42, 0.0, 0.58, 1.0
				);
			}

			func();
			Refresh(hWnd);
		}

		if (eventtype == vinaEvent::mouseOver) {
			this->IsHoverd = true; 
			Refresh(hWnd);
		}
		else {
			this->IsHoverd = false; 
			Refresh(hWnd);
		}
		return 0;
	}

	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaSwitch> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	bool GetValue()
	{
		return this->value;
	}
	void SetValue(bool val)
	{
		this->value = val;
		this->switchProgress = val ? 1.0 : 0.0;
	}
	void SetValue2(bool val)
	{
		if (this->value == val) return; 

		this->value = val;
		double target = val ? 1.0 : 0.0;

		VinaWindow* parent = vinaWinMap[this->hWnd];
		if (parent) {
		
			if (switchAnimationId != -1) {
				parent->StopAnimation(switchAnimationId);
			}
			
			switchAnimationId = parent->AnimateVariableWithBezier<double>(
				hWnd, switchProgress, switchProgress, target, 0.2, 0.42, 0.0, 0.58, 1.0
			);
		}
		else {
	
			this->switchProgress = target;
		}

		Refresh(this->hWnd);
	}
	std::wstring txt;
	std::wstring c;
	std::wstring _event;
	int id = -1;

protected:
	int x, y, cx, cy;
	HWND hWnd;

	double hoverProgress = 0.0;
	int hoverAnimationId = -1;
	bool isHovering = false;

	double switchProgress = 0.0;
	int switchAnimationId = -1;

	unsigned long innerClr;
	unsigned long borderClr;
	unsigned long Clr;
	unsigned long aInnerClr;
	unsigned long aBorderClr;
	unsigned long aClr;
	std::function<void()>func;
	bool value = false;
	bool preValue;
};
class VinaCaptionBar : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, const wchar_t* txt, unsigned long clr = VERTEXUICOLOR_MIDNIGHT, int TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE, int fixedSize = -1, int offset = 0 , std::function<void()>events = [] {})
	{
		this->func = events;
		this->Clr = clr;
		this->txtsz = TxtSize;
		this->txtClr = TxtColor;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
		this->fixedsize = fixedSize;
		this->offset = offset;
	}

	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		//D2DDrawSolidRect(hdc, x, y, cx, cy, Clr, 1);
		if (fixedsize != -1)
		{
			D2DDrawText2(hdc, txt.c_str(), x + 20+offset, y + 10, cx, cy, fixedsize, txtClr, L"Segoe UI", 1, false);
		}
		else
		{
			if (this->cx < 300)
			{
				D2DDrawText2(hdc, txt.c_str(), x + 20, y + 10, cx, cy, txtsz - 6, txtClr, L"Segoe UI", 1, false);
			}
			else if (this->cx < 400)
			{
				D2DDrawText2(hdc, txt.c_str(), x + 20, y + 10, cx, cy, txtsz - 3, txtClr, L"Segoe UI", 1, false);
			}
			else D2DDrawText2(hdc, txt.c_str(), x + 20, y + 10, cx, cy, txtsz, txtClr, L"Segoe UI", 1, false);
		}

	}

	virtual int OnMouseUp()
	{
		Refresh(hWnd);
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		Refresh(hWnd);
		func();
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{

		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {
			Refresh(hWnd);
		}
		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaCaptionBar> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}

	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:

	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	int fixedsize = -1;
	int offset = 0;
	std::function<void()>func;
	std::wstring text;
};

class VinaMultiTextBox : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, const wchar_t* txt, int TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE, unsigned long clr = VERTEXUICOLOR_MIDNIGHT)
	{
		//this->func = events;
		this->Clr = clr;
		this->txtsz = TxtSize;
		this->txtClr = TxtColor;
		this->txt = txt;
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
	}

	int GetTxtLine2(const wchar_t* str) {
		int n = 0;
		if (NULL == str) return 0;
		if (str[0] == 0) return 0;
		while (*str) { if (*str == L'\n') n++; str++; }
		if (str[-1] != L'\n') n++;//最后一行末尾没有'\n'也算一行
		return n;
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		ScrollDepth += this->GetParent()->GetInstantScrollDepth();


		int TextArea = (GetTxtLine2(this->txt.c_str()) * txtsz * gScale);
		//	MonitorValue(TextArea);
		ScrollDepth = GetMaxValue(GetMinValue(ScrollDepth, 1), TextArea - cy);
		//	MonitorValue(ScrollDepth);
		float SlideRate = static_cast<float>(TextArea) / cy;
		float BlankRate = static_cast<float>(TextArea) / GetMinValue(ScrollDepth, 1);
		float dist = (cy / BlankRate);
		float height = (cy / SlideRate);

		D2DDrawRoundRect(hdc, x, y, cx, cy, VuiFadeColor(VERTEXUICOLOR_MIDNIGHT, 10), 12, 1, 2, VERTEXUICOLOR_MIDNIGHTPLUS);
		D2DDrawInClippedRoundRect(hWnd, hdc, x, y, cx, cy, 12,
			[this, dist, height](HWND hWnd, HRT hdc2, int x, int y, int cx, int cy)->void {
				D2DDrawText(hdc2, this->txt.c_str(), x + 20, y + 20 - GetMinValue(ScrollDepth, 1), cx + 240, cy + 240, this->txtsz, VERTEXUICOLOR_WHITE);
				D2DDrawRoundRect(hdc2, x + cx - 6, y + dist, 5, height, VuiFadeColor(VERTEXUICOLOR_MIDNIGHT, 40), 4, 1, 0, VERTEXUICOLOR_MIDNIGHTPLUS);
			}
		);

	}

	virtual int OnMouseUp()
	{
		//ap = 0;
		Refresh(hWnd);
		//func();
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{
		//	ap = 0;
		this->IsPushed = true;
		Refresh(hWnd);
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{

		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {

			this->IsHoverd = true;
			Refresh(hWnd);
		}
		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr< VinaMultiTextBox> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}

	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:
	int ScrollDepth = 0;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};
class VinaBarrier : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, std::function<void()>fun = [] {})
	{
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
		this->func = fun;
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{

	}

	virtual int OnMouseUp()
	{
		//ap = 0;
		Refresh(hWnd);
		//func();
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{
		//	ap = 0;
		//this->IsPushed = true;
		func();
		Refresh(hWnd);
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{

		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();

		if (eventtype == vinaEvent::mouseOver) {

			this->IsHoverd = true;
	    	Refresh(hWnd);
		}
		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr< VinaBarrier> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}

	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:
	int ScrollDepth = 0;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};
class VinaReceiver : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, std::function<void(const std::vector<std::wstring>&)>fun = [](const std::vector<std::wstring>&) {},std::function<void()> rF=[]{})
	{
		this->x = x;
		this->y = y;
		this->cx = cx;
		this->cy = cy;
		this->func = fun;
		this->rFunc = rF;
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{

	}

	virtual int OnMouseUp()
	{
		//ap = 0;
		Refresh(hWnd);
		//func();
		//if(func)vinaFuncMap[_event.c_str()]();
		return 0;
	}
	virtual int OnMouseDown()
	{
		//	ap = 0;
		//this->IsPushed = true;
		
		Refresh(hWnd);
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{

		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();
		if (eventtype == vinaEvent::rMouseUp)this->rFunc();
		if (eventtype == vinaEvent::mouseOver) {
			if(vinaWinMap[hWnd]->aniMgr.GetActiveAnimationCount() == 0)Refresh(hWnd);
			this->IsHoverd = true;
		}
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype, std::vector<std::wstring> a)
	{
		if (eventtype == vinaEvent::fileDrop) {

			func(a);
			Refresh(hWnd);
			//WriteTESTUserData();

		}

		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr< VinaReceiver> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}

	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:
	int ScrollDepth = 0;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	std::function<void(const std::vector<std::wstring>&)>func;
	std::function<void()>rFunc = [] {};
	std::wstring text;
};
class VinaProgress : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, int Value = -1, unsigned long clr = VERTEXUICOLOR_DARKEN, const wchar_t* placeholder = L"")
	{
		this->txt = placeholder;
		this->x = x - 10;
		this->y = y - 10;
		this->cx = cx + 20;
		this->cy = cy + 20;
		if (Value != -1)this->Value = Value;
		this->Clr = clr;
	}

	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		RECT crc;

		double progress = ((double)(cx - 20) * Value) / 100.0;
		D2DDrawRoundRect(hdc, x + 10, y + 10, cx - 20, cy - 20, VuiFadeColor(Clr, 60), 8, 0.2f, 1.2f, VuiFadeColor(Clr, 100), 0.6f);
		D2DDrawRoundRect(hdc, x + 10, y + 10, progress, cy - 20, VuiFadeColor(Clr, 10), 8);
		D2DDrawText2(hdc, this->txt.c_str(), x + 20, y + cy / 2 - 9, cx - 20, cy - 20, 12, VuiFadeColor(Clr, 140), L"Segoe UI", 0.75f);
	}

	virtual int OnMouseUp()
	{

		Refresh(hWnd);
		return 0;
	}
	virtual int OnMouseDown()
	{

		Refresh(hWnd);
		return 0;
	}
	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype)
	{
		if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
		if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();
		if (eventtype == vinaEvent::mouseOver) {
			if (this->OnEdit == false)return 0;
			Refresh(hWnd);
		}

		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaProgress> vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	virtual VertexUIPos GetCurrentRect() {
		VertexUIPos _{ x,y,cx,cy };
		return _;
	}
	void SetInternalEvent(std::wstring ev)
	{
		this->_event = ev;
	}
	int GetValue()
	{
		return Value;
	}
	void SetValue(int val)
	{
		this->Value = val;
	}
	std::wstring txt;
	std::wstring c;
	std::wstring _event = L"";
	unsigned long Clr;

	int id = -1;
protected:
	bool OnEdit = false;
	int Value = 0;
	HWND hWnd;
	int ap = 0;
	int flag = 0;
	float txtsz = 15;
	unsigned long txtClr;
	std::function<void()>func;
	std::wstring text;
};
class VinaFileSelector : public VertexUIControl {
public:
	struct FileInfo {
		std::wstring name;
		std::wstring path;
		bool isDirectory;
		std::wstring iconPath;
		int x, y, cx, cy;
		bool useIconText;
		int ap = 0;
		int flag = 0;
		bool isSpecialFolder = false;
		FILETIME lastWriteTime = { 0, 0 };
	};

	enum class ViewMode { DriveSelection, FileBrowser };
	enum class SortMode { ByName, ByTime };

	bool evercreated = false;
	ViewMode currentMode = ViewMode::DriveSelection;
	SortMode currentSortMode = SortMode::ByName;

	void Set(int x, int y, int cx, int cy, unsigned long bgColor = VERTEXUICOLOR_MIDNIGHT) {
		this->x = x; this->y = y; this->cx = cx; this->cy = cy;
		this->bgColor = bgColor;
		this->itemHeight = 30;

		if (evercreated == false) {
			this->hoveredIndex = -1;
			this->selectedIndex = -1;
			this->scrollOffset = 0;
			currentMode = ViewMode::DriveSelection;
			currentSortMode = SortMode::ByName;
			currentPath = L"";
			RefreshFileList();
		}
		evercreated = true;
	}
	void SetPath(const std::wstring& newPath) {
		if (newPath.empty()) {
			currentMode = ViewMode::DriveSelection;
			currentPath = L"";
			RefreshFileList();
			if (pathChangedCallback) pathChangedCallback(L"");
			return;
		}

		DWORD dwAttrib = GetFileAttributesW(newPath.c_str());
		bool exists = (dwAttrib != INVALID_FILE_ATTRIBUTES);

		if (exists) {
			if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) {
				
				currentPath = newPath;
				if (currentPath.back() != L'\\') currentPath += L'\\';
				currentMode = ViewMode::FileBrowser;
				RefreshFileList();
			}
			else {
				
				size_t pos = newPath.find_last_of(L"\\/");
				if (pos != std::wstring::npos) {
					currentPath = newPath.substr(0, pos + 1);
					std::wstring fileName = newPath.substr(pos + 1);
					currentMode = ViewMode::FileBrowser;
					RefreshFileList();

		
					for (int i = 0; i < (int)fileList.size(); i++) {
						if (_wcsicmp(fileList[i].name.c_str(), fileName.c_str()) == 0) {
							selectedIndex = i;
				
							int visible = (cy - 50) / itemHeight;
							if (i < scrollOffset || i >= scrollOffset + visible) {
								scrollOffset = std::max(0, i - visible / 2);
							}
							break;
						}
					}
				}
			}
		
			if (pathChangedCallback) pathChangedCallback(currentPath);
		}
		else {
	
			if (pathChangedCallback) pathChangedCallback(currentPath.empty() ? L"" : currentPath);
		}
	}
	virtual void CreateCtl(HWND hWnd, HRT hdc) {
		if (!Isvalid) return;
		this->hWnd = hWnd;
		scrollOffset += this->GetParent()->GetInstantScrollDepth() / 6;
		scrollOffset = GetMinValue(scrollOffset, 0);

		D2DDrawRoundRect(hdc, x, y, cx, cy, bgColor, 8, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);

		DrawPathBar(hWnd, hdc);
		DrawSortButtons(hWnd, hdc); 
		DrawFileList(hWnd, hdc);
		DrawScrollBar(hWnd, hdc);
	}

	virtual int AddEvent(const vinaPoint& pt, vinaEvent eventtype) override {
		if (!Isvalid) return 0;
		if (eventtype == vinaEvent::mouseDown) {
			HandleMouseDown(pt);
			Refresh(hWnd);
		}
		if (eventtype == vinaEvent::mouseUp) {
			HandleMouseUp(pt);
			Refresh(hWnd);
		}
		if (eventtype == vinaEvent::mouseOver) {
			this->IsHoverd = true;
			HandleMouseMove(pt);
			Refresh(hWnd);
		}
		else {
			this->IsHoverd = false;
			hoveredIndex = -1;
		}
		return 0;
	}

	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, std::shared_ptr<VinaFileSelector> vuic) {
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}

	virtual VertexUIPos GetCurrentRect() override { return { x, y, cx, cy }; }

	void SetFileOpenCallback(std::function<void(const std::wstring&)> cb) { fileSelectedCallback = cb; }
	void SetPathDebugCallback(std::function<void(const std::wstring&)> cb) { pathChangedCallback = cb; }

private:

	VertexUIPos btnNameRect;
	VertexUIPos btnTimeRect;

	void DrawSortButtons(HWND hWnd, HRT hdc) {
		int btnSize = 30; int btnY = y + 5; int gap = 5;
		int btn2X = x + cx - gap - btnSize;
		int btn1X = btn2X - gap - btnSize;


		btnNameRect = { btn1X, btnY, btnSize, btnSize };
		btnTimeRect = { btn2X, btnY, btnSize, btnSize };

		unsigned long color1 = (currentSortMode == SortMode::ByName) ? VERTEXUICOLOR_DARKNIGHT : VuiFadeColor(bgColor, 20);
		D2DDrawRoundRect(hdc, btn1X, btnY, btnSize, btnSize, color1, 4, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);
		D2DDrawText(hdc, L"\uf15d", btn1X + 8, btnY + 7, btnSize, btnSize, 14, VERTEXUICOLOR_WHITE, L"Font Awesome 6 Free Solid", 1);

		unsigned long color2 = (currentSortMode == SortMode::ByTime) ? VERTEXUICOLOR_DARKNIGHT : VuiFadeColor(bgColor, 20);
		D2DDrawRoundRect(hdc, btn2X, btnY, btnSize, btnSize, color2, 4, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);
		D2DDrawText(hdc, L"\uf017", btn2X + 8, btnY + 7, btnSize, btnSize, 14, VERTEXUICOLOR_WHITE, L"Font Awesome 6 Free Solid", 1);
	}

	void DrawPathBar(HWND hWnd, HRT hdc) {
		int btnSize = 30; int gap = 5;
		int rightPadding = (gap + btnSize) * 2 + gap;
		int barX = x + 5; int barWidth = cx - 10 - rightPadding;
		D2DDrawRoundRect(hdc, barX, y + 5, barWidth, 30, VuiFadeColor(bgColor, 20), 6, 1, 1.5f, VERTEXUICOLOR_MIDNIGHTPLUS);
		std::wstring pathText = (currentMode == ViewMode::DriveSelection) ? L"计算机" : (currentPath.empty() ? L"计算机" : currentPath);
		D2DDrawText2(hdc, pathText.c_str(), barX + 10, y + 12, barWidth - 20, 20, 14, VERTEXUICOLOR_WHITE, L"Segoe UI", 1, false);
	}

	void DrawFileList(HWND hWnd, HRT hdc) {
		int maxScroll = std::max(0, (int)fileList.size() - (cy - 50) / itemHeight);
		scrollOffset = std::max(0, std::min(scrollOffset, maxScroll));
		int startY = y + 40; int visible = (cy - 50) / itemHeight;
		int startIndex = scrollOffset;
		int endIndex = std::min(startIndex + visible + 1, (int)fileList.size());

		for (int i = startIndex; i < endIndex; i++) {
			int itemY = startY + (i - startIndex) * itemHeight;
			fileList[i].x = x + 5; fileList[i].y = itemY; fileList[i].cx = cx - 30; fileList[i].cy = itemHeight - 2;
			if (itemY + itemHeight < y + 40 || itemY > y + cy - 10) continue;
			HandleItemAnimationInCreate(i);
			DrawItemBackground(hdc, i, x + 5, itemY, cx - 30, itemHeight - 2);

			if (fileList[i].useIconText) {
				D2DDrawText(hdc, L"\uf060", x + 15, itemY + 7, cx - 50, itemHeight - 4, 15, VERTEXUICOLOR_WHITE, L"Font Awesome 6 Free Solid", 1);
				std::wstring dName = GetParentFolderName(fileList[i].name);
				D2DDrawText2(hdc, dName.c_str(), x + 35, itemY + 8, cx - 50, itemHeight - 4, 13, VERTEXUICOLOR_WHITE, L"Segoe UI", 1, false);
			}
			else {
				D2DDisplayIcon(hdc, fileList[i].path.c_str(), x + 10, itemY + 5, 20);
				D2DDrawText2(hdc, fileList[i].name.c_str(), x + 35, itemY + 8, cx - 50, itemHeight - 4, 13, VERTEXUICOLOR_WHITE, L"Segoe UI", 1, false);
			}
		}
	}

	void HandleItemAnimationInCreate(int index) {
		FileInfo& f = fileList[index];
		if (index == hoveredIndex) {
			if (f.flag == 0) f.flag = 1;
			if (f.ap < 10) f.ap++;
			if (f.ap >= 10) f.flag = 0;
		}
		else {
			if (f.flag == 0) f.flag = 1;
			if (f.ap > 0) f.ap--;
			if (f.ap == 0) f.flag = 0;
		}
	}

	void DrawItemBackground(HRT hdc, int index, int x, int y, int cx, int cy) {
		FileInfo& f = fileList[index];
		float num = (f.ap > 0) ? CalcEaseOutCurve(f.ap, 0, 0.5, 10) : 0;
		unsigned long bgClr; unsigned long brdClr = VERTEXUICOLOR_MIDNIGHTPLUS;
		if (index == selectedIndex) {
			bgClr = VERTEXUICOLOR_SEA;
			D2DDrawRoundRect(hdc, x + num, y + num, cx - num * 2, cy - num * 2, bgClr, 4, 1, 1.0f, brdClr);
		}
		else if (f.ap > 0 || index == hoveredIndex) {
			bgClr = VuiFadeColor(VERTEXUICOLOR_MIDNIGHT, (int)(30 * (f.ap / 10.0f)));
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, bgClr, 4, 1, 1.0f + num * 0.3f, brdClr);
		}
	}

	void DrawScrollBar(HWND hWnd, HRT hdc) {
		if (fileList.size() * itemHeight <= cy - 50) return;
		int maxS = std::max(0, (int)fileList.size() - (cy - 50) / itemHeight);
		int vH = cy - 50; int tH = std::max(1, (int)fileList.size() * itemHeight);
		int sbH = std::max(20, (vH * vH) / tH);
		float sR = (float)scrollOffset / (float)maxS;
		int sbY = y + 40 + static_cast<int>(sR * (vH - sbH));
		D2DDrawRoundRect(hdc, x + cx - 15, y + 40, 10, vH, VuiFadeColor(bgColor, 10), 5, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);
		D2DDrawRoundRect(hdc, x + cx - 15, sbY, 10, sbH, VuiFadeColor(VERTEXUICOLOR_MIDNIGHTPLUS, 20), 5, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);
	}

	void HandleMouseDown(const vinaPoint& pt) { this->IsPushed = true; }

	void HandleMouseUp(const vinaPoint& pt) {
		this->IsPushed = false;

		if (GetPtInfo(pt, btnNameRect.x, btnNameRect.y, btnNameRect.cx, btnNameRect.cy)) {
			if (currentSortMode != SortMode::ByName) {
				currentSortMode = SortMode::ByName;
				SortFileList();
			}
			return;
		}
		if (GetPtInfo(pt, btnTimeRect.x, btnTimeRect.y, btnTimeRect.cx, btnTimeRect.cy)) {
			if (currentSortMode != SortMode::ByTime) {
				currentSortMode = SortMode::ByTime;
				SortFileList();
			}
			return;
		}

		int visible = (cy - 50) / itemHeight;
		int startIndex = scrollOffset;
		int endIndex = std::min(startIndex + visible + 1, (int)fileList.size());
		int clickedIdx = -1;
		for (int i = startIndex; i < endIndex; i++) {
			if (GetPtInfo(pt, fileList[i].x, fileList[i].y, fileList[i].cx, fileList[i].cy)) {
				clickedIdx = i; break;
			}
		}

		if (clickedIdx != -1) {
			selectedIndex = clickedIdx;
			FileInfo& file = fileList[selectedIndex];
			if (currentMode == ViewMode::DriveSelection) {
				currentMode = ViewMode::FileBrowser;
				currentPath = file.path;
				RefreshFileList();
			}
			else {
				if (file.isDirectory) {
					if (file.name == L"计算机") { currentMode = ViewMode::DriveSelection; currentPath = L""; }
					else { currentPath = file.path; }
					RefreshFileList();
				}
				else if (fileSelectedCallback) {
					fileSelectedCallback(file.path);
				}
			}
		}
	}

	void HandleMouseMove(const vinaPoint& pt) {
		int visible = (cy - 50) / itemHeight;
		int start = scrollOffset;
		int end = std::min(start + visible + 1, (int)fileList.size());
		hoveredIndex = -1;
		for (int i = start; i < end; i++) {
			if (GetPtInfo(pt, fileList[i].x, fileList[i].y, fileList[i].cx, fileList[i].cy)) {
				hoveredIndex = i; break;
			}
		}
	}

	void RefreshFileList() {
		fileList.clear();
		if (currentMode == ViewMode::DriveSelection) LoadDriveList();
		else LoadRealFileList();
		SortFileList();
		selectedIndex = -1; scrollOffset = 0; hoveredIndex = -1;
	}

	void SortFileList() {
		std::sort(fileList.begin(), fileList.end(), [&](const FileInfo& a, const FileInfo& b) -> bool {
			auto getP = [](const FileInfo& f) {
				if (f.name == L"..") return 0;
				if (f.name == L"计算机" && f.path.empty()) return 1;
				return f.isDirectory ? 2 : 3;
			};
			int pA = getP(a); int pB = getP(b);
			if (pA != pB) return pA < pB;
			if (currentSortMode == SortMode::ByTime) {
				ULARGE_INTEGER tA, tB;
				tA.LowPart = a.lastWriteTime.dwLowDateTime; tA.HighPart = a.lastWriteTime.dwHighDateTime;
				tB.LowPart = b.lastWriteTime.dwLowDateTime; tB.HighPart = b.lastWriteTime.dwHighDateTime;
				if (tA.QuadPart != tB.QuadPart) return tA.QuadPart > tB.QuadPart;
			}
			return _wcsicmp(a.name.c_str(), b.name.c_str()) < 0;
			});
	}

	void LoadDriveList() {
		wchar_t drvs[1024];
		if (GetLogicalDriveStringsW(1024, drvs)) {
			wchar_t* d = drvs;
			while (*d) {
				FileInfo info; info.name = d; info.path = d; info.isDirectory = true; info.useIconText = false;
				fileList.push_back(info);
				d += wcslen(d) + 1;
			}
		}
		auto addS = [&](REFKNOWNFOLDERID id, std::wstring name) {
			PWSTR path = nullptr;
			if (SUCCEEDED(SHGetKnownFolderPath(id, 0, NULL, &path))) {
				FileInfo info; info.name = name; info.path = std::wstring(path) + L"\\";
				info.isDirectory = true; info.useIconText = false;
				fileList.push_back(info); CoTaskMemFree(path);
			}
		};
		addS(FOLDERID_Desktop, L"桌面");
		addS(FOLDERID_Documents, L"文档");
	}

	void LoadRealFileList() {
		if (currentPath.empty()) return;
		FileInfo nav; nav.isDirectory = true; nav.useIconText = true;
		if (IsRootPath(currentPath)) { nav.name = L"计算机"; nav.path = L""; }
		else { nav.name = L".."; nav.path = GetParentPath(currentPath); }
		fileList.push_back(nav);

		WIN32_FIND_DATAW fd;
		HANDLE h = FindFirstFileW((currentPath + L"*").c_str(), &fd);
		if (h != INVALID_HANDLE_VALUE) {
			do {
				if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0) continue;
				FileInfo info; info.name = fd.cFileName; info.path = currentPath + fd.cFileName;
				info.useIconText = false;
				info.lastWriteTime = fd.ftLastWriteTime;
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { info.isDirectory = true; info.path += L"\\"; }
				else info.isDirectory = false;
				fileList.push_back(info);
			} while (FindNextFileW(h, &fd));
			FindClose(h);
		}
		if (pathChangedCallback) pathChangedCallback(currentPath);
	}

	std::wstring GetParentFolderName(const std::wstring& name) {
		if (name == L"..") {
			std::wstring p = GetParentPath(currentPath);
			if (p.length() > 3) {
				std::wstring s = p; s.pop_back(); size_t pos = s.find_last_of(L'\\');
				return (pos != std::wstring::npos) ? s.substr(pos + 1) : p;
			}
			return L"";
		}
		return name;
	}

	bool IsRootPath(const std::wstring& path) { return (path.length() == 3 && path[1] == L':' && path[2] == L'\\'); }

	std::wstring GetParentPath(const std::wstring& path) {
		if (path.length() <= 3) return path;
		std::wstring s = path; s.pop_back(); size_t pos = s.find_last_of(L'\\');
		return (pos != std::wstring::npos) ? s.substr(0, pos + 1) : L"C:\\";
	}

protected:
	std::wstring currentPath;
	std::vector<FileInfo> fileList;
	int itemHeight, scrollOffset, selectedIndex, hoveredIndex;
	unsigned long bgColor;
	std::function<void(const std::wstring&)> fileSelectedCallback, pathChangedCallback;
	bool Isvalid = true, IsHoverd = false, IsPushed = false;
	HWND hWnd;
};