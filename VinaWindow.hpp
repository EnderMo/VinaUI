#pragma once
#include "VinaCommonCtls.hpp"
#include <Windowsx.h>
/*Vina Window Loader API
* By Vilinko,CimiMoly
* Copyright 2025
* All rights reserved
*/
typedef int RUNFUN;
#define LFM_EXTENSION 0xff1
#define LFM_ONCREATEEXTENSION 0xff2
#define LFM_REFRESHEFFECT 0xff4
using FunctionalProc = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;
static std::unordered_map < std::wstring, FunctionalProc> ProcManager;
class VinaWindow
{

public:
    enum outFrame {
        Normal,Disable,Client
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
                else return DefWindowProc(hWnd, umsg, wParam, lParam);;
            }
            case WM_CREATE:
            {
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
                }else
                    return DefWindowProc(hWnd, umsg, wParam, lParam);;
                break;
            }
            case  WM_MOUSEMOVE:
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
                if (this->NewUIPanel->AddEvent(pt, vinaEvent::mouseOver))Refresh(hWnd);
                break;
            }
            case WM_MOUSELEAVE: {
                vinaPoint pt;
                pt.x = GET_X_LPARAM(lParam);
                pt.y = GET_Y_LPARAM(lParam);
                this->NewUIPanel->AddEvent(pt, vinaEvent::mouseUnfocus);
                Refresh(hWnd);
                break;
            }
            case WM_TIMER:
            {
                switch (wParam)
                {
                case 10:
                {

                    RefreshWindow(hWnd);

                    break;
                }
                case 11:
                {
                    if (GlobalAnimationCount >= 1)
                    {
                        RefreshWindow(hWnd);
                    }
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
                this->NewUIPanel->AddEvent(pt, vinaEvent::mouseUp);
                break;
            }
            case WM_RBUTTONUP:
            {
                SendRClickEvent(hWnd, wParam, lParam);
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
                DestroyWindow(0);
                break;
            default:
                return DefWindowProc(hWnd, umsg, wParam, lParam);
            }
            return 0;
            });
    }
    void CreatePanel(D2DHWNDDRAWPANEL* UI)
    {
        this->UI = UI;
    }
    void SetWindowInfo(const wchar_t* cls,const wchar_t* name)
    {
        szTitle = name;
        szWindowClass = cls;
    }
    void SetWindowSize(int w,int h)
    {
        this->Width = w;
        this->Height = h;
        if (hWnd!=0)SetWindowPos(hWnd, 0, 0, 0, w, h, SWP_NOMOVE);
    }
    void SetPos(int w, int h)
    {
        this->x = w;
        this->y = h;
        if (hWnd!=0)SetWindowPos(hWnd, 0, w, h,0,0 ,SWP_NOSIZE);
    }
    void SetProc(FunctionalProc proc)
    {
        ProcManager[std::wstring(this->szWindowClass)] = proc;
    }
    void SetOutFrame(int f)
    {
        this->NoOuterFrame = f;
    }
    void SetRoundRgn(int r)
    {
        this->RoundRgnRad = r;
    }
    HWND GetHandle()
    {
        return this->hWnd;
    }
    int Create()
    {
        if (this->WndProc != nullptr) { __RegisterClass(); }
        else { MessageBox(0, 0, 0, 0); return -1; }; if (this->CheckExtension != false) { if (FindWindow(L"LIGHTFRAME", L"LightFrame")) { HWND h = FindWindow(L"LIGHTFRAME", L"LightFrame"); PostMessage(h, LFM_EXTENSION, 0, 0); } else { if ((this->DependOnLightFrameMain == true))return -1; } }HWND hWnd; if (this->OnCreateWindow != __noop) { hWnd = this->OnCreateWindow(); }
        else { hWnd = CreateWindow(this->szWindowClass, this->szTitle, this->wStyle, this->x, this->y, this->Width, this->Height, nullptr, nullptr, this->hInst, nullptr); this->hWnd = hWnd; }
        if (!hWnd) { return FALSE; }
        if (this->NoOuterFrame == outFrame::Disable) { LONG_PTR Style = ::GetWindowLongPtr(hWnd, GWL_STYLE); Style = Style & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX; ::SetWindowLongPtr(hWnd, GWL_STYLE, Style); ::SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW); ::SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE); }if (this->RoundRgnRad != 0) {
            RECT rc;
            GetClientRect(hWnd,&rc);
            HRGN hRgn = CreateRoundRectRgn(0, 0, rc.right + 1, rc.bottom + 1, this->RoundRgnRad * ceil(gScale), this->RoundRgnRad * ceil(gScale));
            SetWindowRgn(hWnd, hRgn, TRUE);
        } ShowWindow(hWnd, this->ShowFlag); if (this->OnCreateCmd != __noop) { this->OnCreateCmd(); SendMessage(hWnd, LFM_ONCREATEEXTENSION, 0, 0); }
        else { SendMessage(hWnd, LFM_ONCREATEEXTENSION, 0, 0); } return TRUE;
    }
    D2DVertexUIPanel* GetPanel() {
        return this->NewUIPanel;
    }
    int RunFull() { if (!this->Create()) { return FALSE; } MSG msg; while (GetMessage(&msg, nullptr, 0, 0)) { {TranslateMessage(&msg); DispatchMessage(&msg); } }return (int)msg.wParam; }
    protected:

        int RoundRgnRad = 0;
        HINSTANCE hInst;
        const wchar_t* szTitle = L"Vina.Class.Unknown";
        const wchar_t* szWindowClass = L"Vina.Unknown";
        LRESULT(CALLBACK* WndProc)(HWND, UINT, WPARAM, LPARAM) = [](HWND hWnd, UINT umsg, WPARAM wParam, LPARAM lParam)->LRESULT
        {
            wchar_t str[256];
            GetClassName(hWnd, str, sizeof(str)); 

            return ProcManager[std::wstring(str)](hWnd, umsg, wParam, lParam);
            //return DefWindowProc(hWnd, umsg, wParam, lParam);
        };
        LPTSTR hIcon;
        COLORREF BkColor = RGB(244, 244, 244);
        UINT wStyle = WS_OVERLAPPEDWINDOW;
        UINT ShowFlag = SW_SHOW;
        int x = CW_USEDEFAULT;
        int y = 0;
        int Width = 400;
        int Height = 200;
        int NoOuterFrame = 0;
        HWND hWnd;//A Copy of hWnd.
        RUNFUN(*OnCreateCmd)() = __noop;//Add some function such as animation and so on.
        bool CheckExtension = true;//Extension will be not avaliable anymore if never check it.Only can be used for LightFrame components inner core.
        bool DependOnLightFrameMain = false;//Extension will lost it's sign if disable this option.
        ATOM __RegisterClass() { WNDCLASSEXW wcex; wcex.cbSize = sizeof(WNDCLASSEX); wcex.style = CS_HREDRAW | CS_VREDRAW; wcex.lpfnWndProc = this->WndProc; wcex.cbClsExtra = 0; wcex.cbWndExtra = 0; wcex.hInstance = this->hInst; wcex.hIcon = LoadIcon(this->hInst, this->hIcon); wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); wcex.hbrBackground = (HBRUSH)CreateSolidBrush(this->BkColor); wcex.lpszMenuName = 0; wcex.lpszClassName = this->szWindowClass; wcex.hIconSm = LoadIcon(wcex.hInstance, this->hIcon); return RegisterClassExW(&wcex); }
        HWND(*OnCreateWindow)() = __noop;
        D2DVertexUIPanel* NewUIPanel = new D2DVertexUIPanel;
        D2DHWNDDRAWPANEL* UI;
};
