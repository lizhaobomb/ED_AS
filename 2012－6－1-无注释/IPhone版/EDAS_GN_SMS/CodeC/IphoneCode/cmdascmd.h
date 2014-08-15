/*******************************************************************************/
/* 功能：保存查询到的数据采集器参数
/* create date: 2009-9-18
/* update:
/* company: geodevice
/* author:  陈阳
/*******************************************************************************/

#ifndef __CMDASCMD_H__
#define __CMDASCMD_H__
#include "response.h"
#include "cmdasframe.h"


//台站参数
typedef struct tagSTNPARS{
	short id;//台号
	char name[32];//台名
	char abbr[8];//台名缩写
	short sens_num;//地震计数
	short chn_sum;//通道数
	float lat;//纬度 
	float lon;//经度 
	float alt;//高程，单位：m
	char run_date[32];//启动时间
	char netid[4];//网络归属
	char version[32];//版本
}STNPARS;
 
//正弦参数
typedef struct tagSINEPARS{
	short btimer;//1-定时启动
	short method;//定时方式：0-每天，1-每星期，2-每月，3-每年
	short tm_start1;//TM_METHOD:0 – 时1 – 星期2 – 日3 – 月
	short tm_start2;//启动时间，TM_METHOD:0 – 分1 – 时2 – 时3 – 日
	short tm_start3;//启动时间，TM_METHOD:1 – 分2 – 分3 – 时
	short tm_start4;//启动时间，TM_METHOD:3 – 分
	long time;//第一次启动时间
	short amptype;
	short prd_num;//频点数
	SINES sine[50];//正弦波频点参数，共prd_num个
}SINEPARS;

//脉冲/强震标定参数 
typedef struct tagPULSEPARS{
	short btimer;//1-定时
	short method;//定时方式：0-每天，1-每星期，2-每月，3-每年
	short tm_start1;//TM_METHOD:0 – 时1 – 星期2 – 日3 – 月
	short tm_start2;//启动时间，TM_METHOD:0 – 分1 – 时2 – 时3 – 日
	short tm_start3;//启动时间，TM_METHOD:1 – 分2 – 分3 – 时
	short tm_start4;//启动时间，TM_METHOD:3 – 分
	long time;//第一次启动时间
	short amptype;
	short amp;//幅度
	short dur;//脉冲间隔，0.1秒
}PULSEPARS;

//随机标定参数 
typedef struct tagPSEUDOPARS{
	short btimer;//1-定时启动
	short method;//定时方式：0-每天，1-每星期，2-每月，3-每年
	short tm_start1;//TM_METHOD:0 – 时1 – 星期2 – 日3 – 月
	short tm_start2;//启动时间，TM_METHOD:0 – 分1 – 时2 – 时3 – 日
	short tm_start3;//启动时间，TM_METHOD:1 – 分2 – 分3 – 时
	short tm_start4;//启动时间，TM_METHOD:3 – 分
	long time;//第一次启动时间
	short amptype;
	short amp;//幅度
	unsigned short dur;//码元间隔，0.001秒
	short signal_flag;//信号标志
	short rep_num;//重复次数
}PSEUDOPARS;
 
//地震计参数  
typedef struct tagSENSPARFRM
{
 	char name[16];//型号
	char provider[32];//供货商
	char series[16];//序列号
	char rectype[4];//类型 D: 位移型 V: 速度型 A: 加速度型
	char type[12];//标准类型,依照行业标准
	char bandstr[16];//带宽
	long depth;//埋深,cm
	char rock[32];//岩基
	char state[4];//状态，GC
	char rspfile[32];//响应文件
	long comp;//分向数
	int das_id[3];//分向对应的A/D通道序号，从1开始依次为高速A/D通道1-6，低速1-6
	char cxid[3][8];//依照DB/T3-2003 [地震及地震前兆测项分类及代码]标准
  	
}SENSPARFRM;
 

//采样状态
typedef struct tagSMPSTAT
{
	short samp;//采样率
	short phase;//采样相位1：线性相移，2：最小相位
	int stat;//1-启动,0-关闭
}SMPSTAT;

//地震计配置信息结构 
typedef struct tagSENSPARS{
 	SENSPARFRM sens_par;
	short samp;//系统采样率	1,10,20,50,100,200,500
	short phase;//系统采样相位	1：线性相移，2：最小相位, 采样+最小相位1,10,20,50,100,200,500
	SMPSTAT smpstat[16];//对应采样+线性相位1,10,20,50,100,125,200,500
	short hp;//高通滤波器代码，0: DC,  1:10S,   2:30S,   3:100S,4:300S, 5:1000S, 6:3000S
	SINEPARS sines;//正弦标定
	PULSEPARS pulse;//脉冲标定
	PULSEPARS strong;//强震标定
	PSEUDOPARS pseudo;//随机码标定
	int cal_delay;//标定等待时间，单位：秒
 	int trig_num;//有效trig_auto数
 	AUTOTRIG trig_auto[6];
 	int gain_id;//量程标志 0:10V， 1：20V,2: 5V,3-2.5V
	CM_INSTPARFRM  inst;
	int flag;//标志 1-非港震地震计，有控制信号, 0- 没有控制信号,2 - BBVS-60/120控制
	int unlock_sig;//开摆信号
	int lock_sig;//锁摆信号
	int adj_sig;//调零信号
	int cal_sig;//标定信号
	int stopcal_sig;//停止标定信号
	int unlock_sig_init;//开摆信号初始化值
	int lock_sig_init;//锁摆信号初始化值
	int adj_sig_init;//调零信号初始化值
	int cal_sig_init;//标定信号初始化值
	int stopcal_sig_init;//停止标定信号初始化值
}SENSPARS;
 

