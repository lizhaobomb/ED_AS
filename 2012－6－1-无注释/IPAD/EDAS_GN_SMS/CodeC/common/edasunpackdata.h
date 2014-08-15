#ifndef __CEDASUNPACKDATA_H__
#define __CEDASUNPACKDATA_H__

/************************************************************************************/
/*���ܣ�����ͨ�����ݲɼ����������ݷ������ͨ��ѹ������֡�����������ļ���ѹ������֡�Ľ���
/*���ߣ�����
/*create date: 2009-9-21
/*update
/*		date			action				common
		2009-9-29		add					UncmpLEdasfrm��ѹ������ͨ������֡
						edit				UncmpEdasfrm����ΪUncmpHEdasfrm���޸ģ����������λ
		2010-5-6		edit				UncmpHEdasfrm:����flag==1 ��samp>1: �Զ�����rec_len=max(10,samp/5)
/*company��geodevice
/***********************************************************************************/


#include "edasuncmp.h"

class CEDASUNPackData
{
public:
	CEDASUNPackData()
	{
	}
	
	~CEDASUNPackData()
	{
	}
 	//��ѹ�������������������ļ�����֡
	//����:
	//	pin: ѹ�����ݣ���ʽ: ���������ļ�����֡
	//  in_len: pin�洢�����ݳ���,��λ:�ֽ�
	//  samp: ������
	//  comp: ͨ������
 	//  out_len:  outdata��󳤶�,��λ:�ֽ� 
	//���:
	//	outdata: ��ѹ����Ĳ�������,���������洢,ȫ��ͨ��. ���ݰ���: [ͨ��1ȫ������], [ͨ��2ȫ������], [ͨ��3ȫ������]
	//	rec_len: ����֡�����ݼ�¼����,��λ: ��
 	//  cali_flag: �궨��ǣ�0����ͨ���ݣ�1������궨��2�����ұ궨��3������궨��
	//  time:ʱ����
	//����: 1-�ɹ�,0-ʧ��
	int UncmpFileblk(char *pin,int in_len,int samp,int comp,long * time,int * rec_len,int * cali_flag,long * outdata,int  out_len);

	//��ѹ���������ɿ������ݷ������ͨ��ѹ������֡�����ݲ���,ǰ��:  ѹ��ǰ�ļ�¼������֪
	//����:
	//	pin: ѹ�����ݣ���ʽ: �������ݷ������ͨ��ѹ������֡,��ʱ���뿪ʼ
	//  rec_len:  ѹ��ǰ�ļ�¼����,��λ:��
	//	flag: flag==1 �� ������>1ʱ, �Զ�����rec_len, rec_len=max(10,samp/5)
 	//  out_len:  outdata��󳤶�,��λ:�ֽ� 
 	//���:
	//	outdata: ��ѹ����Ĳ�������,���������洢
	//	second:  ʱ����
	//	frac_sec: ��һ�����������ƫ������
	//	samp: ������
	//	phase: ��λ
	//	chn_id: ͨ������,��1��ʼ
 	//  cali_flag: �궨��ǣ�0����ͨ���ݣ�1������궨��2�����ұ궨��3������궨��
 	//����: 1-�ɹ�,0-ʧ��
 	int UncmpHEdasfrm(char *pin,int rec_len,int flag,long * second,int * frac_sec,int * samp,int * phase,int * chn_id,int * cali_flag,char * outdata,int out_len);
  
	//��ѹ���������ɿ������ݷ������ͨ��ѹ������֡�����ݲ��� 
	//����:
	//	pin: ѹ�����ݣ���ʽ: �������ݷ������ͨ��ѹ������֡,��ʱ���뿪ʼ
  	//  out_len:  outdata��󳤶�,��λ:�ֽ� 
 	//���:
	//	outdata: ��ѹ����Ĳ�������,���������洢
	//	second:  ʱ����
	//  chn_sum��ͨ������
  	//	chn_id: ͨ���������飬������鳤��6����7��ʼ
 	//  cali_flag: �궨������飬������鳤��6���궨���Ϊ0����ͨ���ݣ�1������궨��2�����ұ궨��3������궨��
 	//����: 1-�ɹ�,0-ʧ��
 	int UncmpLEdasfrm(char *pin, long * second,int * chn_sum,int *  chn_id,int *  cali_flag,char * outdata,int out_len);

};

#endif  
