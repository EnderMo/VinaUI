#pragma once
#include <variant>
#include "VinaInternalFunctionMapping.hpp"
using namespace VertexUI::Click;


typedef ID2D1DCRenderTarget* DCRT;
typedef ID2D1HwndRenderTarget* HRT;
typedef void (D2DHWNDDRAWLAYERPANEL)(HWND, ID2D1HwndRenderTarget*, int x, int y, int cx, int cy);
struct VertexUICtlColor
{
	unsigned long color = VERTEXUICOLOR_MIDNIGHT;
	unsigned long borderColor = VERTEXUICOLOR_MIDNIGHTPLUS;
	unsigned long innerColor =  VERTEXUICOLOR_WHITEDRAW;
	unsigned long hoverColor = VERTEXUICOLOR_MIDNIGHT;
	unsigned long hoverBorderColor = VERTEXUICOLOR_MIDNIGHTPLUS;
	unsigned long hoverInnerColor = VERTEXUICOLOR_WHITEDRAW;
	unsigned long activeColor = VERTEXUICOLOR_LAVENDER;
	unsigned long activeBorderColor = VERTEXUICOLOR_MIDNIGHTPLUS;
	unsigned long activeInnerColor = VERTEXUICOLOR_WHITEDRAW;

};
class VinaButton : public VertexUIControl{
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
			D2DDrawRoundRect(hdc, x + num, y + num, cx - num * 2, cy - num * 2, nClr, 8, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);

