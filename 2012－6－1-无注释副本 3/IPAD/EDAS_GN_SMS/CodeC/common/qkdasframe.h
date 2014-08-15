#ifndef _qkdasframe_h
#define _qkdasframe_h

/*************************************************************************************
/*����: ͨ�����ݲɼ���/�Ҷ��ǿ��ٷ��������֡��ʽ
/*���ߣ�����
/*create date: 2009-9-10
/*update date:
/*	date			action				common
/*	2009-9-24		QK_HDSTATFRM		����be_curmass
/*  2009-11-3		QK_DASOFFFRM		�޸ĸ�ʽ
/*  2009-11-27		���ݿ������ݷ������֡����1.3�޸ĸ�ʽ,�޶���������������
/*  2010-2-2		QK_PULSEFRM			���ݿ������ݷ������֡����1.4�޸ĸ�ʽ
					QK_SINEFRM
					QK_PSEUDOFRM
/*  2010-2-24		QK_SENSROFFFRM		��sens_idΪint(�������ݷ������֡����1.5)
/*  2010-3-4		QK_SENSSIGCTLFRM	���ӵ���ƿ����ź�
/*  2010-3-31		QK_SMALDFRM		�����Ҷȼ�����(�Ҷ�����)
					QK_TAOPDFRM		�����ֺ�����ֵ(�Ҷ�����)
/*  2010-7-8		QK_SENSSIGCTLFRM	�޸ĵ���ƿ����ź�
/*  2010-7-9		QK_SMALDFRM
					QK_TAOPDFRM
					QK_EVTTRIGFRM
					QK_EVTDETRIGFRM
	2010-9-14		QK_SENSPARFRM       �����Ҷ��ǿ������ݷ������֡����1.2���Ӻ궨������������EDAS-24GN���������Ҷ���
					QK_HEARTBEATFRM
					QK_VOLFRM
					QK_GPSINTFRM
	2010-11-3		QK_NTPSRVFRM		�����Ҷ��ǿ������ݷ������֡����1.3
					QK_SMSCODEFRM
/*  2011-9-18		QK_SENSSIGCTLFRM	�޸ĵ���ƿ����ź�
 /*company: geodevice
/***********************************************************************************/
#include "iecomplex.h"


//֡ͷ��
typedef struct tagQKHEAD
{
 	unsigned short cmd;//֡���
	unsigned short length;//֡����
}QKHEAD;



//̨վ����֡
typedef struct tagQK_STNPARFRM
{
	QKHEAD head;
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
	unsigned short chk_sum;
}QK_STNPARFRM;

//����Ʋ���֡ 
#ifdef EDAS_IML
typedef struct tagQK_SENSPARFRM
{
	QKHEAD head;
	int sens_id;//��������
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
	unsigned short chk_sum;
}QK_SENSPARFRM;
#else
typedef struct tagQK_SENSPARFRM
{
	QKHEAD head;
	int sens_id;//��������
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
	unsigned short chk_sum;
}QK_SENSPARFRM;
#endif

typedef struct tagQK_INSTPARFRM 
{
	QKHEAD head; 
	int sens_id;//��������
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
	unsigned short chk_sum;//У���
}QK_INSTPARFRM;

//������֡	
typedef struct tagQK_SMPRATEFRM
{
	QKHEAD head; 
	short sens_id;//��������
	char phase_id;//��λ����1: ������λ  2����С��λ
	char samp_id;//�����ʴ���
	char be_ad;//���ڶ����ʱ��0-�رղ�����1-��������
	char be_syssamp;//���ڶ����ʱ��0-����ϵͳ�����ʣ�1-��ϵͳ������
	unsigned short chk_sum;
}QK_SMPRATEFRM;
 
//����֡
typedef struct tagQK_GAINFRM
{
	QKHEAD head; 
	short sens_id;//��������
	short gain_id;//���̴��� 0:10V�� 1��20V, 2: 5V 3��2.5V
	unsigned short chk_sum;
}QK_GAINFRM;
 
