/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#ifndef __YMSGTOOL_H__
#define __YMSGTOOL_H__

#include "Yalone.hpp"
#include "Ybasis.h"
#include "Ychain.hpp"
#include "YSocketClient.h"
#include <msgpack.hpp>

template <typename T>
void    packT(T &t, Ychain_t &chain)
{
    msgpack::sbuffer buffer_;
    msgpack::packer<msgpack::sbuffer>  pack_(&buffer_);
    pack_.pack(t);//序列化对象//T need MSGPACK_DEFINE
    chain.Write((void *)buffer_.data(), buffer_.size());
}

template <typename T>
void    unpackT(T &t, Ychain_t &chain)
{
    msgpack::unpacked _msg;
    msgpack::unpack(&_msg, (char*)&chain, (size_t)chain.Size());
    _msg.get().convert(&t);//T need MSGPACK_DEFINE
}

struct msg
{
    //MSGPACK_DEFINE(m_id);
    msg(Yint _id):m_id(_id){}

    virtual msg* Clone() = 0;

    virtual bool    Read(Ychain_t &chain) = 0;
    /*{
        unpackT<msg>(*this, chain);
        return true;
    }*/

    virtual bool    Write(Ychain_t &chain) = 0;
    /*{
        packT<msg>(*this, chain);
        return true;
    }*/

    Ychain_t    GetSendBuf()
    {
        Ychain_t chain;
        chain << m_id;
        Write(chain);
        return chain;
    }

    Yint m_id;
    SocketClient m_sock;
};

/*
struct msgSS : public msg
{
    msgSS(Yint _id) : msg(_id)
    {
        m_isSC = false;
    }

    virtual msg* Clone()
    {
        return new msgSS(m_id);
    }
};

struct msgSC : public msg
{
    msgSC(Yint _id) : msg(_id)
    {
        m_isSC = true;
    }

    virtual msg* Clone()
    {
        return new msgSC(m_id);
    }
};
*/

class msgTool : public alone<msgTool>
{
public:
    msgTool();
public:
    bool    Register(msg *);
    msg*    CreateMsg(Yint id);
private:
    std::map<Yint, msg*> _msgMap;
};

#define gMSGTOOL msgTool::GetEntity()


enum msgID
{
    MSG_C_2_S_LOGIN = 1,
};


struct msgC2SLogin : public msg
{
    Yint m_userId;
    MSGPACK_DEFINE(m_userId);
    msgC2SLogin() : msg(MSG_C_2_S_LOGIN){}

    virtual bool    Read(Ychain_t &chain)
    {
        unpackT<msgC2SLogin>(*this, chain);
        return true;
    }

    virtual bool    Write(Ychain_t &chain)
    {
        packT<msgC2SLogin>(*this, chain);
        return true;
    }

    virtual msg* Clone(){return new msgC2SLogin();}
};

#endif // __YMSGTOOL_H__
