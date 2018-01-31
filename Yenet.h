/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#ifndef __YENET_H__

#include <enet/enet.h>

#include "Ythreadpool.h"
#include "xlist.h"

typedef enum _ENET
{
    ENET_SERVER = 1,
    ENET_CLIENT = 2,
} ENET;

typedef struct _Yenet_t
{
    threadpool_t *threadPool;
    ENetHost *handle;
    ENET model;
    int  isInit;
} Yenet_t;

Yenet_t*    Yenet_Create(ENET model, const char *ip, const unsigned short port);
void        Yenet_Destory(Yenet_t*, int flag);
int         Yenet_Start(Yenet_t *, int workNum);

#define __YENET_H__
#endif // __YENET_H__