//�����ź��������ֵ
typedef struct tagQK_DASOFFFRM
{
	QKHEAD head; 
	int sens_id;//��������
  	long ud;//��ֱ
	long ew;//����
	long ns;//����
	unsigned short chk_sum;
}QK_DASOFFFRM;

//����궨֡
typedef struct tagQK_PULSEFRM
{
	QKHEAD head; 
	short sens_id;//��������
	short btimer;//1-��ʱ
	long time;//��һ������ʱ��
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	short amptype;//1-����Ϊ����������λuA��0-countֵ
	short amp;//����
	unsigned short dur;//��������0.1��
	unsigned short chk_sum;
}QK_PULSEFRM;


//���ұ궨����
typedef struct tagQK_SINES
{
	short cycle;//�ź�������
	short prd;//Ƶ�ʻ�����,����ʾ���ڣ���λ��0.1S��Ϊ����ʾƵ�ʣ���λ��0.1Hz��
	short aten;//˥������/���Ҳ������ֵ
}QK_SINES;
//���ұ궨֡
typedef struct tagQK_SINEFRM{
	QKHEAD head;
	short sens_id;//��������
	short btimer;//1-��ʱ����
	long time;//��һ������ʱ��
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	short amptype;//����,1-sine[i].atenΪ���Ҳ������ֵ����λuA��0-˥������
	short prd_num;//Ƶ����
	QK_SINES sine[50];//���Ҳ�Ƶ���������prd_num��
	unsigned short chk_sum;
}QK_SINEFRM;


//�����֡
typedef struct tagQK_PSEUDOFRM
{
	QKHEAD head;
	short sens_id;//��������
	short btimer;//1-��ʱ����
	long time;//��һ������ʱ��
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	short amptype;//1-����Ϊ����������λuA��0-countֵ
	short amp;//����
	unsigned short dur;//��Ԫ�����0.001��
	short signal_flag;//�źű�־
	short rep_num;//�ظ�����
	unsigned short chk_sum;
}QK_PSEUDOFRM;

//ǿ��궨֡
typedef struct tagQK_STRONGFRM
{
	QKHEAD head;
	short sens_id;//��������
	short btimer;//1-��ʱ
	long time;//��һ������ʱ��
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
 	unsigned short chk_sum;
}QK_STRONGFRM;

//�궨�ȴ�ʱ��
typedef struct tagQK_CALIWAITFRM
{
	QKHEAD head;
	short sens_id;//��������
	short time;//�ȴ�ʱ�䣬��
	unsigned short chk_sum;
}QK_CALIWAITFRM;

//�����궨
typedef struct tagQK_STARTCALFRM
{
	QKHEAD head;//cmd=0xA024: ��������궨 0xA025: �������ұ궨 0xA026: ��������궨 0xA027: ����ǿ��궨
	int sens_id;//��������
	long time;//--1:�����ֹ������궨, ʵ�ʵ�����ʱ���������궨������ݵ�ǰ��������ʱ������,>=0�����ڶ�ʱ�����궨��Ϊ��Ч������ʱ�� 
	unsigned short chk_sum;
}QK_STARTCALFRM;

//ֹͣ�궨
typedef struct tagQK_STOPCALFRM
{
	QKHEAD head; 
	short sens_id;//��������
 	unsigned short chk_sum;
}QK_STOPCALFRM;

//����Ƶ���/ֹͣ����
typedef struct tagQK_SENSOFFFRM
{
	QKHEAD head;
	short sens_id;//��������
 	unsigned short chk_sum;
}QK_SENSOFFFRM;

//����Ƽ�����
typedef struct tagQK_SENSCTLFRM
{
	QKHEAD head;
	short sens_id;//��������
	short bctl;//0:����, 1:����
	unsigned short chk_sum;
}QK_SENSCTLFRM;

//����ƿ����ź�
typedef struct tagQK_SENSSIGCTLFRM
{
	QKHEAD head; 
	short sens_id;//����ƺ�
	short flag;//��־ 1-�Ǹ������ƣ��п����ź�, 0- û�п����ź�,2 - BBVS-60/120����
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

	unsigned short chk_sum;//У���

}QK_SENSSIGCTLFRM;

