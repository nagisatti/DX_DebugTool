/*
*   inspector.cpp
*   ゲーム実行中に変数等を確認、変更できる機能
*/

//********************************************************************
// インクルード
//********************************************************************
#include "inspector.h"
#include "imgui.h"

//********************************************************************
// floatの変数登録
//********************************************************************
void Inspector::RegisterFloat(const std::string& name, float* value)
{
	// ガード処理
	if (value == nullptr)
		return;

	m_items.push_back({
		name,
		ValueType::Float,
		value
		});

}

//********************************************************************
// intの変数登録
//********************************************************************
void Inspector::RegisterInt(const std::string& name, int* value)
{
	// ガード処理
	if (value == nullptr)
		return;

	m_items.push_back({
		name,
		ValueType::Int,
		value
		});

}

//********************************************************************
// boolの変数登録
//********************************************************************
void Inspector::RegisterBool(const std::string& name, bool* value)
{
	// ガード処理
	if (value == nullptr)
		return;

	m_items.push_back({
		name,
		ValueType::Bool,
		value
		});

}

//********************************************************************
// Inspectorの描画
//********************************************************************
void Inspector::Render()
{
	for (auto& item : m_items)
	{

		ImGui::PushID(item.name.c_str());

		// 左側に変数名を表示
		ImGui::TextUnformatted(item.name.c_str());

		// 次の要素を同じ行に配置
		ImGui::SameLine(150.0f);

		// 入力欄の幅
		ImGui::SetNextItemWidth(-1.0f);


		switch (item.type)
		{
		case ValueType::Float:
			//ImGui::DragFloat(item.name.c_str(), static_cast<float*>(item.value));
			ImGui::SliderFloat(item.name.c_str(), static_cast<float*>(item.value), 0.0f, 10.0f);
			break;

		case ValueType::Int:
			ImGui::DragInt(item.name.c_str(), static_cast<int*>(item.value));
			break;

		case ValueType::Bool:
			ImGui::Checkbox(item.name.c_str(), static_cast<bool*>(item.value));
			break;

		default:
			break;
		}

		ImGui::PopID();
	}

}

//********************************************************************
// 登録した変数の削除
//********************************************************************
void Inspector::Delete()
{
	m_items.clear();
}