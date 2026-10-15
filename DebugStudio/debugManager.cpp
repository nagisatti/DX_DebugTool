/*
*  debugManager.cpp
*  このクラスは、デバッグ用のマネージャークラス
*/

//********************************************************************
// インクルード
//********************************************************************
#include "debugManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


//********************************************************************
// ImGuiの初期化
//********************************************************************
bool DebugManager::Initialize(
	HWND hwnd,
	ID3D11Device* device,
	ID3D11DeviceContext* context
)
{
	// ガード処理
	if(hwnd == nullptr || device == nullptr || context == nullptr)
		return false;

	// ImGuiのバージョンチェック
	IMGUI_CHECKVERSION();

	// ImGuiのコンテキストを作成
	ImGui::CreateContext();

	// Win32バックエンドの初期化
	if (!ImGui_ImplWin32_Init(hwnd))
	{
		// ImGuiのコンテキストを破棄
		ImGui::DestroyContext();
		return false;
	}


	// DirectX11バックエンドの初期化
	if(!ImGui_ImplDX11_Init(device, context))
	{
		// win32バックエンドのシャットダウン
		ImGui_ImplWin32_Shutdown();
		// ImGuiのコンテキストを破棄
		ImGui::DestroyContext();
		return false;
	}

	// 初期化成功
	m_initialized = true;

	return true;
}

//********************************************************************
// デバッグ機能の更新
//********************************************************************
void DebugManager::Update()
{
	// ガード処理
	if (!m_initialized)
		return;

	// F1キーが押された場合にモードの切り替え
	if (GetAsyncKeyState(VK_F1) & 1)
	{
		m_visible = !m_visible;
	}

}

//********************************************************************
// 描画開始
//********************************************************************
void DebugManager::BeginFrame()
{
	// ガード処理
	if (!m_initialized)
		return;

	// DX11バックエンドのフレーム準備
	ImGui_ImplDX11_NewFrame();

	// Win32バックエンドのフレーム準備
	ImGui_ImplWin32_NewFrame();

	// ImGuiのフレームを開始
	ImGui::NewFrame();
}

//********************************************************************
// ImGuiの描画
//********************************************************************
void DebugManager::Render()
{
	// ガード処理
	if (!m_initialized)
		return;

	if (m_visible)
	{
		// ImGuiのフレームを開始
		ImGui::Begin("Game Debug Studio", &m_visible);

		// デバッグ情報を表示

		// Inspectorを描画
		if (ImGui::TreeNode("Inspector"))
		{

			ImGui::Separator();

			m_inspector.Render();

			ImGui::TreePop();
		}

		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);


		ImGui::End();
	}

	// ImGuiの描画を実行
	// ウィンドウ非表示でもフレームは完成させる
	ImGui::Render();

	// ImGuiの描画データをDirectX11に送信
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

//********************************************************************
// ImGuiの終了処理
//********************************************************************
void DebugManager::Release()
{
	// ガード処理
	if (!m_initialized)
		return;

	// DX11バックエンドを終了
	ImGui_ImplDX11_Shutdown();

	// Win32バックエンドを終了
	ImGui_ImplWin32_Shutdown();

	// ImGuiのコンテキストを破棄
	ImGui::DestroyContext();

	// 解放完了
	m_initialized = false;
}

//********************************************************************
// Inspectorへ変数登録
//********************************************************************
#pragma region Inspector登録関数
	// float
	void DebugManager::RegisterFloat(const std::string & name, float* value)
	{
		m_inspector.RegisterFloat(name, value);
	}

	// int
	void DebugManager::RegisterInt(const std::string & name, int* value)
	{
		m_inspector.RegisterInt(name, value);
	}

	// bool
	void DebugManager::RegisterBool(const std::string & name, bool* value)
	{
		m_inspector.RegisterBool(name, value);
	}
#pragma endregion

//********************************************************************
// グループ登録開始
//********************************************************************
void DebugManager::BeginInspectorGroup(const std::string& groupName)
{
	m_inspector.BeginGroup(groupName);
}

//********************************************************************
// グループ登録終了
//********************************************************************
void DebugManager::EndInspectorGroup()
{
	m_inspector.EndGroup();
}