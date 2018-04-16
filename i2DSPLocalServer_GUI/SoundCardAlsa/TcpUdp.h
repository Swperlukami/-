
#ifndef _TCPSOCKET_H
#define _TCPSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>						
#include<signal.h>
#include <sys/time.h>
#include <string.h>


#define BUF_SIZE  2048

extern int fd_LocalTcp;                                          
extern int fd_LocalUdp;                              
extern pthread_mutex_t tcpsendmutex;
extern char UdpBuf[BUF_SIZE];
extern char TcpBuf[BUF_SIZE];
extern struct sockaddr_in  udp_from_addr; 
extern struct sockaddr_in  udp_ser_addr; 

#define TCP_PORT              6000
#define TCP_SER_IP            "192.168.1.51"

#define UDP_PORT              9000							
#define UDP_SER_IP            "192.168.1.51"

void TcpSocketInitial(void);
void TcpSocketExit(void);
void UdpSocketInitial(void);
void UdpSocketExit(void);

int TcpSendDat(char *buf,int size);
void UdpSendDat(char *buf,int size);
void Udp_Initial(void);
void Tcp_Initial(void);

#endif
