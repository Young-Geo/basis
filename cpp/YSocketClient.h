/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#ifndef __YSOCKETCLIENT_H__
#define __YSOCKETCLIENT_H__

#include "Ybasis.h"
#include "Ysocket.h"

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>


class SocketClient
{
public:
    SocketClient();
    SocketClient(bufferevent *_ptr);
    SocketClient(struct event_base *base, Yint _fd, Yint opt);
    SocketClient(Socket *socket);
    ~SocketClient();
public:
    Yint    GetFd();
    void    Destory();
private:
    bufferevent *m_socketBuf;
    Socket      *m_socketFd;
    bool         m_isbuff;
};

#endif // YSOCKETCLIENT_H
