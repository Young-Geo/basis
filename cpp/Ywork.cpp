/*
 * Ywork.cpp
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */

#include "Ywork.h"

work::work()
{
    this->isStop    =   false;
    this->recMsgs   =   &this->queueMsgs[0];
    this->doingMsgs =   &this->queueMsgs[1];
}

work::~work()
{
    if (this->recMsgs) {
        this->recMsgs->clear();
    }

    if (this->doingMsgs) {
        this->doingMsgs->clear();
    }
}

void            work::Push(void *msg)
{
    std::lock_guard<std::mutex> lock(mutex);
    recMsgs->push_back(msg);
}

void *          work::Pop()
{
    void *msg = nullptr;
    if (this->doingMsgs->empty()) {
        Swap();
    }
    if (this->doingMsgs->empty()) {
        return nullptr;
    }
    msg = this->doingMsgs->front();
    this->doingMsgs->pop_front();
    return msg;
}

void            work::Swap()
{
    std::lock_guard<std::mutex> lock(mutex);
    std::list<void *> *temp = nullptr;
    temp    =   this->recMsgs;
    this->recMsgs = this->doingMsgs;
    this->doingMsgs = temp;
}

void        work::Start()
{
    if (!thread_ptr) {
        thread_ptr = std::make_shared<std::thread>(std::thread(std::bind(&work::Run, this)));
    }
}
void		work::Run()
{
    this->Loop();
}

void        work::Stop()
{
    this->isStop = true;
}
bool        work::IsStop()
{
    return this->isStop;
}

void        work::Join()
{
    thread_ptr->join();
}

void        work::Detach()
{
    thread_ptr->detach();
}
