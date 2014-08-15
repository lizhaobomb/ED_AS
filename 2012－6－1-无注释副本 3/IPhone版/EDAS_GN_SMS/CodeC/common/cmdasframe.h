#ifndef _cmdasframe_h
#define _cmdasframe_h

/*************************************************************************************
/*∂®“Â: Õ®”√ ˝æ›≤…ºØ∆˜ µ ± ˝æ›∑˛ŒÒ,¥Æø⁄ ˝æ›∑˛ŒÒ,÷˜∂Ø∑¢ÀÕ∑˛ŒÒ ‰≤Œ ˝÷°∏Ò Ω
/*◊˜’ﬂ£∫≥¬—Ù
/*create date: 2009-9-10
/*update date:
/*	date			action				common
/*	2009-9-24		add					CM_HDSTATFRM:‘ˆº”be_curmass
/*  2009-11-3		edit				CM_DASOFFFRM:–ﬁ∏ƒ∏Ò Ω
/*  2009-11-4		edit				CMHEAD: cmd∏ƒŒ™unsigned short
/*  2010-2-2		CM_PULSEFRM			∏˘æ›≤Œ ˝÷°∂®“Â1.4–ﬁ∏ƒ∏Ò Ω
					CM_SINEFRM
					CM_PSEUDOFRM		
/*  2010-3-4		CM_SENSSIGCTLFRM	∏˘æ›≤Œ ˝÷°∂®“Â1.5,‘ˆº”µÿ’º∆øÿ÷∆–≈∫≈
/*  2010-7-8		CM_SENSSIGCTLFRM	∏˘æ›≤Œ ˝÷°∂®“Â1.6,–ﬁ∏ƒµÿ’º∆øÿ÷∆–≈∫≈
/*  2010-7-9		CM_EVTTRIGFRM
					CM_EVTDETRIGFRM
	2010-9-14		CM_SENSPARFRM       ∏˘æ›¡“∂»“«≤Œ ˝÷°∂®“Â1.1‘ˆº”∫Í∂®“Â«¯∑÷  ”√”ÎEDAS-24GNªÚ  ”√”⁄¡“∂»“«
					CM_HEARTBEATFRM
					CM_VOLFRM
					CM_GPSINTFRM
 				
	2010-11-3		CM_NTPSRVFRM		∏˘æ›¡“∂»“«≤Œ ˝÷°∂®“Â1.2
					CM_SMSCODEFRM
/*  2011-9-18		CM_SENSSIGCTLFRM	∏˘æ›≤Œ ˝÷°∂®“Â1.7,–ﬁ∏ƒµÿ’º∆øÿ÷∆–≈∫≈

 /*company: geodevice
/***********************************************************************************/
#include "iecomplex.h"



//÷°Õ∑∂Œ
typedef struct tagCMDHEAD
{
	short sens_id;//µÿ’º∆∫≈
	unsigned short cmd;//÷°±Íº«
	short length;//÷°≥§∂»
	short reserved;//±£¡Ù
}CMDHEAD;

//”¶¥÷°
typedef struct tagACKFRAME
{
	CMDHEAD head;//0x7777
	long time;// ±º‰¬Î
	short chk_sum;
}ACKFRAME;


// ˝æ›«Î«Û÷°
typedef struct tagASKDATAFRM
{
	CMDHEAD head;
	long time;// ˝æ›∆ º ±º‰,1:¥”ø…ƒ‹µƒ◊Ó‘Á ˝æ›ø™ º£ª2:¥”ø…ƒ‹µƒ◊Ó–¬ ˝æ›ø™ º£ª3~8£∫±£¡Ù£¨Œ¥∂®“Â£ª9:Õ£÷π ˝æ›¡˜∑˛ŒÒ£ª∆‰À¸£∫÷∏∂® ±º‰æ¯∂‘√Î£¨»Ù÷∏∂® ±º‰∂Œƒ⁄ ˝æ›»´≤ø¥Ê‘⁄£¨Ã·π©»´≤ø ˝æ›£¨≤ø∑÷¥Ê‘⁄£¨Ã·π©≤ø∑÷ ˝æ›£¨≤ª¥Ê‘⁄£¨ ˝æ›∑˛ŒÒ≤ªª·±ªº§ªÓ°£
	short len;// ˝æ›≥§∂»,-1£∫¡¨–¯¡˜
	short chk_sum;
}ASKDATAFRM;

//Ã®’æ≤Œ ˝÷°
typedef struct tagCM_STNPARFRM
{
	CMDHEAD head;
	int id;//Ã®∫≈
	char name[32];//Ã®√˚
	char abbr[8];//Ã®√˚Àı–¥
	short sens_sum;//µÿ’º∆ ˝
	short chn_sum;//Õ®µ¿ ˝
	long lat;//Œ≥∂»,µ•Œª£∫masŒ™(-90.0~90.0)*3600000
	long lon;//æ≠∂»,µ•Œª£∫masŒ™(-180.0~180.0)*3600000
	long alt;//∏ﬂ≥Ã£¨µ•Œª£∫cm
	char run_date[32];//∆Ù∂Ø ±º‰
	char netid[4];//Õ¯¬ÁπÈ Ù
	char version[32];//∞Ê±æ
	short chk_sum;
}CM_STNPARFRM;

