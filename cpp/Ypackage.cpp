#include "Ypackage.h"

Ypackage::Ypackage(){}

msg* Ypackage::ParseBuf(Ychain_t &chain, SocketClient sock)
{
    Yint _id = 0;
    msg * _msg = NULL;

    _msg = gMSGTOOL->CreateMsg(_id);

    _msg->m_sock = sock;

    if (!_msg->Read(chain))
        return nullptr;

    return _msg;
}
