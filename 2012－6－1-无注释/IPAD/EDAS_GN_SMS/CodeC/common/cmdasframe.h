#ifndef _cmdasframe_h
#define _cmdasframe_h

/*************************************************************************************
/*����: ͨ�����ݲɼ���ʵʱ���ݷ���,�������ݷ���,�������ͷ��������֡��ʽ
/*���ߣ�����
/*create date: 2009-9-10
/*update date:
/*	date			action				common
/*	2009-9-24		add					CM_HDSTATFRM:����be_curmass
/*  2009-11-3		edit				CM_DASOFFFRM:�޸ĸ�ʽ
/*  2009-11-4		edit				CMHEAD: cmd��Ϊunsigned short
/*  2010-2-2		CM_PULSEFRM			���ݲ���֡����1.4�޸ĸ�ʽ
					CM_SINEFRM
					CM_PSEUDOFRM		
/*  2010-3-4		CM_SENSSIGCTLFRM	���ݲ���֡����1.5,���ӵ���ƿ����ź�
/*  2010-7-8		CM_SENSSIGCTLFRM	���ݲ���֡����1.6,�޸ĵ���ƿ����ź�
/*  2010-7-9		CM_EVTTRIGFRM
					CM_EVTDETRIGFRM
	2010-9-14		CM_SENSPARFRM       �����Ҷ��ǲ���֡����1.1���Ӻ궨������������EDAS-24GN���������Ҷ���
					CM_HEARTBEATFRM
					CM_VOLFRM
					CM_GPSINTFRM
 				
	2010-11-3		CM_NTPSRVFRM		�����Ҷ��ǲ���֡����1.2
					CM_SMSCODEFRM
/*  2011-9-18		CM_SENSSIGCTLFRM	���ݲ���֡����1.7,�޸ĵ���ƿ����ź�

 /*company: geodevice
/***********************************************************************************/
#include "iecomplex.h"



//֡ͷ��
typedef struct tagCMDHEAD
{
	short sens_id;//����ƺ�
	unsigned short cmd;//֡���
	short length;//֡����
	short reserved;//����
}CMDHEAD;

//Ӧ��֡
typedef struct tagACKFRAME
{
	CMDHEAD head;//0x7777
	long time;//ʱ����
	short chk_sum;
}ACKFRAME;


//��������֡
typedef struct tagASKDATAFRM
{
	CMDHEAD head;
	long time;//������ʼʱ��,1:�ӿ��ܵ��������ݿ�ʼ��2:�ӿ��ܵ��������ݿ�ʼ��3~8��������δ���壻9:ֹͣ����������������ָ��ʱ������룬��ָ��ʱ���������ȫ�����ڣ��ṩȫ�����ݣ����ִ��ڣ��ṩ�������ݣ������ڣ����ݷ��񲻻ᱻ���
	short len;//���ݳ���,-1��������
	short chk_sum;
}ASKDATAFRM;

//̨վ����֡
typedef struct tagCM_STNPARFRM
{
	CMDHEAD head;
	int id;//̨��
	char name[32];//̨��
	char abbr[8];//̨����д
	short sens_sum;//�������
	short chn_sum;//ͨ����
	long lat;//γ��,��λ��masΪ(-90.0~90.0)*3600000
	long lon;//����,��λ��masΪ(-180.0~180.0)*3600000
	long alt;//�̣߳���λ��cm
	char run_date[32];//����ʱ��
	char netid[4];//�������
	char version[32];//�汾
	short chk_sum;
}CM_STNPARFRM;

//����Ʋ���֡ 
#ifdef EDAS_IML//�Ҷ���
typedef struct tagCM_SENSPARFRM
{
	CMDHEAD head;
	char name[16];//�ͺ�
	char provider[32];//������
	char series[16];//���к�
	char rectype[4];//���� D: λ���� V: �ٶ��� A: ���ٶ���
	char type[12];//��׼����,������ҵ��׼
	char bandstr[16];//����
	long depth;//����,cm
	char rock[32];//�һ�
	float azim;//��λ��
	char rspfile[32];//��Ӧ�ļ�
	long comp;//������
	int das_id[3];//�����Ӧ��A/Dͨ����ţ���1��ʼ����Ϊ����A/Dͨ��1-6������1-6
	char cxid[3][8];//����DB/T3-2003 [���𼰵���ǰ�ײ�����༰����]��׼
	short chk_sum;
}CM_SENSPARFRM;

