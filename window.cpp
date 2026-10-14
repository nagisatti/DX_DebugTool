/*
*  window.cpp
*  このクラスは、ウィンドウの作成と管理を行うため
*/


//********************************************************************
// インクルード
//********************************************************************
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_win32.h"

//********************************************************************
// コンストラクタ
//********************************************************************
Window::Window()
    : m_hwnd(nullptr),
    m_hInstance(nullptr)
{
}

//********************************************************************
// デストラクタ
//********************************************************************
Window::~Window()
{
    if (m_hwnd != nullptr)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

//********************************************************************
// ウィンドウを作成する
// 作成に失敗した場合はfalseを返す
//********************************************************************
bool Window::Initialize(
    HINSTANCE hInstance,
    int width,
    int height,
    const wchar_t* title
)
{
    m_hInstance = hInstance;

    // ウィンドウクラスの設定
	// WNDCLASSEXW構造体を使用してウィンドウクラスを定義
	// windowsが用意しているウィンドウクラスの情報を格納する構造体
    WNDCLASSEXW wc = {};

	wc.cbSize = sizeof(WNDCLASSEXW);               // 構造体のサイズを設定
	wc.style = CS_HREDRAW | CS_VREDRAW;            // ウィンドウのサイズが変更されたときに再描画する
	wc.lpfnWndProc = WindowProc;                   // ウィンドウプロシージャの関数ポインタを設定、windowのメッセージを処理する関数
	wc.hInstance = hInstance;                      // ウィンドウクラスを登録するアプリケーションのインスタンスハンドル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);   // カーソルの形状を設定、IDC_ARROWは矢印のカーソル
    wc.hbrBackground = nullptr;                    // ウィンドウの背景色を設定、COLOR_WINDOWはウィンドウの背景色、WinUser.hに定義されている
	wc.lpszClassName = L"GameDebugStudioWindow";   // ウィンドウクラスの名前を設定

    // ウィンドウクラスを登録
    if (!RegisterClassExW(&wc))
    {
        return false;
    }

    // クライアント領域を1280×720にするための調整
    //           左 上 　 右 　下
    RECT rect = { 0, 0, width, height };

	// AdjustWindowRect関数を使用して、ウィンドウのサイズを調整する
	// 枠を含むクライアント領域のサイズを指定して、ウィンドウのサイズを計算
    AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,FALSE);

    // ウィンドウを作成
    m_hwnd = CreateWindowExW(
		0,                       // 拡張スタイルなし
		wc.lpszClassName,        // ウィンドウクラス名
		title,                   // ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,     // ウィンドウのスタイル、WS_OVERLAPPEDWINDOWは標準的なウィンドウスタイル
		CW_USEDEFAULT,           // ウィンドウの初期位置をデフォルトに設定
		CW_USEDEFAULT,           // ウィンドウの初期位置をデフォルトに設定
		rect.right - rect.left,  // ウィンドウの幅を設定、AdjustWindowRectで調整済み
		rect.bottom - rect.top,  // ウィンドウの高さを設定、AdjustWindowRectで調整済み
		nullptr,                 // 親ウィンドウなし
		nullptr,                 // メニューなし
		hInstance,               // アプリケーションのインスタンスハンドル
		nullptr                  // ウィンドウ作成時の追加情報なし
    );

    return m_hwnd != nullptr;
}

//********************************************************************
// ウィンドウ表示
//********************************************************************
void Window::Show(int nCmdShow)
{
    // m_hwnd が表すウィンドウを表示状態
	ShowWindow(m_hwnd, nCmdShow);
    // 描画が必要な領域がある場合に、ウィンドウの描画処理を行う
	UpdateWindow(m_hwnd);        
}

//********************************************************************
// HWNDのゲッター
//********************************************************************
HWND Window::GetHWND() const
{
    return m_hwnd;
}

//********************************************************************
// ImGuiのウィンドウプロシージャハンドラを呼び出す
//********************************************************************
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

//******************************************************************** 
// ウィンドウのメッセージを処理する
// 必要に応じて、メッセージを処理するコードを追加する
//********************************************************************
LRESULT CALLBACK Window::WindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
	// ImGuiのコンテキストが存在する場合は、ImGuiのウィンドウプロシージャハンドラを呼び出す
    if (ImGui::GetCurrentContext() != nullptr)
        // ImGuiにWindowsメッセージを渡す
        if (ImGui_ImplWin32_WndProcHandler(
            hwnd,
            message,
            wParam,
            lParam
        ))
            return 1;

	// メッセージの種類に応じて処理を分岐
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(
        hwnd,
        message,
        wParam,
        lParam
    );

}