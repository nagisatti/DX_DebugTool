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
#pragma region 変数登録
void Inspector::RegisterFloat(const std::string& name, float* value)
{
	// ガード処理
	if (value == nullptr)
		return;

	// 重複登録防止
	for (const auto& item : m_items)
	{
		if (item.groupName == m_currentGroup && item.name == name)
			return;
	}

	m_items.push_back({
		name,
		 m_currentGroup,
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

	// 重複登録防止
	for (const auto& item : m_items)
	{
		if (item.groupName == m_currentGroup && item.name == name)
			return;
	}

	m_items.push_back({
		name,
		 m_currentGroup,
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

	// 重複登録防止
	for (const auto& item : m_items)
	{
		if (item.groupName == m_currentGroup && item.name == name)
			return;
	}

	m_items.push_back({
		name,
		 m_currentGroup,
		ValueType::Bool,
		value
		});

}
#pragma endregion

//********************************************************************
// Inspectorの描画
//********************************************************************
void Inspector::Render()
{
	for (const auto& gropItem : m_items)
	{

		const std::string& group = gropItem.groupName;

		// 重複した表示の防止
		bool isDisplay = false;

		
		for (const auto& previous : m_items)
		{
			// 現在の要素まで調べたら検索終了
			if (&previous == &gropItem)
				break;

			// 以前に同じグループが存在したら重複と判定
			if (previous.groupName == group)
			{
				isDisplay = true;
				break;
			}
		}

		// 表示していたら飛ばす
		if (isDisplay)
			continue;


		ImGui::PushID(group.c_str());

		if (ImGui::TreeNode(group.empty() ? "General" : group.c_str()))
		{
			for (const auto& item : m_items)
			{
				if (item.groupName != group)
					continue;

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
					ImGui::SliderFloat(item.name.c_str(), static_cast<float*>(item.value), -100.0f, 5000.0f);
					break;

				case ValueType::Int:
					//ImGui::DragInt(item.name.c_str(), static_cast<int*>(item.value));
					ImGui::SliderInt(item.name.c_str(), static_cast<int*>(item.value), -100, 5000);
					break;

				case ValueType::Bool:
					ImGui::Checkbox(item.name.c_str(), static_cast<bool*>(item.value));
					break;

				default:
					break;
				}
				ImGui::PopID();
			}			
			// TreeNodeを開いた場合だけ呼び出す
			ImGui::TreePop();
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

//********************************************************************
// グループ作成開始
//********************************************************************
void Inspector::BeginGroup(const std::string& groupName)
{
	m_currentGroup = groupName;
}

//********************************************************************
// グループ作成終了
//********************************************************************
void Inspector::EndGroup()
{
	m_currentGroup.clear();
}