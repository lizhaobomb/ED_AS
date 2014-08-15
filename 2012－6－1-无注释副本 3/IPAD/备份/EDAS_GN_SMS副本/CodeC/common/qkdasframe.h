#ifndef _qkdasframe_h
#define _qkdasframe_h

/*************************************************************************************
/*定义: 通用数据采集器/烈度仪快速服务输参数帧格式
/*作者：陈阳
/*create date: 2009-9-10
/*update date:
/*	date			action				common
/*	2009-9-24		QK_HDSTATFRM		增加be_curmass
/*  2009-11-3		QK_DASOFFFRM		修改格式
/*  2009-11-27		根据快速数据服务参数帧定义1.3修改格式,修订短整数对齐问题
/*  2010-2-2		QK_PULSEFRM			根据快速数据服务参数帧定义1.4修改格式
					QK_SINEFRM
					QK_PSEUDOFRM
/*  2010-2-24		QK_SENSROFFFRM		改sens_id为int(快速数据服务参数帧定义1.5)
/*  2010-3-4		QK_SENSSIGCTLFRM	增加地震计控制信号
/*  2010-3-31		QK_SMALDFRM		增加烈度计算结果(烈度仪用)
					QK_TAOPDFRM		增加灾害评估值(烈度仪用)
/*  2010-7-8		QK_SENSSIGCTLFRM	修改地震计控制信号
/*  2010-7-9		QK_SMALDFRM
					QK_TAOPDFRM
					QK_EVTTRIGFRM
					QK_EVTDETRIGFRM
	2010-9-14		QK_SENSPARFRM       根据烈度仪快速数据服务参数帧定义1.2增加宏定义区分适用与EDAS-24GN或适用于烈度仪
					QK_HEARTBEATFRM
					QK_VOLFRM
					QK_GPSINTFRM
	2010-11-3		QK_NTPSRVFRM		根据烈度仪快速数据服务参数帧定义1.3
					QK_SMSCODEFRM
/*  2011-9-18		QK_SENSSIGCTLFRM	修改地震计控制信号
 /*company: geodevice
/***********************************************************************************/
#include "iecomplex.h"


//帧头段
typedef struct tagQKHEAD
{
 	unsigned short cmd;//帧标记
	unsigned short length;//帧长度
}QKHEAD;



//台站参数帧
typedef struct tagQK_STNPARFRM
{
	QKHEAD head;
	int id;//台号
	char name[32];//台名
	char abbr[8];//台名缩写
	short sens_sum;//地震计数
	short chn_sum;//通道数
	long lat;//纬度,单位：mas为(-90.0~90.0)*3600000
	long lon;//经度,单位：mas为(-180.0~180.0)*3600000
	long alt;//高程，单位：cm
	char run_date[32];//启动时间
	char netid[4];//网络归属
	char version[32];//版本
	unsigned short chk_sum;
}QK_STNPARFRM;

//地震计参数帧 
#ifdef EDAS_IML
typedef struct tagQK_SENSPARFRM
{
	QKHEAD head;
	int sens_id;//地震计序号
	char name[16];//型号
	char provider[32];//供货商
	char series[16];//序列号
	char rectype[4];//类型 D: 位移型 V: 速度型 A: 加速度型
	char type[12];//标准类型,依照行业标准
	char bandstr[16];//带宽
	long depth;//埋深,cm
	char rock[32];//岩基
	float azim;//方位角
	char rspfile[32];//响应文件
	long comp;//分向数
	int das_id[3];//分向对应的A/D通道序号，从1开始依次为高速A/D通道1-6，低速1-6
	char cxid[3][8];//依照DB/T3-2003 [地震及地震前兆测项分类及代码]标准
	unsigned short chk_sum;
}QK_SENSPARFRM;
#else
typedef struct tagQK_SENSPARFRM
{
	QKHEAD head;
	int sens_id;//地震计序号
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
	unsigned short chk_sum;
}QK_SENSPARFRM;
#endif

