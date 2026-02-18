#pragma once
#include "VinaCommonCtls.hpp"
#include <Windowsx.h>
#include <ole2.h>
#include <shlobj.h>
#include <shellapi.h>
#include "VinaFileDrag.hpp"
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")

/*Vina Window Loader API
* By Vilinko,CimiMoly
* Copyright 2026
* All rights reserved
*/
typedef int RUNFUN;
#define LFM_EXTENSION 0xff1
#define LFM_ONCREATEEXTENSION 0xff2
#define LFM_REFRESHEFFECT 0xff4
#define VINA_DROP 0xff6
#define VINA_TRAY_MSG 0xff7 

class gDropInfo {
public:
	bool IsDragging;
	bool DelayedDragStatus;
	std::vector<std::wstring> fileName;
	std::wstring filenamealias;
};


gDropInfo g_DropInfo;
void replace_all2(std::wstring& inout, std::wstring what, std::wstring with) //跟着cppreference抄的（
{
	for (std::wstring::size_type pos{};
		inout.npos != (pos = inout.find(what.data(), pos, what.length()));
		pos += with.size()) {
		inout.replace(pos, what.length(), with.data(), with.length());
	}
}
std::wstring GenerateRandomString(int len) {
	if (len <= 0) {
		return L"";
	}
	const std::wstring charset = L"0123456789abcdef";
	thread_local static std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<size_t> distribution(0, charset.size() - 1);

	std::wstring result;
	result.reserve(len);
	for (int i = 0; i < len; ++i) {
		result += charset[distribution(generator)];
	}
	return result;
}

class AnimationManager {
private:
	struct AnimationTask {
		int id;
		std::function<double(double)> progressFunc; 
		std::function<void(double)> updateFunc;     
		std::function<void()> completeFunc;       
		double startTime;
		double duration;
		bool isActive;
		HWND targetHwnd; 
	};

	std::vector<AnimationTask> tasks;
	int nextId = 1;
	static constexpr double FRAME_RATE = 10.0;  //60FPS

public:
	// 创建基础动画
	int CreateAnimation(HWND hwnd, double duration,
		std::function<double(double)> progressFunc,
		std::function<void(double)> updateFunc,
		std::function<void()> completeFunc = nullptr) {
		AnimationTask task;
		task.id = nextId++;
		task.progressFunc = progressFunc ? progressFunc : [](double t) { return t; };
		task.updateFunc = updateFunc;
		task.completeFunc = completeFunc;
		task.startTime = GetTickCount64() / 1000.0;
		task.duration = duration;
		task.isActive = true;
		task.targetHwnd = hwnd;

		tasks.push_back(task);
		return task.id;
	}

	template<typename T>
	int AnimateVariable(HWND hwnd, T& variable, T from, T to, double duration,
		std::function<double(double)> easingFunc = nullptr,
		std::function<void()> onComplete = nullptr) {
		auto updateFunc = [&variable, from, to](double progress) {
			if constexpr (std::is_arithmetic_v<T>) {
				variable = static_cast<T>(from + (to - from) * progress);
			}
			else if constexpr (std::is_same_v<T, float>) {
				variable = static_cast<float>(from + (to - from) * progress);
			}
			else if constexpr (std::is_same_v<T, double>) {
				variable = from + (to - from) * progress;
			}
		};

		if (!easingFunc) {
			easingFunc = [](double t) { return t; };  
		}

		return CreateAnimation(hwnd, duration, easingFunc, updateFunc, onComplete);
	}

	template<typename T>
	int AnimateVariableWithBezier(HWND hwnd, T& variable, T from, T to, double duration,
		double p1x = 0.25, double p1y = 0.1, double p2x = 0.25, double p2y = 1.0,
		std::function<void()> onComplete = nullptr) {
		auto easingFunc = [p1x, p1y, p2x, p2y](double t) -> double {
			return CalcBezierCurve(t, 0, 1, 1, p1x, p1y, p2x, p2y);
		};

		return AnimateVariable(hwnd, variable, from, to, duration, easingFunc, onComplete);
	}

