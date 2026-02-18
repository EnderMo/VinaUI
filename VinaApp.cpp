// VinaApp.cpp : 定义应用程序的入口点。
//
#include "framework.h"
#include "VinaApp.h"
#include "VertexUI/VertexUI.ClickArea.h"
#include "VertexUI/VertexUI.Panel.h"
#include "VertexUI/VertexUI.min.h"
#include "MainUI.hpp"
#include "VinaWindow.hpp"

#include <Psapi.h>
#include <ShlObj.h>
#include <direct.h>
#include <io.h>
#include <regex>
#pragma warning(disable:4996)
HANDLE hMyFont;
HGLOBAL hFntMem;
#define VERSION_STR L"v0.1.1"
VertexUIInit;
#define MAX_LOADSTRING 100


void GetAppdataPath(wchar_t* ud)
{
    wchar_t szDocument[MAX_PATH] = { 0 };

    LPITEMIDLIST pidl = NULL;
    SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
    if (pidl && SHGetPathFromIDList(pidl, szDocument))
    {
        GetShortPathName(szDocument, ud, _MAX_PATH);
    }
}
void GetLocalAppdataPath(wchar_t* ud)
{
    wchar_t szDocument[MAX_PATH] = { 0 };

    LPITEMIDLIST pidl = NULL;
    SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
    if (pidl && SHGetPathFromIDList(pidl, szDocument))
    {
        GetShortPathName(szDocument, ud, _MAX_PATH);
    }
}
void GetAppdataPathA(char ud[MAX_PATH])
{
    char szDocument[MAX_PATH] = { 0 };

    LPITEMIDLIST pidl = NULL;
    SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
    if (pidl && SHGetPathFromIDListA(pidl, szDocument))
    {
        GetShortPathNameA(szDocument, ud, _MAX_PATH);
        //strcat("\\CommandCube", ud);
    }
}
void GetLocalAppdataPathA(char ud[MAX_PATH])
{
    char szDocument[MAX_PATH] = { 0 };

    LPITEMIDLIST pidl = NULL;
    SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
    if (pidl && SHGetPathFromIDListA(pidl, szDocument))
    {
        GetShortPathNameA(szDocument, ud, _MAX_PATH);
        //strcat("\\CommandCube", ud);
    }
}
void SetTagW(wchar_t* buf, wchar_t* OldTag, wchar_t* newTag)
{
    wchar_t* start;
    int newlen = wcslen(newTag);
    int oldlen = wcslen(OldTag);
    start = wcsstr(buf, OldTag);
    while (start) {
        if (newlen > oldlen) wmemmove(start + newlen - oldlen, start, wcslen(start) + 1);
        if (newlen < oldlen) wmemmove(start, start + oldlen - newlen, wcslen(start) + 1);
        wmemmove(start, newTag, newlen);
        start += newlen;
        start = wcsstr(buf, OldTag);
    }

}
void mkdirs(wchar_t* muldir)
{
    int i, len;
    wchar_t str[512];
    wcsncpy(str, muldir, 512);
    len = wcslen(str);
    for (i = 0; i < len; i++)
    {
        if (str[i] == '/')
        {
            str[i] = '\0';
            if (_waccess(str, 0) != 0)
            {
                _wmkdir(str);
            }
            str[i] = '/';
        }
    }
    if (len > 0 && _waccess(str, 0) != 0)
    {
        _wmkdir(str);
    }
    return;
}
void SetDataBase()
{
    wchar_t test[260] = L"";
    GetAppdataPath(LocalData);
    GetAppdataPath(LocalAppData);
    GetAppdataPath(test);
    GetAppdataPathA(LocalDataA);
    GetAppdataPath(LocalLFData);
    GetAppdataPathA(LocalLFDataA);


    wcscat(LocalData, L"\\Vilinko\\VinaTest");
    strcat(LocalDataA, "\\Vilinko\\VinaTest");

    wcscpy(LocalData2, LocalData);
    wcscpy(LocalRes, LocalData);
    wcscpy(LocalCache, LocalData);
    strcpy(LocalCacheA, LocalDataA);
    wcscpy(LocalLFCache, LocalLFData);
    strcpy(LocalLFCacheA, LocalLFDataA);

    wcscpy(LocalCom, LocalData);
    wcscpy(LocalCards, LocalData);
    strcpy(LocalComA, LocalDataA);

    SetTagW(LocalData2, L"\\", L"/");
    SetTagW(LocalCache2, L"\\", L"/");
    if (_waccess(LocalData, 0) == -1)
    {
        mkdirs(LocalData2);
    }
}
/*引用代码*/
BOOL FreeAnyResource(int Id, const wchar_t* Type, const wchar_t* Dir)
{
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule == NULL)
    {
        std::cerr << "错误：获取模块句柄失败。" << std::endl;
        return FALSE;
    }

    HRSRC hRsrc = FindResource(hModule, MAKEINTRESOURCE(Id), Type);
    if (hRsrc == NULL)
    {
        std::cerr << "错误：无法找到资源。" << std::endl;
        return FALSE;
    }

    DWORD dwSize = SizeofResource(hModule, hRsrc);
    if (dwSize == 0)
    {
        std::cerr << "错误：无效的资源大小。" << std::endl;
        return FALSE;
    }

    HGLOBAL hGlobal = LoadResource(hModule, hRsrc);
    if (hGlobal == NULL)
    {
        std::cerr << "错误：无法加载资源。" << std::endl;
        return FALSE;
    }

    LPVOID lpVoid = LockResource(hGlobal);
    if (lpVoid == NULL)
    {
        std::cerr << "错误：无法锁定资源。" << std::endl;
        FreeResource(hGlobal);
        return FALSE;
    }

    FILE* fp = _wfopen(Dir, L"wb+");
    if (fp == NULL)
    {
        std::cerr << "错误：无法创建或打开文件。" << std::endl;
        FreeResource(hGlobal);
        return FALSE;
    }

    fwrite(lpVoid, sizeof(char), dwSize, fp);
    fclose(fp);

    FreeResource(hGlobal);

    return TRUE;
}