typedef struct tagQK_INSTPARFRM 
{
	QKHEAD head; 
	int sens_id;//地震计序号
	float ao;//传递函数系数
	float ud;//垂直向灵敏度
	float ew;//东西向灵敏度
	float ns;//北南向灵敏度
	short pnum;//极点数
	short znum;//零点数
	iecomplex pole[25];//极点
	iecomplex zero[25];//零点
	float ud_damp;//垂直向阻尼
	float ew_damp;//东西向阻尼
	float ns_damp;//北南向阻尼
	float ud_freq;//垂直向自振周期,单位：HZ
	float ew_freq;//东西向自振周期,单位：HZ
	float ns_freq;//北南向自振周期,单位：HZ
	unsigned short chk_sum;//校验和
}QK_INSTPARFRM;

//采样率帧	
typedef struct tagQK_SMPRATEFRM
{
	QKHEAD head; 
	short sens_id;//地震计序号
	char phase_id;//相位代码1: 线性相位  2：最小相位
	char samp_id;//采样率代码
	char be_ad;//用于多采样时，0-关闭采样，1-启动采样
	char be_syssamp;//用于多采样时，0-不是系统采样率，1-是系统采样率
	unsigned short chk_sum;
}QK_SMPRATEFRM;
 
//量程帧
typedef struct tagQK_GAINFRM
{
	QKHEAD head; 
	short sens_id;//地震计序号
	short gain_id;//量程代码 0:10V， 1：20V, 2: 5V 3：2.5V
	unsigned short chk_sum;
}QK_GAINFRM;
 
//输入信号零点修正值
typedef struct tagQK_DASOFFFRM
{
	QKHEAD head; 
	int sens_id;//地震计序号
  	long ud;//垂直
	long ew;//东西
	long ns;//北南
	unsigned short chk_sum;
}QK_DASOFFFRM;

//脉冲标定帧
typedef struct tagQK_PULSEFRM
{
	QKHEAD head; 
	short sens_id;//地震计序号
	short btimer;//1-定时
	long time;//第一次启动时间
	short method;//定时方式：0-每天，1-每星期，2-每月，3-每年
	short tm_start1;//TM_METHOD:0 – 时1 – 星期2 – 日3 – 月
	short tm_start2;//启动时间，TM_METHOD:0 – 分1 – 时2 – 时3 – 日
	short tm_start3;//启动时间，TM_METHOD:1 – 分2 – 分3 – 时
	short tm_start4;//启动时间，TM_METHOD:3 – 分
	short amptype;//1-幅度为电流量，单位uA；0-count值
	short amp;//幅度
	unsigned short dur;//脉冲间隔，0.1秒
	unsigned short chk_sum;
}QK_PULSEFRM;


//正弦标定参数
typedef struct tagQK_SINES
{
	short cycle;//信号周期数
	short prd;//频率或周期,负表示周期，单位：0.1S，为正表示频率，单位：0.1Hz。
	short aten;//衰减因子/正弦波单峰幅值
}QK_SINES;
//正弦标定帧
typedef struct tagQK_SINEFRM{
	QKHEAD head;
	short sens_id;//地震计序号
	short btimer;//1-定时启动
	long time;//第一次启动时间
	short method;//定时方式：0-每天，1-每星期，2-每月，3-每年
	short tm_start1;//TM_METHOD:0 – 时1 – 星期2 – 日3 – 月
	short tm_start2;//启动时间，TM_METHOD:0 – 分1 – 时2 – 时3 – 日
	short tm_start3;//启动时间，TM_METHOD:1 – 分2 – 分3 – 时
	short tm_start4;//启动时间，TM_METHOD:3 – 分
	short amptype;//类型,1-sine[i].aten为正弦波单峰幅值，单位uA；0-衰减因子
	short prd_num;//频点数
	QK_SINES sine[50];//正弦波频点参数，共prd_num个
	unsigned short chk_sum;
}QK_SINEFRM;


//随机码帧
typedef struct tagQK_PSEUDOFRM
{
	QKHEAD head;
	short sens_id;//地震计序号
	short btimer;//1-定时启动
	long time;//第一次启动时间
	short method;//定时方式：0-每天，1-每星期，2-每月，3-每年
	short tm_start1;//TM_METHOD:0 – 时1 – 星期2 – 日3 – 月
	short tm_start2;//启动时间，TM_METHOD:0 – 分1 – 时2 – 时3 – 日
	short tm_start3;//启动时间，TM_METHOD:1 – 分2 – 分3 – 时
	short tm_start4;//启动时间，TM_METHOD:3 – 分
	short amptype;//1-幅度为电流量，单位uA；0-count值
	short amp;//幅度
	unsigned short dur;//码元间隔，0.001秒
	short signal_flag;//信号标志
	short rep_num;//重复次数
	unsigned short chk_sum;
}QK_PSEUDOFRM;

