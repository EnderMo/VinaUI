#pragma once
#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>
#include <vector>
#include <string>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")

namespace VinaDragHelper
{
    /**
     * 设置窗口鼠标穿透属性及透明度
     */
    static RECT _rc;
    static void SetWindowClickAttribute(HWND hWnd, bool bThrough, BYTE alpha = 60) {
        LONG_PTR exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
        exStyle |= WS_EX_LAYERED;

        if (bThrough) {
            exStyle |= WS_EX_TRANSPARENT;
            SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle);
            SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
        }
        else {
            exStyle &= ~WS_EX_TRANSPARENT;
            SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle);
            SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
        }

        SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    static bool idrag = false;

    class CEnumFormatEtc : public IEnumFORMATETC {
        LONG m_cRef;
        size_t m_nIndex;
        FORMATETC m_fmt;
    public:
        CEnumFormatEtc(const FORMATETC& fmt) : m_cRef(1), m_nIndex(0), m_fmt(fmt) {}
        STDMETHODIMP QueryInterface(REFIID r, void** p) {
            if (r == IID_IUnknown || r == IID_IEnumFORMATETC) { *p = this; AddRef(); return S_OK; }
            *p = NULL; return E_NOINTERFACE;
        }
        STDMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&m_cRef); }
        STDMETHODIMP_(ULONG) Release() { LONG c = InterlockedDecrement(&m_cRef); if (c == 0) delete this; return c; }
        STDMETHODIMP Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched) {
            if (pceltFetched) *pceltFetched = 0;
            if (celt > 0 && m_nIndex == 0) {
                rgelt[0] = m_fmt; m_nIndex++;
                if (pceltFetched) *pceltFetched = 1;
                return S_OK;
            }
            return S_FALSE;
        }
        STDMETHODIMP Skip(ULONG celt) { m_nIndex += celt; return S_OK; }
        STDMETHODIMP Reset() { m_nIndex = 0; return S_OK; }
        STDMETHODIMP Clone(IEnumFORMATETC** pp) { *pp = new CEnumFormatEtc(m_fmt); return S_OK; }
    };

    class CDataObject : public IDataObject {
        LONG m_cRef;
        HGLOBAL m_hGlobal;
        FORMATETC m_fmt;
    public:
        CDataObject(HGLOBAL h) : m_cRef(1), m_hGlobal(h) {
            m_fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        }
        STDMETHODIMP QueryInterface(REFIID riid, void** ppv) {
            if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDataObject)) {
                *ppv = this; AddRef(); return S_OK;
            }
            *ppv = NULL; return E_NOINTERFACE;
        }
        STDMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&m_cRef); }
        STDMETHODIMP_(ULONG) Release() {
            LONG c = InterlockedDecrement(&m_cRef);
            if (c == 0) { if (m_hGlobal) GlobalFree(m_hGlobal); delete this; }
            return c;
        }
        STDMETHODIMP GetData(FORMATETC* pf, STGMEDIUM* pm) {
            if (pf->cfFormat == CF_HDROP && (pf->tymed & TYMED_HGLOBAL)) {
                SIZE_T s = GlobalSize(m_hGlobal);
                HGLOBAL hC = GlobalAlloc(GHND, s);
                void* d = GlobalLock(m_hGlobal);
                void* sc = GlobalLock(hC);
                memcpy(sc, d, s);
                GlobalUnlock(hC);
                GlobalUnlock(m_hGlobal);
                pm->tymed = TYMED_HGLOBAL; pm->hGlobal = hC; pm->pUnkForRelease = NULL;
                return S_OK;
            }
            return DV_E_FORMATETC;
        }
        STDMETHODIMP QueryGetData(FORMATETC* pf) {
            return (pf->cfFormat == CF_HDROP) ? S_OK : S_FALSE;
        }
        STDMETHODIMP EnumFormatEtc(DWORD dwDir, IEnumFORMATETC** pp) {
            if (dwDir == DATADIR_GET) {
                *pp = new CEnumFormatEtc(m_fmt);
                return S_OK;
            }
            return E_NOTIMPL;
        }
        STDMETHODIMP GetDataHere(FORMATETC*, STGMEDIUM*) { return E_NOTIMPL; }
        STDMETHODIMP GetCanonicalFormatEtc(FORMATETC*, FORMATETC*) { return E_NOTIMPL; }
        STDMETHODIMP SetData(FORMATETC*, STGMEDIUM*, BOOL) { return E_NOTIMPL; }
        STDMETHODIMP DAdvise(FORMATETC*, DWORD, IAdviseSink*, DWORD*) { return E_NOTIMPL; }
        STDMETHODIMP DUnadvise(DWORD) { return E_NOTIMPL; }
        STDMETHODIMP EnumDAdvise(IEnumSTATDATA**) { return E_NOTIMPL; }
    };

    class CDropSource : public IDropSource {
        LONG m_cRef;
    public:
        CDropSource() : m_cRef(1) {}
        STDMETHODIMP QueryInterface(REFIID r, void** p) {
            if (r == IID_IUnknown || r == IID_IDropSource) { *p = this; AddRef(); return S_OK; }
            *p = NULL; return E_NOINTERFACE;
        }
        STDMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&m_cRef); }
        STDMETHODIMP_(ULONG) Release() { LONG c = InterlockedDecrement(&m_cRef); if (c == 0) delete this; return c; }

        STDMETHODIMP QueryContinueDrag(BOOL esc, DWORD key) {
            if (esc) return DRAGDROP_S_CANCEL;
            bool isLeftPressed = (key & MK_LBUTTON) || (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
            if (!isLeftPressed)
            {
                POINT pt;
                GetCursorPos(&pt);
                if (!PtInRect(&VinaDragHelper::_rc, pt))
                {
                    return DRAGDROP_S_DROP;
                }
                else return DRAGDROP_S_CANCEL;
            }
            return S_OK;
        }
        STDMETHODIMP GiveFeedback(DWORD) { return DRAGDROP_S_USEDEFAULTCURSORS; }
    };

    /**
     * @param files            文件路径列表
     * @param dwAllowedEffects 拖拽效果：DROPEFFECT_COPY 或 DROPEFFECT_MOVE，默认为 COPY
     */
    static void SetFileDrag(const std::vector<std::wstring>& files, DWORD dwAllowedEffects = DROPEFFECT_COPY) {
        if (files.empty()) return;
        idrag = true;
        HWND h = GetForegroundWindow();
        SetWindowClickAttribute(h, true);
        GetWindowRect(h, &_rc);

        HRESULT hr = OleInitialize(NULL);
        if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) return;

        size_t totalLen = 0;
        for (const auto& f : files) totalLen += (f.length() + 1);
        totalLen += 1;

        HGLOBAL hGlobal = GlobalAlloc(GHND, sizeof(DROPFILES) + (totalLen * sizeof(wchar_t)));
        if (!hGlobal) {
            OleUninitialize();
            return;
        }

        DROPFILES* df = (DROPFILES*)GlobalLock(hGlobal);
        df->pFiles = sizeof(DROPFILES);
        df->fWide = TRUE;

        wchar_t* pPtr = (wchar_t*)((BYTE*)df + sizeof(DROPFILES));
        for (const auto& f : files) {
            wcscpy(pPtr, f.c_str());
            pPtr += (f.length() + 1);
        }
        *pPtr = L'\0';
        GlobalUnlock(hGlobal);

        CDataObject* pDO = new CDataObject(hGlobal);
        CDropSource* pDS = new CDropSource();

        DWORD dwEffect;
        // 使用传入的 dwAllowedEffects 参数
        DoDragDrop(pDO, pDS, dwAllowedEffects, &dwEffect);

        pDS->Release();
        pDO->Release();
        OleUninitialize();

        SetWindowClickAttribute(h, false);
        _rc = { 0,0,0,0 };
        idrag = false;
    }
}