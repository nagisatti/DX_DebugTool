/*
*   renderer.cpp
*   このクラスは、DirectX 11を使用して描画を行うためのレンダラークラス
* 
*/

//********************************************************************
// インクルード
//********************************************************************
#include "renderer.h"

//********************************************************************
// リンク設定
//********************************************************************
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

//********************************************************************
// 描画初期化
//********************************************************************
bool Renderer::Initialize(HWND hwnd, int width, int height)
{
	// 引数のウィンドウハンドルが有効かどうかをチェック
	if(hwnd == nullptr)
		return false;

	// SwapChainの設定
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	// スワップチェインの設定
	swapChainDesc.BufferCount = 2;                                // バッファ数(2 = ダブルバッファ)
	swapChainDesc.BufferDesc.Width = width;                       // バッファの幅
	swapChainDesc.BufferDesc.Height = height;                     // バッファの高さ
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // バッファのフォーマット     赤、緑、青、アルファの順で8ビットずつの32ビットカラー
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;          // リフレッシュレートの分子
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;         // リフレッシュレートの分母
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // バッファの使用方法、レンダーターゲットとして使用する
	swapChainDesc.OutputWindow = hwnd;                            // 出力先のウィンドウハンドル
	swapChainDesc.SampleDesc.Count = 1;                           // MSAA(マルチサンプリングアンチエイリアシング)のサンプル数、1はMSAAなし
	swapChainDesc.SampleDesc.Quality = 0;                         // MSAAの品質レベル、0は標準品質
	swapChainDesc.Windowed = TRUE;                                // ウィンドウモードで起動するかどうか、TRUEはウィンドウモード、FALSEはフルスクリーンモード
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // スワップ効果、フリップモデルを使用する場合はFLIP_DISCARDを使用する

	// 使用するDirectXの機能レベルを指定する配列
	// D3D_FEATURE_LEVEL_11_0はDirectX 11.0の機能レベルを表す
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0, // DirectX 11.0
	};

	// 実際に使用される機能レベルを格納する変数
	D3D_FEATURE_LEVEL featureLevel; 

	// DirectX 11のデバイスとコンテキスト、スワップチェインを作成する
	HRESULT hr = D3D11CreateDeviceAndSwapChain(  
		nullptr,                    
		D3D_DRIVER_TYPE_HARDWARE, 
		nullptr,
		0,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		m_swapChain.GetAddressOf(),
		m_device.GetAddressOf(),
		&featureLevel,
		m_context.GetAddressOf()
	);

	// ガード処理
	// HRESULT はWindows APIで処理結果を表す型であり、
	// 下記のマクロを使用して、処理が成功したかどうかを判定する事ができる
	if (FAILED(hr))
		return false;

	// バックバッファを取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	hr = m_swapChain->GetBuffer(
		0,
		IID_PPV_ARGS(backBuffer.GetAddressOf())
	);

	if (FAILED(hr))
	{
		Release();
		return false;
	}


	// RenderTargetViewを作成
	// バックバッファをDX11の描画先として使用するために、RenderTargetViewを作成する
	// ただ作成しただけでは描画先として使用できないため、毎フレームOMsetRenderTargets関数で設定する必要がある
	hr = m_device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		m_renderTargetView.GetAddressOf()
	);

	// ガード処理
	if (FAILED(hr))
	{
		Release();
		return false;
	}
		
	// 描画範囲(Viewport)の設定
	D3D11_VIEWPORT viewport = {};

	// ビューポートの設定
	viewport.TopLeftX = 0.0f;                     // ビューポートの左上のX座標
	viewport.TopLeftY = 0.0f;                     // ビューポートの左上のY座標
	viewport.Width = static_cast<float>(width);   // ビューポートの幅
	viewport.Height = static_cast<float>(height); // ビューポートの高さ
	viewport.MinDepth = 0.0f;                     // ビューポートの最小深度
	viewport.MaxDepth = 1.0f;                     // ビューポートの最大深度

	// ビューポートを設定
	// 今回は、ウィンドウ全体を描画範囲として使用するため、1つのビューポートを設定する
	m_context->RSSetViewports(1, &viewport);    
	
	return true;
}

//********************************************************************
// 描画開始
//********************************************************************
void Renderer::BeginFrame()
{
	// 背景色
	const float clearColor[4] = { 0.1f, 0.0f, 0.3f, 1.0f }; // RGBA

    // 描画先の設定
	ID3D11RenderTargetView* renderTaget = m_renderTargetView.Get();

	m_context->OMSetRenderTargets(
		1,
		&renderTaget,
		nullptr
	);

	m_context->ClearRenderTargetView(
		renderTaget,
		clearColor
	);

}

//********************************************************************
// 描画終了
//********************************************************************
void Renderer::EndFrame()
{
	// 1は垂直同期を有効にする、0はフラグなし
	m_swapChain->Present(1, 0);
}

//********************************************************************
// ウィンドウサイズ変更時の処理
//********************************************************************
void Renderer::Resize(int width, int height)
{

}

//********************************************************************
// DirectXのリソース解放
//********************************************************************
void Renderer::Release()
{
	// ComPtrを使用しているため、解放処理を書く必要はないが、
	// 念のため、Reset()を呼び出してリソースを解放する
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_context.Reset();
	m_device.Reset();

}

//********************************************************************
// DirectXのデバイスを取得
//********************************************************************
ID3D11Device* Renderer::GetDevice() const
{
	return m_device.Get();
}

//********************************************************************
// DirectXのデバイスコンテキストを取得
//********************************************************************
ID3D11DeviceContext* Renderer::GetContext() const 
{
	return m_context.Get();
}