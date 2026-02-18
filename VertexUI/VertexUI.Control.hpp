#pragma once
#include "framework.h"
#include <vector>
namespace VertexUI {
	struct vinaPoint {
		int x, y;
	};
	class vinaWindow {
	public:
		HWND hWnd;

	};
	enum class vinaLayout {
		Free,
		Left,
		Pushing,
		Flex
	};
	enum class vinaEvent {
		mouseDown,
		mouseUp,
		rMouseDown,
		rMouseUp,
		mouseOver,
		mouseUnfocus,
		keyDown,
		keyUp,
		fileDrop,
		ime_char,
		null,
	};
	class VertexUIColorGradientInfo
	{
	public:
		COLORREF From = 0;
		COLORREF To = 0;
		int Degree = 0;
	};
	class VertexUIColorInfo
	{
	public:
		COLORREF BkColor = VERTEXUICOLOR_SEA;
		COLORREF TxtColor = VERTEXUICOLOR_WHITE;
		COLORREF HoverColor = VERTEXUICOLOR_DEEPSEA;
		int BtnHoverMode = 0; //0:底部颜色 | 1:整体颜色 | 2:整体渐变
		VertexUIColorGradientInfo Gradient = { 0,0,0 };
		int TxtSize = 18;
		int Corner = 8;
	};
	class VertexUIColor {
	public:
		short r;
		short g;
		short b;
	};
	enum Layout
	{
		Free,
		Left,
		Pushing,
		Flex
	};
	struct VinaOption {
		std::wstring text;
		std::wstring description; // 新增：描述文本
		std::function<void()> onClick;
		std::function<void(bool)> b_onClick;
		std::function<void(POINT)> onHover;
		std::vector<VinaOption> subOptions;
		std::vector<VinaOption> hoverOptions;
		int activeStat = 1;
		bool isSwitch = false; // 新增：控制是否为 Switch 类型

		VinaOption() : text(L""), description(L""), activeStat(1), isSwitch(false) {}

		// 保持兼容性的构造函数（通过末尾默认参数增加 isSwitch）
		VinaOption(std::wstring t, std::function<void()> c, int a = 1, bool s = false)
			: text(t), description(L""), onClick(c), activeStat(a), isSwitch(s) {
		}

		VinaOption(std::vector<VinaOption> subs)
			: subOptions(subs), isSwitch(false) {
		}

		VinaOption(std::wstring t, std::vector<VinaOption> hovers, int a = 1, bool s = false)
			: text(t), description(L""), hoverOptions(hovers), activeStat(a), isSwitch(s) {
		}

