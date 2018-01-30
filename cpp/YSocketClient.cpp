/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#include "YSocketClient.h"

SocketClient::SocketClient()
{
    m_socketFd = NULL;
    m_isbuff = NULL;
    m_socketBuf = NULL;
}

SocketClient::SocketClient(bufferevent *ptr)
{
    m_socketBuf   = ptr;
    m_isbuff    = true;
    m_socketFd    = NULL;
}

SocketClient::SocketClient(struct event_base *base, Yint _fd, Yint opt)
{
    m_socketFd = NULL;
    m_isbuff = true;
    m_socketBuf = bufferevent_socket_new(base, _fd, opt);
}

SocketClient::SocketClient(Socket *socket)
{
    this->m_socketFd = socket;
    this->m_isbuff = false;
    this->m_socketBuf = NULL;
}

SocketClient::~SocketClient(){}

Yint        SocketClient::GetFd()
{
    Yint fd = 0;

    if (m_socketBuf && m_isbuff) {
        fd = bufferevent_getfd(m_socketBuf);
    }

    if (m_socketFd && !m_isbuff) {
        fd = m_socketFd->GetFd();
    }
    return fd;
}

void        SocketClient::Destory()
{
    if (m_socketBuf && m_isbuff) {
        bufferevent_free(m_socketBuf);
    }

    if (m_socketFd && !m_isbuff) {
        m_socketFd->Destory();
    }
}