//ʵʱ�������ݷ���,�������ݷ���,��ʵʱ�������������Ʋ��� 
typedef struct tagQK_NETSRVCTLFRM
{
	QKHEAD head; 
	int port;//���ƶ˿� 
	int dataport;//��ʼ�����ݶ˿ں�
	short tm_ctl;//��ʱ���ƣ���λ����
	short max_con;//���ͻ�����Ĭ�ϣ�4
	unsigned short chk_sum;//У���
}QK_NETSRVCTLFRM;

//��������ͻ�����
typedef struct tagQK_CLIENTCONPAR
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
}QK_CLIENTCONPAR;

//��������
typedef struct tagQK_CLIENTCONFRM
{
	QKHEAD head; 
	QK_CLIENTCONPAR client[4];
	unsigned short chk_sum;
}QK_CLIENTCONFRM;

//���ڲ������޸Ĳ�����
typedef struct tagQK_COMBAUDFRM
{
	QKHEAD head; 
	long baud;//������,4800,9600,19200,38400,57600,115200	
 	unsigned short chk_sum;//У���
}QK_COMBAUDFRM;

//���ڲ��������Ʒ��ͷ�ʽ���߼����ã�
typedef struct tagQK_COMCTLFRM
{
	QKHEAD head;//0x1033
	short back;//1-Ӧ��ʽ��0-��Ӧ��
	short send;//1-ϵͳ������ֱ�ӷ��Ͳ��Σ�0-�ȴ��������
	short be_broad;//���͹㲥֡,1-���ͣ�0-�����ͣ�Ĭ�ϣ�
	short be_stat;//����״̬֡,1-���ͣ�0-�����ͣ�Ĭ�ϣ�
	unsigned short chk_sum;//У���
}QK_COMCTLFRM;

//���������
typedef struct tagQK_AUTOTRIG{
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
}QK_AUTOTRIG;

//�Զ���������
typedef struct tagQK_AUTOTRIGFRM
{
	QKHEAD head;
	int sens_id;//��������
	QK_AUTOTRIG autotrig[6];
	unsigned short chk_sum;//У���
}QK_AUTOTRIGFRM;


//�ۺ��ж�
typedef struct tagQK_DETERMINEVTFRM
{
	QKHEAD head; 
	short method;//	�����ж�������1-���ۺ��ж���0-����������ж�
	short trig_num;//�����������,����1ʱ��Ч�������������
	short trig_win;//�����ж�ʱ�䴰	,����1ʱ��Ч�������ж�ʱ�䴰����λ����
	short end_num;//���������������,����1��Ч�����������������
	unsigned short chk_sum;//У���
}QK_DETERMINEVTFRM;

//�¼�������Ϣ֡
typedef struct tagQK_EVTTRIGFRM
{
	QKHEAD head;
	int sens_id;//��������
	long trig_time;//����ʱ��,����
	short frac_sec;//����ʱ��,���벿��
	unsigned short chk_sum;
}QK_EVTTRIGFRM;

//�¼���������֡
typedef struct tagQK_EVTDETRIGFRM
{
	QKHEAD head; 
	int sens_id;//��������
	long trig_time;//����ʱ��
	long detrig_time;//��������ʱ��
	short trigfrac_sec;//����ʱ��,���벿��
	short detrigfrac_sec;//��������ʱ��,���벿��
 	unsigned short chk_sum;
}QK_EVTDETRIGFRM;

//����״̬��Ϣ������
typedef struct tagQK_HEARTBEATINTFRM
{
	QKHEAD head; 
	short interval;//������,>30��λ:��
	unsigned short chk_sum;//У���
}QK_HEARTBEATINTFRM;

//��ѹ֡
#ifdef EDAS_IML

typedef struct tagQK_VOLFRM
{
	QKHEAD head; 
	short ext_vol;//�ⲿ,0.1v
	short net_vol;//���繩��,0.1v
	short stor_vol;//���,0.1v
	short mon_vol;//�ⲿ����״̬����ѹ,0.1v
	unsigned short chk_sum;

}QK_VOLFRM;
#else
typedef struct tagQK_VOLFRM
{
	QKHEAD head; 
	short out_vol;//�ⲿ,0.1v
	short in_vol;//�ڲ�,0.1v
	short stor_vol;//���,0.1v
	unsigned short chk_sum;

}QK_VOLFRM;
#endif

