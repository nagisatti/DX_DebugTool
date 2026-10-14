/*
*  debugManager.h
*  このクラスは、デバッグ用のマネージャークラス
*/

#ifndef DEBUGMANAGER_H
#define DEBUGMANAGER_H

//********************************************************************
// インクルード
//********************************************************************
#include <Windows.h>
#include <d3d11.h>
#include "inspector.h"

//********************************************************************
// クラス
//********************************************************************
class DebugManager 
{
public:
	// debugManagerの初期化
	bool Initialize(
		HWND hwnd, 
		ID3D11Device* device,
		ID3D11DeviceContext* context
	);

	void BeginFrame();   // 描画開始
	void Update();       // デバッグ機能の更新
	void Render();       // デバッグ描画
	void Release();      // DirectXのリソース解放

	// Inspectorへの変数登録
	void RegisterFloat(const std::string& name, float* value);
	void RegisterInt(const std::string& name, int* value);
	void RegisterBool(const std::string& name, bool* value);

private:
	bool m_visible = true;     // デバッグ描画の可視状態
	bool m_initialized = true; // 初期化成功・失敗のフラッグ
	Inspector m_inspector;

};


#endif // !DEBUGMANAGER_H

