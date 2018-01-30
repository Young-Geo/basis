/*
 * Ytime.h
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */
#ifndef __YTIME_H__
#define __YTIME_H__

#include "Ybasis.h"

#define Second(x) (x)
#define Minute(x) (x * 60)
#define Hour(x) (x * 3600)
#define Day(x) (x * 3600 * 24)
#define NowYear(x) (1900 + (x))

class Ytime
{
public:
    Ytime();
    Ylong   GetMilliSecond();
    Ylong   GetSecs();
    Ystring GetStringLocalTime();
private:
    struct timeval _tv;
    struct tm *_local;
};

#endif // __YTIME_H__
