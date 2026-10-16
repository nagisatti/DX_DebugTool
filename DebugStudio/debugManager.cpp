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
bool DebugManager::Initialize(HWND hwnd,ID3D11Device* device,ID3D11DeviceContext* context)
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

	UpdateKey();

}

//********************************************************************
// キーボード状態の更新
//********************************************************************
void DebugManager::UpdateKey()
{
	static bool previousKeys[12] = {};

	for (int key = VK_F1; key <= VK_F12; key++)
	{
		int index = key - VK_F1;

		// 現在押されているかどうか
		isKey = (GetAsyncKeyState(key) & 0x8000) != 0;

		// 押された瞬間のみ実行
		if (isKey && !previousKeys[index])
		{
			switch (key)
			{
			// F1キーが押された場合にモードの切り替え
			case VK_F1:
				m_visible = !m_visible;
				break;

			// 削除関係のデバッグ用コマンド
			case VK_F2:
				m_inspector.OnceDelete("Player Speed", "player");

			case VK_F3:
				break;

			default:
				break;
			}
		}
		// 現在の状態を保存
		previousKeys[index] = isKey;
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
			m_inspector.Render();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("DebugConsole"))
		{

			ImGui::Separator();
			m_console.Render();
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
	void DebugManager::RegisterFloat(const std::string & name, float* value, bool readOnly)
	{
		m_inspector.RegisterFloat(name, value,readOnly);
	}

	// int
	void DebugManager::RegisterInt(const std::string & name, int* value, bool readOnly)
	{
		m_inspector.RegisterInt(name, value, readOnly);
	}

	// bool
	void DebugManager::RegisterBool(const std::string & name, bool* value, bool readOnly)
	{
		m_inspector.RegisterBool(name, value, readOnly);
	}

	// vector2
	void DebugManager::RegisterVector2(const std::string& name, Inspector::Vector2* value, bool readOnly)
	{
		m_inspector.RegisterVector2(name, value, readOnly);
	}

	//vector3
	void DebugManager::RegisterVector3(const std::string& name, Inspector::Vector3* value, bool readOnly)
	{
		m_inspector.RegisterVector3(name, value, readOnly);
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

//********************************************************************
// ログ登録
//********************************************************************
#pragma region ログ登録
// info
void DebugManager::LogInfo(const std::string& message)
{
	m_console.AddLog(DebugConsole::LogLevel::Info, message);
}

// warning
void DebugManager::LogWarning(const std::string& message)
{
	m_console.AddLog(DebugConsole::LogLevel::Warning, message);
}

// error
void DebugManager::LogError(const std::string& message)
{
	m_console.AddLog(DebugConsole::LogLevel::Error, message);
}

#pragma endregion