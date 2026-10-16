/*
*   debugConsole.h
*   コンソールログの描画を管理
*/
#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H


//********************************************************************
// インクルード
//********************************************************************
#include <string>
#include <vector>
#include "inspector.h"
#include <time.h>
#include <ctime>

//********************************************************************
// クラス
//********************************************************************
class DebugConsole
{
public:
	// 表示レベル
	enum class LogLevel
	{
		Info,
		Warning,
		Error,
	};

    struct LogEntry
    {
        LogLevel level;
        std::string message;
        std::time_t time;
    };

private:
    std::vector<LogEntry> m_logs;
    char m_search[128] = {};
    Inspector m_inspector;

    // ログのレベルフィルタリング
    bool LogSearch(LogLevel level, LogLevel level1);

    bool level[3] = { false };

    // 現在時刻取得用構造体
    struct tm {
        int tm_sec;      // 秒 [0-61] 最大2秒までのうるう秒を考慮
        int tm_min;      // 分 [0-59] 
        int tm_hour;     // 時 [0-23] 
        int tm_mday;     // 日 [1-31] 
        int tm_mon;      // 月 [0-11] 0から始まることに注意 
        int tm_year;     // 年 [1900からの経過年数] 
        char* tm_zone;   //タイムゾーン
    };

public:
    void AddLog(LogLevel level, const std::string& message);
    void Clear();
    void Render();

private:
    static constexpr size_t MAX_LOGS = 1000;                 


};




#endif // DEBUGCONSOLE_H