/*
 * Ylog.cpp
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */

#include "Ylog.h"

void        Log::ConsoleLog(YLOG_LEVEL level, const Ychar *file, const Ychar *func, const Yint line, const Ychar *format, ...)
{
    LogData *logdata = new LogData;
    YNULL(logdata);
    logdata->_level = level;
    logdata->_file  = Ystring(file);
    logdata->_func  = Ystring(func);
    logdata->_line  = line;
    va_list va;
    Ychar logBuf[BUF_SIZE] = { 0 };
    va_start(va, format);
    if (vsnprintf(logBuf, (BUFSIZ - 1), format, va) > 0) {
        logdata->_log   = Ystring(logBuf);
    }
    va_end(va);
}

void        Log::SetFileName(Ystring fileName)
{
    this->_fileName = fileName;
    work::Start();//直接开始线程
}

bool		Log::Init()
{
    return true;
}

Ystring        Log::FormatLogData(LogData *logdata)
{
    //初始化好数据格式放到队列中
    Ystring msg;

    YYNULL(logdata, msg);

    msg += "[";
    msg += _LOGSTRING(logdata->_level);
    msg += "]\t[";
    msg += logdata->_file;
    msg += "]\t[";
    msg += logdata->_func;
    msg += ":";
    Ychar strNumber[20] = { 0 };
    Ysprintf(strNumber, "%d", logdata->_line);
    msg += Ystring(strNumber);
    msg += "]\t";
    msg += logdata->_log;

    return msg;
}

void        Log::WriteFileLog(const Ychar *data)
{
    YNULL(data);
    //write file
    Ytime nTime;
    Ystring nTimeStr;
    nTimeStr = nTime.GetStringLocalTime();
    if (!_ofs.is_open() || _nTimeStr != nTimeStr) {
        _ofs.close();
        _nTimeStr = nTimeStr;
        nTimeStr += "_";
        nTimeStr += _fileName;
        _ofs.open(nTimeStr.c_str(), std::ios::app | std::ios::out);
    }
    if (_ofs.is_open()) {
        _ofs << data<< std::endl;
    }
}
void		Log::Loop()
{
    while (IsStop())
    {
        //应该读出数据写到日志中
        LogData *logdata = nullptr;
        logdata = (LogData *)this->Pop();
        if (!logdata) {
            //usleep(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        Ystring msg = FormatLogData(logdata);
        //write
        WriteFileLog(msg.c_str());
        delete logdata;
    }
}

void    Log::Exit()
{
    //
}
