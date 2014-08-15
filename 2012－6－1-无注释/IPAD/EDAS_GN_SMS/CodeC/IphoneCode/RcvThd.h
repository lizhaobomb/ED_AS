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
	//ͨ�Ŷ���
	CCommonRcv * m_trfobj;
	//���淢������
//	TXCMD *m_txp1, *m_txp2, *m_txp;
//	TXCMD m_txbf[40];
	char m_rcv_buffer[MAXFRAMESIZE];
	CSitelinkObj * m_selsite;
	CHANNEL *m_pchannels;
private:
	BOOL ConnectSite();

	//����
	BOOL Receive();
	//����
//	BOOL Send();

	//���벨�ζ���
//	int Insert_Datalnk(REVBLKS * pblk,int chn_id);
	//�����������
//	BOOL Insert_Cmdlnk(REVBLKS * pblk);

	//�������Ӷ���
	BOOL AllocRcvObj();
	//�ͷ����Ӷ���
	void FreeRcvObj();
};

#endif // !defined(AFX_RCVTHD_H__4775D213_0A11_40DF_AD78_4E18FE456C0F__INCLUDED_)
