
//********************************************************************
// インクルード
//********************************************************************
#include <Windows.h>
#include "Window.h"
#include "renderer.h"
#include "debugManager.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    // ウィンドウ管理クラスを生成
    Window window;

    // ウィンドウを初期化
    if (!window.Initialize(
        hInstance,
        1280,
        720,
        L"Game Debug Studio"
    ))
    {
        MessageBoxW(
            nullptr,
            L"ウィンドウの作成に失敗しました",
            L"Error",
            MB_OK | MB_ICONERROR
        );

        return -1;
    }

	// DirectX 11のレンダラーを生成
    Renderer renderer;

    if (!renderer.Initialize(
        window.GetHWND(),
        1280,
        720
    ))
    {
        MessageBoxW(
            nullptr,
            L"DirectX 11の初期化に失敗しました",
            L"Error",
            MB_OK | MB_ICONERROR
        );

        return -1;
    }

    float playerSpeed = 5.0f;
    int playerHP = 100;
    bool invincible = false;

	// ImGui生成
	DebugManager debugManager;

    if (!debugManager.Initialize(
        window.GetHWND(),
        renderer.GetDevice(),
        renderer.GetContext()
    ))
    {
        MessageBoxW(
            nullptr,
            L"ImGuiの初期化に失敗しました",
            L"Error",
            MB_OK | MB_ICONERROR
        );
        return -1;
	}

    debugManager.BeginInspectorGroup("player");
    debugManager.RegisterFloat("Player Speed", &playerSpeed);
    debugManager.RegisterInt("Player HP", &playerHP);
    debugManager.RegisterBool("Invincible", &invincible);
    debugManager.EndInspectorGroup();
    // ウィンドウを表示
    window.Show(nCmdShow);

    // メインループ
    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessageW(
            &msg,
            nullptr,
            0,
            0,
            PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        else
        {
            renderer.BeginFrame();


            debugManager.Update();
			debugManager.BeginFrame();

            // 後でゲーム・ImGuiの描画を追加

			debugManager.Render();
             
            renderer.EndFrame();
        }
    }

    // DirectX 11の終了処理
    renderer.Release();

    return static_cast<int>(msg.wParam);
}