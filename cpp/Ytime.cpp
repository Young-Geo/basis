/*
 * Ytime.cpp
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */
#include "Ytime.h"

Ytime::Ytime()
{
    gettimeofday(&_tv, NULL);//获取当前时间
    time_t t;
    t = time(NULL);
    this->_local = localtime(&t);
}

Ylong   Ytime::GetMilliSecond()
{
    return _tv.tv_sec * 1000 + _tv.tv_usec / 1000;
}
Ylong   Ytime::GetSecs()
{
    return _tv.tv_sec + _tv.tv_usec / (1000 * 1000);
}

/*
local->tm_year+1900,
local->tm_mon+1,
local->tm_mday,
local->tm_hour,
local->tm_min,
local->tm_sec;
*/
Ystring Ytime::GetStringLocalTime()
{
    Ychar timeBuf[BUF_MIN] = { 0 };
    Ysprintf(timeBuf, "%d-%d:%d", NowYear(_local->tm_year), _local->tm_mon+1, _local->tm_mday);
    return Ystring(timeBuf);
}
