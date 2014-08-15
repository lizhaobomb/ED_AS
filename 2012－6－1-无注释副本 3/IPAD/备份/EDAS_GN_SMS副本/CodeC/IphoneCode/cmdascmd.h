/*******************************************************************************/
/* ���ܣ������ѯ�������ݲɼ�������
/* create date: 2009-9-18
/* update:
/* company: geodevice
/* author:  ����
/*******************************************************************************/

#ifndef __CMDASCMD_H__
#define __CMDASCMD_H__
#include "response.h"
#include "cmdasframe.h"


//̨վ����
typedef struct tagSTNPARS{
	short id;//̨��
	char name[32];//̨��
	char abbr[8];//̨����д
	short sens_num;//�������
	short chn_sum;//ͨ����
	float lat;//γ�� 
	float lon;//���� 
	float alt;//�̣߳���λ��m
	char run_date[32];//����ʱ��
	char netid[4];//�������
	char version[32];//�汾
}STNPARS;
 
//���Ҳ���
typedef struct tagSINEPARS{
	short btimer;//1-��ʱ����
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	long time;//��һ������ʱ��
	short amptype;
	short prd_num;//Ƶ����
	SINES sine[50];//���Ҳ�Ƶ���������prd_num��
}SINEPARS;

//����/ǿ��궨���� 
typedef struct tagPULSEPARS{
	short btimer;//1-��ʱ
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	long time;//��һ������ʱ��
	short amptype;
	short amp;//����
	short dur;//��������0.1��
}PULSEPARS;

//����궨���� 
typedef struct tagPSEUDOPARS{
	short btimer;//1-��ʱ����
	short method;//��ʱ��ʽ��0-ÿ�죬1-ÿ���ڣ�2-ÿ�£�3-ÿ��
	short tm_start1;//TM_METHOD:0 �C ʱ1 �C ����2 �C ��3 �C ��
	short tm_start2;//����ʱ�䣬TM_METHOD:0 �C ��1 �C ʱ2 �C ʱ3 �C ��
	short tm_start3;//����ʱ�䣬TM_METHOD:1 �C ��2 �C ��3 �C ʱ
	short tm_start4;//����ʱ�䣬TM_METHOD:3 �C ��
	long time;//��һ������ʱ��
	short amptype;
	short amp;//����
	unsigned short dur;//��Ԫ�����0.001��
	short signal_flag;//�źű�־
	short rep_num;//�ظ�����
}PSEUDOPARS;
 
//����Ʋ���  
typedef struct tagSENSPARFRM
{
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
  	
}SENSPARFRM;
 

//����״̬
typedef struct tagSMPSTAT
{
	short samp;//������
	short phase;//������λ1���������ƣ�2����С��λ
	int stat;//1-����,0-�ر�
}SMPSTAT;

//�����������Ϣ�ṹ 
typedef struct tagSENSPARS{
 	SENSPARFRM sens_par;
	short samp;//ϵͳ������	1,10,20,50,100,200,500
	short phase;//ϵͳ������λ	1���������ƣ�2����С��λ, ����+��С��λ1,10,20,50,100,200,500
	SMPSTAT smpstat[16];//��Ӧ����+������λ1,10,20,50,100,125,200,500
	short hp;//��ͨ�˲������룬0: DC,  1:10S,   2:30S,   3:100S,4:300S, 5:1000S, 6:3000S
	SINEPARS sines;//���ұ궨
	PULSEPARS pulse;//����궨
	PULSEPARS strong;//ǿ��궨
	PSEUDOPARS pseudo;//�����궨
	int cal_delay;//�궨�ȴ�ʱ�䣬��λ����
 	int trig_num;//��Чtrig_auto��
 	AUTOTRIG trig_auto[6];
 	int gain_id;//���̱�־ 0:10V�� 1��20V,2: 5V,3-2.5V
	CM_INSTPARFRM  inst;
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
}SENSPARS;
 

