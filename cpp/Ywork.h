/*
 * Ywork.h
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */

#ifndef __YWORK_H__
#define __YWORK_H__

#include "Ybasis.h"


class work
{
public:
    work();
    ~work();
    void            Push(void *msg);
    void *          Pop();
    void            Swap();
public:
    void		Start();
    void        Stop();
    bool        IsStop();
    void        Join();
    void        Detach();
    virtual bool		Init() = 0;
    virtual void		Loop() = 0;
    virtual void        Exit() = 0;
private:
    void		Run();
private:
    std::shared_ptr<std::thread> thread_ptr;
    std::mutex    mutex;
    bool          isStop;
private:
    std::list<void *> *recMsgs;
    std::list<void *> *doingMsgs;
    std::list<void *>  queueMsgs[2];
};

#endif /* __YWORK_H__ */
