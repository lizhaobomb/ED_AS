#ifndef __SITEMAGSTRUC_H__
#define __SITEMAGSTRUC_H__
#include "CommonDef.h"
#define UD 0
#define EW 1
#define NS 2
#define ALL 3
//显示控制
/*
typedef struct DSPCTL{
	int scantime;//扫描时间
	int voffset;//通道偏移
	int hoffset;//时间偏移
	int channelnum;//显示通道数
	int channeltype;//显示通道类型
	int maxrange_Hscroll,maxrange_Vscroll;//水平滑杆最大长度，垂直滑杆最大长度
	int iHscrollPos,iVscrollPos;//水平滑杆位置，垂直滑杆位置
	int totalchannel,channel_range;//总通道数，通道显示宽度
}DSPCTL;
*/
typedef struct tagDSPPAR{
	float max;//显示最大值
	float min;//显示最小值
	int unit;//0-count 1-V 2-um/s 3-gal
}DSPPAR;

struct SHDAT1 {
 	struct SHDAT1 *pn;
	struct SHDAT1 *pb;
	short  check_sum;
	long  flg;//数据fifo标志
	long time;//数据时间
	long cali_flag;//标定标志 4-脉冲 2-正弦 6-随机码
	long  *ds;//数据
};
//存储区
 typedef struct tagREVBLKS{
	struct SHDAT1 *ps,*ps1;
	struct SHDAT1 *pe,*pe1;
	struct SHDAT1 *p_rev,*p_rev1;
	struct SHDAT1 *dat;
	struct SHDAT1 *p_dsp;
	char * p_dat,*p_dat1;//分配的整体数据区
	struct SHDAT1 *wr_dat;
	int st;
}REVBLKS;

typedef struct tagPOINT{
	int x;
	int y;
}POINT;



typedef struct tagCHANNEL{
	char stn[40];//台名
	int chn_id;//通道号
	char chn[20];//通道名
	char chn_label[20];//通道屏幕表示
	int samp,wlen;//采样，字长
	BOOL bView;//1-允许显示
	BOOL bShow;//1-显示状态
	int timex;
	long maxvalue,minvalue,offset;//最新数据最大、最小值和中间值
	long lsttm,curtm;//上一秒时间，当前时间

	//point
	REVBLKS revblk;//存收到的波形
	REVBLKS cmdblk;//存收到的参数
	//draw parameter
	int position,data_cnt,max_posi,cnt;
    double xa,xi;
	int xaj,xij;
//screen resolution width*2
	POINT *point;//波形屏幕坐标
	DSPPAR dsp;//显示参数
	int wr_cnt;//存盘数
	int cali_flag;//标定标志 4-脉冲 2-正弦 6-随机码
	float factor;//灵敏度
	struct tagCHANNEL * lpnext;
}CHANNEL;

typedef struct tagTXCMD {
	struct tagTXCMD *pn;
	int cnt;
	char buf[512];
 }TXCMD;



#endif