//强震标定帧
typedef struct tagQK_STRONGFRM
{
	QKHEAD head;
	short sens_id;//地震计序号
	short btimer;//1-定时
	long time;//第一次启动时间
	short method;//定时方式：0-每天，1-每星期，2-每月，3-每年
	short tm_start1;//TM_METHOD:0 – 时1 – 星期2 – 日3 – 月
	short tm_start2;//启动时间，TM_METHOD:0 – 分1 – 时2 – 时3 – 日
	short tm_start3;//启动时间，TM_METHOD:1 – 分2 – 分3 – 时
	short tm_start4;//启动时间，TM_METHOD:3 – 分
 	unsigned short chk_sum;
}QK_STRONGFRM;

//标定等待时间
typedef struct tagQK_CALIWAITFRM
{
	QKHEAD head;
	short sens_id;//地震计序号
	short time;//等待时间，秒
	unsigned short chk_sum;
}QK_CALIWAITFRM;

//启动标定
typedef struct tagQK_STARTCALFRM
{
	QKHEAD head;//cmd=0xA024: 启动脉冲标定 0xA025: 启动正弦标定 0xA026: 启动随机标定 0xA027: 启动强震标定
	int sens_id;//地震计序号
	long time;//--1:用于手工启动标定, 实际的启动时间由启动标定程序根据当前最新数据时间设置,>=0：用于定时启动标定，为有效的启动时间 
	unsigned short chk_sum;
}QK_STARTCALFRM;

//停止标定
typedef struct tagQK_STOPCALFRM
{
	QKHEAD head; 
	short sens_id;//地震计序号
 	unsigned short chk_sum;
}QK_STOPCALFRM;

//地震计调零/停止调零
typedef struct tagQK_SENSOFFFRM
{
	QKHEAD head;
	short sens_id;//地震计序号
 	unsigned short chk_sum;
}QK_SENSOFFFRM;

//地震计检测控制
typedef struct tagQK_SENSCTLFRM
{
	QKHEAD head;
	short sens_id;//地震计序号
	short bctl;//0:开摆, 1:锁摆
	unsigned short chk_sum;
}QK_SENSCTLFRM;

//地震计控制信号
typedef struct tagQK_SENSSIGCTLFRM
{
	QKHEAD head; 
	short sens_id;//地震计号
	short flag;//标志 1-非港震地震计，有控制信号, 0- 没有控制信号,2 - BBVS-60/120控制
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

	unsigned short chk_sum;//校验和

}QK_SENSSIGCTLFRM;

//实时网络数据服务,快速数据服务,非实时数据请求服务控制参数 
typedef struct tagQK_NETSRVCTLFRM
{
	QKHEAD head; 
	int port;//控制端口 
	int dataport;//起始的数据端口号
	short tm_ctl;//超时控制，单位：秒
	short max_con;//最大客户数，默认：4
	unsigned short chk_sum;//校验和
}QK_NETSRVCTLFRM;

//主动服务客户参数
typedef struct tagQK_CLIENTCONPAR
{
	short id;
	short bon;//1-发送,0-不发送
	short broot;//1-管理员身份，0-非管理员身份
	short bevent;//发送的数据类型，1-事件波形，0-连续波形，2-烈度(保留) 
	char host[44];//服务器地址
	long port;//服务器端口,大于5000的整数
	char sens[16];//地震计标志
	short pre_evt;//事件前长度，秒
	short aft_evt;//事件后长度，秒
	short tm_ctl;//超时控制，单位：秒
	short be_broad;//发送广播帧,1-发送，0-不发送（默认）
	short be_stat;//发送状态帧,1-发送，0-不发送（默认）
}QK_CLIENTCONPAR;

//主动服务
typedef struct tagQK_CLIENTCONFRM
{
	QKHEAD head; 
	QK_CLIENTCONPAR client[4];
	unsigned short chk_sum;
}QK_CLIENTCONFRM;

//串口参数—修改波特率
typedef struct tagQK_COMBAUDFRM
{
	QKHEAD head; 
	long baud;//波特率,4800,9600,19200,38400,57600,115200	
 	unsigned short chk_sum;//校验和
}QK_COMBAUDFRM;

