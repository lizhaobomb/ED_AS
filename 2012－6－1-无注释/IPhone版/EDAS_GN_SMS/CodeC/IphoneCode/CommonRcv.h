// CommonRcv.h: interface for the CCommonRcv class.
// 接收基类
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
	//return -1: disconnect 0: receive error 1-ok 2-data no ready,数据存放在m_dataframe
	virtual int Receive(long * prcv_data)=0;
	virtual void ReConnection()=0;
	virtual void CloseConnection()=0;
	//数据重发
	virtual void Resend(long time,int length,int chn_id)=0;
	virtual BOOL IsConnected()=0;
public:
	WaveDataFrame m_dataframe;//接收数据区
	long *m_rcv_data;
	CLogfile m_logfile;
	long m_elapse;//接收中断时间
	int m_bdatarestore;//数据恢复标志，1--数据刚恢复

	CSitelinkObj * m_sitelink;
	
	char m_sync[4];

	CSteim2 m_steim;
	//连接状态
	BOOL m_bconnected;

public:
	BOOL AllocateMemory();
	//释放内存
	void FreeMemory();
	//解压缩
	BOOL ExpandCode(short * pexpand,short *pcompress,int cmd);
	//解压缩分道包0x5502
	BOOL ExpandCodeSingle(char *pexpand,char *pcompress);

	//设置数据到接收区
	void SetData(char *data,long * prcv_dat,int comp,int wlen);
	void SetCmd();
	void StorePackData();

};

#endif 
