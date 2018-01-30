/*
 * Ysocket.h
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */
#ifndef __YSOCKET_H__
#define __YSOCKET_H__

#include "Ybasis.h"
#include "Ylog.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>

namespace YSOCKET
{
    enum    SOCKET_MODEL
    {
      SOCKET_CLIENT = 1,
      SOCKET_SERVER = 2,
    };

    enum    SOCKET_STREAM_MODEL
    {
        SOCKET_STREAM_TCP = SOCK_STREAM,
        SOCKET_STREAM_UDP = SOCK_DGRAM,
    };
    struct sock_addr_t
    {
        Ystring _ip;
        Yint    _port;
    };
}
class Socket
{
#define MAXLISTENNUM 128
public:
    Socket();
    Socket(YSOCKET::SOCKET_MODEL mode, YSOCKET::SOCKET_STREAM_MODEL streamMode, YSOCKET::sock_addr_t addr);
public:
    void    SetSocketModel(YSOCKET::SOCKET_MODEL mode);
    void    SetSocketStreamModel(YSOCKET::SOCKET_STREAM_MODEL streamMode);
    void    SetAddr(YSOCKET::sock_addr_t addr);
    void    SetMaxListenNum(Yint num);
    Yint    GetFd();
    //
    bool    Init();
    bool    Listen();
    bool    IsS();
    bool    IsC();
    void    Destory();
private:
    YSOCKET::SOCKET_MODEL _mode;
    YSOCKET::SOCKET_STREAM_MODEL _streamMode;
    YSOCKET::sock_addr_t _addr;
    Yint        _fd;
    Yint        _maxListenNum;
    bool        _isInit;
};

class socketTool
{
public:
    socketTool();
    static Socket* GetSocket(YSOCKET::SOCKET_MODEL mode, YSOCKET::SOCKET_STREAM_MODEL streamMode, YSOCKET::sock_addr_t addr)
    {
        Socket *_socket = NULL;
        Yassert(_socket = new Socket(mode, streamMode, addr));

        if (!_socket->Init()) {
            Ydelete _socket;
            return NULL;
        }
        if (_socket->IsS() && !_socket->Listen()) {
            Ydelete _socket;
            return NULL;
        }
        return _socket;
    }
private:

};

#endif // __YSOCKET_H__