//�¶�֡
typedef struct tagQK_TEMPFRM
{
	QKHEAD head; 
	short temperature;//�¶ȣ�0.1���϶�
	unsigned short chk_sum;
}QK_TEMPFRM;

//�Ӳ�֡
typedef struct tagQK_CLKERRFRM
{
	QKHEAD head;
	short clk_err;//�Ӳ�
	short freq_err;//Ƶ��,0.01PPM
	unsigned short chk_sum;
}QK_CLKERRFRM;

//��س��֡
typedef struct tagQK_BATCHARGEFRM
{
	QKHEAD head; 
	long time;//����/ֹͣ���ʱ��
	int vol;//���ǰ/���ص�ѹ,0.1v
	short bstop;//1-��ѯֹͣ������ ,0-��ѯ�����������
	unsigned short chk_sum;
}QK_BATCHARGEFRM;

//��ѯ����״̬��Ϣ֡
#ifdef EDAS_IML

typedef struct tagQK_HEARTBEATFRM
{
	QKHEAD head; 
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
	short clk_err;//΢��
	short freq_err;//0.01 ppm
	long buf_total;//�洢���ܿռ�,kb
	long buf_free;//�洢�����пռ䣬kb
	unsigned short chk_sum;
}QK_HEARTBEATFRM;

#endif

//GPS״̬
typedef struct tagQK_GPSSTATFRM
{
	QKHEAD head; 
	short bgps;//1-���ӣ�0-�Ƕ���
	unsigned short chk_sum;
}QK_GPSSTATFRM;

//IRIG����Ϣ
typedef struct tagQK_IRIGFRM
{
	QKHEAD head; 
	long time;//ʱ����,��GMT 1970-01-01 00:00:00���������
	long lat;//γ��,��λ��ma=1/3600000��
	long lon;//����,��λ��mas=1/3600000��
	long alt;//�߳�,��λ��0.1M
	short dop;
 	unsigned short chk_sum;
}QK_IRIGFRM;

//�洢��״̬
typedef struct tagQK_HDSTATFRM
{
	QKHEAD head; 
	short id;//�洢�����
	short be_curmass;//1-��ǰ�洢����0-���ǵ�ǰ�洢��
	long total;//�ܿռ�,��λ:Kb
	long free;//���ÿռ�,��λ:Kb
	long io_error;//���1��I/O����ʱ��
 	unsigned short chk_sum;//У���
}QK_HDSTATFRM;
  

//��ѯ�������λ
typedef struct tagQK_SENSROFFFRM
{
	QKHEAD head; 
	int sens_id;//��������
	long ch1;//1ͨ��
	long ch2;//2ͨ��
	long ch3;//3ͨ��
	unsigned short chk_sum;
}QK_SENSROFFFRM;

//��¼����
typedef struct tagQK_MASSWRCTLFRM
{
	QKHEAD head; 
	short ctl;//1-������洢��д���ݣ�0-��д����
	unsigned short chk_sum;//У���
}QK_MASSWRCTLFRM;

//��մ洢��
typedef struct tagQK_CLEARSDFRM
{
	QKHEAD head; 
	short id;//�洢�����,��1��ʼ
	unsigned short chk_sum;//У���
}QK_CLEARSDFRM;

//�ļ����ݹ���
typedef struct tagQK_BACKRULEFRM
{
	QKHEAD head; 
	//ɾ��ʱ���,-1������ά�����ڴ洢��ռ�ÿռ�ﵽ95%ʱ������������Ϣ 
	//0�����ռ�ά����Ӳ��ռ�ÿռ�ﵽ95%ʱ������������Ϣ����������һ��������ļ���ʼɾ����ֱ��ռ�ÿռ�С��70%
	//>0����ʱ��ά�����洢���������ļ���������������������ɾ��
 	short ctn_ttl;
 	unsigned short chk_sum;//У���
}QK_BACKRULEFRM;