			D2DDrawText2(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz-num, txtClr, L"Segoe UI", 1, true);
		}
		else if (this->IsHoverd==true)
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
			num = CalcEaseOutCurve(ap, 0,0.5, 10);
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
			nR = GetMaxValue(GetRValue(Clr)+num*20,255);
			nG = GetMaxValue(GetGValue(Clr)+num*20,255);
			nB = GetMaxValue(GetBValue(Clr)+num*20,255);
			nClr = RGB(nR, nG, nB);
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 8, 1, 1.0f + num, VERTEXUICOLOR_MIDNIGHTPLUS);

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
			if (ap !=0)
			{
				int nR, nG, nB;
				nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
				nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
				nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
				nClr = RGB(nR, nG, nB);
			}
			D2DDrawRoundRect(hdc, x - num, y - num, cx + num * 2, cy + num * 2, nClr, 8, 1, 1.0f, VERTEXUICOLOR_MIDNIGHTPLUS);
			D2DDrawText2(hdc, txt.c_str(), x,(float)(y+cy/2-txtsz/1.5), cx, cy, txtsz, txtClr,L"Segoe UI",1,true);
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
	virtual int AddEvent(const vinaPoint& pt,vinaEvent eventtype)
	{
	
			if (eventtype == vinaEvent::mouseUp)this->OnMouseUp();
			if (eventtype == vinaEvent::mouseDown)this->OnMouseDown();
				
			if (eventtype == vinaEvent::mouseOver) { 
				
				this->IsHoverd = true;
				Refresh(hWnd);
		}
		return 0;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc,  std::shared_ptr<VinaButton> vuic)
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
	std::wstring _event=L"";
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
class VinaText : public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, const wchar_t* txt,int TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE , std::function<void()>events = [] {})
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
			nR = GetMaxValue(GetRValue(Clr) + num * 20*fact, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20*fact, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20*fact, 255);
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
				nR = GetMaxValue(GetRValue(Clr) + num * 20*fact, 255);
				nG = GetMaxValue(GetGValue(Clr) + num * 20 * fact, 255);
				nB = GetMaxValue(GetBValue(Clr) + num * 20 * fact, 255);
				nClr = RGB(nR, nG, nB);
			}
			D2DDrawText(hdc, txt.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Segoe UI", 1);
		}
	}
	virtual int AddEvent()
	{
		if (GetPtInfo(hWnd, x, y, cx, cy))
		{
			if (ClickMsg == 1)
			{
				ClickMsg = 0;

			}
			if (KeepDownMsg == 1)
			{

			}
			if (RClickMsg == 1)
			{
				RClickMsg = 0;

			}
			if (hState == 0)
			{
				Refresh(hWnd);
				hState = 1;
			}
			return 0;
		}
		return -1;
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
class VinaFAIcon : public VertexUIControl {
public:
	std::wstring CvtFont(std::wstring from)
	{
		if (from == std::wstring(L"test-right"))return std::wstring(L"\uf178");
	}
	void Set(int x, int y,  const wchar_t* txt, int TxtSize = 15, unsigned long TxtColor = VERTEXUICOLOR_WHITE, std::function<void()>events = [] {})
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
			D2DDrawText(hdc,newStr.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
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
			D2DDrawText(hdc, newStr.c_str(), x, (float)(y + cy / 2 - txtsz / 1.5), cx, cy, txtsz, nClr, L"Font Awesome 6 Free Solid", 1);
		}
	}
	virtual int AddEvent()
	{
		if (GetPtInfo(hWnd, x, y, cx, cy))
		{
			if (ClickMsg == 1)
			{
				ClickMsg = 0;

			}
			if (KeepDownMsg == 1)
			{

			}
			if (RClickMsg == 1)
			{
				RClickMsg = 0;

			}
			if (hState == 0)
			{
				Refresh(hWnd);
				hState = 1;
			}
			return 0;
		}
		return -1;
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
class VinaSwitch: public VertexUIControl {
public:
	void Set(int x, int y, int cx, int cy, VertexUICtlColor ci , std::function<void()>events = [] {} )
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
	virtual void CreateCtl(HWND hWnd, HRT hdc)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		if (GetPtInfo(hWnd, x, y, cx, cy))
		{

			if (flag == 0 )
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
			CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
				VertexUI::Window::SimpleShadow::iDropShadow Shadow;
				Shadow.SetSharpness(25);
				Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
				Shadow.SetSize(5 + num * 10);
				Shadow.SetDarkness(100 - (10 - ap) * 5);
				Shadow.SetPosition(0, 0);
				Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, cy/2	);
				});
			unsigned long nClr;
			int nR, nG, nB;
			nR = GetMaxValue(GetRValue(Clr) + num * 20, 255);
			nG = GetMaxValue(GetGValue(Clr) + num * 20, 255);
			nB = GetMaxValue(GetBValue(Clr) + num * 20, 255);
			nClr = RGB(nR, nG, nB);
			int dist = cy * 0.15;
			int innerBlockSize = cy - dist * 2;
			if (value == false)
			{
				D2DDrawRoundRect(hdc, x, y, cx, cy, nClr, cy / 2, 1, 1.0f, borderClr);
				D2DDrawRoundRect(hdc, x + dist, y + dist, innerBlockSize, innerBlockSize, innerClr, cy / 2, 1, 1.0f, borderClr);
			}
			else {
				D2DDrawRoundRect(hdc, x, y, cx, cy, aClr, cy / 2, 1, 1.0f, aBorderClr);
				D2DDrawRoundRect(hdc, x + cx - innerBlockSize - dist, y + dist, innerBlockSize, innerBlockSize, aInnerClr, cy / 2, 1, 1.0f, aBorderClr);
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
				CompGdiD2D(hWnd, hdc, [this, num](HWND hWnd, HDC hdc) {
					VertexUI::Window::SimpleShadow::iDropShadow Shadow;
					Shadow.SetSharpness(25);
					Shadow.SetColor(VuiCalcShadow(VERTEXUICOLOR_DARKNIGHT));
					Shadow.SetSize(5 + num * 10);
					Shadow.SetDarkness(100 - (10 - ap) * 5);
					Shadow.SetPosition(0, 0);
					Shadow.Create(hdc, this->x, this->y, this->cx, this->cy, cy / 2);
					});
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
			int dist = cy*0.15;
			int innerBlockSize = cy - dist*2;
			if (value == false)
			{
				D2DDrawRoundRect(hdc, x, y, cx, cy, nClr, cy / 2, 1, 1.0f, borderClr);
				D2DDrawRoundRect(hdc, x + dist, y + dist, innerBlockSize, innerBlockSize, innerClr, cy / 2, 1, 1.0f, borderClr);
			}
			else {
				D2DDrawRoundRect(hdc, x, y, cx, cy, aClr, cy / 2, 1, 1.0f, aBorderClr);
				D2DDrawRoundRect(hdc, x+cx-innerBlockSize-dist, y + dist, innerBlockSize, innerBlockSize, aInnerClr, cy / 2, 1, 1.0f, aBorderClr);
			}
		}
	}
	virtual int AddEvent()
	{
		if (GetPtInfo(hWnd, x, y, cx, cy))
		{
			if (ClickMsg == 1)
			{
				ClickMsg = 0;
				preValue = value;
				value = !value;
				Refresh(hWnd);
			}
			if (KeepDownMsg == 1)
			{

			}
			if (RClickMsg == 1)
			{
				RClickMsg = 0;

			}
			if (hState == 0)
			{
				Refresh(hWnd);
				hState = 1;
			}
			return 0;
		}
		return -1;
	}
	virtual void CreateInheritedCtl(HWND hWnd, HRT hdc, VinaSwitch* vuic)
	{
		this->hWnd = hWnd;
		CreateCtl(hWnd, hdc);
	}
	bool GetValue()
	{
		return this->value;
	}
	void SetValue(bool val)
	{
		this->value = val;
	}
	std::wstring txt;
	std::wstring c;
	std::wstring _event;


	int id = -1;
protected:
	int x, y, cx, cy;
	HWND hWnd;
	int ap = 0;
	int ap2 = 0;
	int flag = 0;
	unsigned long innerClr;
	unsigned long borderClr;
	unsigned long Clr;
	unsigned long aInnerClr;
	unsigned long aBorderClr;
	unsigned long aClr;
	std::function<void()>func;
	bool value=false;
	bool preValue;
};