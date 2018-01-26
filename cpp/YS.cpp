/*
 * YS.cpp
 *
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 */
#include "YS.h"

static void bufferevent_r(struct bufferevent *bev, void *ctx)
{
    entity_t *entity = NULL;
    Ychain_t chain;
    struct evbuffer *input = NULL;
    struct evbuffer *ouput = NULL;
    Yint _fd = -1, _dataSize = 0, curSize = 0;
    //unsigned char *buf = NULL;

    Yassert(bev);
    Yassert(entity = (entity_t *)ctx);
    //

    Yassert(input = bufferevent_get_input(bev));
    Yassert(ouput = bufferevent_get_output(bev));
    _fd = bufferevent_getfd(bev);

    _dataSize = evbuffer_get_length(input);
    //Yassert(buf = (unsigned char *)Ycalloc(1, (_dataSize + 1)));

    if (entity->chain->GetSurPlusSize() < _dataSize) {
        //need rwrite
        entity->chain->AddSizeAndOldWrite(_dataSize);//add size
    }

    evbuffer_remove(input, (void *)entity->chain->Data()+entity->chain->Size(), _dataSize);//need par


    *entity->chain >> curSize;
    chain.Write(entity->chain->Data()+entity->chain->GetOffset(), curSize);
    entity->chain->Cut(curSize + sizeof(curSize));//buf qian yi size

    if (entity->call.call_r)
        entity->call.call_r(SocketClient(bev), chain, entity->call.arg);
}

static void bufferevent_w(struct bufferevent *bev, void *ctx)
{
    entity_t *entity = NULL;
    Yint _fd = -1;

    Yassert(bev);
    Yassert(entity = (entity_t *)ctx);

    _fd = bufferevent_getfd(bev);

    if (entity->call.call_w)
        entity->call.call_w(SocketClient(bev), entity->call.arg);

}

static void bufferevent_event(struct bufferevent *bev, short what, void *ctx)
{
    entity_t *entity = NULL;
    Yint _fd = -1;

    Yassert(bev);
    Yassert(entity = (entity_t *)ctx);

    _fd = bufferevent_getfd(bev);

    if (entity->call.call_e)
        entity->call.call_e(SocketClient(bev), what, entity->call.arg);
}


entity_t* Entity_Init(event_rwe_t call)
{
    entity_t *entity = NULL;
    Yint    pfd[2] = { 0 };

    Yassert((entity = (entity_t *)Ymalloc(sizeof(entity_t))));
    Yzero(entity, sizeof(entity_t));

    if (pipe(pfd)) {
        YLOG_ERR("Can't create notify pipe");
        return NULL;
    }

    entity->notify_receive_fd = pfd[0];
    entity->notify_send_fd = pfd[1];
    Yassert(entity->chain = new Ychain_t());
    entity->call = call;

    if (!(entity->base = event_base_new())) {
        YLOG_ERR("Entity_Init base new error");
        return NULL;
    }

    event_callback_fn EntityWorkFunc = [=] (Yint fd, Yshort which, void *arg)->void
    {
        //master send fd .. this rec
        Yint cfd = 0, count = 0;
        entity_t *entity = NULL;
        struct bufferevent *bev = NULL;

        Yassert(arg);
        entity = (entity_t *)arg;

        if (sizeof(cfd) != (count = read(fd, &cfd, sizeof(cfd)))) {
            //error
            return;
        }
        //cfd par
        bev = bufferevent_socket_new(entity->base, cfd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, bufferevent_r, bufferevent_w, bufferevent_event, entity);//r w error
        bufferevent_enable(bev, EV_READ|EV_WRITE);

        ++entity->conn_num;
    };

    event_set(&entity->notify_event, entity->notify_receive_fd, (EV_READ | EV_PERSIST), EntityWorkFunc, entity);
    event_base_set(entity->base, &entity->notify_event);
    if (event_add(&entity->notify_event, 0) < 0) {
        //
        YLOG_ERR("Entity_Init event_add eror");
        return NULL;
    }
    std::function<void(entity_t *)> entity_Func = [=](entity_t *entity)->void
    {
        //event_base_loop(entity->base, 0);//thread--
        event_base_dispatch(entity->base);
    };

    //entity->thread_handle = new std::thread(std::bind(entity_Func, entity));
    if (!entity->thread_ptr) {
        entity->thread_ptr = std::make_shared<std::thread>(std::thread(std::bind(entity_Func, entity)));
    }

    entity->thread_ptr->detach();

    return entity;
}

master_t* Master_Init(YSOCKET::sock_addr_t addr)
{
    master_t *master = NULL;

    Yassert((master = (master_t *)Ymalloc(sizeof(master_t))));
    Yzero(master, sizeof(master_t));

    master->last_event = -1;


    Socket* _socket = socketTool::GetSocket(YSOCKET::SOCKET_SERVER, YSOCKET::SOCKET_STREAM_TCP, addr);

    master->master_base = event_base_new();
    if (!master->master_base) {
        fprintf(stderr, "Can't allocate event base\n");
        exit(1);
    }

    //std::function<void(Yint, Yshort, void *)> MasterWorkFunc = [=] (Yint fd, Yshort which, void *arg)->void
    event_callback_fn MasterWorkFunc = [=] (Yint fd, Yshort which, void *arg)->void
    {
        //an 应该接受链接 分配到不同的event

        int cfd = 0;
        struct sockaddr_in caddr;
        socklen_t slen;
        // accept
        Yassert(arg);
        master_t *master = (master_t *)arg;

        slen = sizeof(caddr);
        cfd = accept(master->conn_receive_fd, (struct sockaddr *)&caddr, &slen);
        if (cfd < 0) {
            YLOG_ERR("accept socket error %d\n", cfd);
            return;
        }

        master->last_event = (master->last_event + 1) % master->_entitys.size();

        if (sizeof(cfd) != write(master->_entitys[master->last_event]->notify_send_fd, &cfd, sizeof(cfd))) {
            YLOG_ERR("sendto %d socket fd error %d\n", master->last_event, cfd);
            close(cfd);
            return;
        }

    };

    master->conn_receive_fd = _socket->GetFd();
    event_set(&master->master_event, master->conn_receive_fd,
              EV_READ | EV_PERSIST, MasterWorkFunc, master);
    event_base_set(master->master_base, &master->master_event);

    if (event_add(&master->master_event, 0) == -1) {
        YLOG_ERR("Can'tevent_add\n");
        exit(1);
    }

    std::function<void(master_t *)> master_Func = [=](master_t *master)->void
    {
        //event_base_loop(master->master_base, 0);
        event_base_dispatch(master->master_base);
    };

    //
    if (!master->thread_ptr) {
        master->thread_ptr = std::make_shared<std::thread>(std::thread(std::bind(master_Func, master)));
    }

    master->thread_ptr->detach();

    return master;
}




//////////////////////////////////////////////////////

void    YS::Start(YSOCKET::sock_addr_t addr)
{
    Yassert(_master = ::Master_Init(addr));
}

bool    YS::AddEvent(event_rwe_t call)
{
    entity_t *entity = NULL;
    if(!_master) {
        YLOG_ERR("YS::AddEvent not init");
        return false;
    }
    Yassert(entity = ::Entity_Init(call));
    _master->_entitys.push_back(entity);
    return true;
}

bool    YS::AddWorkEvent(event_function_r call_r, event_function_w call_w, event_function_e call_e, void *arg)
{
    event_rwe_t rwe;

    rwe.call_r = call_r;
    rwe.call_w = call_w;
    rwe.call_e = call_e;
    rwe.arg = arg;
    return this->AddEvent(rwe);
}
