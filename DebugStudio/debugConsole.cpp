/*
*   debugConsole.cpp
*   コンソールログの描画を管理
*/

//********************************************************************
// インクルード
//********************************************************************
#include "debugConsole.h"
#include "imgui.h"
#include <fstream>


//********************************************************************
// ログの追加
//********************************************************************
void DebugConsole::AddLog(LogLevel level, const std::string& message)
{
    // 最大件数に達していたら古いログを削除
    if (m_logs.size() >= MAX_LOGS)
    {
        m_logs.erase(m_logs.begin());
    }

    // 現在時刻を取得
    std::time_t now = std::time(nullptr);

	m_logs.push_back({ level, message, now });

    isAdd = true;
}

//********************************************************************
// ログの描画
//********************************************************************
void DebugConsole::Render()
{    
    // 検索欄
    ImGui::InputTextWithHint("##Serach", "Search variables ...", m_search, sizeof(m_search));

    // フィルタリング用ボタン

    ImGui::Checkbox("Info",&level[0]);
    ImGui::SameLine();
    ImGui::Checkbox("Warning",&level[1]);
    ImGui::SameLine();
    ImGui::Checkbox("Error",&level[2]);

    ImGui::Separator();

    ImGui::BeginChild("ConsoleLog");

    for (const auto& log : m_logs)
    {
        // 保存された時刻を、時・分・秒に変更
        std::tm  when = {};
        localtime_s(&when, &log.time);

        char timeBuf[16] = {};

        // 時刻を文字列に変換
        std::strftime(timeBuf,sizeof(timeBuf),"%H:%M:%S",&when);

        ImVec4 color;

        const std::string& group = log.message;

        if (!(m_inspector.Search(group, m_search)))
            continue;

        // ログレベルのフィルタリング
        if (level[0] || level[1] || level[2])
        {
            if (LogSearch(LogLevel::Info, log.level) && !level[0])
                continue;

            if (LogSearch(LogLevel::Warning, log.level) && !level[1])
                continue;

            if (LogSearch(LogLevel::Error, log.level) && !level[2])
                continue;
        }

        switch (log.level)
        {
        case LogLevel::Info:
            color = ImVec4(0.4f, 0.7f, 1.0f, 1.0f);
            break;

        case LogLevel::Warning:
            color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);
            break;

        case LogLevel::Error:
            color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
            break;
        }

        ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 1.0f }, "[%s]", timeBuf);
        ImGui::SameLine();
        ImGui::TextColored(color, "% s", log.message.c_str());
    }

    // 自動スクロール
    AutoScroll();

    ImGui::EndChild();

    // ログの消去
    if (ImGui::Button("Clear"))
    {
        Clear();
    }

}

//********************************************************************
// ログの消去
//********************************************************************
void DebugConsole::Clear()
{
	m_logs.clear();
}

//********************************************************************
// ログのレベルフィルタリング
//********************************************************************
bool const DebugConsole::LogSearch(LogLevel level , LogLevel level1)
{
    return level == level1;
}

//********************************************************************
// ログの自動スクロール
//********************************************************************
void DebugConsole::AutoScroll()
{
    // ログが追加されたら
    if (isAdd)
    {
        // 画面下部まで自動スクロール
        //ImGui::SetScrollHereY(1.0f);
        ImGui::SetScrollY(ImGui::GetScrollMaxY());

        isAdd = false;
    }
}

//********************************************************************
// ログのファイル出力
//********************************************************************
void DebugConsole::OutPut() const
{
    // ガード処理
    if (!isAdd)
        return;

    // 出力ファイルを開く
    std::ofstream outputFile("log.txt");

    // ファイルが開けなかった場合
    if (!outputFile.is_open())
        return;

    // ログを1件ずつ出力
    for (const auto& log : m_logs)
    {
        outputFile << log.message << '\n';
    }

    // ファイルを閉じる
    outputFile.close();

}