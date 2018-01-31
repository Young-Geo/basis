/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#ifndef __YEVENT_H__
#define __YEVENT_H__

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>

#include "Ythreadpool.h"
#include "xlist.h"
#include "xchain.h"
#include "xlog.h"


typedef void (*event_function_r)(int fd, xchain *, void *);
typedef void (*event_function_w)(int fd, void *);
typedef void (*event_function_e)(int fd, short, void *);

typedef struct _event_rwe_t
{
    event_function_r call_r;
    event_function_w call_w;
    event_function_e call_e;
    void *arg;
    _event_rwe_t()
    {
        call_r = NULL;
        call_w = NULL;
        call_e = NULL;
        arg    = NULL;
    }
} event_rwe_t;

typedef struct _entity_t
{
    int inx;
    struct event_base *base;
    struct event notify_event;
    int notify_receive_fd;
    int notify_send_fd;
    int conn_num;
    event_rwe_t call;
    xchain  *chain;
    void    (*func)(void *);
} entity_t;


typedef struct _master_t
{
    struct event_base *master_base;
    struct event master_event;
    int conn_receive_fd;
    int last_event;

    threadpool_t *threadPool;
    xlist   *entitys;
    int     entitysSize;
    void    (*func)(void *);
} master_t;


typedef master_t Yevent_t;

Yevent_t*       Yevent_Create(const char *ip, const unsigned short port);
void            Yevent_Destory(Yevent_t *yevent);
int             Yevent_Start(Yevent_t *yevent, int workNum);


#endif // __YEVENT_H__
