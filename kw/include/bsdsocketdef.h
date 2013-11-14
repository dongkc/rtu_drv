#ifndef __BSDSOCKETDEF_H__
#define __BSDSOCKETDEF_H__



#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <fcntl.h>

#include <net/if_arp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/ethernet.h>	/* struct ether_addr */
#include <libio.h>
#include <unistd.h>

//#include <sockaddr.h>
//typedef int socklen_t;

#define bsdSocket       ::socket
#define bsdBind         ::bind
#define bsdSelect       ::select
#define bsdListen       ::listen
#define bsdClose        ::close
#define bsdAccept       ::accept
#define bsdConnect      ::connect
#define bsdSend         ::send
#define bsdRecv         ::recv
#define bsdSendto       ::sendto
#define bsdRecvfrom     ::recvfrom
#define bsdSetsockopt   ::setsockopt
#define bsdGetsockname  ::getsockname

#endif //#define __BSDSOCKETDEF_H__