struct CharAnim {
    wchar_t ch;
    float alpha;
    float xOffset;
    float yOffset;
    bool isRemoving = false; 
};

float VuiMeasureStringWidth(const std::wstring& str, int fontSize) {
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

VinaWindow* MainWindow = new VinaWindow;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);

    InitGlobalD2D();


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VINAAPP));


    VuiColorSystemInit();
    gScale = GetScreenScale();
    //LoadVinaCom();


    hMyFont = INVALID_HANDLE_VALUE;
    HRSRC  hFntRes = FindResource(hInstance, MAKEINTRESOURCE(IDF_FONTAWESOME), L"BINARY");
    if (hFntRes) { 
        hFntMem = LoadResource(hInstance, hFntRes); 
        if (hFntMem != nullptr) {
            void* FntData = LockResource(hFntMem); 
            DWORD nFonts = 0, len = SizeofResource(hInstance, hFntRes);
            hMyFont = AddFontMemResourceEx(FntData, len, nullptr, &nFonts); 
        }
    }
    std::wstring fntBase = std::wstring(LocalData) + std::wstring(L"\\Font-AwesomeFree.ttf");
    FreeAnyResource(IDF_FONTAWESOME, L"BINARY", fntBase.c_str());
    AddFontResource(fntBase.c_str());


    MainWindow->Set(100, 100, 720 * gScale, 480 * gScale, L"Vina.Class.App.Main.Test", L"Vilinko VinaUI");
    MainWindow->CreatePanel([](HWND hWnd, ID2D1HwndRenderTarget* hrt)->void {


        RECT rc;
        GetClientRect(hWnd, &rc);
        D2DDrawSolidRect(hrt, 0, 0, rc.right, rc.bottom, VERTEXUICOLOR_DARKNIGHT);
        MainWindow->GetPanel()->Set(hWnd, hrt);

        /*
        CreatePanel 会在窗口刷新时被调用。

        控件需要使用静态标识，或者在 CreatePanel 外部初始化。

        使用用 std::vector 等容器储存控件也是较好的方法。
        */

        static std::shared_ptr<VinaButton>btn = std::make_shared<VinaButton>();
        btn->Set(40, 60, 140, 60, L"Button", [] {return 0; });
        MainWindow->GetPanel()->Add(btn);

        static std::shared_ptr<VinaSlider>sli = std::make_shared<VinaSlider>();
        sli->Set(40, 140, 140, 30, -1, VERTEXUICOLOR_DARKEN, L"Slider", [] { });
        MainWindow->GetPanel()->Add(sli);

        static bool value = false;
        static std::shared_ptr<VinaSwitch>sw = std::make_shared<VinaSwitch>();
        sw->Set(40, 190, 60, 30, {VERTEXUICOLOR_DARKEN}, [] {});
        MainWindow->GetPanel()->Add(sw);
        value = sw->GetValue();

        if (value)
        {
            static std::shared_ptr<VinaFileSelector>fi = std::make_shared<VinaFileSelector>();
            fi->Set(40, 240, 260, 160);
            fi->SetFileOpenCallback([](std::wstring path)->void {
                MessageBox(0, path.c_str(), L"Current", 0);
                });
            fi->SetParent(MainWindow->GetPanel());
            MainWindow->GetPanel()->Add(fi);
        }

        static std::shared_ptr<VinaMultiTextBox>mt = std::make_shared<VinaMultiTextBox>();
        mt->Set(260, 60, 300, 100, L"This is a test string.\nAnd this is a multi line text area.\ne.g:\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10");
        mt->SetParent(MainWindow->GetPanel());
        MainWindow->GetPanel()->Add(mt);

        static std::shared_ptr<VinaEdit>edt = std::make_shared<VinaEdit>();
        if (edt->cx == 0)/*这里为了演示方便，做简单的初始化。实际使用建议将Set移动到外层。*/
        {
            edt->Set(260, 180, 300, 30, L"This is a Edit...");
        }
        edt->Set2(260, 180, 300, 35,VERTEXUICOLOR_MIDNIGHT,VERTEXUICOLOR_WHITE);
        MainWindow->GetPanel()->Add(edt);

        /*
        此处演示动画效果，以及动画播放时的内存管理。

        动画效果可以直接在 CreatePanel（也就是 OnPaint ）时操作，也可以在控件内部操作。

		此处为了演示方便，直接在 CreatePanel 内部操作。
        */

        std::wstring text;
        text = edt->GetText();

        static std::wstring lastText;
        static std::list<CharAnim> animatingChars;

        std::wstring currentText = edt->GetText();

        if (currentText != lastText) {
            if (currentText.length() > lastText.length()) {
               
                for (size_t i = lastText.length(); i < currentText.length(); ++i) {
                   
                    float lockedX = VuiMeasureStringWidth(currentText.substr(0, i), 18);

                    animatingChars.push_back({ currentText[i], 0.0f, lockedX, 18.0f, false });
                    CharAnim& ref = animatingChars.back();

                    MainWindow->AnimateVariableWithBezier(hWnd, ref.alpha, 0.0f, 1.0f, 0.4);
                    MainWindow->AnimateVariableWithBezier(hWnd, ref.yOffset, 18.0f, 0.0f, 0.4);
                }
            }
            else if (currentText.length() < lastText.length()) {
               
                size_t diff = lastText.length() - currentText.length();
                for (size_t i = 0; i < diff; ++i) {
                   
                    auto it = std::find_if(animatingChars.rbegin(), animatingChars.rend(),
                        [](const CharAnim& c) { return !c.isRemoving; });

                    if (it != animatingChars.rend()) {
                        it->isRemoving = true;
                        CharAnim& ref = *it;

                       
                        MainWindow->AnimateVariableWithBezier(hWnd, ref.alpha, ref.alpha, 0.0f, 0.3);
                        MainWindow->AnimateVariableWithBezier(hWnd, ref.yOffset, ref.yOffset, 10.0f, 0.3,
                            0.25, 0.1, 0.25, 1.0,
                            [&]() {
                               
                            }
                        );
                    }
                }
            }
            lastText = currentText;
        }

        //清理
        animatingChars.remove_if([](const CharAnim& c) {
            return c.isRemoving && c.alpha <= 0.01f;
            });

        float startX = 260.0f;
        float startY = 230.0f;

        for (auto& item : animatingChars) {
            D2DDrawText2(hrt, std::wstring(1, item.ch).c_str(),
                startX + item.xOffset, startY + item.yOffset,
                20, 20, 18,
                VERTEXUICOLOR_WHITE, L"Segoe UI", item.alpha);
        }

        static std::shared_ptr<VinaCaptionBar>capt = std::make_shared<VinaCaptionBar>();
        capt->Set(0, 0, rc.right / gScale - 160, 40, L"VinaUI App", VERTEXUICOLOR_DARKNIGHT, 18);
        MainWindow->GetPanel()->Add(capt);

        static std::shared_ptr<VinaFAIcon>close = std::make_shared<VinaFAIcon>();
        close->Set(rc.right / gScale - 32, 20, L"win-close", 15, VERTEXUICOLOR_WHITE, [] {DestroyWindow(MainWindow->GetHandle()); PostQuitMessage(0); });
        MainWindow->GetPanel()->Add(close);

        static bool IsMaximized = false;
        if (IsZoomed(hWnd) == true)IsMaximized = true;
        else IsMaximized = false;

        if (IsMaximized == false)
        {
            static std::shared_ptr<VinaFAIcon>max = std::make_shared<VinaFAIcon>();
            max->Set(rc.right / gScale - 32 - 32, 20, L"win-max", 15, VERTEXUICOLOR_WHITE, [hWnd] {
                IsMaximized = !IsMaximized;
                SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0); });
            MainWindow->GetPanel()->Add(max);
        }
        else {
            static std::shared_ptr<VinaFAIcon>max = std::make_shared<VinaFAIcon>();
            max->Set(rc.right / gScale - 32 - 32, 20, L"win-restore", 15, VERTEXUICOLOR_WHITE, [hWnd] {
                IsMaximized = !IsMaximized;
                SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0); });
            MainWindow->GetPanel()->Add(max);
        }
        static std::shared_ptr<VinaFAIcon>min = std::make_shared<VinaFAIcon>();
        min->Set(rc.right / gScale - 32 - 32 - 32, 20, L"win-min", 15, VERTEXUICOLOR_WHITE, [hWnd] {MainWindow->KillAnimation(); SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0); });
        MainWindow->GetPanel()->Add(min);

        //LoadBitmapFromFile(hrt, m_ImageFactory, L"D:\\素材\\FLUENTEMOJI\\four_leaf_clover_3d.png", 20, 20, 100, 100);

        });

    MainWindow->SetOutFrame(VinaWindow::Client);
    MainWindow->OnCreateCmd = [] {
        CenterWindow(MainWindow->GetHandle());
        MainWindow->InitAnimation();
        MainWindow->StartAnimationSystem();
    };
    MainWindow->RunFull();

    return 0;
}