		VinaOption(std::wstring t, std::wstring desc, std::function<void()> c, int a = 1, bool s = false)
			: text(t), description(desc), onClick(c), activeStat(a), isSwitch(s) {
		}
		VinaOption(std::wstring t, std::wstring desc, std::function<void(bool)> c, int a = 1, bool s = false)
			: text(t), description(desc), b_onClick(c), activeStat(a), isSwitch(s) {
		}
	};
	class VertexUIPos
	{
	public:
		int x = 0;
		int y = 0;
		int cx = 100;
		int cy = 100;
		bool operator == (const VertexUIPos& vp)
		{
			return ((x == vp.x) && (y == vp.y) && (cx == vp.cx) && (cy = vp.cy));
		}
	};
	void MonitorValue(int i)
	{
		wchar_t a[128];
		_itow(i, a, 10);
		OutputDebugString(a);
		OutputDebugString(L"\n");
		return;
	}
	void MonitorValue2(float f)
	{
		wchar_t a[128];
		swprintf_s(a, L"%f", f);  // 使用swprintf_s将float转换为宽字符字符串
		OutputDebugString(a);
		OutputDebugString(L"\n");
		return;
	}
	int GetPtInfo2(const vinaPoint& vp, int x, int y, int sizex, int sizey)
	{
		POINT pt;
		//wchar_t txt[80];
		pt.x = vp.x;
		pt.y = vp.y;
		//ClientToScreen(hWnd, &pt);
		RECT  rc = { x * gScale,y * gScale,(x + sizex) * gScale,(y + sizey) * gScale };
		if (PtInRect(&rc, pt))
		{
			return 1;
		}
		else
		{
			return 0;
		}
		return -1;
	}
	template < typename T >
	void ClearVector(std::vector< T >& vt)
	{
		std::vector< T > veTemp;
		veTemp.swap(vt);
	}
	class D2DVertexUIPanel;
	class VertexUIControl {
	public:
		virtual int CreateCtl(const wchar_t* type, HWND h, HDC hdc, int x, int y, int wid, int height, std::function<void()> func, const wchar_t* Text, VertexUIColorInfo vci) {
			return 0;
		}
		virtual int CreateCtl(const wchar_t* type, int x, int y, int wid, int height, std::function<void()> func, const wchar_t* Text, VertexUIColorInfo vci) {
			return 0;
		}
		virtual int AddEvent() {

			return -1;
		}
		virtual int AddEvent(const vinaPoint& pt, vinaEvent _) {

			return -1;
		}
		virtual int AddEvent(const vinaPoint& pt, vinaEvent _, std::wstring ExInfo) {

			return -1;
		}
		virtual int AddEvent(const vinaPoint& pt, vinaEvent _, std::vector<std::wstring> ExInfo) {

			return -1;
		}
		virtual int AddEvent(const vinaPoint& pt, vinaEvent _, unsigned long ExInfo) {

			return -1;
		}
		virtual int OnClick()
		{
			return 0;
		}
		virtual int SetInfo(VertexUIPos vp, const wchar_t* txt = nullptr, bool changepos = true)
		{
			this->IsInfoChanged = true;
			if (txt != nullptr)this->Text = txt;
			if (changepos == true)
			{
				this->x = vp.x;
				this->y = vp.y;
				this->cx = vp.cx;
				this->cy = vp.cy;
			}
			return 0;
		}
		virtual int SetInfo(int x, int y, int cx, int cy)
		{
			this->IsInfoChanged = true;

			this->x = x;
			this->y = y;
			this->cx = cx;
			this->cy = cy;

			return 0;
		}
		virtual void SetInheritance(VertexUIControl vuic)
		{
			this->CtlName = vuic.CtlName;
			if (this->Text == nullptr)Text = L"NULL";
			this->hWnd = vuic.hWnd;
			this->hdc = vuic.hdc;
		}
		virtual void CreateInheritedCtl(HWND hWnd, HDC hdc, VertexUIControl vuic)
		{
			CreateCtl(vuic.CtlName, hWnd, hdc, vuic.x, vuic.y, vuic.cy, vuic.cy, vuic.runfun, vuic.Text, vuic.pInfo);
		}
		virtual void CreateInheritedCtl(VertexUIControl vuic)
		{
			CreateCtl(vuic.CtlName, vuic.hWnd, vuic.hdc, vuic.x, vuic.y, vuic.cx, vuic.cy, vuic.runfun, vuic.Text, vuic.pInfo);
		}
		void SetParent(D2DVertexUIPanel* parent_)
		{
			parent = parent_;
		}
		D2DVertexUIPanel* GetParent()
		{
			return parent;
		}
		virtual VertexUIPos GetCurrentRect() {
			VertexUIPos _{ x,y,cx,cy };
			return _;
		}
		void ResetHoverFlag()
		{

			this->IsHoverd = false;
		}
		void ResetClickFlag()
		{
			this->IsPushed = false;
		}
		bool GetFocusFlag()
		{
			return this->IsFocused;
		}
		virtual void SetFocusFlag(bool b)
		{
			this->IsFocused=b;
		}
		int x, y, cx, cy;
	private:

		HWND hWnd;
		HDC hdc;
		const wchar_t* CtlName;
		std::function<void()> runfun;
		const wchar_t* Text;
		VertexUIColorInfo pInfo;
	protected:

		bool IsFocused = false;
		bool IsHoverd = false;
		bool IsPushed = false;
		bool IsInfoChanged = false;
		D2DVertexUIPanel* parent;
	};
	class DrawPanel
	{
	public:
		HWND hWnd;
		HDC hdc;
	};
	class VertexUILayoutInfo
	{
	public:
		VertexUILayoutInfo()
		{
		}
		VertexUILayoutInfo(vinaLayout Mode, int x, int y, int cx, int cy, int interval = 0)
		{
			this->Mode = Mode;
			this->x = x;
			this->y = y;
			this->uniCx = cx;
			this->uniCy = cy;
			this->interval = interval;
		}
		vinaLayout Mode = vinaLayout::Free;
		int x = 0;
		int y = 0;
		int uniCx = 0;
		int uniCy = 0;
		int interval = 0;
	};

