/*
*   inspector.h
*   ゲーム実行中に変数等を確認、変更できる機能
*/
#ifndef INSPECTOR_H
#define INSPECTOR_H

//********************************************************************
// インクルード
//********************************************************************
#include <string>
#include <vector>

//********************************************************************
// クラス
//********************************************************************
class Inspector
{
public:
	// Vecor2型
	struct Vector2
	{
		float x;
		float y;
	};

	// Vecor3型
	struct Vector3
	{
		float x;
		float y;
		float z;
	};

	// 変数を登録
	void RegisterFloat(const std::string& name, float* value);
	void RegisterInt(const std::string& name, int* value);
	void RegisterBool(const std::string& name, bool* value);
	void RegisterVector2(const std::string& name, Vector2* value);
	void RegisterVector3(const std::string& name, Vector3* value);

	// グループ作成開始
	void BeginGroup(const std::string& groupName);
	// グループ作成終了
	void EndGroup();

	// Inspectorの描画
	void Render();

	// 登録した変数の削除
	void Delete();

	// 登録したグループの変数削除
	void GroupDelete(const std::string& groupName);

	// 登録した1つの変数の削除
	void OnceDelete(const std::string& name, const std::string& groupName);

private:
	// 登録する変数の型
	enum class ValueType
	{
		Float,
		Int,
		Bool,
		VECTOR2,
		VECTOR3,
	};

	// 登録する変数の情報
	struct InspectorItem
	{
		std::string name;
		std::string groupName;
		ValueType type;
		void* value;
	};

	// 登録された変数の一覧
	std::vector<InspectorItem> m_items;

	// 現在所属しているグループ名
	std::string m_currentGroup;
};


#endif // INSPECTOR_H
