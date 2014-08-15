// NetRcv.h: interface for the CNetRcv class.
//网络接收
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
	//return -1: disconnect 0: receive error 1-ok 2-data no ready,数据存放在m_dataframe
	int Receive(long * prcv_data);
	void ReConnection();
	void Resend(long time,int length,int chn_id);
	void CloseConnection();
	BOOL IsConnected();

private:
	LOC_NET m_connect;//net连接
	int m_b109;
	CSocketAPI m_sock;
private:
	BOOL ConnectToCmdServer();
	BOOL ConnectDataServer();
	BOOL Connection();
	BOOL AskData();

	////////////////////////////receive /send data/////////////////////////////////
	//接收同步字
	int ReceiveSync();
	//接收后续字节
	BOOL ReadData();
	//发送应答
	void SendResponse(long time,int chn_id);
	//判定是否是波形数据，是否需要解压缩--解压，读相关信息到m_dataframe中
	BOOL DecodeData();


	//中断检测，如果大于1分钟收不到数据，认为数据中断
	int CheckInterrupt();
	//检测收到全部同步字
	int CheckSync();

	
};

#endif // !defined(AFX_NETRCV_H__051C43C0_D48F_4AE1_B88A_DE04ED27A22B__INCLUDED_)
