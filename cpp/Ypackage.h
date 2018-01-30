/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#ifndef __YPACKAGE_H__
#define __YPACKAGE_H__

#include "Ybasis.h"
#include "Ylog.h"
#include "Yalone.hpp"
#include "Ychain.hpp"
#include "Ymsgtool.h"
#include "YSocketClient.h"
#include <msgpack.hpp>


class Ypackage : public alone<Ypackage>
{
public:
    Ypackage();
public:
    msg* ParseBuf(Ychain_t &chain, SocketClient sock);
};

#define gPACKAGE Ypackage::GetEntity()

#endif // __YPACKAGE_H__
