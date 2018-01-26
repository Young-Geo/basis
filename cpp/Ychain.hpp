/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#ifndef __YCHAIN_H__
#define __YCHAIN_H__

#include "Ybasis.h"
#include "Ylog.h"

#define BUFF_SIZE 1024


class Buff
{
//private:
    //Buff(const Buff&);

public:

    Buff():m_pData(NULL),m_offset(0),m_curPos(0),m_size(BUFF_SIZE)
    {
        m_pData = new char[m_size];
    }

    ~Buff()
    {
        if(m_pData != NULL)
        {
            delete[] m_pData;
        }
    }
    Buff(const Buff&buf)
    {
        *this = buf;
    }
    /*
    *	brief 重新初始化
    */
    void* Alloc(unsigned long dwSize)
    {
        if(m_pData != NULL)
        {
            delete[] m_pData;
        }
        m_pData = new char[dwSize];
        m_curPos = m_offset = 0;
        m_size = dwSize;
        return m_pData;
    }

    void*  AddSizeAndOldWrite(unsigned long dwSize)
    {
        int _size = -1;
        char *_data = NULL;

        _size = BUFF_SIZE*((m_curPos + dwSize)/BUFF_SIZE + 1);

        if(this->DataSize() > 0)
        {
            Yassert(_data = new char[this->DataSize()]);
            this->Read(_data, this->DataSize());
            this->Alloc(_size);
            this->Write(_data, strlen(_data));
        }
        return m_pData;
    }

    /*
    *	brief 获取LBuff的起始指针
    */
    char* Data()const
    {
        return m_pData;
    }

    int GetSurPlusSize()
    {
        return m_size - m_curPos;
    }

    /*
    *	brief 获取LBuff的容量
    */
    int Capacity()const
    {
        return m_size;
    }

    /*
    *	brief 获取LBuff的当前长度
    */
    int   Size()const
    {
        return m_curPos;
    }

    int DataSize()const
    {
        return m_curPos - m_offset;
    }

    /*
    *	brief 获取LBuff的当前读取位置的偏移
    */
    int   GetOffset()const
    {
        return m_offset;
    }

    int&   GetOffPos()
    {
        return m_offset;
    }

    /*
    *	brief 设置LBuff的当前读取位置的偏移
    */
    void  SetOffset(int offset)
    {
        m_offset = offset;
    }

    /*
    *	brief 设置LBuff的当前读取位置的偏移
    */
    void  Cut(int len)
    {
        if(len >= m_curPos)
        {
            m_curPos = 0;
        }
        else
        {
            memcpy(m_pData,m_pData+len,m_curPos-len);
            m_curPos -= len;
        }
    }

    /*
    *	brief 向LBuff里面写入数据
    */
    void  Write(void const* data,int len)
    {
        if(m_curPos + len > m_size)
        {
            m_size = BUFF_SIZE*((m_curPos + len)/BUFF_SIZE + 1);

            char* pNew = new char[m_size];
            memcpy(pNew,m_pData,m_curPos);
            delete[] m_pData;
            m_pData = pNew;
        }

        memcpy(m_pData+m_curPos,data,len);
        m_curPos += len;
    }

    /*
    *	brief 从LBuff里读出数据
    */
    void  Read(void* data,int len)
    {
        if(m_offset + len > m_curPos)
            return;

        memcpy(data,m_pData+m_offset,len);
        m_offset += len;
    }

    template<class T>
    void Write(T const & value)
    {
        Write(&value,sizeof(value));
    }

    template<class T>
    Buff& operator << (T const& value)
    {
        Write(value);
        return *this;
    }

    template<class T>
    void Read(T& value)
    {
        Read(&value,sizeof(value));
    }

    template<class T>
    Buff& operator >> (T& value)
    {
        Read(value);
        return *this;
    }

    //[bing] 拿出来作为public使用
    Buff& operator = (const Buff& theBuff)
    {
        this->Write( theBuff.Data(), theBuff.Capacity() );
        this->m_offset	=	theBuff.GetOffset();
        this->m_size	=	theBuff.Capacity();
        this->m_curPos	=	theBuff.Size();

        return *this;
    }

    char* operator &()
    {
        return this->Data();
    }

private:
    char* m_pData;
    int m_offset;
    int m_curPos;
    int m_size;
};

//char*
template<>
inline void Buff::Write(char* const & value)
{
    int size = 0;
    if(value == NULL || (size = strlen(value)) == 0)
    {
        Write(&size,sizeof(size));
    }
    else
    {
        Write(&size,sizeof(size));
        Write(value,size);
    }
}

//std::string
template<>
inline void Buff::Write(std::string const& value)
{
    int size = value.size();
    Write(&size,sizeof(size));
    if(size != 0)
    {
        Write(value.c_str(),size);
    }
}

//LBuff
template<>
inline void Buff::Write(Buff const& value)
{
    Write(value.Size());
    Write(value.Data(),value.Size());
}

template<>
inline void Buff::Read(std::string& value)
{
    int len = 0;
    Read(&len,sizeof(len));
    if(len > 0 && len < m_curPos)
    {
        char* tmp = new char[len+1];
        Read(tmp,len);
        tmp[len] = 0;
        value = tmp;
        delete[] tmp;
    }
    else
    {
        value.clear();
    }
}

template<>
inline void Buff::Read(Buff& value)
{
    int size = 0;
    Read(size);
    value.Write(Data()+GetOffset(),size);
    SetOffset(GetOffset()+size);
}

typedef std::shared_ptr<Buff> BuffPtr;
typedef Buff    Ychain_t;

#endif // __YCHAIN_H__
