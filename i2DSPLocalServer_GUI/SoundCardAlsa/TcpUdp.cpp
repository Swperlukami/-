
#include "TcpUdp.h"
#include "main.h"
			
struct sockaddr_in  tcp_ser_addr;                                 
int fd_LocalTcp;                                                       
int fd_LocalUdp;                                              
pthread_mutex_t tcpsendmutex;
struct sockaddr_in  udp_ser_addr;  
struct sockaddr_in  udp_from_addr; 
struct sockaddr_in  udp_local_addr;

extern char *aimed_ip;                                 

char UdpBuf[BUF_SIZE]={0};
char TcpBuf[BUF_SIZE]={0};

void TcpSocketInitial(void)
{	
	if(pthread_mutex_init(&tcpsendmutex,NULL) != 0)
    {
		perror("pthread_mutex_init");	
    }
    
	Tcp_Initial();
}

void TcpSocketExit(void)
{
	pthread_mutex_destroy(&tcpsendmutex);
	close(fd_LocalTcp);	
}

void UdpSocketInitial(void)
{
	Udp_Initial();
}

void UdpSocketExit(void)
{
	 close(fd_LocalUdp);
}

int TcpSendDat(char *buf,int size)
{
    int nSendByte;
		
	nSendByte = send(fd_LocalTcp,buf,size,0);
	if(nSendByte != size)
	{
		perror("send cmd error\n");	
	}
		
	return nSendByte;
}

void Udp_Initial(void)
{
	int contain=0;
	int flags=0;
	  
	memset(&udp_ser_addr, 0, sizeof(udp_ser_addr));            
	printf("aimed ip is %s\n",aimed_ip);
  
	udp_ser_addr.sin_family = AF_INET;
    udp_ser_addr.sin_addr.s_addr = inet_addr(aimed_ip);          
    udp_ser_addr.sin_port = htons(UDP_PORT);

	udp_local_addr.sin_family = AF_INET;
	udp_local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	udp_local_addr.sin_port = htons(UDP_PORT);
		   
    if((fd_LocalUdp = socket(AF_INET, SOCK_DGRAM, 0))==-1)
    {
         perror("socket");
         exit(1);
    }
    else
    {
     	   printf("create udp socket\n");
    }

	bind(fd_LocalUdp,(struct sockaddr*)&udp_local_addr,sizeof(udp_local_addr));
         
    if(flags=fcntl(fd_LocalUdp,F_GETFL,0)<0)
    {
         perror("fcntl");
    }
    flags |=O_NONBLOCK;
    if(fcntl(fd_LocalUdp,F_SETFL,flags)<0)
    {
         perror("fcntl");
    }
}

void Tcp_Initial(void)
{
	int contain;
	int flags;
	 
    if((fd_LocalTcp=socket(AF_INET,SOCK_STREAM,0))<0)     
    {
        perror("socket");
        exit(1);
    }
    else
    {
    	printf("create tcp socket.\n");
    }
   
    bzero(&tcp_ser_addr,sizeof(tcp_ser_addr));
    tcp_ser_addr.sin_family =AF_INET;
    tcp_ser_addr.sin_port=htons(TCP_PORT);
    tcp_ser_addr.sin_addr.s_addr=inet_addr(TCP_SER_IP);
 
    setsockopt(fd_LocalTcp,SOL_SOCKET, SO_REUSEADDR, &contain, sizeof(int));

    if(connect(fd_LocalTcp,(struct sockaddr *)&tcp_ser_addr,sizeof(tcp_ser_addr))<0)				
    {
        perror("connect");
        exit(1);
    }
    else
    {	
    	  printf("tcp connect successful!\n");
    }
}

void UdpSendDat(char *buf,int size)
{
	int addr_len=sizeof(udp_ser_addr);
		
    sendto(fd_LocalUdp,buf,size,0,(struct sockaddr *)&udp_ser_addr,addr_len);
}