#else
typedef struct tagCM_SENSPARFRM
{
	CMDHEAD head;
	char name[16];//�ͺ�
	char provider[32];//������
	char series[16];//���к�
	char rectype[4];//���� D: λ���� V: �ٶ��� A: ���ٶ���
	char type[12];//��׼����,������ҵ��׼
	char bandstr[16];//����
	long depth;//����,cm
	char rock[32];//�һ�
	char state[4];//״̬��GC
	char rspfile[32];//��Ӧ�ļ�
	long comp;//������
	int das_id[3];//�����Ӧ��A/Dͨ����ţ���1��ʼ����Ϊ����A/Dͨ��1-6������1-6
	char cxid[3][8];//����DB/T3-2003 [���𼰵���ǰ�ײ�����༰����]��׼
	short chk_sum;
}CM_SENSPARFRM;
#endif

typedef struct tagCM_INSTPARFRM 
{
	CMDHEAD head; 
	float ao;//���ݺ���ϵ��
	float ud;//��ֱ��������
	float ew;//������������
	float ns;//������������
	short pnum;//������
	short znum;//�����
	iecomplex pole[25];//����
	iecomplex zero[25];//���
	float ud_damp;//��ֱ������
	float ew_damp;//����������
	float ns_damp;//����������
	float ud_freq;//��ֱ����������,��λ��HZ
	float ew_freq;//��������������,��λ��HZ
	float ns_freq;//��������������,��λ��HZ
	short chk_sum;//У���
}CM_INSTPARFRM;

//������֡	
typedef struct tagCM_SMPRATEFRM
{
	CMDHEAD head; 
	char phase_id;//��λ����1: ������λ  2����С��λ
	char samp_id;//�����ʴ���
	char be_ad;//���ڶ����ʱ��0-�رղ�����1-��������
	char be_syssamp;//���ڶ����ʱ��0-����ϵͳ�����ʣ�1-��ϵͳ������
	short chk_sum;
}CM_SMPRATEFRM;
 
//����֡
typedef struct tagCM_GAINFRM
{
	CMDHEAD head; 
	short gain_id;//���̴��� 0:10V�� 1��20V, 2: 5V 3��2.5V
	short chk_sum;
}CM_GAINFRM;
 
//�����ź��������ֵ
typedef struct tagCM_DASOFFFRM
{
	CMDHEAD head;
 	long ud;//��ֱ
	long ew;//����
	long ns;//����
	short chk_sum;
}CM_DASOFFFRM;

//����궨֡
typedef struct tagCM_PULSEFRM
{
	CMDHEAD head; 
	short btimer;//1-��ʱ
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	long time;//��һ������ʱ��
	short amptype;//1-����Ϊ����������λuA��0-countֵ
	short amp;//����
	unsigned short dur;//��������0.1��
	short chk_sum;
}CM_PULSEFRM;


//���ұ궨����
typedef struct tagSINES
{
	short cycle;//�ź�������
	short prd;//Ƶ�ʻ�����,����ʾ���ڣ���λ��0.1S��Ϊ����ʾƵ�ʣ���λ��0.1Hz��
	short aten;//˥������/���Ҳ������ֵ
}SINES;
//���ұ궨֡
typedef struct tagCM_SINEFRM{
	CMDHEAD head;//0x1021
	short btimer;//1-��ʱ����
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	long time;//��һ������ʱ��
	short amptype;//����,1-sine[i].atenΪ���Ҳ������ֵ����λuA��0-˥������
	short prd_num;//Ƶ����
	SINES sine[50];//���Ҳ�Ƶ���������prd_num��
	short chk_sum;
}CM_SINEFRM;


//�����֡
typedef struct tagCM_PSEUDOFRM
{
	CMDHEAD head;//0x1022
	short btimer;//1-��ʱ����
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	long time;//��һ������ʱ��
	short amptype;//1-����Ϊ����������λuA��0-countֵ
	short amp;//����
	unsigned short dur;//��Ԫ�����0.001��
	short signal_flag;//�źű�־
	short rep_num;//�ظ�����
	short chk_sum;
}CM_PSEUDOFRM;

