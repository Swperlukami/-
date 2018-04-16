#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include"stdbool.h"  //bool变量

#pragma pack(1)
typedef struct                   //队列结点数据的结构
{
    char  MusicFileName[60];     //音乐文件名称的长度
    long  FileLen;               //文件的长度
    char *MusicLoc;              //文件的位置
}MUSICINFO;
#pragma pack()


typedef struct node * PNode;
#pragma pack(1)
typedef struct        //队列结点数据的结构
{
    char Buf[2048];
    int  Len;
}NODE_DATA;
#pragma pack()

#pragma pack(1)
typedef struct node
{
    NODE_DATA data;
    PNode next;
}Node;
#pragma pack()

#pragma pack(1)
typedef struct
{
    PNode front;
    PNode rear;
    int size;
}Queue;
#pragma pack()
/*构造一个空队列*/
Queue *InitQueue();

/*销毁一个队列*/
void DestroyQueue(Queue *pqueue);

/*清空一个队列*/
void ClearQueue(Queue *pqueue);

/*判断队列是否为空*/
int QueueIsEmpty(Queue *pqueue);

/*返回队列大小*/
int GetSize(Queue *pqueue);

/*返回队头元素*/
PNode GetFront(Queue *pqueue,NODE_DATA *pitem);

/*返回队尾元素*/
PNode GetRear(Queue *pqueue,NODE_DATA *pitem);

/*将新元素入队*/
PNode QueueAppend(Queue *pqueue,NODE_DATA item);

/*队头元素出队*/
PNode QueueDelete(Queue *pqueue,NODE_DATA *pitem);

/*遍历队列并对各数据项调用visit函数*/
void QueueTraverse(Queue *pqueue,void (*visit)());


#endif // QUEUE_H