	class VertexUIGroup
	{
	public:
		DrawPanel Handle()
		{
			DrawPanel p;
			p.hWnd = hWnd;
			p.hdc = hdc;
			return p;
		}
		void Handle(HWND h, HDC hdc)
		{
			this->hWnd = h; this->hdc = hdc;
		}
		void Add(VertexUIControl ctl)
		{
			//ctl.CreateInheritedCtl(this->Handle().hWnd, this->Handle().hdc, ctl);
			this->ctl.push_back(ctl);
		}
		std::vector<VertexUIControl>ctl;
	protected:
		HWND hWnd; HDC hdc;
	};
	class VertexUIPanel
	{
	public:
		int Set(HWND hWnd, HDC hdc)
		{
			ClearVector(Ctl);
			ClearVector(Group);
			ClearVector(CtlPtr);
			this->hWnd = hWnd;
			this->hdc = hdc;
			return 0;
		}
		int Set(HWND hWnd, HDC hdc, VertexUILayoutInfo lo)
		{
			ClearVector(Ctl);
			ClearVector(Group);
			ClearVector(CtlPtr);
			this->hWnd = hWnd;
			this->hdc = hdc;
			this->Layout = lo;
			return 0;
		}
		void Add(VertexUIGroup gp)
		{
			gp.Handle(this->hWnd, this->hdc);
			this->Group.push_back(gp);

			for (auto i : Group)
			{
				for (auto j : i.ctl)
				{
					j.CreateInheritedCtl(this->hWnd, this->hdc, j);
				}
			}

		}
		void Add(VertexUIControl ctl)
		{
			ctl.CreateInheritedCtl(hWnd, hdc, ctl);
			this->Ctl.push_back(ctl);
		}
		template <class T>
		void Add(T ctl)
		{
			SetLayout(ctl);
			ctl->CreateInheritedCtl(hWnd, hdc, ctl);
			this->CtlPtr.push_back(ctl);
		}
		virtual int AddEvent()
		{
			int ret = -1;
			/**
			if (Group.size() > 1)
			{
				for (auto i : Group)
				{
					for (auto j : i.ctl)
					{
						j.CreateInheritedCtl(hWnd, hdc, j);
						int preret = j.AddEvent();
						if (preret != -1)
						{
							ret = preret;
							break;
						}
					}
				}
			}
			if (Ctl.size() >= 1)
			{
				for (int i = 0; i < Ctl.size(); i++)
				{
					int preret = Ctl[i].AddEvent();
					if (preret != -1)
					{
						ret = preret;
						break;
					}
				}
			}
			*/
			if (CtlPtr.size() >= 1)
			{
				for (int i = 0; i < CtlPtr.size(); i++)
				{
					int preret = CtlPtr[i]->AddEvent();
					if (preret != -1)
					{
						ret = preret;
						break;
					}
				}
			}
			return ret;
		}
		virtual int AddEvent(const vinaPoint& pt, vinaEvent event_)
		{
			int ret = -1;
			if (CtlPtr.size() >= 1)
			{
				for (int i = 0; i < CtlPtr.size(); i++)
				{

					int preret = CtlPtr[i]->AddEvent(pt, event_);
					if (preret != -1)
					{
						ret = preret;
						break;
					}
				}
			}
			return ret;
		}
		virtual int GetInstantScrollDepth()
		{
			int prev = instScrollDepth;
			instScrollDepth = 0;
			return prev;
		}
		virtual void SetInstantScrollDepth(int i)
		{
			instScrollDepth = i;
		}
	protected:
		template <class T>
		void SetLayout(T ctl) {
			if (Layout.Mode == vinaLayout::Left)
			{
				RECT rc;
				GetClientRect(hWnd, &rc);
				int y = Layout.y;
				int OneSize = (Layout.uniCx + Layout.interval);
				int count = (rc.right - Layout.x) / OneSize;

				int CrLine = ceil((CtlPtr.size()) / count);
				OutputDebugString(L"CURRENT COUNT:\n");
				MonitorValue(count);
				ctl->SetInfo(Layout.x + OneSize * (CtlPtr.size()) - (count * CrLine) * OneSize, y + CrLine * (Layout.uniCy + Layout.interval), Layout.uniCx, Layout.uniCy);
			}
			if (Layout.Mode == vinaLayout::Pushing)
			{
				RECT rc;
				GetClientRect(hWnd, &rc);
				Layout.uniCx = rc.right * 0.25;
				int y = Layout.y;
				int OneSize = (Layout.uniCx + Layout.interval);
				int count = (rc.right - Layout.x) / OneSize;

				int CrLine = ceil((CtlPtr.size()) / count);
				OutputDebugString(L"CURRENT COUNT:\n");
				MonitorValue(count);
				ctl->SetInfo(Layout.x + OneSize * (CtlPtr.size()) - (count * CrLine) * OneSize, y + CrLine * (Layout.uniCy + Layout.interval), Layout.uniCx, Layout.uniCy);
			}
			if (Layout.Mode == vinaLayout::Flex)
			{
				RECT rc;
				GetClientRect(hWnd, &rc);

				int y = Layout.y;
				int OneSize = (Layout.uniCx + Layout.interval);
				int count = (rc.right - Layout.x) / OneSize;

				int CrLine = ceil((CtlPtr.size()) / count);

				ctl->SetInfo(Layout.x + ((rc.right / 2) - ((count * OneSize) + Layout.interval / 2) / 2) + OneSize * (CtlPtr.size()) - (count * CrLine) * OneSize, y + CrLine * (Layout.uniCy + Layout.interval), Layout.uniCx, Layout.uniCy);
			}
		}
		HWND hWnd;
		HDC hdc;
		int instScrollDepth = 0;
		std::vector<VertexUIGroup>Group;
		std::vector<VertexUIControl>Ctl;
		std::vector<std::shared_ptr<VertexUIControl>>CtlPtr;
		VertexUILayoutInfo Layout;

	};
	class D2DVertexUIPanel : public VertexUIPanel
	{
	public:
		/*
		int Set(VinaWindow* win, ID2D1HwndRenderTarget* hdc)
		{
			ClearVector(Ctl);
			ClearVector(Group);
			ClearVector(CtlPtr);
			this->hWnd = win->GetHandle();
			this->win = win;
			this->pDC = hdc;
			return 0;
		}
		*/
		int Set(HWND hWnd, ID2D1HwndRenderTarget* hdc)
		{
			ClearVector(Ctl);
			ClearVector(Group);
			ClearVector(CtlPtr);
			this->hWnd = hWnd;
			this->pDC = hdc;
			return 0;
		}
		int Set(HWND hWnd, ID2D1HwndRenderTarget* hdc, VertexUILayoutInfo lo)
		{
			ClearVector(Ctl);
			ClearVector(Group);
			ClearVector(CtlPtr);
			this->hWnd = hWnd;
			this->pDC = hdc;
			this->Layout = lo;
			return 0;
		}
		int Set(HWND hWnd, ID2D1HwndRenderTarget* hdc, int xo, int yo)
		{
			ClearVector(Ctl);
			ClearVector(Group);
			ClearVector(CtlPtr);
			this->hWnd = hWnd;
			this->pDC = hdc;
			this->xOffSet = xo;
			this->yOffSet = yo;
			return 0;
		}
		vinaPoint GetOffset()
		{
			vinaPoint pt;
			pt.x = xOffSet;
			pt.y = yOffSet;
			return pt;
		}
		virtual int AddEvent(const vinaPoint& pt, vinaEvent event_)
		{
			int ret = -1;

			if (CtlPtr.size() >= 1)
			{
				int iterPos = 0;
				for (int i = 0; i < CtlPtr.size(); i++)
				{
					CtlPtr[i]->ResetHoverFlag();
					if (event_ == vinaEvent::mouseUp || event_ == vinaEvent::mouseUnfocus ) {
						VertexUIPos vp = CtlPtr[i]->GetCurrentRect();
						if (GetPtInfo2(pt, vp.x, vp.y, vp.cx, vp.cy)==0 && event_ == vinaEvent::mouseUp)
						{
							CtlPtr[i]->SetFocusFlag(false);
						}
						CtlPtr[i]->ResetClickFlag();
					}
				}
				for (int i = CtlPtr.size() - 1; i >= 0; i--)
				{


					VertexUIPos vp = CtlPtr[i]->GetCurrentRect();
					if (GetPtInfo2(pt, vp.x, vp.y, vp.cx, vp.cy))
					{
						int preret = CtlPtr[i]->AddEvent(pt, event_);
						if (preret != -1)
						{
							ret = preret;
							break;
						}
					}
					iterPos++;

				}
			}

			return ret;
		}
		virtual int AddEvent(const vinaPoint& pt, vinaEvent event_, std::wstring ExInfo)
		{
			int ret = -1;

			if (CtlPtr.size() >= 1)
			{
				for (int i = 0; i < CtlPtr.size(); i++)
				{
					CtlPtr[i]->ResetHoverFlag();
					if (event_ == vinaEvent::mouseUp || event_ == vinaEvent::mouseUnfocus)CtlPtr[i]->ResetClickFlag();
				}
				int iterPos = 0;
			

				for (int i = CtlPtr.size() - 1; i >= 0; i--)
				{


					VertexUIPos vp = CtlPtr[i]->GetCurrentRect();
					if (GetPtInfo2(pt, (vp.x + xOffSet), (vp.y + yOffSet), vp.cx, vp.cy))
					{
						int preret = CtlPtr[i]->AddEvent(pt, event_, ExInfo);
						if (preret != -1)
						{
							ret = preret;
							break;
						}
					}
					iterPos++;

				}
			}

			return ret;
		}
		virtual int AddEvent(const vinaPoint& pt, vinaEvent event_, std::vector<std::wstring> ExInfo)
		{
			int ret = -1;

			if (CtlPtr.size() >= 1)
			{
				int iterPos = 0;
				for (int i = 0; i < CtlPtr.size(); i++)
				{
					CtlPtr[i]->ResetHoverFlag();
					if (event_ == vinaEvent::mouseUp || event_ == vinaEvent::mouseUnfocus)CtlPtr[i]->ResetClickFlag();
				}
				for (int i = CtlPtr.size() - 1; i >= 0; i--)
				{


					VertexUIPos vp = CtlPtr[i]->GetCurrentRect();
					if (GetPtInfo2(pt, (vp.x + xOffSet), (vp.y + yOffSet), vp.cx, vp.cy))
					{
						int preret = CtlPtr[i]->AddEvent(pt, event_, ExInfo);
						if (preret != -1)
						{
							ret = preret;
							break;
						}
					}
					iterPos++;

				}
			}

			return ret;
		}
		virtual int AddEvent(const vinaPoint& pt, vinaEvent event_, unsigned long ExInfo)
		{
			int ret = -1;

			if (CtlPtr.size() >= 1)
			{
				int iterPos = 0;
				for (int i = 0; i < CtlPtr.size(); i++)
				{
					CtlPtr[i]->ResetHoverFlag();
					if (event_ == vinaEvent::mouseUp || event_ == vinaEvent::mouseUnfocus)CtlPtr[i]->ResetClickFlag();
				}
				if (event_ == vinaEvent::keyDown || event_ == vinaEvent::ime_char) {
					for (auto& ctl : CtlPtr) {
						if (ctl->GetFocusFlag()) {
							return ctl->AddEvent(pt, event_, ExInfo);
						}
					}
				}
				for (int i = CtlPtr.size() - 1; i >= 0; i--)
				{


					VertexUIPos vp = CtlPtr[i]->GetCurrentRect();
					if (GetPtInfo2(pt, vp.x / gScale, vp.y / gScale, vp.cx / gScale, vp.cy / gScale))
					{

						int preret = CtlPtr[i]->AddEvent(pt, event_, ExInfo);
						if (preret != -1)
						{
							ret = preret;
							break;
						}
					}
					iterPos++;

				}
			}

			return ret;
		}
		virtual int AddEvent()
		{
			int ret = -1;

			if (CtlPtr.size() >= 1)
			{
				for (int i = 0; i < CtlPtr.size(); i++)
				{
					int preret = CtlPtr[i]->AddEvent();
					if (preret != -1)
					{
						ret = preret;
						break;
					}
				}
			}

			return ret;
		}
		/*
		VinaWindow* GetWindow()
		{
			return this->win;
		}
		*/
		template <class T>
		void Add(T ctl)
		{
			SetLayout(ctl);
			ctl->CreateInheritedCtl(hWnd, pDC, ctl);
			ctl->SetParent(this);
			this->CtlPtr.push_back(ctl);
		}
	private:
			int xOffSet = 0;
			int yOffSet = 0;
	protected:
		ID2D1HwndRenderTarget* pDC;
		//VinaWindow* win;
		bool hoverHandled = false;
	};
		template<typename T>
		class VinaLayout {
		private:
			int _startX, _startY, _itemWidth, _itemHeight, _gap, _boundWidth;
			int _curX, _curY;

		public:
			// 初始化排版参数
			void Begin(int startX, int startY, int itemW, int itemH, int gap, int boundWidth) {
				_startX = startX;
				_startY = startY;
				_itemWidth = itemW;
				_itemHeight = itemH;
				_gap = gap;
				_boundWidth = boundWidth;

				_curX = startX;
				_curY = startY;
			}

			// 核心方法：传入组件，自动计算并设置它的位置，然后移动指针
			void AddElement(std::shared_ptr<T> element) {
				if (!element) return;

				// 检查是否需要换行：如果当前 X + 组件宽度超过了边界，且当前行不是空的
				if (_curX + _itemWidth > _boundWidth && _curX > _startX) {
					_curX = _startX;
					_curY += _itemHeight + _gap;
				}

				// 直接设置组件位置
				element->SetInfo(_curX, _curY, _itemWidth, _itemHeight);

				// 指针向后移动
				_curX += _itemWidth + _gap;
			}

			// 获取当前排版到的高度（方便后续接其他内容）
			int GetCurrentY() { return _curY + _itemHeight; }
		};
}