//串口参数—控制发送方式（高级设置）
typedef struct tagQK_COMCTLFRM
{
	QKHEAD head;//0x1033
	short back;//1-应答方式，0-非应答
	short send;//1-系统启动后直接发送波形，0-等待请求后发送
	short be_broad;//发送广播帧,1-发送，0-不发送（默认）
	short be_stat;//发送状态帧,1-发送，0-不发送（默认）
	unsigned short chk_sum;//校验和
}QK_COMCTLFRM;

//检测器参数
typedef struct tagQK_AUTOTRIG{
	short flag;//检测器标志,设置时：1-修改触发器参数,-1 删除触发器,99：表示追加一个触发器。返回：1-有效触发器，0-无效触发器
	short trig_id;//检测器序号,从0开始
	short method;//检测方法,检测方法，0-电平，1-STA/LTA，2-电平+STA/LTA
	short filt_id;//滤波器代码
	short lwin;//长窗值,单位：0.1秒
	short swin;//短窗值，单位：0.1秒
	short vwin;//校验窗值，单位：0.1秒
	short ewin;//结束窗值，单位：0.1秒
	short td;//	STA/LTA触发阈值
	short vd;//	STA/LTA触发验证阈值
	short te;//STA/LTA结束阈值
	short ld;//电平触发阈值
	short le;//电平触发结束阈值
	short ud_w;//垂直向投票权重
	short ew_w;//东西向投票权重
	short ns_w;//北南向投票权重
	short total_w;//触发总票数
	short trig_len;//最大触发长度，单位：秒
}QK_AUTOTRIG;

//自动触发参数
typedef struct tagQK_AUTOTRIGFRM
{
	QKHEAD head;
	int sens_id;//地震计序号
	QK_AUTOTRIG autotrig[6];
	unsigned short chk_sum;//校验和
}QK_AUTOTRIGFRM;


//综合判定
typedef struct tagQK_DETERMINEVTFRM
{
	QKHEAD head; 
	short method;//	触发判定方法：1-做综合判定，0-仅作地震计判定
	short trig_num;//触发地震计数,方法1时有效，触发地震计数
	short trig_win;//触发判定时间窗	,方法1时有效，触发判定时间窗，单位：秒
	short end_num;//结束触发地震计数,方法1有效，结束触发地震计数
	unsigned short chk_sum;//校验和
}QK_DETERMINEVTFRM;

//事件触发信息帧
typedef struct tagQK_EVTTRIGFRM
{
	QKHEAD head;
	int sens_id;//地震计序号
	long trig_time;//触发时刻,到秒
	short frac_sec;//触发时刻,毫秒部分
	unsigned short chk_sum;
}QK_EVTTRIGFRM;

//事件触发结束帧
typedef struct tagQK_EVTDETRIGFRM
{
	QKHEAD head; 
	int sens_id;//地震计序号
	long trig_time;//触发时刻
	long detrig_time;//触发结束时刻
	short trigfrac_sec;//触发时刻,毫秒部分
	short detrigfrac_sec;//结束触发时刻,毫秒部分
 	unsigned short chk_sum;
}QK_EVTDETRIGFRM;

//运行状态信息传输间隔
typedef struct tagQK_HEARTBEATINTFRM
{
	QKHEAD head; 
	short interval;//传输间隔,>30单位:秒
	unsigned short chk_sum;//校验和
}QK_HEARTBEATINTFRM;

//电压帧
#ifdef EDAS_IML

typedef struct tagQK_VOLFRM
{
	QKHEAD head; 
	short ext_vol;//外部,0.1v
	short net_vol;//网络供电,0.1v
	short stor_vol;//电池,0.1v
	short mon_vol;//外部供电状态监测电压,0.1v
	unsigned short chk_sum;

}QK_VOLFRM;
#else
typedef struct tagQK_VOLFRM
{
	QKHEAD head; 
	short out_vol;//外部,0.1v
	short in_vol;//内部,0.1v
	short stor_vol;//电池,0.1v
	unsigned short chk_sum;

}QK_VOLFRM;
#endif

//温度帧
typedef struct tagQK_TEMPFRM
{
	QKHEAD head; 
	short temperature;//温度，0.1摄氏度
	unsigned short chk_sum;
}QK_TEMPFRM;

