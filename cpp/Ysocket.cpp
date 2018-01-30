/*
 * Ysocket.cpp
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */
#include "Ysocket.h"

Socket::Socket()
{
    _maxListenNum = MAXLISTENNUM;
    _fd = -1;
    this->_isInit = false;
}

Socket::Socket(YSOCKET::SOCKET_MODEL mode, YSOCKET::SOCKET_STREAM_MODEL streamMode, YSOCKET::sock_addr_t addr)
{
    this->_mode = mode;
    this->_streamMode = streamMode;
    this->_addr = addr;
    _maxListenNum = MAXLISTENNUM;
    _fd = -1;
    this->_isInit = false;
}

void    Socket::SetSocketModel(YSOCKET::SOCKET_MODEL mode)
{
    this->_mode = mode;
}

void    Socket::SetSocketStreamModel(YSOCKET::SOCKET_STREAM_MODEL streamMode)
{
    this->_streamMode = streamMode;
}

void    Socket::SetAddr(YSOCKET::sock_addr_t addr)
{
    this->_addr = addr;
}

void    Socket::SetMaxListenNum(Yint num)
{
    this->_maxListenNum = num;
}

Yint    Socket::GetFd()
{
    return this->_fd;
}

bool    Socket::Init()
{
    struct sockaddr_in addr;
    socklen_t slen;

    if (_isInit) {
        YLOG_LOG("Socket::Init _isInit double\n");
        return false;
    }

    if((_fd = ::socket(AF_INET, YSOCKET::SOCKET_STREAM_TCP, 0)) < 0) {
        YLOG_ERR("Socket::Init socket error %d\n", _fd);
        return false;
    }

    Yzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    //addr.sin_addr.s_addr = htonl(_ip);
    inet_pton(AF_INET, _addr._ip.c_str(), (void *)(&addr.sin_addr.s_addr));
    addr.sin_port = htons(_addr._port);
    slen = sizeof(addr);
    if (::bind(_fd, (struct sockaddr *)&addr, slen) < 0) {
        YLOG_ERR("Socket::bind socket error %d\n", _fd);
        return false;
    }
    _isInit = true;
    return _isInit;
}

bool    Socket::Listen()
{
    if (!_isInit || _mode != YSOCKET::SOCKET_SERVER || _streamMode != YSOCKET::SOCKET_STREAM_TCP) {
        YLOG_BUG("Socket::Listen error\n");
        return false;
    }

    if (_fd < 0) {
        YLOG_BUG("Socket::Listen fd %d error\n", _fd);
        return false;
    }

    if (::listen(this->_fd, this->_maxListenNum)) {
        YLOG_BUG("Socket::Listen error\n");
        return false;
    }

    return true;
}

bool    Socket::IsS()
{
    return this->_mode == YSOCKET::SOCKET_SERVER;
}
bool    Socket::IsC()
{
    return this->_mode == YSOCKET::SOCKET_CLIENT;
}

void    Socket::Destory()
{
    if (_fd > 0)
        close(_fd);
}


///////////////////////////////////////////////////////
socketTool::socketTool(){}