//串口发送方式
typedef struct tagCOMCTL
{
	short back;//1-应答方式，0-非应答
	short binit;//1-系统启动后直接发送波形，0-等待请求后发送
	short be_broad;//发送广播帧,1-发送，0-不发送（默认）
	short be_stat;//发送状态帧,1-发送，0-不发送（默认）
	long baud;//波特率
}COMCTL;
//gps信息
//接收星参数
typedef struct tagSTARSTAT
{
	char I;
	char H;
	char S;
	char D;
}STARSTAT;
typedef struct tagGPS
{
	char sync;//同步标志
	long time;//定位时间
	short dop;//DOP
	float lat;//纬度
	float lon;//经度
	float alt;//高程
	STARSTAT star[6];
	char rev_stat;//接收状态
	int star_sum;//星总数
	long next_tm;//下次对钟时间
} GPS;		

//电压
typedef struct tagVOL
{
	float out_v;//外部电压， 单位：V
	float in_v;//内部电压, 	单位：V
	float stor_v;//电瓶电压, 单位：V
}VOL;

//温度
typedef struct tagTEMPERATURE
{
	float temperature;//机箱温度,单位：度
}TEMPERATURE;

//钟差信息
typedef struct tagCLKERR
{
	float time_err;//时间误差
	float freq_err;//频率误差
}CLKERR;


//电池充电时间/停止充电时间 
typedef struct tagBATCHARGE
{
	long time;//启动/停止充电时间
	float vol;//充电前/后电池电压,v
}BATCHARGE;

//网络参数
typedef struct tagIPPARS
{
	char ip[64];//IP
	char mask[64];//掩码
	char gw[64];//网关
}IPPARS;


//SD卡状态
typedef struct tagSDSTAT
{
	long total;//总空间,单位:Kb
	long free;//可用空间,单位:Kb
	long temp;//硬盘温度，0.1摄氏度
	long be_curmass;//1-正在写的存储卡
	long io_error;//最后1次I/O错误
	short flag;//卡号
}SDSTAT;

//网络数据服务器控制参数
typedef struct tagNETSRVCTL
{
	int port;//控制端口 
	int dataport;//起始数据端口
	short tm_ctl;//超时控制，单位：秒
	short max_con;//最大客户数，默认：4
}NETSRVCTL;

 
//时区帧
typedef struct tagTIMEZONE
{
	char zone[10];//时区名
	short offset;//与GMT时区偏差
}TIMEZONE;

//综合判定 
typedef struct tagDETERMINEVT
{
	short method;//	触发判定方法：1-做综合判定，0-仅作地震计判定
	short trig_num;//触发地震计数,方法1时有效，触发地震计数
	short trig_win;//触发判定时间窗	,方法1时有效，触发判定时间窗，单位：秒
	short end_num;//结束触发地震计数,方法1有效，结束触发地震计数

}DETERMINEVT;


//数据采集器通用参数
typedef struct tagDASPARS{
  	COMCTL comctl;//串口服务
	NETSRVCTL rts_netsrv;//实时数据服务
	NETSRVCTL qk_netsrv;//快速数据服务
	CLIENTCONPAR data_srv[4];//主动发送服务
	GPS gps;//GPS
	int gps_int;//GPS间隔
	VOL vol;//电压
	TEMPERATURE temperature;//温度
	CLKERR clk_err;//钟差
	BATCHARGE bat,sbat;
	TIMEZONE timezone;//时区
	SDSTAT sd_stat[2];//存储器状态
 	short ctn_ttl;// -1：不做硬盘维护，在硬盘占用空间达到95%时，发出报警信息0：硬盘占用空间达到95%时，发出报警信息，并从最早一天的连续文件开始删除，直到占用空间小于80%>0：在外存储器中连续文件保存的天数，如果过期则删除
	IPPARS ip;//网络地址
	DETERMINEVT determine;//地震时间判断
}DASPARS;

typedef struct DASCMDSTRUC
{
	STNPARS stnpar;//台站参数
	DASPARS das;//采集器工作参数
	SENSPARS sens[12];//地震计
	int heart_interval;//心跳间隔
	int bwr;//1-允许写存储器，0-不允许写
}DASCMDSTRUC;


 
#endif