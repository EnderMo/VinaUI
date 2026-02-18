# 欢迎探索 VinaUI！

这是一个基于 Direct2D 构建的轻量级、高性能、高灵活性的 C++ 界面框架。VinaUI 旨在为开发者提供近似原生的体验同时，创造简单、轻松的开发体验，为创造力的释放搭建空间。

## 🚀 核心特性

- **轻量高效**：VinaUI 在原生库的基础上进行高效灵活的封装，可以轻松生成美观且小体积的应用程序。
- **Direct2D 渲染**：使用原生 API 的 GPU 加速渲染。
- **现代控件库**：内置 Button, Slider, Switch, TextBox，Edit，Card 等组件。
- **动画接口**：仅需一行代码即可调用基于 Bezier 曲线的并行、高性能动画。
- **DPI 自适应**：原生支持屏幕缩放，解决高分屏模糊问题。

## ✏️ 快速入门

### 1. 创建主窗口

VinaUI 使用 `VinaWindow` 对象管理窗口生命周期。你可以直接实例化并设置初始参数。

```cpp
#include <VinaWindow.hpp>

std::shared_ptr<VinaWindow> MainWindow = std::make_shared<VinaWindow>();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 初始化全局资源与颜色系统
    InitGlobalD2D();
    VuiColorSystemInit();
    gScale = GetScreenScale();

    // 配置窗口
    MainWindow->Set(100, 100, 720 * gScale, 480 * gScale, L"MyApp", L"My VinaUI App");

    // 配置边框窗口样式 
    MainWindow->SetOutFrame(VinaWindow::Client);

    // 设置创建回调
    MainWindow->OnCreateCmd = [] {
        CenterWindow(MainWindow->GetHandle());
        MainWindow->InitAnimation();
        MainWindow->StartAnimationSystem();
        //此处必须要启动动画系统。
    };

    // 运行消息循环
    // 仅当窗口需要消息循环时才使用 RunFull()。一般创建窗口使用 Create()。
    return MainWindow->RunFull();
}
```

### 2. 创建绘图面板 (CreatePanel)

VinaUI 的核心逻辑在于 Panel。它接受一个 Lambda 表达式。当窗口重绘时，此函数会被调用。

> **规范**：建议将 UI 变量封装在结构体或类中，通过 Lambda 捕获。

```cpp
struct AppContext {
    std::shared_ptr<VinaButton> btn = std::make_shared<VinaButton>();
    std::shared_ptr<VinaEdit> edt = std::make_shared<VinaEdit>();
};

auto ctx = std::make_shared<AppContext>();

MainWindow->CreatePanel([ctx](HWND hWnd, ID2D1HwndRenderTarget* hrt) {
    RECT rc;
    GetClientRect(hWnd, &rc);

    // 绘制背景层
    D2DDrawSolidRect(hrt, 0, 0, rc.right, rc.bottom, VERTEXUICOLOR_DARKNIGHT);
    
    // 绑定当前绘图句柄到 Panel (必须在 Add 控件之前执行)
    MainWindow->GetPanel()->Set(hWnd, hrt);

    // 控件代码 
});
```

### 3. 添加控件

所有控件都通过 `MainWindow->GetPanel()->Add()` 加入渲染队列。  
以下为部分控件的示例。

#### 按钮 (VinaButton)

```cpp
ctx->btn->Set(40, 60, 140, 40, L"Click", [] {
    MessageBox(0, L"Hello VinaUI!", L"Tips", 0);
    return 0;
});
MainWindow->GetPanel()->Add(ctx->btn);
```

#### 输入框 (VinaEdit)

```cpp
if (!ctx->edt->cx) { // 简单的初始化检查
    ctx->edt->Set(260, 180, 300, 35, L"请输入内容...");
}
MainWindow->GetPanel()->Add(ctx->edt);
```

#### 开关 (VinaSwitch)

```cpp
ctx->sw->Set(40, 190, 60, 30, {VERTEXUICOLOR_DARKEN}, [] {});
MainWindow->GetPanel()->Add(ctx->sw);
```

> **注意**：部分有滑动条的控件需要先使用 `SetParent()` 方法绑定 Panel。

### 4. 使用 Bezier 动画

VinaUI 提供了简单、灵活的动画接口。你可以通过 `AnimateVariableWithBezier` 动态修改任何变量，实现动画效果。

```cpp
// 示例：让一个字符的 Y 轴位置从 18.0 变动到 0.0，耗时 0.4 秒
MainWindow->AnimateVariableWithBezier(
    hWnd, 
    item.yOffset,     // 目标变量引用
    18.0f,            // 起始值
    0.0f,             // 结束值
    0.4,              // 时长(秒)
    0.25, 0.1, 0.25, 1.0 // Bezier
);
```

特别鸣谢：
@Haceau-Zoac (vui.parser)
@KitraMGP （规范性提议）
THANKS TO Vilinko All.
