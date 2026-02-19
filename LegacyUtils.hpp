#pragma once
#include "VinaComponents.hpp"
/*
本文件为旧版示例的一些辅助函数，实际使用时可删除。
*/

HANDLE hMyFont;
HGLOBAL hFntMem;
#define VERSION_STR L"v0.1.1"
VertexUIInit;
#define MAX_LOADSTRING 100

#define MAX_PATH 260

wchar_t LocalAppData[MAX_PATH] = L""; //Tag"\\"
wchar_t LocalData[MAX_PATH] = L""; //Tag"\\"
char LocalLFDataA[MAX_PATH] = ""; //Tag"\\"
wchar_t LocalLFData[MAX_PATH] = L""; //Tag"\\"
char LocalDataA[MAX_PATH] = ""; //Tag"\\"
wchar_t LocalData2[MAX_PATH] = L"";//Tag "/"
wchar_t LocalCache[MAX_PATH] = L""; //Tag"vui"
char LocalCacheA[MAX_PATH] = ""; //Tag"vui"
wchar_t LocalLFCache[MAX_PATH] = L""; //Tag"vui"
char LocalLFCacheA[MAX_PATH] = ""; //Tag"vui"
wchar_t LocalCache2[MAX_PATH] = L""; //Tag"vui //"
wchar_t LocalRes[MAX_PATH] = L""; //Tag"\\"

wchar_t LocalCom[MAX_PATH] = L"";
wchar_t LocalCards[MAX_PATH] = L"";
char LocalComA[MAX_PATH] = "";
struct FontResourceDeleter {
    void operator()(HANDLE h) const { if (h && h != INVALID_HANDLE_VALUE) RemoveFontMemResourceEx(h); }
};
using UniqueFontHandle = std::unique_ptr<void, FontResourceDeleter>;

class AppResourceManager {
public:
    static std::vector<std::byte> LoadRawResource(HINSTANCE hInst, int resId, const std::wstring& type) {
        HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(resId), type.c_str());
        if (!hRes) return {};

        HGLOBAL hData = LoadResource(hInst, hRes);
        if (!hData) return {};

        const void* pData = LockResource(hData);
        size_t size = SizeofResource(hInst, hRes);

        auto start = static_cast<const std::byte*>(pData);
        return { start, start + size };
    }
};

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
    bool isRemoving;

    int alphaAnimId = -1;
    int yOffsetAnimId = -1;
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


D2DVertexUIPanel* NewUIPanel = new D2DVertexUIPanel;
/*

     void GetFirstControlPoints(
            __in const std::vector<FLOAT>&rhs,
         __out std::vector<FLOAT>&x)
     {
         _ASSERTE(rhs.size() == x.size());
       int n = rhs.size();
      std::vector<FLOAT> tmp(n);    // Temp workspace.
   
     FLOAT b = 2.0f;
      x[0] = rhs[0] / b;
      for (int i = 1; i < n; i++) // Decomposition and forward substitution.
         {
                tmp[i] = 1 / b;
               b = (i < n - 1 ? 4.0f : 3.5f) - tmp[i];
             x[i] = (rhs[i] - x[i - 1]) / b;
          }
     for (int i = 1; i < n; i++)
         {
                x[n - i - 1] -= tmp[n - i] * x[n - i]; // Back substitution.
            }
    }

    void GetCurveControlPoints(
            __in const std::vector<D2D1_POINT_2F>&knots,
         __out std::vector<D2D1_POINT_2F>&firstCtrlPt,
        __out std::vector<D2D1_POINT_2F>&secondCtrlPt)
     {
        _ASSERTE((firstCtrlPt.size() == secondCtrlPt.size())
            && (knots.size() == firstCtrlPt.size() + 1));
    
    int n = knots.size() - 1;
        _ASSERTE(n >= 1);

if (n == 1)
    {
       // Special case: Bezier curve should be a straight line.
         // 3P1 = 2P0 + P3
        firstCtrlPt[0].x = (2 * knots[0].x + knots[1].x) / 3.0f;
             firstCtrlPt[0].y = (2 * knots[0].y + knots[1].y) / 3.0f;
    
          // P2 = 2P1 – P0
      secondCtrlPt[0].x = 2 * firstCtrlPt[0].x - knots[0].x;
             secondCtrlPt[0].y = 2 * firstCtrlPt[0].y - knots[0].y;
             return;
        }

        // Right hand side vector
    std::vector<FLOAT> rhs(n);

    // Set right hand side X values
       for (int i = 1; i < (n - 1); ++i)
        {
              rhs[i] = 4 * knots[i].x + 2 * knots[i + 1].x;
           }
    rhs[0] = knots[0].x + 2 * knots[1].x;
        rhs[n - 1] = (8 * knots[n - 1].x + knots[n].x) / 2.0f;
        // Get first control points X-values
   std::vector<FLOAT> x(n);
      GetFirstControlPoints(rhs, x);
   
      // Set right hand side Y values
   for (int i = 1; i < (n - 1); ++i)
    {
          rhs[i] = 4 * knots[i].y + 2 * knots[i + 1].y;
    }
   rhs[0] = knots[0].y + 2 * knots[1].y;
      rhs[n - 1] = (8 * knots[n - 1].y + knots[n].y) / 2.0f;
      // Get first control points Y-values
   std::vector<FLOAT> y(n);
      GetFirstControlPoints(rhs, y);
  
      for (int i = 0; i < n; ++i)
      {
             // First control point
      firstCtrlPt[i] = D2D1::Point2F(x[i], y[i]);
             // Second control point
      if (i < (n - 1))
             {
                        secondCtrlPt[i] = D2D1::Point2F(2 * knots[i + 1].x - x[i + 1], 2 * knots[i + 1].y - y[i + 1]);
                    }
        else
            {
                         secondCtrlPt[i] = D2D1::Point2F((knots[n].x + x[n - 1]) / 2, (knots[n].y + y[n - 1]) / 2);
                   }
       }
    }

    HRESULT CreateBezierSpline(
        __in ID2D1Factory* pD2dFactory,
        __in const std::vector<D2D1_POINT_2F>& points,
        __out ID2D1PathGeometry** ppPathGeometry,ID2D1GeometrySink** spSink)
    {

        _ASSERTE(points.size() > 1);
        int n = points.size();
        std::vector<D2D1_POINT_2F> firstCtrlPt(n - 1);
        std::vector<D2D1_POINT_2F> secondCtrlPt(n - 1);
        GetCurveControlPoints(points, firstCtrlPt, secondCtrlPt);

        HRESULT hr = pD2dFactory->CreatePathGeometry(ppPathGeometry);

        if (FAILED(hr))
            return hr;


        hr = (*ppPathGeometry)->Open(&*spSink);
        if (SUCCEEDED(hr))
        {
            (*spSink)->SetFillMode(D2D1_FILL_MODE_WINDING);
            (*spSink)->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
            for (int i = 1; i < n; i++) {
                (*spSink)->AddBezier(D2D1::BezierSegment(firstCtrlPt[i - 1], secondCtrlPt[i - 1], points[i]));
            }(*spSink)->EndFigure(D2D1_FIGURE_END_OPEN);        (*spSink)->Close();
        }    return hr;
    }
    */
