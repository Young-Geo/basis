/*
 * YS.h
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */
#ifndef __YS_H__
#define __YS_H__

#include "Ybasis.h"
#include "Ysocket.h"
#include "Ychain.hpp"
#include "YSocketClient.h"
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>

#include <functional>

#define WORK_THREAD 4

typedef void (*event_function_r)(SocketClient sock, Ychain_t &, void *);
typedef void (*event_function_w)(SocketClient sock, void *);
typedef void (*event_function_e)(SocketClient sock, Yshort, void *);

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
    //pthread_t thread_id;
    //std::thread *thread_handle;
    std::shared_ptr<std::thread> thread_ptr;
    struct event_base *base;
    struct event notify_event;
    int notify_receive_fd;
    int notify_send_fd;
    int conn_num;
    Ychain_t *chain;
    event_rwe_t call;
} entity_t;


typedef struct _master_t
{
    struct event_base *master_base;
    struct event master_event;
    int conn_receive_fd;
    int last_event;
    std::vector<entity_t *> _entitys;
    std::shared_ptr<std::thread> thread_ptr;
} master_t;

entity_t* Entity_Init(event_rwe_t call);

master_t* Master_Init(YSOCKET::sock_addr_t addr);


//封装libevent
class YS
{
public:
    void    Start(YSOCKET::sock_addr_t addr);
    bool    AddEvent(event_rwe_t call);
    bool    AddWorkEvent(event_function_r call_r, event_function_w call_w, event_function_e call_e, void *arg);
private:
    master_t *_master;
};

#endif // __YS_H__
