/*
*   Windowクラスの実装
*   このクラスは、Windowsアプリケーションのウィンドウを作成し、管理する用
*/

#include <Windows.h>

class Window
{
public:
    Window();
    ~Window();

    // ウィンドウを作成する
    bool Initialize(
        HINSTANCE hInstance,
        int width,
        int height,
        const wchar_t* title
    );

    // ウィンドウを表示する
    void Show(int nCmdShow);

    // ウィンドウハンドルを取得する
    HWND GetHWND() const;

    // ウィンドウのメッセージを処理する
    static LRESULT CALLBACK WindowProc(
        HWND hwnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );

private:
    HWND m_hwnd;
    HINSTANCE m_hInstance;
};
