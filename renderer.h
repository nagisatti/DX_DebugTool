/*
*   renderer.h 
*   このクラスは、DirectX 11を使用して描画を行うためのレンダラークラス
* 　DirectX 11の初期化、描画開始・終了、リサイズ処理などを行う
*/

#ifndef RENDERER_H
#define RENDERER_H

//********************************************************************
// インクルード
//********************************************************************
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>

//********************************************************************
// クラス
//********************************************************************
class Renderer
{
public:
    bool Initialize(HWND hwnd, int width, int height); 

	void BeginFrame();                  // 描画開始
	void EndFrame();                    // 描画終了
	void Resize(int width, int height); // ウィンドウサイズ変更時の処理
	void Release();                     // DirectXのリソース解放

	ID3D11Device* GetDevice() const;         // DirectXのデバイスを取得
	ID3D11DeviceContext* GetContext() const; // DirectXのデバイスコンテキストを取得

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;                     // DirectXのデバイス
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;             // DirectXのデバイスコンテキスト
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;                // スワップチェイン
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView; // レンダーターゲットビュー
};

#endif // RENDERER_H