//地震计参数帧
//#ifdef EDAS_IML//烈度仪
typedef struct tagCM_SENSPARFRM
{
	CMDHEAD head;
	char name[16];//型号
	char provider[32];//供货商
	char series[16];//序列号
	char rectype[4];//类型 D: 位移型 V: 速度型 A: 加速度型
	char type[12];//标准类型，依照行业标准
	char bandstr[16];//带宽
	long depth;//埋深，cm
	char rock[32];//岩基
	float azim;//方位角
	char rspfile[32];//响应文件
	long comp;//分向数
	int das_id[3];//分向对应的A/D通道序号，从1开始依次为高速A/D通道1-6，低速1-6
	char cxid[3][8];//依照DB/T3-2003 [地震及地震前兆测项分类及代码]标准
	short chk_sum;
}CM_SENSPARFRM;

//#else
/*
typedef struct tagCM_SENSPARFRM
{
	CMDHEAD head;
	char name[16];//型号
	char provider[32];//供货商
	char series[16];//序列号
	char rectype[4];//类型 D:位移型 V:速度型 A:加速度型
	char type[12];//标准类型，依照行业标准
	char bandstr[16];//带宽
	long depth;//埋深，cm
	char rock[32];//岩基
    float azim;//方位角
	char state[4];//状态，GC
	char rspfile[32];//响应文件
	long comp;//分向数
	int das_id[3];//分向对应的A/D通道序号，从1开始依次为高速A/D通道1-6，低速1-6
	char cxid[3][8];//依照DB/T3-2003 [地震及地震前兆测项分类及代码]标准
	short chk_sum;
}CM_SENSPARFRM;*/
//#endif

typedef struct tagCM_INSTPARFRM 
{
	CMDHEAD head; 
	float ao;//¥´µ›∫Ø ˝œµ ˝
	float ud;//¥π÷±œÚ¡È√Ù∂»
	float ew;//∂´Œ˜œÚ¡È√Ù∂»
	float ns;//±±ƒœœÚ¡È√Ù∂»
	short pnum;//º´µ„ ˝
	short znum;//¡„µ„ ˝
	iecomplex pole[25];//º´µ„
	iecomplex zero[25];//¡„µ„
	float ud_damp;//¥π÷±œÚ◊Ëƒ·
	float ew_damp;//∂´Œ˜œÚ◊Ëƒ·
	float ns_damp;//±±ƒœœÚ◊Ëƒ·
	float ud_freq;//¥π÷±œÚ◊‘’Ò÷‹∆⁄,µ•Œª£∫HZ
	float ew_freq;//∂´Œ˜œÚ◊‘’Ò÷‹∆⁄,µ•Œª£∫HZ
	float ns_freq;//±±ƒœœÚ◊‘’Ò÷‹∆⁄,µ•Œª£∫HZ
	short chk_sum;//–£—È∫Õ
}CM_INSTPARFRM;

//≤…—˘¬ ÷°	
typedef struct tagCM_SMPRATEFRM
{
	CMDHEAD head; 
	char phase_id;//œ‡Œª¥˙¬Î1: œﬂ–‘œ‡Œª  2£∫◊Ó–°œ‡Œª
	char samp_id;//≤…—˘¬ ¥˙¬Î
	char be_ad;//”√”⁄∂‡≤…—˘ ±£¨0-πÿ±’≤…—˘£¨1-∆Ù∂Ø≤…—˘
	char be_syssamp;//”√”⁄∂‡≤…—˘ ±£¨0-≤ª «œµÕ≥≤…—˘¬ £¨1- «œµÕ≥≤…—˘¬ 
	short chk_sum;
}CM_SMPRATEFRM;
 
//¡ø≥Ã÷°
typedef struct tagCM_GAINFRM
{
	CMDHEAD head; 
	short gain_id;//¡ø≥Ã¥˙¬Î 0:10V£¨ 1£∫20V, 2: 5V 3£∫2.5V
	short chk_sum;
}CM_GAINFRM;
 
// ‰»Î–≈∫≈¡„µ„–ﬁ’˝÷µ
typedef struct tagCM_DASOFFFRM
{
	CMDHEAD head;
 	long ud;//¥π÷±
	long ew;//∂´Œ˜
	long ns;//±±ƒœ
	short chk_sum;
}CM_DASOFFFRM;