//IP������
typedef struct tagQK_IPGWFRM
{
	QKHEAD head; 
	char ip[44];//��ַ
	char netmask[44];//����
	char gate[44];//����
	unsigned short chk_sum;//У���
}QK_IPGWFRM;


//����ϵͳʱ��֡
typedef struct tagQK_SYSTIMEFRM
{
	QKHEAD head; 
	long sec;//��1970��ʼ����
	unsigned short chk_sum;
}QK_SYSTIMEFRM;


//����ʱ��֡
typedef struct tagQK_TIMEZONEFRM
{
	QKHEAD head; 
	char zone[10];//ʱ����
	short offset;//��GMTʱ��ƫ��
	unsigned short chk_sum;
}QK_TIMEZONEFRM;

//GPS���Ӽ��
#ifdef EDAS_IML

typedef struct tagQK_GPSINTFRM{
	QKHEAD head; 
	short interval;//1-60��,-1:��������
	short type;//���ӷ�ʽ��2-IRIG�룬1-�ڲ�GPS 3-������ʱ
	unsigned short chk_sum;
}QK_GPSINTFRM;
#else

typedef struct tagQK_GPSINTFRM{
	QKHEAD head; 
	short interval;//1-60��,-1:��������
 	unsigned short chk_sum;
}QK_GPSINTFRM;
#endif

//NTPʱ�������
typedef struct tagQK_NTPSRVFRM
{
	QKHEAD head; 
	char host1[20];//NTP����������ַ
	char host2[20];//NTP���������õ�ַ
	char host3[20];//NTP���������õ�ַ
	unsigned short chk_sum;

}QK_NTPSRVFRM;
//���ź���
typedef struct tagQK_SMSCODEFRM
{
	QKHEAD head;
	short type;//1-����״̬������2-������Ϣ����
	short total;//�������������20��
	char phone[20][12];//���Ž����˺���
	unsigned short chk_sum;
}QK_SMSCODEFRM;

//�㲥֡
typedef struct tagQK_BROADCASTFRM
{
	QKHEAD head; 
	//1-���������ã�2-�������ã�3-����ƿ��ƣ�4-���������㣬5-�洢���ռ䳬��80%��6-�洢���ռ䳬��70%��7-�洢��I/O����8-���ֵ�ѹ����,9-�¶ȴ���,10-��س��,11-GPS���� 
	short type;
	unsigned short chk_sum;
}QK_BROADCASTFRM;


////�Ҷ���Ϣ֡��
//�Ҷȼ�����
typedef struct tagQK_SMALDFRM
{
	QKHEAD head; 
	int sens_id;//����ƺ�
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
	unsigned short chk_sum;
}QK_SMALDFRM;

//�ֺ�����ֵ���⣩
typedef struct tagQK_TAQPDFRM
{
	QKHEAD head; 
	int sens_id;//����ƺ�
	char name[8];//̨��
	long trig_time;//����ʱ�� 	��GMT 1970-01-01 00:00:00��������� 
	int frac_time;//����ʱ��,���벿��
	float tao_c;
	float pd;//��λ��cm
	unsigned  short chk_sum;//CRC-16У��

}QK_TAQPDFRM; 


//�¼���¼
typedef struct tagQK_EVTRECFRM
{
	QKHEAD head;
	short be_wr;//1-��¼
	short pre_evt;//�¼�ǰ��¼���ȣ���
	short aft_evt;//�¼����¼���ȣ���
	unsigned short chk_sum;// У��
}QK_EVTRECFRM;

//����Ӧ��֡
typedef struct tagQK_ACKFRM
{
	QKHEAD head;
	long time;//����ʱ����
	char frac_sec;//��һ�����������ƫ��ֵ
	char chn_id;//ͨ�����
	short samp;//������
	short pha_id;//��λ��1-���ԣ�2-��С
	unsigned  short chk_sum;//CRC-16У��
}QK_ACKFRM;
 
//��֤��
typedef struct tagQK_AUTHCODE
{
	QKHEAD head;
	char code[40];
	unsigned  short chk_sum;
}QK_AUTHCODE;
#endif