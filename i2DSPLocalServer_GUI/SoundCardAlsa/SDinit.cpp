
#include "TcpUdp.h"
#include "main.h"
#include "Queue.h"
#include "Socket.h"
#include "UdaPcm.h"

pthread_t Id_CaptureThread;         
pthread_t Id_PlayBackThread;       
pthread_t Id_UdpRecvThread;       


pthread_mutex_t talk_mutex;    
Queue *BcOrTalk;               
char bReadyPlay = 0;         
extern int fd_LocalUdp;
char *aimed_ip;

extern struct sockaddr_in  udp_ser_addr;

void SystemInitial(void)
{
    int err;
    int i=0;
    int addr_len=sizeof(udp_ser_addr);

    //Udp_Initial();

    PlayBack_Initial();       

    Capture_Initial();

	pthread_mutex_init(&talk_mutex,NULL);
    BcOrTalk = InitQueue();

    /*err = pthread_create(&Id_UdpRecvThread,NULL,UdpRecv_Thread,NULL);
    if (err)
    {
        printf("UdpRecv thread is not created!\n");
    }*/
	
    	
	err = pthread_create(&Id_CaptureThread,NULL,Capture_thread,NULL);
	if (err)
	{
		printf("Capture_thread thread is not created!\n");
	}

	err = pthread_create(&Id_PlayBackThread,NULL,PlayBack_thread,NULL);
	if (err)
	{
		printf("PlayBack_thread thread is not created!\n");
	}
}

void SystemExit(void)
{
    UdpSocketExit();
    PlayBack_Exit();

    sleep(1);                              
}

int main(int argc,char **argv)
{
    if(argc != 2){
		printf("please enter oppsite ip address!\n");
		return 0;
	}
	
	aimed_ip = strdup(argv[1]);
	printf("oppsite ip: %s\n",aimed_ip);

    SystemInitial();

    while (1)
    {
		usleep(2000000);
    }

    SystemExit();

	free(aimed_ip);

    return 0;
}

void *Capture_thread(void *arg)
{
    int count=0;
    printf("Capture Thread Is Runing!\n");

    while (1)
    {
        ReadOneFrame();
        printf(".");
		count++;
		if(count == 10)
		{
			count = 0;
			printf("\n");
		}
       // UdpSendDat(CaptureBuf,2048);
    }

    pthread_exit(NULL);
}

void *PlayBack_thread(void *arg)
{
    NODE_DATA NodeData;
    int ret=0;

    printf("PlayBack Thread Is Runing!\n");

    while(1)
    {	    
        if ((QueueIsEmpty(BcOrTalk) != 1) && (bReadyPlay == 1))
        {
			pthread_mutex_lock(&talk_mutex);
			QueueDelete(BcOrTalk,&NodeData);
			pthread_mutex_unlock(&talk_mutex);
			PlayOneFrame(NodeData.Buf,NodeData.Len);
			usleep(1000);
        }
        else
        {
			usleep(10000);
        }
    }

    pthread_exit(NULL);
}

void *UdpRecv_Thread(void *arg)
{
    int udpret=0;
    int addr_len=0;
    int res=0;
    fd_set readfds;
	int count=0;

    struct timeval tv;
    printf("UdpRecv Thread Is Runing!\n");
   
    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(fd_LocalUdp,&readfds);

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        res = select(fd_LocalUdp+1,&readfds,NULL,NULL,&tv);
        if (res <0)
        {
            printf("select error!\n");
        }
        else if (res == 0)                                     
        {
            usleep(1000);
        }
        else if (res >0)
        {      
            udpret = recvfrom(fd_LocalUdp,UdpBuf,BUF_SIZE,0,(struct sockaddr *)&udp_ser_addr,&addr_len);
            if (udpret <0)
            {
                 printf("udp recieve data error!\n");
            }

            if (udpret >0)
            {
				count++;
				if(count >= 10)
				{
					count =0;
					printf("udpret = %d\n",udpret);
				}
                UdpDatProcess(UdpBuf,udpret);
            }
        }
    }
}

void UdpDatProcess(char *buf,int size)
{
    int ret=0;
    NODE_DATA NodeData;

    memcpy(NodeData.Buf,(char *)buf,size);
    NodeData.Len=size;
	pthread_mutex_lock(&talk_mutex);
    QueueAppend(BcOrTalk,NodeData);
	pthread_mutex_unlock(&talk_mutex);

	if((ret = GetSize(BcOrTalk)) >= 2)
	{
		bReadyPlay = 1;
	}
}
