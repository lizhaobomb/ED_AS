#ifndef  __RCVSTRUC_H__
#define __RCVSTRUC_H__

#include "limitdef.h"

//////////////////�����̨վ�������ݵĴ��ڣ�����ṹ��̨վ�����ṹ///////////////
//2004/3/8 ����

//����

typedef struct tagLOC_COM {
	//������
	char com[20];
	//������
	int  baud;
	//���ھ��
	//Handle  hcom;
	//�����豸����
	//DCB     dcb;
	//1-�򿪣�0-�� , 2-���ڳ�ʼ״̬��ȷ���Ƿ���edas-24ip���� 
	//BOOL    status;
}LOC_COM;

//����ipu
typedef struct tagLOC_NET{
	//ipu��ַ
	char host[128];
	//���ƶ˿�
	int cmd_port;
	//���ݶ˿�
	int data_port;
	//�û�
	char user[128];
	//����
	char passwd[128];
	//���ƶ˿����Ӿ��
	int  cfd;
	//���ݶ˿����Ӿ��
	int dfd;
}LOC_NET;

//ͨ������
typedef struct tagLOCALREVBLK{
	//ͨ������
	int  local_id;
	//������
	int chn_sum;
	//������
	long samp;
	//�����������ڣ���
	float natural_prd;
//time: latest time,time_error: time error counter,lasttime: used in resend
	//����ʱ��
	long time;
	//ʱ��������
	long time_err;
	//�ط�����ʱ��
	long lasttime;
	long in_resend;
	long resend_begin,resend_end;
	//���ݲɼ���ͨ����das_id[0]==-1: ���ֵ�,ud,ew,ns
	int	 das_id[MAXCOMP];
	//�������ͣ�A-���ٶ�V-�ٶ�D-λ��
	char data_type;
	//�۲���ţ��ֵ�ʱʹ��
	int obs_id;
	//�۲�������ֵ�ʱʹ��
	char obs_name[8];
	//��Ӧ�ļ���
	char rsp_file[50];
	struct tagLOCALREVBLK *lpnext;
}LOCALREVBLK;

//̨վ�����ṹ
typedef struct tagSITEBLK{
	//̨վ��
	char site[8];
	//̨վid
	int site_id;
	//�ڵ��
	int section_id;
	//�ڵ�����д 
	char section_name[12];
	//�ֳ�
	int wlen;
	//1-���ؽڵ�
	int blocal;
	//ͨ�ŷ�ʽ 0: com,1:network
	int  protocol;
	//������� 1: �з���
	int	 beresend;
	//��������
	LOC_COM *com_con;
	//��������
	LOC_NET *net_con;
	//ͨ������
	LOCALREVBLK * local_rev;
	struct tagSITEBLK * lpnext;
}SITEREVBLK;
#endif