// SitelinkObj.h: interface for the CSitelinkObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITELINKOBJ_H__E395E4B4_6FF7_4D81_BD53_146D3069C03E__INCLUDED_)
#define AFX_SITELINKOBJ_H__E395E4B4_6FF7_4D81_BD53_146D3069C03E__INCLUDED_

#include "rcvstruc.h"

typedef struct tagSITEMAG{
	//̨վ��
	char site[12];
	//�������
	int chn_sum;
	//ͨ�ŷ�ʽ 0: com,1:network
	int  protocol;
	//1-3ͨ��/��
	int packmethod;
	//1-�����ط�
	int bresend;
	//��������
	LOC_COM com_con;
	//��������
	LOC_NET net_con;

}SITEMAG;

#define SITEMAGLEN sizeof(SITEMAG)

class CSitelinkObj
{
public:
	CSitelinkObj(SITEMAG * psite,LOCALREVBLK * plocal);
	virtual ~CSitelinkObj();
	SITEMAG m_site;
	LOCALREVBLK * m_plocal;
};

#endif // !defined(AFX_SITELINKOBJ_H__E395E4B4_6FF7_4D81_BD53_146D3069C03E__INCLUDED_)

