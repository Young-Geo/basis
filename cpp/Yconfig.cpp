/*
 * Yconfig.cpp
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */
#include "Yconfig.h"

#define MAX_LINE_CHAR 256

config::config(){}

config::config(Ystring file)
{
    LoadFile(file);
}
config::~config(){}

void config::LoadFile(Ystring file)
{
    m_ini_map.clear();
    char line_content[MAX_LINE_CHAR];
    std::ifstream ifs(file.c_str());
    while(ifs)
    {
        ifs.getline(line_content,MAX_LINE_CHAR);
        std::string s = line_content;
        std::string::iterator it = s.begin();
        for(;it != s.end();)
        {
            if((*it) == ' ' || (*it) == '\t')
            {
                it = s.erase(it);
            }
            else if((*it) == ';' || (*it) == '#')
            {
                it = s.erase(it,s.end());
            }
            else
            {
                ++it;
            }
        }
        int pos = s.find('=');
        if(pos != std::string::npos)
        {
            m_ini_map[s.substr(0,pos)]=s.substr(pos+1,s.size());
        }
    }
    ifs.close();
}

Ystring	config::GetString(std::string key)
{
    if (m_ini_map.count(key))
    {
        return m_ini_map[key];
    }
    else
    {
        return "";
    }
}

Ystring	config::GetString(std::string key,std::string def)
{
    if (m_ini_map.count(key))
    {
        return m_ini_map[key];
    }
    else
    {
        return def;
    }
}

Yint config::GetInt(Ystring key)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return atoi(map_value.c_str());
    }
    else
    {
        return 0;
    }
}

Yint config::GetInt(std::string key, Yint def)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return atoi(map_value.c_str());
    }
    else
    {
        return def;
    }
}

Yshort config::GetShort(std::string key)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return (Yshort)atoi(map_value.c_str());
    }
    else
    {
        return 0;
    }
}

Yshort config::GetShort(std::string key, Yshort def)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return (Yshort)atoi(map_value.c_str());
    }
    else
    {
        return def;
    }
}

Ychar config::GetChar(std::string key)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return (Ychar)atoi(map_value.c_str());
    }
    else
    {
        return 0;
    }
}

Ychar config::GetChar(std::string key, Ychar def)
{
    std::string map_value=GetString(key);
    if (map_value.size())
    {
        return (Ychar)atoi(map_value.c_str());
    }
    else
    {
        return def;
    }
}

void config::UpdateUnSortedSet(const Ystring& key, std::unordered_set<int>& InOutSet)
{
    InOutSet.clear();
    auto it = m_ini_map.find(key);
    if (it == m_ini_map.end())
    {
        return;
    }

    Ystring strItem = it->second;
    if (strItem.empty())
        return;

    for (auto c : strItem)
    {
        if (!isdigit(c) && c != ',')
        {
            return;
        }
    }

    int nPos = 0;
    int nType = 0;
    Ystring strNum;
    while ((nPos = strItem.find(',')) != std::string::npos)
    {
        strNum = strItem.substr(0, nPos);
        if (!strNum.empty())
            nType = stoi(strNum);
        InOutSet.insert(nType);
        strItem = strItem.substr(nPos + 1);
    }
    if (!strItem.empty())
    {
        nType = stoi(strItem);
        InOutSet.insert(nType);
    }
}
