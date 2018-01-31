#include "Yenet.h"

Yenet_t*    Yenet_Create(ENET model, const char *ip, const unsigned short port)
{
    Yenet_t *enet = NULL;

    //init
    xassert(0 == enet_initialize());
    xassert(ip);
    xassert(enet = (Yenet_t *)xmalloc(sizeof(Yenet_t)));

    enet->isInit    =   1;
    enet->model     =   model;
    enet->threadPool = NULL;
    enet->handle = NULL;

    if (model == ENET_SERVER) {
        ENetAddress addr;
        enet_address_set_host(&addr, ip);
        addr.port = port;
        enet->model = ENET_SERVER;
        enet->handle = enet_host_create(&addr, 32, 2, 0, 0);
    } else if (model == ENET_CLIENT) {
        enet->model = ENET_CLIENT;
        enet->handle = enet_host_create(NULL, 1, 2, 57600, 14400);
    }

    if (!enet->handle) {
        atexit(enet_deinitialize);
        xfree(enet);
        return NULL;
    }

    return enet;
}

void        Yenet_Destory(Yenet_t *enet, int flag)
{
    xassert(enet);


    if (enet->threadPool) {
        xassert(threadpool_destroy(enet->threadPool, threadpool_graceful) == 0);//threadpool_graceful
    }

    if (enet->handle) {
        enet_host_destroy(enet->handle);
    }

    atexit(enet_deinitialize);

    xfree(enet);
}


static void master_work(void *arg)
{
    ENetEvent event;
    Yenet_t *enet = NULL;

    xassert(arg);

    enet = (Yenet_t *)arg;

    while (true)
    {
        if (enet_host_service(enet->handle, &event, 1000) <= 0)
            continue;

        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            {
                printf ("A new client connected from %x:%u.\n",
                        event.peer -> address.host,
                        event.peer -> address.port);
                /* Store any relevant client information here. */
                event.peer -> data = (void *)"Client information";

                //event.peer
            }
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            {
                printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                        event.packet -> dataLength,
                        event.packet -> data,
                        event.peer -> data,
                        event.channelID);
                /* Clean up the packet now that we're done using it. */
                //push work chuli
                //enet_packet_destroy(event.packet);

            }

            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            {
                printf ("%s disconnected.\n", event.peer -> data);
                /* Reset the peer's client information. */
                event.peer -> data = NULL;
                //
            }
        }
    }
}


static void work(void *arg)
{
    Yenet_t *enet = NULL;

    xassert(arg);

    enet = (Yenet_t *)arg;

    //chu li data
}

int         Yenet_Start(Yenet_t *enet, int workNum)
{
    int i = 0;
    xassert(enet);

    if (workNum <= 0) return -1;

    if (!enet->threadPool) {
        xassert(enet->threadPool = threadpool_create((workNum + 1), 256, 0));
    }

    assert(threadpool_add(enet->threadPool, master_work, enet, 0) == 0);

    for (i = 0; i < workNum; ++i)
    {
        assert(threadpool_add(enet->threadPool, work, enet, 0) == 0);
    }

    return 0;
}