//¬ˆ≥Â±Í∂®÷°
typedef struct tagCM_PULSEFRM
{
	CMDHEAD head; 
	short btimer;//1-∂® ±
	short method;//∂® ±∑Ω Ω£∫0-√øÃÏ£¨1-√ø–«∆⁄£¨2-√ø‘¬£¨3-√øƒÍ
	short tm_start1;//TM_METHOD:0 ®C  ±1 ®C –«∆⁄2 ®C »’3 ®C ‘¬
	short tm_start2;//∆Ù∂Ø ±º‰£¨TM_METHOD:0 ®C ∑÷1 ®C  ±2 ®C  ±3 ®C »’
	short tm_start3;//∆Ù∂Ø ±º‰£¨TM_METHOD:1 ®C ∑÷2 ®C ∑÷3 ®C  ±
	short tm_start4;//∆Ù∂Ø ±º‰£¨TM_METHOD:3 ®C ∑÷
	long time;//µ⁄“ª¥Œ∆Ù∂Ø ±º‰
	short amptype;//1-∑˘∂»Œ™µÁ¡˜¡ø£¨µ•ŒªuA£ª0-count÷µ
	short amp;//∑˘∂»
	unsigned short dur;//¬ˆ≥Âº‰∏Ù£¨0.1√Î
	short chk_sum;
}CM_PULSEFRM;


//’˝œ“±Í∂®≤Œ ˝
typedef struct tagSINES
{
	short cycle;//–≈∫≈÷‹∆⁄ ˝
	short prd;//∆µ¬ ªÚ÷‹∆⁄,∏∫±Ì æ÷‹∆⁄£¨µ•Œª£∫0.1S£¨Œ™’˝±Ì æ∆µ¬ £¨µ•Œª£∫0.1Hz°£
	short aten;//À•ºı“Ú◊”/’˝œ“≤®µ•∑Â∑˘÷µ
}SINES;
//’˝œ“±Í∂®÷°
typedef struct tagCM_SINEFRM{
	CMDHEAD head;//0x1021
	short btimer;//1-∂® ±∆Ù∂Ø
	short method;//∂® ±∑Ω Ω£∫0-√øÃÏ£¨1-√ø–«∆⁄£¨2-√ø‘¬£¨3-√øƒÍ
	short tm_start1;//TM_METHOD:0 ®C  ±1 ®C –«∆⁄2 ®C »’3 ®C ‘¬
	short tm_start2;//∆Ù∂Ø ±º‰£¨TM_METHOD:0 ®C ∑÷1 ®C  ±2 ®C  ±3 ®C »’
	short tm_start3;//∆Ù∂Ø ±º‰£¨TM_METHOD:1 ®C ∑÷2 ®C ∑÷3 ®C  ±
	short tm_start4;//∆Ù∂Ø ±º‰£¨TM_METHOD:3 ®C ∑÷
	long time;//µ⁄“ª¥Œ∆Ù∂Ø ±º‰
	short amptype;//¿‡–Õ,1-sine[i].atenŒ™’˝œ“≤®µ•∑Â∑˘÷µ£¨µ•ŒªuA£ª0-À•ºı“Ú◊”
	short prd_num;//∆µµ„ ˝
	SINES sine[50];//’˝œ“≤®∆µµ„≤Œ ˝£¨π≤prd_num∏ˆ
	short chk_sum;
}CM_SINEFRM;


//ÀÊª˙¬Î÷°
typedef struct tagCM_PSEUDOFRM
{
	CMDHEAD head;//0x1022
	short btimer;//1-∂® ±∆Ù∂Ø
	short method;//∂® ±∑Ω Ω£∫0-√øÃÏ£¨1-√ø–«∆⁄£¨2-√ø‘¬£¨3-√øƒÍ
	short tm_start1;//TM_METHOD:0 ®C  ±1 ®C –«∆⁄2 ®C »’3 ®C ‘¬
	short tm_start2;//∆Ù∂Ø ±º‰£¨TM_METHOD:0 ®C ∑÷1 ®C  ±2 ®C  ±3 ®C »’
	short tm_start3;//∆Ù∂Ø ±º‰£¨TM_METHOD:1 ®C ∑÷2 ®C ∑÷3 ®C  ±
	short tm_start4;//∆Ù∂Ø ±º‰£¨TM_METHOD:3 ®C ∑÷
	long time;//µ⁄“ª¥Œ∆Ù∂Ø ±º‰
	short amptype;//1-∑˘∂»Œ™µÁ¡˜¡ø£¨µ•ŒªuA£ª0-count÷µ
	short amp;//∑˘∂»
	unsigned short dur;//¬Î‘™º‰∏Ù£¨0.001√Î
	short signal_flag;//–≈∫≈±Í÷æ
	short rep_num;//÷ÿ∏¥¥Œ ˝
	short chk_sum;
}CM_PSEUDOFRM;

//«ø’±Í∂®÷°
typedef struct tagCM_STRONGFRM
{
	CMDHEAD head;//0x1023
	short btimer;//1-∂® ±
	short method;//∂® ±∑Ω Ω£∫0-√øÃÏ£¨1-√ø–«∆⁄£¨2-√ø‘¬£¨3-√øƒÍ
	short tm_start1;//TM_METHOD:0 ®C  ±1 ®C –«∆⁄2 ®C »’3 ®C ‘¬
	short tm_start2;//∆Ù∂Ø ±º‰£¨TM_METHOD:0 ®C ∑÷1 ®C  ±2 ®C  ±3 ®C »’
	short tm_start3;//∆Ù∂Ø ±º‰£¨TM_METHOD:1 ®C ∑÷2 ®C ∑÷3 ®C  ±
	short tm_start4;//∆Ù∂Ø ±º‰£¨TM_METHOD:3 ®C ∑÷
	long time;//µ⁄“ª¥Œ∆Ù∂Ø ±º‰
	short chk_sum;
}CM_STRONGFRM;

