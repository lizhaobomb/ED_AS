// RcvThd.h: interface for the CRcvThd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCVTHD_H__4775D213_0A11_40DF_AD78_4E18FE456C0F__INCLUDED_)
#define AFX_RCVTHD_H__4775D213_0A11_40DF_AD78_4E18FE456C0F__INCLUDED_


#include "sitelinkobj.h"
#include "cmdascmd.h"
#include "netrcv.h"
//#include "listenrcv.h"
#include "sitemagstruc.h"



class CRcvThd  
{
public:
	CRcvThd();
	virtual ~CRcvThd();
	void main();
	void Init(CSitelinkObj * pselsite,CHANNEL * pchannels);
//	void SettoSendbuf(char *cmd,int cmd_len);
	int IsConnected();
	BOOL Send(char *cmd,int cmd_len);
	void CloseCon();

private:
	//通信对象
	CCommonRcv * m_trfobj;
	//缓存发送数据
//	TXCMD *m_txp1, *m_txp2, *m_txp;
//	TXCMD m_txbf[40];
	char m_rcv_buffer[MAXFRAMESIZE];
	CSitelinkObj * m_selsite;
	CHANNEL *m_pchannels;
private:
	BOOL ConnectSite();

	//接收
	BOOL Receive();
	//发送
//	BOOL Send();

	//插入波形队列
//	int Insert_Datalnk(REVBLKS * pblk,int chn_id);
	//插入参数队列
//	BOOL Insert_Cmdlnk(REVBLKS * pblk);

	//建立连接对象
	BOOL AllocRcvObj();
	//释放连接对象
	void FreeRcvObj();
};

#endif // !defined(AFX_RCVTHD_H__4775D213_0A11_40DF_AD78_4E18FE456C0F__INCLUDED_)
