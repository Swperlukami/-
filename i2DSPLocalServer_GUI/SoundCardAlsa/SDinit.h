
#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

void SystemInitial(void);
void UdpDatProcess(char *buf,int size);
void SystemExit(void);
void CreateSomeThread(void);
void *Capture_thread(void *arg);
void *PlayBack_thread(void *arg);
void *UdpRecv_Thread(void *arg);

#endif