//±Í∂®µ»¥˝ ±º‰
typedef struct tagCM_CALIWAITFRM
{
	CMDHEAD head;//0x1029
	short time;//µ»¥˝ ±º‰£¨√Î
	short chk_sum;
}CM_CALIWAITFRM;

//∆Ù∂Ø±Í∂®
typedef struct tagCM_STARTCALFRM
{
	CMDHEAD head;//cmd=0x6024: ∆Ù∂Ø¬ˆ≥Â±Í∂® 0x6025: ∆Ù∂Ø’˝œ“±Í∂® 0x6026: ∆Ù∂ØÀÊª˙±Í∂® 0x6027: ∆Ù∂Ø«ø’±Í∂®
	long time;//--1:”√”⁄ ÷π§∆Ù∂Ø±Í∂®,  µº µƒ∆Ù∂Ø ±º‰”…∆Ù∂Ø±Í∂®≥Ã–Ú∏˘æ›µ±«∞◊Ó–¬ ˝æ› ±º‰…Ë÷√,>=0£∫”√”⁄∂® ±∆Ù∂Ø±Í∂®£¨Œ™”––ßµƒ∆Ù∂Ø ±º‰ 
	short chk_sum;
}CM_STARTCALFRM;

//µÿ’º∆ºÏ≤‚øÿ÷∆
typedef struct tagCM_SENSCTLFRM
{
	CMDHEAD head;
	short bctl;//0:ø™∞⁄, 1:À¯∞⁄
	short chk_sum;
}CM_SENSCTLFRM;

// µ ±Õ¯¬Á ˝æ›∑˛ŒÒ,øÏÀŸ ˝æ›∑˛ŒÒ,∑« µ ± ˝æ›«Î«Û∑˛ŒÒøÿ÷∆≤Œ ˝ 
typedef struct tagCM_NETSRVCTLFRM
{
	CMDHEAD head; 
	int port;//øÿ÷∆∂Àø⁄ 
	int dataport;//∆ ºµƒ ˝æ›∂Àø⁄∫≈
	short tm_ctl;//≥¨ ±øÿ÷∆£¨µ•Œª£∫√Î
	short max_con;//◊Ó¥ÛøÕªß ˝£¨ƒ¨»œ£∫4
	short chk_sum;//–£—È∫Õ
}CM_NETSRVCTLFRM;

//÷˜∂Ø∑˛ŒÒøÕªß≤Œ ˝
typedef struct tagCLIENTCONPAR
{
	short id;
	short bon;//1-∑¢ÀÕ,0-≤ª∑¢ÀÕ
	short broot;//1-π‹¿Ì‘±…Ì∑›£¨0-∑«π‹¿Ì‘±…Ì∑›
	short bevent;//∑¢ÀÕµƒ ˝æ›¿‡–Õ£¨1- ¬º˛≤®–Œ£¨0-¡¨–¯≤®–Œ£¨2-¡“∂»(±£¡Ù) 
	char host[44];//∑˛ŒÒ∆˜µÿ÷∑
	long port;//∑˛ŒÒ∆˜∂Àø⁄,¥Û”⁄5000µƒ’˚ ˝
	char sens[16];//µÿ’º∆±Í÷æ
	short pre_evt;// ¬º˛«∞≥§∂»£¨√Î
	short aft_evt;// ¬º˛∫Û≥§∂»£¨√Î
	short tm_ctl;//≥¨ ±øÿ÷∆£¨µ•Œª£∫√Î
	short be_broad;//∑¢ÀÕπ„≤•÷°,1-∑¢ÀÕ£¨0-≤ª∑¢ÀÕ£®ƒ¨»œ£©
	short be_stat;//∑¢ÀÕ◊¥Ã¨÷°,1-∑¢ÀÕ£¨0-≤ª∑¢ÀÕ£®ƒ¨»œ£©
	short unused;//±£¡Ù
}CLIENTCONPAR;

//÷˜∂Ø∑˛ŒÒ
typedef struct tagCM_CLIENTCONFRM
{
	CMDHEAD head; 
	CLIENTCONPAR client[4];
	short chk_sum;
}CM_CLIENTCONFRM;

//¥Æø⁄≤Œ ˝°™–ﬁ∏ƒ≤®Ãÿ¬ 
typedef struct tagCM_COMBAUDFRM
{
	CMDHEAD head; 
	long baud;//≤®Ãÿ¬ ,4800,9600,19200,38400,57600,115200	
 	short chk_sum;//–£—È∫Õ
}CM_COMBAUDFRM;

