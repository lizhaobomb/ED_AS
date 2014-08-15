#ifndef __LIMITDEF_H__
#define __LIMITDEF_H__
//最大采样率
#define MAX_SMP_RATE 500
//每个地震计最多通道数
#define MAXCOMP 3
//网络接收缓冲区
#define MAX_RCV_BUF 8192

#define MAXFRAMESIZE MAX_SMP_RATE*4*3+1000

#endif