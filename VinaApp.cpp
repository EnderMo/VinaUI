// VinaApp.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "VinaApp.h"
#include "VertexUI/VertexUI.ClickArea.h"
#include "VertexUI/VertexUI.Panel.h"
#include "VertexUI/VertexUI.min.h"
#include "MainUI.hpp"
#include "VinaWindow.hpp"
VertexUIInit;
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_VINAAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VINAAPP));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
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
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VINAAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_VINAAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中
   InitGlobalD2D();
   VuiColorSystemInit();
   gScale = GetScreenScale();
   LoadVinaCom();
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   SetTimer(hWnd, 11, 16, 0);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))

#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        static VinaWindow* win = new VinaWindow;
        win->Set(100, 100, 400, 300, L"aifhlshefgweh", L"Test");
        win->SetOutFrame(VinaWindow::Client);
        win->SetRoundRgn(14);
        win->CreatePanel([](HWND hWnd, ID2D1HwndRenderTarget* hrt)->void {
            RECT rc; GetClientRect(hWnd, &rc);
            D2DDrawSolidRect(hrt, 0, 0, rc.right / gScale, rc.bottom / gScale, VERTEXUICOLOR_DARKEN, 1);
            win->GetPanel()->Set(hWnd, hrt);
            static std::shared_ptr<VinaButton>test = std::make_shared<VinaButton>();
            test->Set(20, 20, 100, 40, L"关闭", [] {DestroyWindow(win->GetHandle()); });
            win->GetPanel()->Add(test);
            });
        win->Create();
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                static VinaWindow* win = new VinaWindow;
                win->Set(100, 100, 400, 300, L"aifhlshefgweh11", L"Test");
                win->SetOutFrame(VinaWindow::Disable);
                win->SetRoundRgn(14);
                win->CreatePanel([](HWND hWnd, ID2D1HwndRenderTarget* hrt)->void {
                    RECT rc; GetClientRect(hWnd, &rc);
                    D2DDrawSolidRect(hrt, 0, 0, rc.right/gScale, rc.bottom/gScale, VERTEXUICOLOR_DARKEN, 1);
                    win->GetPanel()->Set(hWnd, hrt);
                    static std::shared_ptr<VinaButton>test = std::make_shared<VinaButton>();
                    test->Set(20, 20, 100, 40, L"关闭", [] {DestroyWindow(win->GetHandle()); });
                    win->GetPanel()->Add(test);
                    });
                win->Create();
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
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
        if (NewUIPanel->AddEvent(pt, vinaEvent::mouseOver))Refresh(hWnd);
        break;
    }
    case WM_MOUSELEAVE:{
        vinaPoint pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        NewUIPanel->AddEvent(pt, vinaEvent::mouseUnfocus);
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
        NewUIPanel->AddEvent(pt, vinaEvent::mouseDown);
        break;
    }
    case WM_LBUTTONUP:
    {
        vinaPoint pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        NewUIPanel->AddEvent(pt,vinaEvent::mouseUp);
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
            CreateD2DPanel(hWnd, MainUI);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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