//¥Æø⁄≤Œ ˝°™øÿ÷∆∑¢ÀÕ∑Ω Ω£®∏ﬂº∂…Ë÷√£©
typedef struct tagCM_COMCTLFRM
{
	CMDHEAD head;//0x1033
	short back;//1-”¶¥∑Ω Ω£¨0-∑«”¶¥
	short send;//1-œµÕ≥∆Ù∂Ø∫Û÷±Ω”∑¢ÀÕ≤®–Œ£¨0-µ»¥˝«Î«Û∫Û∑¢ÀÕ
	short be_broad;//∑¢ÀÕπ„≤•÷°,1-∑¢ÀÕ£¨0-≤ª∑¢ÀÕ£®ƒ¨»œ£©
	short be_stat;//∑¢ÀÕ◊¥Ã¨÷°,1-∑¢ÀÕ£¨0-≤ª∑¢ÀÕ£®ƒ¨»œ£©
	short chk_sum;//–£—È∫Õ
}CM_COMCTLFRM;

//ºÏ≤‚∆˜≤Œ ˝
typedef struct tagAUTOTRIG{
	short flag;//ºÏ≤‚∆˜±Í÷æ,…Ë÷√ ±£∫1-–ﬁ∏ƒ¥•∑¢∆˜≤Œ ˝,-1 …æ≥˝¥•∑¢∆˜,99£∫±Ì æ◊∑º”“ª∏ˆ¥•∑¢∆˜°£∑µªÿ£∫1-”––ß¥•∑¢∆˜£¨0-Œﬁ–ß¥•∑¢∆˜
	short trig_id;//ºÏ≤‚∆˜–Ú∫≈,¥”0ø™ º
	short method;//ºÏ≤‚∑Ω∑®,ºÏ≤‚∑Ω∑®£¨0-µÁ∆Ω£¨1-STA/LTA£¨2-µÁ∆Ω+STA/LTA
	short filt_id;//¬À≤®∆˜¥˙¬Î
	short lwin;//≥§¥∞÷µ,µ•Œª£∫0.1√Î
	short swin;//∂Ã¥∞÷µ£¨µ•Œª£∫0.1√Î
	short vwin;//–£—È¥∞÷µ£¨µ•Œª£∫0.1√Î
	short ewin;//Ω· ¯¥∞÷µ£¨µ•Œª£∫0.1√Î
	short td;//	STA/LTA¥•∑¢„–÷µ
	short vd;//	STA/LTA¥•∑¢—È÷§„–÷µ
	short te;//STA/LTAΩ· ¯„–÷µ
	short ld;//µÁ∆Ω¥•∑¢„–÷µ
	short le;//µÁ∆Ω¥•∑¢Ω· ¯„–÷µ
	short ud_w;//¥π÷±œÚÕ∂∆±»®÷ÿ
	short ew_w;//∂´Œ˜œÚÕ∂∆±»®÷ÿ
	short ns_w;//±±ƒœœÚÕ∂∆±»®÷ÿ
	short total_w;//¥•∑¢◊‹∆± ˝
	short trig_len;//◊Ó¥Û¥•∑¢≥§∂»£¨µ•Œª£∫√Î
}AUTOTRIG;

//◊‘∂Ø¥•∑¢≤Œ ˝
typedef struct tagCM_AUTOTRIGFRM
{
	CMDHEAD head;
	AUTOTRIG autotrig[6];
	short chk_sum;//–£—È∫Õ
}CM_AUTOTRIGFRM;


//◊€∫œ≈–∂®
typedef struct tagCM_DETERMINEVTFRM
{
	CMDHEAD head; 
	short method;//	¥•∑¢≈–∂®∑Ω∑®£∫1-◊ˆ◊€∫œ≈–∂®£¨0-Ωˆ◊˜µÿ’º∆≈–∂®
	short trig_num;//¥•∑¢µÿ’º∆ ˝,∑Ω∑®1 ±”––ß£¨¥•∑¢µÿ’º∆ ˝
	short trig_win;//¥•∑¢≈–∂® ±º‰¥∞	,∑Ω∑®1 ±”––ß£¨¥•∑¢≈–∂® ±º‰¥∞£¨µ•Œª£∫√Î
	short end_num;//Ω· ¯¥•∑¢µÿ’º∆ ˝,∑Ω∑®1”––ß£¨Ω· ¯¥•∑¢µÿ’º∆ ˝
	short chk_sum;//–£—È∫Õ
}CM_DETERMINEVTFRM;

// ¬º˛¥•∑¢–≈œ¢÷°
typedef struct tagCM_EVTTRIGFRM
{
	CMDHEAD head;
	long trig_time;//¥•∑¢ ±øÃ,√Î≤ø∑÷
	short frac_sec;//¥•∑¢ ±øÃ,∫¡√Î≤ø∑÷
	short chk_sum;
}CM_EVTTRIGFRM;

// ¬º˛¥•∑¢Ω· ¯÷°
typedef struct tagCM_EVTDETRIGFRM
{
	CMDHEAD head; 
	long trig_time;//¥•∑¢ ±øÃ
	long detrig_time;//¥•∑¢Ω· ¯ ±øÃ
	short trigfrac_sec;//¥•∑¢ ±øÃ,∫¡√Î≤ø∑÷
	short detrigfrac_sec;//¥•∑¢Ω· ¯ ±øÃ,∫¡√Î≤ø∑÷
 	short chk_sum;
}CM_EVTDETRIGFRM;

