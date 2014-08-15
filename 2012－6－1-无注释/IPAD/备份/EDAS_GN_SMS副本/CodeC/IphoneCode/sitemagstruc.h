#ifndef __SITEMAGSTRUC_H__
#define __SITEMAGSTRUC_H__
#include "CommonDef.h"
#define UD 0
#define EW 1
#define NS 2
#define ALL 3
//��ʾ����
/*
typedef struct DSPCTL{
	int scantime;//ɨ��ʱ��
	int voffset;//ͨ��ƫ��
	int hoffset;//ʱ��ƫ��
	int channelnum;//��ʾͨ����
	int channeltype;//��ʾͨ������
	int maxrange_Hscroll,maxrange_Vscroll;//ˮƽ������󳤶ȣ���ֱ������󳤶�
	int iHscrollPos,iVscrollPos;//ˮƽ����λ�ã���ֱ����λ��
	int totalchannel,channel_range;//��ͨ������ͨ����ʾ���
}DSPCTL;
*/
typedef struct tagDSPPAR{
	float max;//��ʾ���ֵ
	float min;//��ʾ��Сֵ
	int unit;//0-count 1-V 2-um/s 3-gal
}DSPPAR;

struct SHDAT1 {
 	struct SHDAT1 *pn;
	struct SHDAT1 *pb;
	short  check_sum;
	long  flg;//����fifo��־
	long time;//����ʱ��
	long cali_flag;//�궨��־ 4-���� 2-���� 6-�����
	long  *ds;//����
};
//�洢��
 typedef struct tagREVBLKS{
	struct SHDAT1 *ps,*ps1;
	struct SHDAT1 *pe,*pe1;
	struct SHDAT1 *p_rev,*p_rev1;
	struct SHDAT1 *dat;
	struct SHDAT1 *p_dsp;
	char * p_dat,*p_dat1;//���������������
	struct SHDAT1 *wr_dat;
	int st;
}REVBLKS;

typedef struct tagPOINT{
	int x;
	int y;
}POINT;



typedef struct tagCHANNEL{
	char stn[40];//̨��
	int chn_id;//ͨ����
	char chn[20];//ͨ����
	char chn_label[20];//ͨ����Ļ��ʾ
	int samp,wlen;//�������ֳ�
	BOOL bView;//1-������ʾ
	BOOL bShow;//1-��ʾ״̬
	int timex;
	long maxvalue,minvalue,offset;//�������������Сֵ���м�ֵ
	long lsttm,curtm;//��һ��ʱ�䣬��ǰʱ��

	//point
	REVBLKS revblk;//���յ��Ĳ���
	REVBLKS cmdblk;//���յ��Ĳ���
	//draw parameter
	int position,data_cnt,max_posi,cnt;
    double xa,xi;
	int xaj,xij;
//screen resolution width*2
	POINT *point;//������Ļ����
	DSPPAR dsp;//��ʾ����
	int wr_cnt;//������
	int cali_flag;//�궨��־ 4-���� 2-���� 6-�����
	float factor;//������
	struct tagCHANNEL * lpnext;
}CHANNEL;

typedef struct tagTXCMD {
	struct tagTXCMD *pn;
	int cnt;
	char buf[512];
 }TXCMD;



#endif