//���ڷ��ͷ�ʽ
typedef struct tagCOMCTL
{
	short back;//1-Ӧ��ʽ��0-��Ӧ��
	short binit;//1-ϵͳ������ֱ�ӷ��Ͳ��Σ�0-�ȴ��������
	short be_broad;//���͹㲥֡,1-���ͣ�0-�����ͣ�Ĭ�ϣ�
	short be_stat;//����״̬֡,1-���ͣ�0-�����ͣ�Ĭ�ϣ�
	long baud;//������
}COMCTL;
//gps��Ϣ
//�����ǲ���
typedef struct tagSTARSTAT
{
	char I;
	char H;
	char S;
	char D;
}STARSTAT;
typedef struct tagGPS
{
	char sync;//ͬ����־
	long time;//��λʱ��
	short dop;//DOP
	float lat;//γ��
	float lon;//����
	float alt;//�߳�
	STARSTAT star[6];
	char rev_stat;//����״̬
	int star_sum;//������
	long next_tm;//�´ζ���ʱ��
} GPS;		

//��ѹ
typedef struct tagVOL
{
	float out_v;//�ⲿ��ѹ�� ��λ��V
	float in_v;//�ڲ���ѹ, 	��λ��V
	float stor_v;//��ƿ��ѹ, ��λ��V
}VOL;

//�¶�
typedef struct tagTEMPERATURE
{
	float temperature;//�����¶�,��λ����
}TEMPERATURE;

//�Ӳ���Ϣ
typedef struct tagCLKERR
{
	float time_err;//ʱ�����
	float freq_err;//Ƶ�����
}CLKERR;


//��س��ʱ��/ֹͣ���ʱ�� 
typedef struct tagBATCHARGE
{
	long time;//����/ֹͣ���ʱ��
	float vol;//���ǰ/���ص�ѹ,v
}BATCHARGE;

//�������
typedef struct tagIPPARS
{
	char ip[64];//IP
	char mask[64];//����
	char gw[64];//����
}IPPARS;


//SD��״̬
typedef struct tagSDSTAT
{
	long total;//�ܿռ�,��λ:Kb
	long free;//���ÿռ�,��λ:Kb
	long temp;//Ӳ���¶ȣ�0.1���϶�
	long be_curmass;//1-����д�Ĵ洢��
	long io_error;//���1��I/O����
	short flag;//����
}SDSTAT;

//�������ݷ��������Ʋ���
typedef struct tagNETSRVCTL
{
	int port;//���ƶ˿� 
	int dataport;//��ʼ���ݶ˿�
	short tm_ctl;//��ʱ���ƣ���λ����
	short max_con;//���ͻ�����Ĭ�ϣ�4
}NETSRVCTL;

 
//ʱ��֡
typedef struct tagTIMEZONE
{
	char zone[10];//ʱ����
	short offset;//��GMTʱ��ƫ��
}TIMEZONE;

//�ۺ��ж� 
typedef struct tagDETERMINEVT
{
	short method;//	�����ж�������1-���ۺ��ж���0-����������ж�
	short trig_num;//�����������,����1ʱ��Ч�������������
	short trig_win;//�����ж�ʱ�䴰	,����1ʱ��Ч�������ж�ʱ�䴰����λ����
	short end_num;//���������������,����1��Ч�����������������

}DETERMINEVT;


//���ݲɼ���ͨ�ò���
typedef struct tagDASPARS{
  	COMCTL comctl;//���ڷ���
	NETSRVCTL rts_netsrv;//ʵʱ���ݷ���
	NETSRVCTL qk_netsrv;//�������ݷ���
	CLIENTCONPAR data_srv[4];//�������ͷ���
	GPS gps;//GPS
	int gps_int;//GPS���
	VOL vol;//��ѹ
	TEMPERATURE temperature;//�¶�
	CLKERR clk_err;//�Ӳ�
	BATCHARGE bat,sbat;
	TIMEZONE timezone;//ʱ��
	SDSTAT sd_stat[2];//�洢��״̬
 	short ctn_ttl;// -1������Ӳ��ά������Ӳ��ռ�ÿռ�ﵽ95%ʱ������������Ϣ0��Ӳ��ռ�ÿռ�ﵽ95%ʱ������������Ϣ����������һ��������ļ���ʼɾ����ֱ��ռ�ÿռ�С��80%>0������洢���������ļ���������������������ɾ��
	IPPARS ip;//�����ַ
	DETERMINEVT determine;//����ʱ���ж�
}DASPARS;

typedef struct DASCMDSTRUC
{
	STNPARS stnpar;//̨վ����
	DASPARS das;//�ɼ�����������
	SENSPARS sens[12];//�����
	int heart_interval;//�������
	int bwr;//1-����д�洢����0-������д
}DASCMDSTRUC;


 
#endif