// NetRcv.h: interface for the CNetRcv class.
//�������
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETRCV_H__051C43C0_D48F_4AE1_B88A_DE04ED27A22B__INCLUDED_)
#define AFX_NETRCV_H__051C43C0_D48F_4AE1_B88A_DE04ED27A22B__INCLUDED_

#include "commonrcv.h"
#include "csocketapi.h"

class CNetRcv : public CCommonRcv 
{
public:
	CNetRcv();
	~CNetRcv();

	BOOL init(CSitelinkObj* psite,char * logpath);

	BOOL Send(char * pdata,int len);
	//return -1: disconnect 0: receive error 1-ok 2-data no ready,���ݴ����m_dataframe
	int Receive(long * prcv_data);
	void ReConnection();
	void Resend(long time,int length,int chn_id);
	void CloseConnection();
	BOOL IsConnected();

private:
	LOC_NET m_connect;//net����
	int m_b109;
	CSocketAPI m_sock;
private:
	BOOL ConnectToCmdServer();
	BOOL ConnectDataServer();
	BOOL Connection();
	BOOL AskData();

	////////////////////////////receive /send data/////////////////////////////////
	//����ͬ����
	int ReceiveSync();
	//���պ����ֽ�
	BOOL ReadData();
	//����Ӧ��
	void SendResponse(long time,int chn_id);
	//�ж��Ƿ��ǲ������ݣ��Ƿ���Ҫ��ѹ��--��ѹ���������Ϣ��m_dataframe��
	BOOL DecodeData();


	//�жϼ�⣬�������1�����ղ������ݣ���Ϊ�����ж�
	int CheckInterrupt();
	//����յ�ȫ��ͬ����
	int CheckSync();

	
};

#endif // !defined(AFX_NETRCV_H__051C43C0_D48F_4AE1_B88A_DE04ED27A22B__INCLUDED_)
