/*
 * Ybasis.h
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */

#ifndef __YBASIS_H__
#define __YBASIS_H__

#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <assert.h>
#include <memory.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <stdarg.h>

typedef short   Yshort;
typedef int     Yint;
typedef long    Ylong;
typedef double  Ydouble;
typedef char    Ychar;
typedef std::string Ystring;

#define Kbyte(x) (1024 * (x))
#define Mbyte(x) (1024 * 1024 * (x))
#define Gbyte(x) (1024 * 1024 * 1024 * (x))
#define Tbyte(x) (1024 * 1024 * 1024 * 1024 * (x))

#define YINTMAX        2147483647    /* maximum (signed) int value */

#define BUF_SIZE 512
#define BUF_MIN  256
#define BUF_MAX  1024
#define BUF_MMAX 1024 * 1024

#define Ynew new
#define Ydelete delete
#define Yassert assert
#define Yzero(x,y) memset((x), 0, (y))
#define Yfree(ptr) free((ptr))
#define Ymalloc(size) malloc((size))
#define Ycalloc(n,s) calloc((n), (s))
#define Ymemcpy(dest, src, n) memcpy((dest), (src), (n))

#define YYNULL(x, y) if(!(x)) return y
#define YNULL(x) if(!(x)) return
#define _TO_STR(var)  (#var)


#define Ysprintf(str, format, ...) sprintf((str), (format), ##__VA_ARGS__)



#endif /* __YBASIS_H__ */