//ǿ��궨֡
typedef struct tagCM_STRONGFRM
{
	CMDHEAD head;//0x1023
	short btimer;//1-��ʱ
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	long time;//��һ������ʱ��
	short chk_sum;
}CM_STRONGFRM;

//�궨�ȴ�ʱ��
typedef struct tagCM_CALIWAITFRM
{
	CMDHEAD head;//0x1029
	short time;//�ȴ�ʱ�䣬��
	short chk_sum;
}CM_CALIWAITFRM;

//�����궨
typedef struct tagCM_STARTCALFRM
{
	CMDHEAD head;//cmd=0x6024: ��������궨 0x6025: �������ұ궨 0x6026: ��������궨 0x6027: ����ǿ��궨
	long time;//--1:�����ֹ������궨, ʵ�ʵ�����ʱ���������궨������ݵ�ǰ��������ʱ������,>=0�����ڶ�ʱ�����궨��Ϊ��Ч������ʱ�� 
	short chk_sum;
}CM_STARTCALFRM;

//����Ƽ�����
typedef struct tagCM_SENSCTLFRM
{
	CMDHEAD head;
	short bctl;//0:����, 1:����
	short chk_sum;
}CM_SENSCTLFRM;

//ʵʱ�������ݷ���,�������ݷ���,��ʵʱ�������������Ʋ��� 
typedef struct tagCM_NETSRVCTLFRM
{
	CMDHEAD head; 
	int port;//���ƶ˿� 
	int dataport;//��ʼ�����ݶ˿ں�
	short tm_ctl;//��ʱ���ƣ���λ����
	short max_con;//���ͻ�����Ĭ�ϣ�4
	short chk_sum;//У���
}CM_NETSRVCTLFRM;

//��������ͻ�����
typedef struct tagCLIENTCONPAR
{
	short id;
	short bon;//1-����,0-������
	short broot;//1-����Ա��ݣ�0-�ǹ���Ա���
	short bevent;//���͵��������ͣ�1-�¼����Σ�0-�������Σ�2-�Ҷ�(����) 
	char host[44];//��������ַ
	long port;//�������˿�,����5000������
	char sens[16];//����Ʊ�־
	short pre_evt;//�¼�ǰ���ȣ���
	short aft_evt;//�¼��󳤶ȣ���
	short tm_ctl;//��ʱ���ƣ���λ����
	short be_broad;//���͹㲥֡,1-���ͣ�0-�����ͣ�Ĭ�ϣ�
	short be_stat;//����״̬֡,1-���ͣ�0-�����ͣ�Ĭ�ϣ�
	short unused;//����
}CLIENTCONPAR;

//��������
typedef struct tagCM_CLIENTCONFRM
{
	CMDHEAD head; 
	CLIENTCONPAR client[4];
	short chk_sum;
}CM_CLIENTCONFRM;

//���ڲ������޸Ĳ�����
typedef struct tagCM_COMBAUDFRM
{
	CMDHEAD head; 
	long baud;//������,4800,9600,19200,38400,57600,115200	
 	short chk_sum;//У���
}CM_COMBAUDFRM;

//���ڲ��������Ʒ��ͷ�ʽ���߼����ã�
typedef struct tagCM_COMCTLFRM
{
	CMDHEAD head;//0x1033
	short back;//1-Ӧ��ʽ��0-��Ӧ��
	short send;//1-ϵͳ������ֱ�ӷ��Ͳ��Σ�0-�ȴ��������
	short be_broad;//���͹㲥֡,1-���ͣ�0-�����ͣ�Ĭ�ϣ�
	short be_stat;//����״̬֡,1-���ͣ�0-�����ͣ�Ĭ�ϣ�
	short chk_sum;//У���
}CM_COMCTLFRM;

