/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#include "Ymsgtool.h"
#include "Ylog.h"

msgTool::msgTool(){}

bool    msgTool::Register(msg *_msg)
{
    if (_msgMap.find(_msg->m_id) == _msgMap.end()) {
        this->_msgMap[_msg->m_id] = _msg;
    }
    return true;
}

msg*    msgTool::CreateMsg(Yint id)
{
    if (_msgMap.find(id) == _msgMap.end()) {
        YLOG_BUG("%d not Register", id);
        return NULL;
    }

    return _msgMap[id]->Clone();
}