//钟差帧
typedef struct tagQK_CLKERRFRM
{
	QKHEAD head;
	short clk_err;//钟差
	short freq_err;//频差,0.01PPM
	unsigned short chk_sum;
}QK_CLKERRFRM;

//电池充电帧
typedef struct tagQK_BATCHARGEFRM
{
	QKHEAD head; 
	long time;//启动/停止充电时间
	int vol;//充电前/后电池电压,0.1v
	short bstop;//1-查询停止充电参数 ,0-查询充电启动参数
	unsigned short chk_sum;
}QK_BATCHARGEFRM;

//查询运行状态信息帧
#ifdef EDAS_IML

typedef struct tagQK_HEARTBEATFRM
{
	QKHEAD head; 
	short ext_vol;//0.1v
	short net_vol;//0.1v
	short stor_vol;//0.1v
	short mon_vol;//0.1v
	short temperature;//0.1C
	short clk_err;//微秒
	short freq_err;//0.01 ppm
	short unused;
	long buf_total;//存储器总空间,kb
	long buf_free;//存储器空闲空间，kb
	unsigned short chk_sum;
}QK_HEARTBEATFRM;
#else 
typedef struct tagQK_HEARTBEATFRM
{
	QKHEAD head; 
	short out_vol;//0.1v
	short in_vol;//0.1v
	short stor_vol;//0.1v
	short temperature;//0.1C
	short clk_err;//微秒
	short freq_err;//0.01 ppm
	long buf_total;//存储器总空间,kb
	long buf_free;//存储器空闲空间，kb
	unsigned short chk_sum;
}QK_HEARTBEATFRM;

#endif

//GPS状态
typedef struct tagQK_GPSSTATFRM
{
	QKHEAD head; 
	short bgps;//1-对钟，0-非对钟
	unsigned short chk_sum;
}QK_GPSSTATFRM;

//IRIG码信息
typedef struct tagQK_IRIGFRM
{
	QKHEAD head; 
	long time;//时间码,从GMT 1970-01-01 00:00:00起算的秒数
	long lat;//纬度,单位：ma=1/3600000度
	long lon;//经度,单位：mas=1/3600000度
	long alt;//高程,单位：0.1M
	short dop;
 	unsigned short chk_sum;
}QK_IRIGFRM;

//存储器状态
typedef struct tagQK_HDSTATFRM
{
	QKHEAD head; 
	short id;//存储器序号
	short be_curmass;//1-当前存储器，0-不是当前存储器
	long total;//总空间,单位:Kb
	long free;//可用空间,单位:Kb
	long io_error;//最后1次I/O错误时间
 	unsigned short chk_sum;//校验和
}QK_HDSTATFRM;
  

//查询地震计零位
typedef struct tagQK_SENSROFFFRM
{
	QKHEAD head; 
	int sens_id;//地震计序号
	long ch1;//1通道
	long ch2;//2通道
	long ch3;//3通道
	unsigned short chk_sum;
}QK_SENSROFFFRM;

//记录控制
typedef struct tagQK_MASSWRCTLFRM
{
	QKHEAD head; 
	short ctl;//1-启动向存储器写数据，0-不写数据
	unsigned short chk_sum;//校验和
}QK_MASSWRCTLFRM;

//清空存储器
typedef struct tagQK_CLEARSDFRM
{
	QKHEAD head; 
	short id;//存储器序号,从1开始
	unsigned short chk_sum;//校验和
}QK_CLEARSDFRM;

//文件备份规则
typedef struct tagQK_BACKRULEFRM
{
	QKHEAD head; 
	//删除时间段,-1：不做维护，在存储器占用空间达到95%时，发出报警信息 
	//0：按空间维护，硬盘占用空间达到95%时，发出报警信息，并从最早一天的连续文件开始删除，直到占用空间小于70%
	//>0：按时间维护，存储器中连续文件保存的天数，如果过期则删除
 	short ctn_ttl;
 	unsigned short chk_sum;//校验和
}QK_BACKRULEFRM;


//IP、网关
typedef struct tagQK_IPGWFRM
{
	QKHEAD head; 
	char ip[44];//地址
	char netmask[44];//掩码
	char gate[44];//网关
	unsigned short chk_sum;//校验和
}QK_IPGWFRM;


//设置系统时间帧
typedef struct tagQK_SYSTIMEFRM
{
	QKHEAD head; 
	long sec;//从1970开始秒数
	unsigned short chk_sum;
}QK_SYSTIMEFRM;