//���������
typedef struct tagAUTOTRIG{
	short flag;//�������־,����ʱ��1-�޸Ĵ���������,-1 ɾ��������,99����ʾ׷��һ�������������أ�1-��Ч��������0-��Ч������
	short trig_id;//��������,��0��ʼ
	short method;//��ⷽ��,��ⷽ����0-��ƽ��1-STA/LTA��2-��ƽ+STA/LTA
	short filt_id;//�˲�������
	short lwin;//����ֵ,��λ��0.1��
	short swin;//�̴�ֵ����λ��0.1��
	short vwin;//У�鴰ֵ����λ��0.1��
	short ewin;//������ֵ����λ��0.1��
	short td;//	STA/LTA������ֵ
	short vd;//	STA/LTA������֤��ֵ
	short te;//STA/LTA������ֵ
	short ld;//��ƽ������ֵ
	short le;//��ƽ����������ֵ
	short ud_w;//��ֱ��ͶƱȨ��
	short ew_w;//������ͶƱȨ��
	short ns_w;//������ͶƱȨ��
	short total_w;//������Ʊ��
	short trig_len;//��󴥷����ȣ���λ����
}AUTOTRIG;

//�Զ���������
typedef struct tagCM_AUTOTRIGFRM
{
	CMDHEAD head;
	AUTOTRIG autotrig[6];
	short chk_sum;//У���
}CM_AUTOTRIGFRM;


//�ۺ��ж�
typedef struct tagCM_DETERMINEVTFRM
{
	CMDHEAD head; 
	short method;//	�����ж�������1-���ۺ��ж���0-����������ж�
	short trig_num;//�����������,����1ʱ��Ч�������������
	short trig_win;//�����ж�ʱ�䴰	,����1ʱ��Ч�������ж�ʱ�䴰����λ����
	short end_num;//���������������,����1��Ч�����������������
	short chk_sum;//У���
}CM_DETERMINEVTFRM;

//�¼�������Ϣ֡
typedef struct tagCM_EVTTRIGFRM
{
	CMDHEAD head;
	long trig_time;//����ʱ��,�벿��
	short frac_sec;//����ʱ��,���벿��
	short chk_sum;
}CM_EVTTRIGFRM;

//�¼���������֡
typedef struct tagCM_EVTDETRIGFRM
{
	CMDHEAD head; 
	long trig_time;//����ʱ��
	long detrig_time;//��������ʱ��
	short trigfrac_sec;//����ʱ��,���벿��
	short detrigfrac_sec;//��������ʱ��,���벿��
 	short chk_sum;
}CM_EVTDETRIGFRM;

//����״̬��Ϣ������

typedef struct tagCM_HEARTBEATINTFRM
{
	CMDHEAD head; 
	short interval;//������,>30��λ:��
	short chk_sum;//У���
}CM_HEARTBEATINTFRM;

//��ѹ֡
#ifdef EDAS_IML//�Ҷ���

typedef struct tagCM_VOLFRM
{
	CMDHEAD head; 
	short ext_vol;//�ⲿ,0.1v
	short net_vol;//���繩���ѹ,0.1v
	short stor_vol;//���,0.1v
	short mon_vol;//�ⲿ����״̬����ѹ,0.1V
	short chk_sum;
}CM_VOLFRM;
#else
typedef struct tagCM_VOLFRM
{
	CMDHEAD head; 
	short out_vol;//�ⲿ,0.1v
	short in_vol;//�ڲ�,0.1v
	short stor_vol;//���,0.1v
	short chk_sum;
}CM_VOLFRM;

#endif

//�¶�֡
typedef struct tagCM_TEMPFRM
{
	CMDHEAD head; 
	short temperature;//�¶ȣ�0.1���϶�
	short chk_sum;
}CM_TEMPFRM;

//�Ӳ�֡
typedef struct tagCM_CLKERRFRM
{
	CMDHEAD head;
	short clk_err;//�Ӳ�
	short freq_err;//Ƶ��,0.01PPM
	short chk_sum;
}CM_CLKERRFRM;

//��س��֡
typedef struct tagCM_BATCHARGEFRM
{
	CMDHEAD head; 
	long time;//����/ֹͣ���ʱ��
	int vol;//���ǰ/���ص�ѹ,0.1v
	short bstop;//1-��ѯֹͣ������ ,0-��ѯ�����������
	short chk_sum;
}CM_BATCHARGEFRM;

//��ѯ����״̬��Ϣ֡
#ifdef EDAS_IML//�Ҷ���

