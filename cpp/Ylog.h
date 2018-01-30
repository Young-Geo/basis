/*
 * Ylog.cpp
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */
#ifndef __YLOG_H__
#define __YLOG_H__

#include "Ybasis.h"
#include "Ywork.h"
#include "Ytime.h"
#include "Yalone.hpp"

#define _LOGSTRING(level) ((level)==1? _TO_STR(LOG):((level)==2? _TO_STR(BUG) : _TO_STR(ERR)))

enum    YLOG_LEVEL
{
    LOG_LEVEL_LOG = 1,
    LOG_LEVEL_BUG = 2,
    LOG_LEVEL_ERR = 3,
};

struct LogData
{
    LogData()
    {
        _level = LOG_LEVEL_LOG;
    }
    ~LogData(){}
    YLOG_LEVEL _level;
    Ystring    _file;
    Ystring    _func;
    Ystring    _log;
    Yint       _line;
};
//name  log 冲突
class Log : public work, public alone<Log>
{
public:
    void                SetFileName(Ystring fileName);
    virtual bool		Init();
    virtual void		Loop();
    virtual void        Exit();
public:
    void        ConsoleLog(YLOG_LEVEL level, const Ychar *file, const Ychar *func, const Yint line, const Ychar *format, ...);
    Ystring     FormatLogData(LogData *logdata);
    void        WriteFileLog(const Ychar *data);
private:
    Ystring _fileName;
    Ystring _nTimeStr;
    std::ofstream _ofs;
};

#define YLOG_LOG(format, ...) Log::GetEntity()->ConsoleLog(LOG_LEVEL_LOG, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define YLOG_BUG(format, ...) Log::GetEntity()->ConsoleLog(LOG_LEVEL_BUG, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define YLOG_ERR(format, ...) Log::GetEntity()->ConsoleLog(LOG_LEVEL_ERR, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#endif // YLOG_H