void MainUI(HWND hWnd, ID2D1HwndRenderTarget* hrt)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	D2DDrawSolidRect(hrt, 0, 0, rc.right, rc.bottom, VERTEXUICOLOR_DARKNIGHT);
    VertexUILayoutInfo lo;
    lo.Mode = vinaLayout::Left;
    lo.interval = 20;
    lo.x = 20;
    lo.y = 20;
    lo.uniCx =100;
    lo.uniCy = 100;
	NewUIPanel->Set(hWnd, hrt);
    
	//btn->Set(20, 20, 100, 40,L"QwQ");
	for (auto iter = comVina.begin(); iter != comVina.end(); ++iter)
	{
       
        if (std::holds_alternative<std::shared_ptr<VinaButton>>(iter->second))
        {
            NewUIPanel->Add(std::get<std::shared_ptr<VinaButton>>(iter->second));
        }
        if (std::holds_alternative<std::shared_ptr<VinaText>>(iter->second))
        {
            NewUIPanel->Add(std::get<std::shared_ptr<VinaText>>(iter->second));
        }
        if (std::holds_alternative<std::shared_ptr<VinaFAIcon>>(iter->second))
        {
            NewUIPanel->Add(std::get<std::shared_ptr<VinaFAIcon>>(iter->second));
        }
	}
   // static VinaSwitch* bs = new VinaSwitch;
    //VertexUICtlColor ci;
    //ci.activeBorderColor = VuiDarkenColor(ci.activeColor,40);
    //bs->Set(200, 400, 60, 30, ci);
    //NewUIPanel->Add(bs);
    /*
    ID2D1GeometrySink* pSink = NULL;
    m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
    m_pPathGeometry->Open(&pSink);

      
        struct NSVGimage* image;
        image = nsvgParseFromFile("test.svg", "px", 96);
        NSVGshape* shape = NULL;
        NSVGpath* path = NULL;
        printf("size: %f x %f\n", image->width, image->height);
        // Use...
        std::vector<D2D1_POINT_2F>mpt;
        for (shape = image->shapes; shape != NULL; shape = shape->next) {
            for (path = shape->paths; path != NULL; path = path->next) {
                for (auto i = 0; i < path->npts - 1; i += 3) {
                    float* p = &path->pts[i * 2];
                    mpt.push_back(D2D1::Point2F(p[0], p[1]));
                    mpt.push_back(D2D1::Point2F(p[2], p[3]));
                    mpt.push_back(D2D1::Point2F(p[4], p[5]));
                    mpt.push_back(D2D1::Point2F(p[6], p[7]));
                   // drawCubicBez(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                }
            }
        }

        CreateBezierSpline(m_pD2DFactory, mpt, &m_pPathGeometry, &pSink);
       
    ID2D1SolidColorBrush* testBrush = 0;
    hrt->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF(RGBToHex(RGB(255, 255, 255)), 1)),
        &testBrush
    );
    hrt->DrawGeometry(m_pPathGeometry, testBrush, 1.f);
    SafeRelease(&testBrush);
    SafeRelease(&pSink);
    SafeRelease(&m_pPathGeometry);

    nsvgDelete(image);
    */
   // hrt->FillGeometry(m_pPathGeometry, m_pLGBrush);
}