	void UpdateAnimations() {
		double currentTime = GetTickCount64() / 1000.0;

		for (auto& task : tasks) {
			if (!task.isActive) continue;

			double elapsed = currentTime - task.startTime;
			double progress = std::min(elapsed / task.duration, 1.0);

			if (elapsed >= task.duration) {
	
				if (task.progressFunc) {
					double finalProgress = task.progressFunc(1.0);
					task.updateFunc(finalProgress);
				}
				task.isActive = false;

		
				if (task.completeFunc) {
					task.completeFunc();
				}

				
				if (task.targetHwnd) {
					Refresh(task.targetHwnd);
				}
			}
			else {
				
				if (task.progressFunc) {
					double currentProgress = task.progressFunc(progress);
					task.updateFunc(currentProgress);
				}
				if (task.targetHwnd) {
					Refresh(task.targetHwnd);
				}
			}
		}

	
		Cleanup();
	}


	void RemoveAnimation(int id) {
		auto it = std::find_if(tasks.begin(), tasks.end(),
			[id](const AnimationTask& t) { return t.id == id; });
		if (it != tasks.end()) {
			it->isActive = false;
		}
	}


	void Cleanup() {
		tasks.erase(
			std::remove_if(tasks.begin(), tasks.end(),
				[](const AnimationTask& t) { return !t.isActive; }),
			tasks.end()
		);
	}

	int GetActiveAnimationCount() const {
		return std::count_if(tasks.begin(), tasks.end(),
			[](const AnimationTask& t) { return t.isActive; });
	}
};


using FunctionalProc = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;
static std::unordered_map < std::wstring, FunctionalProc> ProcManager;


class VinaDropTarget : public IDropTarget
{
public:
	VinaDropTarget(HWND hwnd, DWORD dwEffect) : m_hWnd(hwnd), m_lRefCount(1), m_dwEffect(dwEffect) {}


	STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override {
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget)) {
			*ppv = this;
			AddRef();
			return S_OK;
		}
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) AddRef() override {
		return InterlockedIncrement(&m_lRefCount);
	}

	STDMETHODIMP_(ULONG) Release() override {
		LONG count = InterlockedDecrement(&m_lRefCount);
		if (count == 0) {
			delete this;
			return 0;
		}
		return count;
	}


	STDMETHODIMP DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override {

		FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		if (pDataObj->QueryGetData(&fmt) == S_OK) {
			*pdwEffect = m_dwEffect;
			g_DropInfo.IsDragging = true; 
			g_DropInfo.DelayedDragStatus = true;

			STGMEDIUM stg;
			if (pDataObj->GetData(&fmt, &stg) == S_OK) {
				HDROP hDrop = (HDROP)stg.hGlobal;
				UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

				g_DropInfo.fileName.clear();
				std::wstring msgStr = L"Detected Drop Files (on DragEnter):\n";

				if (fileCount > 0) {
					for (UINT i = 0; i < fileCount; ++i) {
						WCHAR szFile[MAX_PATH];
						if (DragQueryFile(hDrop, i, szFile, MAX_PATH)) {
							std::wstring filePath(szFile);
							g_DropInfo.fileName.push_back(filePath);
							msgStr += filePath + L"\n";
						}
					}
					Refresh(m_hWnd);


				}

				ReleaseStgMedium(&stg);
			}

		}
		else {
			*pdwEffect = DROPEFFECT_NONE;
			g_DropInfo.IsDragging = false;
		}
		return S_OK;
	}

	STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override {

		if (g_DropInfo.IsDragging) {
			*pdwEffect = m_dwEffect;
		}
		else {
			*pdwEffect = DROPEFFECT_NONE;
		}
		POINT pt2;
		GetCursorPos(&pt2);
		ScreenToClient(m_hWnd, &pt2);
		LPARAM lParam = MAKELPARAM(pt2.x, pt2.y);
		WPARAM wParam = 0;

		PostMessage(m_hWnd, WM_MOUSEMOVE, wParam, lParam);
		return S_OK;
	}

	STDMETHODIMP DragLeave() override {
		g_DropInfo.IsDragging = false; //重置拖拽状态
		return S_OK;
	}

	STDMETHODIMP Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override {
		g_DropInfo.IsDragging = false;
		g_DropInfo.DelayedDragStatus = true;
		*pdwEffect = m_dwEffect;

		// Drop
		FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		STGMEDIUM stg;

		POINT pt2;
		GetCursorPos(&pt2);
		if (!PtInRect(&VinaDragHelper::_rc, pt2)) {

			SendMessage(m_hWnd, VINA_DROP, 0, 0);
		}

		if (pDataObj->GetData(&fmt, &stg) == S_OK) {
			HDROP hDrop = (HDROP)stg.hGlobal;
			UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

			if (fileCount > 0) {
				*pdwEffect = m_dwEffect;
			}
			ReleaseStgMedium(&stg);
		}
		return S_OK;
	}