typedef struct tagCM_HEARTBEATFRM
{
	CMDHEAD head; 
	short ext_vol;//0.1v
	short net_vol;//0.1v
	short stor_vol;//0.1v
	short mon_vol;//0.1v
	short temperature;//0.1C
	short clk_err;//΢��
	short freq_err;//0.01 ppm
	short unused;
	long buf_total;//�洢���ܿռ�,kb
	long buf_free;//�洢�����пռ䣬kb
	short chk_sum;
}CM_HEARTBEATFRM;

#else
typedef struct tagCM_HEARTBEATFRM
{
	CMDHEAD head; 
	short out_vol;//0.1v
	short in_vol;//0.1v
	short stor_vol;//0.1v
	short temperature;//0.1C
	short clk_err;//΢��
	short freq_err;//0.01 ppm
	long buf_total;//�洢���ܿռ�,kb
	long buf_free;//�洢�����пռ䣬kb
	short chk_sum;
}CM_HEARTBEATFRM;
#endif
//GPS״̬
typedef struct tagCM_GPSSTATFRM
{
	CMDHEAD head; 
	short bgps;//1-���ӣ�0-�Ƕ���
	short chk_sum;
}CM_GPSSTATFRM;

//IRIG����Ϣ
typedef struct tagCM_GPSFRM
{
	CMDHEAD head; 
	long time;//ʱ����,��GMT 1970-01-01 00:00:00���������
	long lat;//γ��,��λ��ma=1/3600000��
	long lon;//����,��λ��mas=1/3600000��
	long alt;//�߳�,��λ��0.1M
	short dop;
 	short chk_sum;
}CM_IRIGFRM;

//�洢��״̬
typedef struct tagCM_HDSTATFRM
{
	CMDHEAD head; 
	short id;//�洢�����
	short be_curmass;//1-��ǰ�洢����0-���ǵ�ǰ�洢��
	long total;//�ܿռ�,��λ:Kb
	long free;//���ÿռ�,��λ:Kb
	long io_error;//���1��I/O����ʱ��
 	short chk_sum;//У���
}CM_HDSTATFRM;
  

//��ѯ�������λ
typedef struct tagCM_SENSROFFFRM
{
	CMDHEAD head; 
	long ch1;//1ͨ��
	long ch2;//2ͨ��
	long ch3;//3ͨ��
	short chk_sum;
}CM_SENSROFFFRM;

//��¼����
typedef struct tagCM_MASSWRCTLFRM
{
	CMDHEAD head; 
	short ctl;//1-������洢��д���ݣ�0-��д����
	short chk_sum;//У���
}CM_MASSWRCTLFRM;

//��մ洢��
typedef struct tagCM_CLEARSDFRM
{
	CMDHEAD head; 
	short id;//�洢�����,��1��ʼ
	short chk_sum;//У���
}CM_CLEARSDFRM;

//�ļ����ݹ���
typedef struct tagCM_BACKRULEFRM
{
	CMDHEAD head; 
	//ɾ��ʱ���,-1������ά�����ڴ洢��ռ�ÿռ�ﵽ95%ʱ������������Ϣ 
	//0�����ռ�ά����Ӳ��ռ�ÿռ�ﵽ95%ʱ������������Ϣ����������һ��������ļ���ʼɾ����ֱ��ռ�ÿռ�С��70%
	//>0����ʱ��ά�����洢���������ļ���������������������ɾ��
 	short ctn_ttl;
 	short chk_sum;//У���
}CM_BACKRULEFRM;


//IP������
typedef struct tagCM_IPGWFRM
{
	CMDHEAD head; 
	char ip[44];//��ַ
	char netmask[44];//����
	char gate[44];//����
	short chk_sum;//У���
}CM_IPGWFRM;


//����ϵͳʱ��֡
typedef struct tagCM_SYSTIMEFRM
{
	CMDHEAD head; 
	long sec;//��1970��ʼ����
	short chk_sum;
}CM_SYSTIMEFRM;


//����ʱ��֡
typedef struct tagCM_TIMEZONEFRM
{
	CMDHEAD head; 
	char zone[10];//ʱ����
	short offset;//��GMTʱ��ƫ��
	short chk_sum;
}CM_TIMEZONEFRM;

//GPS���Ӽ��
#ifdef EDAS_IML//�Ҷ���

