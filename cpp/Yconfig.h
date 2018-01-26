/*
 * Yalone.h
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */
#ifndef __YCONFIG_H__
#define __YCONFIG_H__

#include "Ybasis.h"
#include <unordered_set>

class config
{
public:
    config();
    config(Ystring file);
    ~config();

    void	LoadFile(Ystring file);

    Ystring	GetString(Ystring key);
    Ystring GetString(Ystring key,Ystring def);

    Yint	GetInt(Ystring key);
    Yint	GetInt(Ystring key, Yint def);

    Yshort	GetShort(Ystring key);
    Yshort  GetShort(Ystring key, Yshort def);

    Ychar	GetChar(Ystring key);
    Ychar	GetChar(Ystring key, Ychar def);

    void	UpdateUnSortedSet(const Ystring& key, std::unordered_set<Yint>& InOutSet);

private:
    std::map<Ystring,Ystring> m_ini_map;
};

#endif // __YCONFIG_H__
