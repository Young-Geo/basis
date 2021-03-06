#ifndef __H_XUDP_H__
#define __H_XUDP_H__
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define UDP_PKT_MAX_SIZE 1024
//////////////////////////////////////////////////////////////////////////
#include "xsocket.h"
//////////////////////////////////////////////////////////////////////////
typedef struct _udp_sender_t
{
  int sk ;
  struct sockaddr_in addr;
  char ip [ 32 ] ;
  int port ;
} udp_sender_t ;
//////////////////////////////////////////////////////////////////////////
int xudp_init_listen_socket ( int port ) ;
int xudp_init_udp_sender ( char * ip , int port , udp_sender_t * sender , int broadcast ) ;
int xudp_recv ( int fd , char buffer[UDP_PKT_MAX_SIZE] , char ip[MINBUFFER_SIZE] , int * port ) ;
int xudp_send ( udp_sender_t * sender, char buffer[UDP_PKT_MAX_SIZE] , int size ) ;
//////////////////////////////////////////////////////////////////////////
#endif
