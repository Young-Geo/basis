#include "Yevent.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>

static void bufferevent_r(struct bufferevent *bev, void *ctx)
{
    /*
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
        */
}

static void bufferevent_w(struct bufferevent *bev, void *ctx)
{
    /*
    entity_t *entity = NULL;
    Yint _fd = -1;

    Yassert(bev);
    Yassert(entity = (entity_t *)ctx);

    _fd = bufferevent_getfd(bev);

    if (entity->call.call_w)
        entity->call.call_w(SocketClient(bev), entity->call.arg);
        */

}

static void bufferevent_event(struct bufferevent *bev, short what, void *ctx)
{
    /*
    entity_t *entity = NULL;
    Yint _fd = -1;

    Yassert(bev);
    Yassert(entity = (entity_t *)ctx);

    _fd = bufferevent_getfd(bev);

    if (entity->call.call_e)
        entity->call.call_e(SocketClient(bev), what, entity->call.arg);
     */
}

static void loop_Func(void *arg)
{
    struct event_base *master_base = NULL;
    xassert((master_base = (struct event_base *)arg));
    event_base_dispatch(master_base);
}

static void EntityWorkFunc(int fd, short which, void *arg)
{
    int cfd = 0, count = 0;
    entity_t *entity = NULL;
    struct bufferevent *bev = NULL;

    xassert(entity = (entity_t *)arg);

    if (sizeof(cfd) != (count = read(fd, &cfd, sizeof(cfd)))) {
        //error
        return;
    }
    //cfd par
    bev = bufferevent_socket_new(entity->base, cfd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, bufferevent_r, bufferevent_w, bufferevent_event, entity);//r w error
    bufferevent_enable(bev, EV_READ|EV_WRITE);

    ++entity->conn_num;
}

entity_t* Entity_Init(event_rwe_t call)
{
    entity_t *entity = NULL;
    int    pfd[2] = { 0 };

    xassert((entity = (entity_t *)xmalloc(sizeof(entity_t))));
    xzero(entity, sizeof(entity_t));

    if (pipe(pfd)) {
        xerror("Can't create notify pipe");
        return NULL;
    }

    entity->notify_receive_fd = pfd[0];
    entity->notify_send_fd = pfd[1];
    xassert(entity->chain = xchain_alloc());
    entity->call = call;

    if (!(entity->base = event_base_new())) {
        xerror("Entity_Init base new error");
        return NULL;
    }

    event_set(&entity->notify_event, entity->notify_receive_fd, (EV_READ | EV_PERSIST), EntityWorkFunc, entity);
    event_base_set(entity->base, &entity->notify_event);
    if (event_add(&entity->notify_event, 0) < 0) {
        //
        xerror("Entity_Init event_add eror");
        return NULL;
    }

    entity->func = loop_Func;
    return entity;
}

static int server_fd_init(const char *ip, const short port)
{
    int fd = 0;
    struct sockaddr_in addr;
    socklen_t slen;


    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        xerror("Socket::Init socket error %d\n", fd);
        return 0;
    }

    xzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    //addr.sin_addr.s_addr = htonl(ip);
    inet_pton(AF_INET, ip, (void *)(&addr.sin_addr.s_addr));
    addr.sin_port = htons(port);
    slen = sizeof(addr);


    if (bind(fd, (struct sockaddr *)&addr, slen) < 0) {
        xerror("Socket::bind socket error %d\n", fd);
        return 0;
    }

    if (listen(fd, 128)) {
        xerror("Socket::Listen error\n");
        return 0;
    }

    return fd;
}


static void     MasterWorkFunc(int fd, short which, void *arg)
{
    //an 应该接受链接 分配到不同的event

    int cfd = 0;
    struct sockaddr_in caddr;
    socklen_t slen;
    entity_t *entity = NULL;
    master_t *master = NULL;
    xlist   *n = NULL;

    xassert(master = (master_t *)arg);

    slen = sizeof(caddr);
    cfd = accept(master->conn_receive_fd, (struct sockaddr *)&caddr, &slen);
    if (cfd < 0) {
        xerror("accept socket error %d\n", cfd);
        return;
    }

    master->last_event = (master->last_event + 1) % master->entitysSize;
    if (!(n = xlist_index(master->entitys, master->last_event))) {
        xerror("get entity ");
        return;
    }
    entity = (entity_t *)n->value;

    if (sizeof(cfd) != write(entity->notify_send_fd, &cfd, sizeof(cfd))) {
        xerror("sendto %d socket fd error %d\n", master->last_event, cfd);
        close(cfd);
        return;
    }

}

master_t* Master_Init(const char *ip, const short port)
{
    master_t *master = NULL;

    xassert((master = (master_t *)xmalloc(sizeof(master_t))));
    xzero(master, sizeof(master_t));

    master->last_event = -1;
    xassert(master->entitys = xlist_init());
    master->entitysSize = 0;
    master->threadPool = NULL;

    master->master_base = event_base_new();
    if (!master->master_base) {
        xerror("Can't allocate event base\n");
        exit(1);
    }

    master->conn_receive_fd = server_fd_init(ip, port);
    event_set(&master->master_event, master->conn_receive_fd,
              EV_READ | EV_PERSIST, MasterWorkFunc, master);
    event_base_set(master->master_base, &master->master_event);

    if (event_add(&master->master_event, 0) == -1) {
        xerror("Can'tevent_add\n");
        exit(1);
    }

    master->func = loop_Func;

    return master;
}


Yevent_t*    Yevent_Create(const char *ip, const unsigned short port)
{
    master_t *master = NULL;

    xassert(master = Master_Init(ip, port));

    return ((Yevent_t*)master);
}

void            Yevent_Destory(Yevent_t *yevent)
{
    master_t *master = NULL;

    xassert(master = (master_t *)yevent);


    if (master->threadPool) {
        xassert(threadpool_destroy(master->threadPool, threadpool_graceful) == 0);//threadpool_graceful
    }

    if (master->entitys) {
        xlist_clean(&master->entitys);
    }


    atexit(enet_deinitialize);

    xfree(master);
    return ;
}
int             Yevent_Start(Yevent_t *yevent, int workNum)
{
    master_t *master = NULL;
    entity_t *entity = NULL;

    int i = 0;
    event_rwe_t rwe;

    xassert(master = (master_t *)yevent);
    if (workNum <= 0) return -1;

    if (!master->threadPool) {
        xassert(master->threadPool = threadpool_create((workNum + 1), 256, 0));
    }

    if (master->func)
        assert(threadpool_add(master->threadPool, master->func, master->master_base, 0) == 0);

    for (i = 0; i < workNum; ++i)
    {
        xassert(entity = Entity_Init(rwe));
        if (entity->func)
            assert(threadpool_add(master->threadPool, entity->func, entity->base, 0) == 0);
        xlist_push(master->entitys, NULL, XLIST_STRING, (char *)entity);
        ++master->entitysSize;
    }


   return 0;
}
