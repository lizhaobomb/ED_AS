 
#ifndef __EDASUNCMP_H__
#define __EDASUNCMP_H__

/************************************************************************************/
/*���ܣ�����EDAS����ѹ���㷨��ѹ������
/*���ߣ�����
/*create date: 2009-9-4
/*company��geodevice
/***********************************************************************************/

#include "inbits.h"

#define MAXCMPDATALEN 5//5��
#define MAXCINDEX2LEN MAXCMPDATALEN*500/10//����������󳤶�
 
 
class CEdasUncmp
{
public:
	CEdasUncmp()
	{
	}
	
	~CEdasUncmp()
	{
	}
	
	//��ѹ��,ǰ��: ԭʼ��ѹ�����ݳ�����֪
	//����:
	//	pin: ѹ������, ��ʽ:
 		//FIRST		long	4		��һ������
		//IDX1 		bit		5		��һ������
		//IDX2		bit		2*N		�ڶ���������ÿ������ռ2bit��N�����*1
		//CDATA1	bit		9*N1	�ӵ�2�����㵽��9�������ѹ�����ݣ�N1-������Ч�ֳ�����*2
		//CDATA2	bit		10*N2	��10��20�������ѹ�����ݣ�N2-������Ч�ֳ� ��*2
		//��			
		//CDATAN 	bit		���10�������ѹ������ ��*2
		//UNUSED	bit		��0����˫�ֽ�

	//  rec_len: ԭʼ���ݳ���,��λ:��
	//���:
	//	outdata: ��ѹ������,ÿ�����ݰ��������洢
 	//����: 1-�ɹ�,0-ʧ��
	int UncompressData(char * pin,int rec_len,long * out_data);

 
};

#endif  
