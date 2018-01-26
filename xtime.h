#ifndef __H_XTIME_H__
#define __H_XTIME_H__
//////////////////////////////////////////////////////////////////////////
#include "afx.h"
//////////////////////////////////////////////////////////////////////////
#include <time.h>
#ifndef WIN32
#include <sys/time.h>
#else
int gettimeofday(struct timeval *tp, void *tzp) ;
#endif

void xtimer_start () ;
float xtimer_end ( char * string ) ;
int xgetminsecond () ;
int xgetimestr ( char time_str[BUFFER_SIZE] , int exact , int nice ) ;
char * xgen_random () ;
void xsleep( int ms_time ) ;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#endif