private:
	LONG m_lRefCount;
	HWND m_hWnd;
	DWORD m_dwEffect;
};

class VinaWindow;
std::unordered_map<HWND, VinaWindow* > vinaWinMap;
class VinaWindow
{

public:
	enum outFrame {
		Normal, Disable, Client
	};
	void Set(int x, int y, int w, int h, const wchar_t* cls, const wchar_t* name)
	{
		this->x = x;
		this->y = y;
		this->Width = w;
		this->Height = h;
		this->szWindowClass = cls;
		this->szTitle = name;
		this->SetProc([this](HWND hWnd, UINT umsg, WPARAM wParam, LPARAM lParam)->LRESULT {
			switch (umsg)
			{
			case WM_NCCALCSIZE:
			{
				if (this->NoOuterFrame == outFrame::Client)
				{
					auto p = (NCCALCSIZE_PARAMS*)lParam;
					p->rgrc->top++;
				}
				else return DefWindowProc(hWnd, umsg, wParam, lParam);
				break;
			}
			case WM_CREATE:
			{
				// 注册拖放目标
				OleInitialize(NULL);
				if (this->m_pDropTarget == nullptr) {
					this->m_pDropTarget = new VinaDropTarget(hWnd, this->m_dwDropEffect);
					RegisterDragDrop(hWnd, this->m_pDropTarget);
				}
				break;
			}
			case WM_COMMAND:
			{
				int wmId = LOWORD(wParam);
				// 分析菜单选择:
				switch (wmId)
				{

				default:
					return DefWindowProc(hWnd, umsg, wParam, lParam);
				}
			}
			break;
			case WM_SIZE:
			{
				D2D1_SIZE_U resize;
				resize.height = HIWORD(lParam);
				resize.width = LOWORD(lParam);
				if (pRts[hWnd] != NULL)pRts[hWnd]->Resize(resize);
				break;
			}
			case WM_NCHITTEST:
			{
				if (this->NoOuterFrame == outFrame::Client)
				{
					POINT pt;
					pt.x = GET_X_LPARAM(lParam);
					pt.y = GET_Y_LPARAM(lParam);
					::ScreenToClient(hWnd, &pt);

					RECT rcClient;
					::GetClientRect(hWnd, &rcClient);

					if (pt.x < rcClient.left + 5 && pt.y < rcClient.top + 5) {
						return HTTOPLEFT;
					}
					else if (pt.x > rcClient.right - 5 && pt.y < rcClient.top + 5)
					{
						return HTTOPRIGHT;
					}
					else if (pt.x<rcClient.left + 5 && pt.y>rcClient.bottom - 5)
					{
						return HTBOTTOMLEFT;
					}
					else if (pt.x > rcClient.right - 5 && pt.y > rcClient.bottom - 5)
					{
						return HTBOTTOMRIGHT;
					}
					else if (pt.x < rcClient.left + 5)
					{
						return HTLEFT;
					}
					else if (pt.x > rcClient.right - 5)
					{
						return HTRIGHT;
					}
					else if (pt.y < rcClient.top + 5)
					{
						return HTTOP;
					}if (pt.y > rcClient.bottom - 5)
					{
						return HTBOTTOM;
					}
					else
					{
						return DefWindowProc(hWnd, umsg, wParam, lParam);
					}
				}
				else
					return DefWindowProc(hWnd, umsg, wParam, lParam);;
				break;
			}
			case   WM_MOUSEMOVE:
			{

				RECT winrc;
				GetClientRect(hWnd, &winrc);
				vinaPoint pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.dwHoverTime = HOVER_DEFAULT;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);
				if (aniMgr.GetActiveAnimationCount() != 0)
				{
					if (this->GetStrictEventDelivery() == false)
					{
						break;
					}
					else if (g_DropInfo.IsDragging == true || g_DropInfo.DelayedDragStatus == true)break;
				}
				else
				{
					/*
					*延迟机制:
					* 因为拖拽状态结束后动画还在播放，鼠标移动引发的Refresh会阻塞窗口，从而导致动画卡顿。
					* 加入延迟即可解决。
					*/
					g_DropInfo.DelayedDragStatus = false;
				}
				if (this->NewUIPanel->AddEvent(pt, vinaEvent::mouseOver)) {
					// 只在动画不活跃时刷新
					if (aniMgr.GetActiveAnimationCount() == 0) {
						Refresh(hWnd);
					}
				}
				break;
			}
			case WM_MOUSELEAVE: {
				vinaPoint pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				this->NewUIPanel->AddEvent(pt, vinaEvent::mouseUnfocus);
				// 只在动画不活跃时刷新
				if (aniMgr.GetActiveAnimationCount() == 0) {
					Refresh(hWnd);
				}
				break;
			}
			case WM_TIMER:
			{
				switch (wParam)
				{
				case 10:
				{
					// 只在动画不活跃时刷新
					if (aniMgr.GetActiveAnimationCount() == 0) {
						Refresh(hWnd);
					}
					break;
				}
				case 11:
				{
					if (GlobalAnimationCount >= 1)
					{
						// 只在动画不活跃时刷新
						if (aniMgr.GetActiveAnimationCount() == 0) {
							Refresh(hWnd);
						}
					}
					break;
				}
				case 12:
				{
					aniMgr.UpdateAnimations();
					break;
				}
				}
				break;
			}
			case WM_LBUTTONDOWN:
			{
				vinaPoint pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				this->NewUIPanel->AddEvent(pt, vinaEvent::mouseDown);
				break;
			}
			case WM_LBUTTONUP:
			{
				vinaPoint pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				if (VinaDragHelper::idrag == false)this->NewUIPanel->AddEvent(pt, vinaEvent::mouseUp);
				break;
			}
			case WM_RBUTTONUP:
			{
				vinaPoint pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				this->NewUIPanel->AddEvent(pt, vinaEvent::rMouseUp);
				break;
			}
			case WM_SIZING:
			{
				if (sizingfunc != __noop)this->sizingfunc(hWnd);
				break;
			}
			case WM_MOUSEWHEEL:
			{

				RECT rc;
				GetClientRect(hWnd, &rc);
				short stat = HIWORD(wParam);

				this->NewUIPanel->SetInstantScrollDepth(-(stat * 0.15));
				// stat * 0.15;


				// 只在动画不活跃时刷新
			//	i(aniMgr.GetActiveAnimationCount() == 0) {
				SendMessage(hWnd, WM_MOUSEMOVE, wParam, lParam);
				Refresh(hWnd);
				//}
				break;
			}
			case WM_KEYDOWN:
			{
				POINT pt2;
				GetCursorPos(&pt2);
				ScreenToClient(hWnd, &pt2);
				vinaPoint pt;
				pt.x = pt2.x / gScale;
				pt.y = pt2.y / gScale;
				this->NewUIPanel->AddEvent(pt, vinaEvent::keyDown, wParam);
				Refresh(hWnd);

				break;
			}
			case WM_CHAR:
			{
				POINT pt2;
				GetCursorPos(&pt2);
				ScreenToClient(hWnd, &pt2);
				vinaPoint pt;
				pt.x = pt2.x / gScale;
				pt.y = pt2.y / gScale;
				this->NewUIPanel->AddEvent(pt, vinaEvent::ime_char, wParam);
				Refresh(hWnd);

				break;
			}
			case WM_ERASEBKGND:
			{
				return (LPARAM)1;
			}
			/*
			case WM_DROPFILES:
			{
				using namespace::std;
				HDROP hDrop = (HDROP)wParam;

				TCHAR filePath[32 * MAX_PATH] = { 0 };

				int nFileCount;

				nFileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

				//文件路径
				for (int i = 0; i < nFileCount; i++)
				{
					DragQueryFile(hDrop, i, (LPTSTR)filePath, 1024);
					wstring wstrFile = filePath;



				}

				RECT rc;
				GetWindowRect(hWnd, &rc);
				POINT pt;
				GetCursorPos(&pt);
				pt.x -= rc.left;
				pt.y -= rc.top;
				vinaPoint vpt{ pt.x,pt.y };
				this->NewUIPanel->AddEvent(vpt, vinaEvent::fileDrop, std::wstring(filePath));
				DragFinish(hDrop);
				break;
			}
			*/
			case VINA_DROP:
			{
				RECT rc;
				GetWindowRect(hWnd, &rc);
				POINT pt;
				GetCursorPos(&pt);
				pt.x -= rc.left;
				pt.y -= rc.top;
				vinaPoint vpt{ pt.x,pt.y };
	
				this->NewUIPanel->AddEvent(vpt, vinaEvent::fileDrop, g_DropInfo.fileName);

				break;
			}
			case VINA_TRAY_MSG:
			{
				if (lParam == WM_LBUTTONUP && this->onTrayLeftClick) {
					this->onTrayLeftClick();
				}
				else if (lParam == WM_RBUTTONUP && this->onTrayRightClick) {
					this->onTrayRightClick();
				}
				break;
			}
			case WM_ACTIVATE:
			{
				if (this->TempFloat == true)
				{
					BOOL fActive = LOWORD(wParam);
					HWND hOther = (HWND)lParam;

					if (fActive == WA_INACTIVE)
					{
						// 如果外部注册了校验逻辑，则询问外部
						bool focusGoesToChild = false;
						if (this->IsFocusValidFunc) {
							focusGoesToChild = this->IsFocusValidFunc(hOther);
						}

						if (!focusGoesToChild)
						{
							KillTimer(hWnd, 10);
							KillTimer(hWnd, 11);
							ActivateExit();
							// 发送销毁消息
							PostMessage(hWnd, WM_CLOSE, 0, 0);
						}
					}
				}
				break;
			}
			case WM_WINDOWPOSCHANGING:
			case WM_WINDOWPOSCHANGED:
			{
				if (this->IsTopMost == true)
				{
					WINDOWPOS* pPos = (WINDOWPOS*)lParam;
					pPos->hwndInsertAfter = HWND_TOPMOST;
				}
				else if (this->IsBottom == true)
				{
					WINDOWPOS* pPos = (WINDOWPOS*)lParam;
					pPos->hwndInsertAfter = HWND_BOTTOM;
				}
				if (this->sizingfunc)this->sizingfunc(hWnd);
				return DefWindowProc(hWnd, umsg, wParam, lParam);
			}
			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: 在此处添加使用 hdc 的任何绘图代码...
				CreateD2DPanel(hWnd, this->UI);
				EndPaint(hWnd, &ps);
			}
			break;
			case WM_DESTROY:
	
				RemoveTrayIcon();
				
				if (this->m_pDropTarget) {
					RevokeDragDrop(hWnd);
					this->m_pDropTarget->Release();
					this->m_pDropTarget = nullptr;
				}
				OleUninitialize();

				KillTimer(hWnd, 10);
				KillTimer(hWnd, 11);
				StopAnimationSystem();
				DestroyWindow(hWnd);
				break;
			default:
				if (this->extrafunc != nullptr)
				{
					return this->extrafunc(hWnd, umsg, wParam, lParam);
				}
				return DefWindowProc(hWnd, umsg, wParam, lParam);
			}
			return 0;
			});
	}
	void CreatePanel(std::function<void(HWND, ID2D1HwndRenderTarget*)>  UI)
	{
		this->UI = UI;
	}
	void SetWindowInfo(const wchar_t* cls, const wchar_t* name)
	{
		szTitle = name;
		szWindowClass = cls;
	}
	void SetWindowSize(int w, int h)
	{
		this->Width = w;
		this->Height = h;
		if (hWnd != 0)SetWindowPos(hWnd, 0, 0, 0, w, h, SWP_NOMOVE);
	}
	void SetPos(int w, int h)
	{
		this->x = w;
		this->y = h;
		if (hWnd != 0)SetWindowPos(hWnd, 0, w, h, 0, 0, SWP_NOSIZE);
	}
	void SetProc(FunctionalProc proc)
	{
		ProcManager[std::wstring(this->szWindowClass)] = proc;
	}
	void InitAnimation() // 旧版的动画函数
	{
		SetTimer(hWnd, 11, 16, 0);
	}
	void KillAnimation() // 旧版的动画函数
	{
		GlobalAnimationCount = 0;
		KillTimer(hWnd, 11);
	}
	void StartAnimationSystem() // 新版的动画函数
	{
		SetTimer(this->hWnd, 12, 10, 0);
	}
	void StopAnimationSystem() // 新版的动画函数
	{
		KillTimer(this->hWnd, 12);
	}

	template<typename T>
	int AnimateVariableWithBezier(HWND hwnd, T& variable, T from, T to, double duration,
		double p1x = 0.25, double p1y = 0.1, double p2x = 0.25, double p2y = 1.0,
		std::function<void()> onComplete = nullptr) {
		return aniMgr.AnimateVariableWithBezier(hwnd, variable, from, to, duration, p1x, p1y, p2x, p2y, onComplete);
	}


	template<typename T>
	int AnimateVariable(HWND hwnd, T& variable, T from, T to, double duration,
		std::function<double(double)> easingFunc = nullptr,
		std::function<void()> onComplete = nullptr) {
		return aniMgr.AnimateVariable(hwnd, variable, from, to, duration, easingFunc, onComplete);
	}

	void StopAnimation(int animationId) {
		aniMgr.RemoveAnimation(animationId);
	}


	// 获取动画状态
	bool IsAnimationRunning(int animationId) {
		
		return aniMgr.GetActiveAnimationCount() > 0; 
	}
	void SetOutFrame(int f)
	{
		this->NoOuterFrame = f;
	}
	void SetRoundRgn(int r)
	{
		this->RoundRgnRad = r;
	}
	void SetTopMost(bool t)
	{
		this->IsBottom = !t;
		this->IsTopMost = t;
	}
	void SetBottom(bool t)
	{
		this->IsBottom = t;
		this->IsTopMost = !t;
	}
	void SetZNormal()
	{
		this->IsBottom = false;
		this->IsTopMost = false;
	}
	void SetActivateFloat(bool a)
	{
		this->TempFloat = a;
	}
	void SetExtraMsg(FunctionalProc f)
	{
		extrafunc = f;
	}
	void SetStrictEventDelivery(bool i)
	{
		this->IsStrictEventDelivery = i;
	}
	bool GetStrictEventDelivery()
	{
		return this->IsStrictEventDelivery;
	}
	HWND GetHandle()
	{
		if (this)return this->hWnd;
		else
		{
			return 0;
		}
	}
	void SetShowFlag(bool b)
	{
		this->ShowFlag = b == true ? SW_SHOW : SW_HIDE;
	}
	void SetDropEffect(DWORD effect)
	{
		this->m_dwDropEffect = effect;
	}
	void InitTrayIcon(std::function<void()> left, std::function<void()> right)
	{
		this->onTrayLeftClick = left;
		this->onTrayRightClick = right;

		memset(&nid, 0, sizeof(NOTIFYICONDATA));
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = this->hWnd;
		nid.uID = 1001;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = VINA_TRAY_MSG;
		nid.hIcon = LoadIcon(this->hInst, MAKEINTRESOURCE(IDI_VINAAPP));
		wcscpy_s(nid.szTip, this->szTitle);

		Shell_NotifyIcon(NIM_ADD, &nid);
	}
	void SetInactivateFunc(std::function<void()> func)
	{
		this->ActivateExit = func;
	}
	void RemoveTrayIcon()
	{
		if (nid.hWnd && nid.uID) {
			Shell_NotifyIcon(NIM_DELETE, &nid);
			memset(&nid, 0, sizeof(NOTIFYICONDATA));
		}
	}
	int Create()
	{
		this->hInst = GetModuleHandle(NULL);
		if (this->WndProc != nullptr) { __RegisterClass(); }
		else { MessageBox(0, 0, 0, 0); return -1; }; if (this->CheckExtension != false) { if (FindWindow(L"LIGHTFRAME", L"LightFrame")) { HWND h = FindWindow(L"LIGHTFRAME", L"LightFrame"); PostMessage(h, LFM_EXTENSION, 0, 0); } else { if ((this->DependOnLightFrameMain == true))return -1; } }HWND hWnd; if (this->OnCreateWindow != __noop) { hWnd = this->OnCreateWindow(); }
		else { hWnd = CreateWindow(this->szWindowClass, this->szTitle, this->wStyle, this->x, this->y, this->Width, this->Height, nullptr, nullptr, this->hInst, nullptr); this->hWnd = hWnd;  vinaWinMap[hWnd] = this; }
		if (!hWnd) { return FALSE; }
		if (this->NoOuterFrame == outFrame::Disable) { LONG_PTR Style = ::GetWindowLongPtr(hWnd, GWL_STYLE); Style = Style & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX; ::SetWindowLongPtr(hWnd, GWL_STYLE, Style); ::SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW); ::SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE); }if (this->RoundRgnRad != 0) {
			RECT rc;
			GetClientRect(hWnd, &rc);
			HRGN hRgn = CreateRoundRectRgn(0, 0, rc.right + 1, rc.bottom + 1, this->RoundRgnRad * ceil(gScale), this->RoundRgnRad * ceil(gScale));
			SetWindowRgn(hWnd, hRgn, TRUE);
		} ShowWindow(hWnd, this->ShowFlag); if (this->OnCreateCmd != __noop) { this->OnCreateCmd(); SendMessage(hWnd, LFM_ONCREATEEXTENSION, 0, 0); }
		else { SendMessage(hWnd, LFM_ONCREATEEXTENSION, 0, 0); } return TRUE;
	}
	D2DVertexUIPanel* GetPanel() {
		return this->NewUIPanel;
	}
	std::function<bool(HWND)> IsFocusValidFunc = nullptr;
	AnimationManager aniMgr;
	std::function<void(const HWND& h)>sizingfunc = __noop;
	std::function<void()> OnCreateCmd = __noop;//Add some function such as animation and so on.
	int RunFull() { if (!this->Create()) { return FALSE; } MSG msg; while (GetMessage(&msg, nullptr, 0, 0)) { {TranslateMessage(&msg); DispatchMessage(&msg); } }return (int)msg.wParam; }
