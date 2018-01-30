/*
 * Yalone.hpp
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */

#ifndef __YALONE_H__
#define __YALONE_H__

#include "Ybasis.h"

template <typename T>
class alone
{
public:
    static T* GetEntity()
    {
        static T t;
        return &t;
    }
};

#endif // __YALONE_H__