//‘À––◊¥Ã¨–≈œ¢¥´ ‰º‰∏Ù

typedef struct tagCM_HEARTBEATINTFRM
{
	CMDHEAD head; 
	short interval;//¥´ ‰º‰∏Ù,>30µ•Œª:√Î
	short chk_sum;//–£—È∫Õ
}CM_HEARTBEATINTFRM;

//µÁ—π÷°
#ifdef EDAS_IML//¡“∂»“«

typedef struct tagCM_VOLFRM
{
	CMDHEAD head; 
	short ext_vol;//Õ‚≤ø,0.1v
	short net_vol;//Õ¯¬Áπ©µÁµÁ—π,0.1v
	short stor_vol;//µÁ≥ÿ,0.1v
	short mon_vol;//Õ‚≤øπ©µÁ◊¥Ã¨º‡≤‚µÁ—π,0.1V
	short chk_sum;
}CM_VOLFRM;
#else
typedef struct tagCM_VOLFRM
{
	CMDHEAD head; 
	short out_vol;//Õ‚≤ø,0.1v
	short in_vol;//ƒ⁄≤ø,0.1v
	short stor_vol;//µÁ≥ÿ,0.1v
	short chk_sum;
}CM_VOLFRM;

#endif

//Œ¬∂»÷°
typedef struct tagCM_TEMPFRM
{
	CMDHEAD head; 
	short temperature;//Œ¬∂»£¨0.1…„ œ∂»
	short chk_sum;
}CM_TEMPFRM;

//÷”≤Ó÷°
typedef struct tagCM_CLKERRFRM
{
	CMDHEAD head;
	short clk_err;//÷”≤Ó
	short freq_err;//∆µ≤Ó,0.01PPM
	short chk_sum;
}CM_CLKERRFRM;

//µÁ≥ÿ≥‰µÁ÷°
typedef struct tagCM_BATCHARGEFRM
{
	CMDHEAD head; 
	long time;//∆Ù∂Ø/Õ£÷π≥‰µÁ ±º‰
	int vol;//≥‰µÁ«∞/∫ÛµÁ≥ÿµÁ—π,0.1v
	short bstop;//1-≤È—ØÕ£÷π≥‰µÁ≤Œ ˝ ,0-≤È—Ø≥‰µÁ∆Ù∂Ø≤Œ ˝
	short chk_sum;
}CM_BATCHARGEFRM;

//≤È—Ø‘À––◊¥Ã¨–≈œ¢÷°
#ifdef EDAS_IML//¡“∂»“«

typedef struct tagCM_HEARTBEATFRM
{
	CMDHEAD head; 
	short ext_vol;//0.1v
	short net_vol;//0.1v
	short stor_vol;//0.1v
	short mon_vol;//0.1v
	short temperature;//0.1C
	short clk_err;//Œ¢√Î
	short freq_err;//0.01 ppm
	short unused;
	long buf_total;//¥Ê¥¢∆˜◊‹ø’º‰,kb
	long buf_free;//¥Ê¥¢∆˜ø’œ–ø’º‰£¨kb
	short chk_sum;
}CM_HEARTBEATFRM;

#else
typedef struct tagCM_HEARTBEATFRM
{
	CMDHEAD head; 
	short out_vol;//0.1v
	short in_vol;//0.1v
	short stor_vol;//0.1v
    short mon_vol;
	short temperature;//0.1C
	short clk_err;//Œ¢√Î
	short freq_err;//0.01 ppm
    short unused;
	long buf_total;//¥Ê¥¢∆˜◊‹ø’º‰,kb
	long buf_free;//¥Ê¥¢∆˜ø’œ–ø’º‰£¨kb
	short chk_sum;
}CM_HEARTBEATFRM;
#endif
//GPS◊¥Ã¨
typedef struct tagCM_GPSSTATFRM
{
	CMDHEAD head; 
	short bgps;//1-∂‘÷”£¨0-∑«∂‘÷”
	short chk_sum;
}CM_GPSSTATFRM;

//IRIG¬Î–≈œ¢
typedef struct tagCM_GPSFRM
{
	CMDHEAD head; 
	long time;// ±º‰¬Î,¥”GMT 1970-01-01 00:00:00∆À„µƒ√Î ˝
	long lat;//Œ≥∂»,µ•Œª£∫ma=1/3600000∂»
	long lon;//æ≠∂»,µ•Œª£∫mas=1/3600000∂»
	long alt;//∏ﬂ≥Ã,µ•Œª£∫0.1M
	short dop;
 	short chk_sum;
}CM_IRIGFRM;

//¥Ê¥¢∆˜◊¥Ã¨
typedef struct tagCM_HDSTATFRM
{
	CMDHEAD head; 
	short id;//¥Ê¥¢∆˜–Ú∫≈
	short be_curmass;//1-µ±«∞¥Ê¥¢∆˜£¨0-≤ª «µ±«∞¥Ê¥¢∆˜
	long total;//◊‹ø’º‰,µ•Œª:Kb
	long free;//ø…”√ø’º‰,µ•Œª:Kb
	long io_error;//◊Ó∫Û1¥ŒI/O¥ÌŒÛ ±º‰
 	short chk_sum;//–£—È∫Õ
}CM_HDSTATFRM;
  