protected:

	FunctionalProc extrafunc;
	NOTIFYICONDATA nid = { 0 };
	std::function<void()> onTrayLeftClick = nullptr;
	std::function<void()> onTrayRightClick = nullptr;
	bool TempFloat = false;
	bool IsTopMost = false;
	bool IsBottom = false;
	bool IsStrictEventDelivery = false;
	int RoundRgnRad = 0;
	HINSTANCE hInst;
	const wchar_t* szTitle = L"Vina.Class.Unknown";
	const wchar_t* szWindowClass = L"Vina.Unknown";
	LRESULT(CALLBACK* WndProc)(HWND, UINT, WPARAM, LPARAM) = [](HWND hWnd, UINT umsg, WPARAM wParam, LPARAM lParam)->LRESULT
	{
		wchar_t str[256];
		GetClassName(hWnd, str, sizeof(str));
		std::wstring str2 = str;
		// OutputDebugString(str2.c_str());
		return ProcManager[str2](hWnd, umsg, wParam, lParam);
		//return DefWindowProc(hWnd, umsg, wParam, lParam);
	};
	LPTSTR hIcon;
	COLORREF BkColor = RGB(244, 244, 244);
	UINT wStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	UINT ShowFlag = SW_SHOW;
	DWORD m_dwDropEffect = DROPEFFECT_COPY | DROPEFFECT_LINK;
	int x = CW_USEDEFAULT;
	int y = 0;
	int Width = 400;
	int Height = 200;
	int NoOuterFrame = 0;
	HWND hWnd;//A Copy of hWnd.

	VinaDropTarget* m_pDropTarget = nullptr;

	bool CheckExtension = true;//Extension will be not avaliable anymore if never check it.Only can be used for LightFrame components inner core.
	bool DependOnLightFrameMain = false;//Extension will lost it's sign if disable this option.
	ATOM __RegisterClass() { WNDCLASSEXW wcex; wcex.cbSize = sizeof(WNDCLASSEX); wcex.style = CS_HREDRAW | CS_VREDRAW; wcex.lpfnWndProc = this->WndProc; wcex.cbClsExtra = 0; wcex.cbWndExtra = 0; wcex.hInstance = this->hInst; wcex.hIcon = LoadIcon(this->hInst, this->hIcon); wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); wcex.hbrBackground = (HBRUSH)CreateSolidBrush(this->BkColor); wcex.lpszMenuName = 0; wcex.lpszClassName = this->szWindowClass; wcex.hIconSm = LoadIcon(wcex.hInstance, this->hIcon); return RegisterClassExW(&wcex); }
	HWND(*OnCreateWindow)() = __noop;

	D2DVertexUIPanel* NewUIPanel = new  D2DVertexUIPanel;
	std::function<void(HWND, ID2D1HwndRenderTarget*)>  UI;
	std::function<void()>  ActivateExit = [] {};
};