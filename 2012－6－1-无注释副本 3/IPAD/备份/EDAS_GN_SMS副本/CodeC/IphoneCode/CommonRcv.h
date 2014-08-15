// CommonRcv.h: interface for the CCommonRcv class.
// ���ջ���
//////////////////////////////////////////////////////////////////////

#ifndef __COMMONRCV__
#define __COMMONRCV__

#include "CommonDef.h"
#include "clogfile.h"
#include "sitelinkobj.h"
#include "frames.h"
#include "steim2edas.h"

#include <stdio.h>
#include "eventfile.h"
#include "response.h"
#include "rcvstruc.h"

class CCommonRcv  
{
public:
	CCommonRcv();
	~CCommonRcv();
	virtual BOOL init(CSitelinkObj* psite,char * logpath)=0;
	virtual BOOL Send(char * pdata,int len)=0;
	//return -1: disconnect 0: receive error 1-ok 2-data no ready,���ݴ����m_dataframe
	virtual int Receive(long * prcv_data)=0;
	virtual void ReConnection()=0;
	virtual void CloseConnection()=0;
	//�����ط�
	virtual void Resend(long time,int length,int chn_id)=0;
	virtual BOOL IsConnected()=0;
public:
	WaveDataFrame m_dataframe;//����������
	long *m_rcv_data;
	CLogfile m_logfile;
	long m_elapse;//�����ж�ʱ��
	int m_bdatarestore;//���ݻָ���־��1--���ݸջָ�

	CSitelinkObj * m_sitelink;
	
	char m_sync[4];

	CSteim2 m_steim;
	//����״̬
	BOOL m_bconnected;

public:
	BOOL AllocateMemory();
	//�ͷ��ڴ�
	void FreeMemory();
	//��ѹ��
	BOOL ExpandCode(short * pexpand,short *pcompress,int cmd);
	//��ѹ���ֵ���0x5502
	BOOL ExpandCodeSingle(char *pexpand,char *pcompress);

	//�������ݵ�������
	void SetData(char *data,long * prcv_dat,int comp,int wlen);
	void SetCmd();
	void StorePackData();

};

#endif 