//≤È—Øµÿ’º∆¡„Œª
typedef struct tagCM_SENSROFFFRM
{
	CMDHEAD head; 
	long ch1;//1Õ®µ¿
	long ch2;//2Õ®µ¿
	long ch3;//3Õ®µ¿
	short chk_sum;
}CM_SENSROFFFRM;

//º«¬ºøÿ÷∆
typedef struct tagCM_MASSWRCTLFRM
{
	CMDHEAD head; 
	short ctl;//1-∆Ù∂ØœÚ¥Ê¥¢∆˜–¥ ˝æ›£¨0-≤ª–¥ ˝æ›
	short chk_sum;//–£—È∫Õ
}CM_MASSWRCTLFRM;

//«Âø’¥Ê¥¢∆˜
typedef struct tagCM_CLEARSDFRM
{
	CMDHEAD head; 
	short id;//¥Ê¥¢∆˜–Ú∫≈,¥”1ø™ º
	short chk_sum;//–£—È∫Õ
}CM_CLEARSDFRM;

//Œƒº˛±∏∑›πÊ‘Ú
typedef struct tagCM_BACKRULEFRM
{
	CMDHEAD head; 
	//…æ≥˝ ±º‰∂Œ,-1£∫≤ª◊ˆŒ¨ª§£¨‘⁄¥Ê¥¢∆˜’º”√ø’º‰¥ÔµΩ95% ±£¨∑¢≥ˆ±®æØ–≈œ¢ 
	//0£∫∞¥ø’º‰Œ¨ª§£¨”≤≈Ã’º”√ø’º‰¥ÔµΩ95% ±£¨∑¢≥ˆ±®æØ–≈œ¢£¨≤¢¥”◊Ó‘Á“ªÃÏµƒ¡¨–¯Œƒº˛ø™ º…æ≥˝£¨÷±µΩ’º”√ø’º‰–°”⁄70%
	//>0£∫∞¥ ±º‰Œ¨ª§£¨¥Ê¥¢∆˜÷–¡¨–¯Œƒº˛±£¥ÊµƒÃÏ ˝£¨»Áπ˚π˝∆⁄‘Ú…æ≥˝
 	short ctn_ttl;
 	short chk_sum;//–£—È∫Õ
}CM_BACKRULEFRM;


//IP°¢Õ¯πÿ
typedef struct tagCM_IPGWFRM
{
	CMDHEAD head; 
	char ip[44];//µÿ÷∑
	char netmask[44];//—⁄¬Î
	char gate[44];//Õ¯πÿ
	short chk_sum;//–£—È∫Õ
}CM_IPGWFRM;

//…Ë÷√œµÕ≥ ±º‰÷°
typedef struct tagCM_SYSTIMEFRM
{
	CMDHEAD head; 
	long sec;//¥”1970ø™ º√Î ˝
	short chk_sum;
}CM_SYSTIMEFRM;


//…Ë÷√ ±«¯÷°
typedef struct tagCM_TIMEZONEFRM
{
	CMDHEAD head; 
	char zone[10];// ±«¯√˚
	short offset;//”ÎGMT ±«¯∆´≤Ó
	short chk_sum;
}CM_TIMEZONEFRM;

//GPS∂‘÷”º‰∏Ù
#ifdef EDAS_IML//¡“∂»“«

typedef struct tagCM_GPSINTFRM{
	CMDHEAD head; 
	short interval;//1-60∑÷,-1:¡¨–¯∂‘÷”
	short type;//∂‘÷”∑Ω Ω£¨2-IRIG¬Î£¨1-ƒ⁄≤øGPS 3-Õ¯¬Á ⁄ ±
	short chk_sum;
}CM_GPSINTFRM;
 
#else
typedef struct tagCM_GPSINTFRM{
	CMDHEAD head; 
	short interval;//1-60∑÷,-1:¡¨–¯∂‘÷”
	short chk_sum;
}CM_GPSINTFRM;
#endif