//设置时区帧
typedef struct tagQK_TIMEZONEFRM
{
	QKHEAD head; 
	char zone[10];//时区名
	short offset;//与GMT时区偏差
	unsigned short chk_sum;
}QK_TIMEZONEFRM;

//GPS对钟间隔
#ifdef EDAS_IML

typedef struct tagQK_GPSINTFRM{
	QKHEAD head; 
	short interval;//1-60分,-1:连续对钟
	short type;//对钟方式，2-IRIG码，1-内部GPS 3-网络授时
	unsigned short chk_sum;
}QK_GPSINTFRM;
#else

typedef struct tagQK_GPSINTFRM{
	QKHEAD head; 
	short interval;//1-60分,-1:连续对钟
 	unsigned short chk_sum;
}QK_GPSINTFRM;
#endif

//NTP时间服务器
typedef struct tagQK_NTPSRVFRM
{
	QKHEAD head; 
	char host1[20];//NTP服务器主地址
	char host2[20];//NTP服务器备用地址
	char host3[20];//NTP服务器备用地址
	unsigned short chk_sum;

}QK_NTPSRVFRM;
//短信号码
typedef struct tagQK_SMSCODEFRM
{
	QKHEAD head;
	short type;//1-仪器状态报警，2-地震信息报警
	short total;//号码总数，最多20个
	char phone[20][12];//短信接收人号码
	unsigned short chk_sum;
}QK_SMSCODEFRM;

//广播帧
typedef struct tagQK_BROADCASTFRM
{
	QKHEAD head; 
	//1-采样率设置，2-量程设置，3-地震计控制，4-调地震计零点，5-存储器空间超过80%，6-存储器空间超过70%，7-存储器I/O错误，8-出现电压错误,9-温度错误,10-电池充电,11-GPS错误 
	short type;
	unsigned short chk_sum;
}QK_BROADCASTFRM;


////烈度信息帧：
//烈度计算结果
typedef struct tagQK_SMALDFRM
{
	QKHEAD head; 
	int sens_id;//地震计号
	char name[8];//台名
	long trig_time;//触发时间 	从GMT 1970-01-01 00:00:00起算的秒数 
	int frac_time;//触发时间,毫秒部分
	float ld;//	烈度
	float pga_u;//PGA垂直向,单位：gal
	float	pga_e;//PGA东西向,	单位：gal
	float pga_n;//	PGA南北向,	单位：gal
	float rsp_1;//1Hz反应谱,	单位：gal
	float rsp_2;//2Hz反应谱,	单位：gal
	float rsp_5;//5Hz反应谱,	单位：gal
	float rsp_8;//8Hz反应谱,	单位：gal
	float fp;//卓越周期,单位：hz
	float td;//持时,单位：秒
	float  pgv_e;//PGV东西向,单位：cm/s
	float pgv_n;//PGV北南向,单位：cm/s
	unsigned short chk_sum;
}QK_SMALDFRM;

//灾害评估值（吴）
typedef struct tagQK_TAQPDFRM
{
	QKHEAD head; 
	int sens_id;//地震计号
	char name[8];//台名
	long trig_time;//触发时间 	从GMT 1970-01-01 00:00:00起算的秒数 
	int frac_time;//触发时间,毫秒部分
	float tao_c;
	float pd;//单位：cm
	unsigned  short chk_sum;//CRC-16校验

}QK_TAQPDFRM; 


//事件记录
typedef struct tagQK_EVTRECFRM
{
	QKHEAD head;
	short be_wr;//1-记录
	short pre_evt;//事件前记录长度，秒
	short aft_evt;//事件后记录长度，秒
	unsigned short chk_sum;// 校验
}QK_EVTRECFRM;

//数据应答帧
typedef struct tagQK_ACKFRM
{
	QKHEAD head;
	long time;//数据时间码
	char frac_sec;//第一个样点的秒内偏移值
	char chn_id;//通道序号
	short samp;//采样率
	short pha_id;//相位，1-线性，2-最小
	unsigned  short chk_sum;//CRC-16校验
}QK_ACKFRM;
 
//认证码
typedef struct tagQK_AUTHCODE
{
	QKHEAD head;
	char code[40];
	unsigned  short chk_sum;
}QK_AUTHCODE;
#endif