typedef struct tagCM_GPSINTFRM{
	CMDHEAD head; 
	short interval;//1-60��,-1:��������
	short type;//���ӷ�ʽ��2-IRIG�룬1-�ڲ�GPS 3-������ʱ
	short chk_sum;
}CM_GPSINTFRM;
 
#else
typedef struct tagCM_GPSINTFRM{
	CMDHEAD head; 
	short interval;//1-60��,-1:��������
	short chk_sum;
}CM_GPSINTFRM;
#endif

//NTPʱ�������
//NTPʱ�������
typedef struct tagCM_NTPSRVFRM
{
	CMDHEAD head;
	char host1[20];//NTP����������ַ
	char host2[20];//NTP���������õ�ַ
	char host3[20];//NTP���������õ�ַ
	short chk_sum;

}CM_NTPSRVFRM;
//���ź���
typedef struct tagCM_SMSCODEFRM
{
	CMDHEAD head;
	short type;//1-����״̬������2-������Ϣ����
	short total;//�������������20��
	char phone[20][12];//���Ž����˺���
	short chk_sum;

}CM_SMSCODEFRM;
//����ƿ����ź�
typedef struct tagCM_SENSSIGCTLFRM
{
	CMDHEAD head; 
 	int flag;//��־ 1-�Ǹ������ƣ��п����ź�, 0- û�п����ź�,2 - BBVS-60/120����
	int unlock_sig;//�����ź�
	int lock_sig;//�����ź�
	int adj_sig;//�����ź�
	int cal_sig;//�궨�ź�
	int stopcal_sig;//ֹͣ�궨�ź�
	int unlock_sig_init;//�����źų�ʼ��ֵ
	int lock_sig_init;//�����źų�ʼ��ֵ
	int adj_sig_init;//�����źų�ʼ��ֵ
	int cal_sig_init;//�궨�źų�ʼ��ֵ
	int stopcal_sig_init;//ֹͣ�궨�źų�ʼ��ֵ

	short chk_sum;

}CM_SENSSIGCTLFRM;

////�Ҷ���Ϣ֡��
//�Ҷȼ�����
typedef struct tagCM_SMALDFRM
{
	CMDHEAD head; 
 	char name[8];//̨��
	long trig_time;//����ʱ�� 	��GMT 1970-01-01 00:00:00��������� 
	int frac_time;//����ʱ��,���벿��
	float ld;//	�Ҷ�
	float pga_u;//PGA��ֱ��,��λ��gal
	float	pga_e;//PGA������,	��λ��gal
	float pga_n;//	PGA�ϱ���,	��λ��gal
	float rsp_1;//1Hz��Ӧ��,	��λ��gal
	float rsp_2;//2Hz��Ӧ��,	��λ��gal
	float rsp_5;//5Hz��Ӧ��,	��λ��gal
	float rsp_8;//8Hz��Ӧ��,	��λ��gal
	float fp;//׿Խ����,��λ��hz
	float td;//��ʱ,��λ����
	float  pgv_e;//PGV������,��λ��cm/s
	float pgv_n;//PGV������,��λ��cm/s
	short chk_sum;
}CM_SMALDFRM;

//�ֺ�����ֵ���⣩
typedef struct tagCM_TAQPDFRM
{
	CMDHEAD head; 
	char name[8];//̨��
	long trig_time;//����ʱ�� 	��GMT 1970-01-01 00:00:00��������� 
	int frac_time;//����ʱ��,���벿��
	float tao_c;
	float pd;//��λ��cm
	short chk_sum;//CRC-16У��

}CM_TAQPDFRM;


//�¼���¼
typedef struct tagCM_EVTRECFRM
{
	CMDHEAD head;
	short be_wr;//1-��¼
	short pre_evt;//�¼�ǰ��¼���ȣ���
	short aft_evt;//�¼����¼���ȣ���
	short chk_sum;// У��
}CM_EVTRECFRM;

//����֡
typedef struct tagCM_ERRORFRM
{
	CMDHEAD head; 
	//1-���������ã�2-�������ã�3-����ƿ��ƣ�4-���������㣬5-�洢���ռ䳬��80%��6-�洢���ռ䳬��70%��7-�洢��I/O����8-���ֵ�ѹ����,9-�¶ȴ���,10-��س��,11-GPS���� 
	short error;
	short chk_sum;
}CM_ERRORFRM ;
  
#endif