//NTP ±º‰∑˛ŒÒ∆˜
//NTP ±º‰∑˛ŒÒ∆˜
typedef struct tagCM_NTPSRVFRM
{
	CMDHEAD head;
	char host1[20];//NTP∑˛ŒÒ∆˜÷˜µÿ÷∑
	char host2[20];//NTP∑˛ŒÒ∆˜±∏”√µÿ÷∑
	char host3[20];//NTP∑˛ŒÒ∆˜±∏”√µÿ÷∑
	short chk_sum;

}CM_NTPSRVFRM;
//∂Ã–≈∫≈¬Î
typedef struct tagCM_SMSCODEFRM
{
	CMDHEAD head;
	short type;//1-“«∆˜◊¥Ã¨±®æØ£¨2-µÿ’–≈œ¢±®æØ
	short total;//∫≈¬Î◊‹ ˝£¨◊Ó∂‡20∏ˆ
	char phone[20][12];//∂Ã–≈Ω” ’»À∫≈¬Î
	short chk_sum;

}CM_SMSCODEFRM;
//µÿ’º∆øÿ÷∆–≈∫≈
typedef struct tagCM_SENSSIGCTLFRM
{
	CMDHEAD head; 
 	int flag;//±Í÷æ 1-∑«∏€’µÿ’º∆£¨”–øÿ÷∆–≈∫≈, 0- √ª”–øÿ÷∆–≈∫≈,2 - BBVS-60/120øÿ÷∆
	int unlock_sig;//ø™∞⁄–≈∫≈
	int lock_sig;//À¯∞⁄–≈∫≈
	int adj_sig;//µ˜¡„–≈∫≈
	int cal_sig;//±Í∂®–≈∫≈
	int stopcal_sig;//Õ£÷π±Í∂®–≈∫≈
	int unlock_sig_init;//ø™∞⁄–≈∫≈≥ı ºªØ÷µ
	int lock_sig_init;//À¯∞⁄–≈∫≈≥ı ºªØ÷µ
	int adj_sig_init;//µ˜¡„–≈∫≈≥ı ºªØ÷µ
	int cal_sig_init;//±Í∂®–≈∫≈≥ı ºªØ÷µ
	int stopcal_sig_init;//Õ£÷π±Í∂®–≈∫≈≥ı ºªØ÷µ

	short chk_sum;

}CM_SENSSIGCTLFRM;

////¡“∂»–≈œ¢÷°£∫
//¡“∂»º∆À„Ω·π˚
typedef struct tagCM_SMALDFRM
{
	CMDHEAD head; 
 	char name[8];//Ã®√˚
	long trig_time;//¥•∑¢ ±º‰ 	¥”GMT 1970-01-01 00:00:00∆À„µƒ√Î ˝ 
	int frac_time;//¥•∑¢ ±º‰,∫¡√Î≤ø∑÷
	float ld;//	¡“∂»
	float pga_u;//PGA¥π÷±œÚ,µ•Œª£∫gal
	float	pga_e;//PGA∂´Œ˜œÚ,	µ•Œª£∫gal
	float pga_n;//	PGAƒœ±±œÚ,	µ•Œª£∫gal
	float rsp_1;//1Hz∑¥”¶∆◊,	µ•Œª£∫gal
	float rsp_2;//2Hz∑¥”¶∆◊,	µ•Œª£∫gal
	float rsp_5;//5Hz∑¥”¶∆◊,	µ•Œª£∫gal
	float rsp_8;//8Hz∑¥”¶∆◊,	µ•Œª£∫gal
	float fp;//◊ø‘Ω÷‹∆⁄,µ•Œª£∫hz
	float td;//≥÷ ±,µ•Œª£∫√Î
	float  pgv_e;//PGV∂´Œ˜œÚ,µ•Œª£∫cm/s
	float pgv_n;//PGV±±ƒœœÚ,µ•Œª£∫cm/s
	short chk_sum;
}CM_SMALDFRM;

//‘÷∫¶∆¿π¿÷µ£®Œ‚£©
typedef struct tagCM_TAQPDFRM
{
	CMDHEAD head; 
	char name[8];//Ã®√˚
	long trig_time;//¥•∑¢ ±º‰ 	¥”GMT 1970-01-01 00:00:00∆À„µƒ√Î ˝ 
	int frac_time;//¥•∑¢ ±º‰,∫¡√Î≤ø∑÷
	float tao_c;
	float pd;//µ•Œª£∫cm
	short chk_sum;//CRC-16–£—È

}CM_TAQPDFRM;


// ¬º˛º«¬º
typedef struct tagCM_EVTRECFRM
{
	CMDHEAD head;
	short be_wr;//1-º«¬º
	short pre_evt;// ¬º˛«∞º«¬º≥§∂»£¨√Î
	short aft_evt;// ¬º˛∫Ûº«¬º≥§∂»£¨√Î
	short chk_sum;// –£—È
}CM_EVTRECFRM;

typedef struct tagCM_EVTRECFILE
{
    CMDHEAD head;
    short file_sum;
}CM_EVTRECFILE;

//¥ÌŒÛ÷°
typedef struct tagCM_ERRORFRM
{
	CMDHEAD head; 
	//1-≤…—˘¬ …Ë÷√£¨2-¡ø≥Ã…Ë÷√£¨3-µÿ’º∆øÿ÷∆£¨4-µ˜µÿ’º∆¡„µ„£¨5-¥Ê¥¢∆˜ø’º‰≥¨π˝80%£¨6-¥Ê¥¢∆˜ø’º‰≥¨π˝70%£¨7-¥Ê¥¢∆˜I/O¥ÌŒÛ£¨8-≥ˆœ÷µÁ—π¥ÌŒÛ,9-Œ¬∂»¥ÌŒÛ,10-µÁ≥ÿ≥‰µÁ,11-GPS¥ÌŒÛ 
	short error;
	short chk_sum;
}CM_ERRORFRM ;
  
#endif