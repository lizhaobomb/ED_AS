// SitelinkObj.h: interface for the CSitelinkObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITELINKOBJ_H__E395E4B4_6FF7_4D81_BD53_146D3069C03E__INCLUDED_)
#define AFX_SITELINKOBJ_H__E395E4B4_6FF7_4D81_BD53_146D3069C03E__INCLUDED_

#include "rcvstruc.h"

typedef struct tagSITEMAG{
	//台站名
	char site[12];
	//地震计数
	int chn_sum;
	//通信方式 0: com,1:network
	int  protocol;
	//1-3通道/包
	int packmethod;
	//1-允许重发
	int bresend;
	//串口连接
	LOC_COM com_con;
	//网络连接
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

