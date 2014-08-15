#ifndef  __RCVSTRUC_H__
#define __RCVSTRUC_H__

#include "limitdef.h"

//////////////////定义从台站接收数据的串口，网络结构，台站参数结构///////////////
//2004/3/8 陈阳

//串口

typedef struct tagLOC_COM {
	//串口名
	char com[20];
	//波特率
	int  baud;
	//串口句柄
	//Handle  hcom;
	//串口设备参数
	//DCB     dcb;
	//1-打开，0-否 , 2-用在初始状态，确定是否是edas-24ip串口 
	//BOOL    status;
}LOC_COM;

//连接ipu
typedef struct tagLOC_NET{
	//ipu地址
	char host[128];
	//控制端口
	int cmd_port;
	//数据端口
	int data_port;
	//用户
	char user[128];
	//密码
	char passwd[128];
	//控制端口连接句柄
	int  cfd;
	//数据端口连接句柄
	int dfd;
}LOC_NET;

//通道参数
typedef struct tagLOCALREVBLK{
	//通道代码
	int  local_id;
	//分向数
	int chn_sum;
	//采样率
	long samp;
	//仪器固有周期，秒
	float natural_prd;
//time: latest time,time_error: time error counter,lasttime: used in resend
	//数据时间
	long time;
	//时间错误计数
	long time_err;
	//重发数据时间
	long lasttime;
	long in_resend;
	long resend_begin,resend_end;
	//数据采集器通道，das_id[0]==-1: 不分道,ud,ew,ns
	int	 das_id[MAXCOMP];
	//数据类型，A-加速度V-速度D-位移
	char data_type;
	//观测点编号，分道时使用
	int obs_id;
	//观测点名，分道时使用
	char obs_name[8];
	//响应文件名
	char rsp_file[50];
	struct tagLOCALREVBLK *lpnext;
}LOCALREVBLK;

//台站参数结构
typedef struct tagSITEBLK{
	//台站名
	char site[8];
	//台站id
	int site_id;
	//节点号
	int section_id;
	//节点名缩写 
	char section_name[12];
	//字长
	int wlen;
	//1-本地节点
	int blocal;
	//通信方式 0: com,1:network
	int  protocol;
	//反向控制 1: 有反向
	int	 beresend;
	//串口连接
	LOC_COM *com_con;
	//网络连接
	LOC_NET *net_con;
	//通道参数
	LOCALREVBLK * local_rev;
	struct tagSITEBLK * lpnext;
}SITEREVBLK;
#endif