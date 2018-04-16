#ifndef SOCKET_H
#define SOCKET_H
#include<netinet/in.h>

#define UDPMULTICASTSERV_PORT    7000								/*定义UDP组播服务器端口号。*/
#define UDPMULTICASTCLIENT_PORT  7500                               /*定义UDP组播目的端口号*/
#define MAX_UDP_BUF   1024


extern char UDPMULTICASTIP[20];
#endif // SOCKET_H

