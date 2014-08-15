// RcvThd.cpp: implementation of the CRcvThd class.
// 与采集器通信线程
//////////////////////////////////////////////////////////////////////

#include "sitemag.h"
#include "RcvThd.h"
#include "CommonDef.h"

#include "GlobalData.h"
extern  CSitemagApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRcvThd::CRcvThd()
{
	m_selsite=NULL;
	m_trfobj=NULL;
//	m_txp1=m_txp2=m_txp=NULL;
	
}

CRcvThd::~CRcvThd()
{
	CloseCon();
	FreeRcvObj();

}
void CRcvThd::Init(CSitelinkObj * pselsite,CHANNEL * pchannels)
{
	m_selsite=pselsite;

	m_pchannels=pchannels;

}
void CRcvThd::main()
{
	ConnectSite();
	while(1){
	//	Send();
		Receive();
		usleep(100000);
	}
}
void CRcvThd::CloseCon()
{
	if(m_trfobj==NULL)
		return;
	m_trfobj->CloseConnection();
	m_trfobj=NULL;
}

BOOL CRcvThd::ConnectSite()
{
	if(m_selsite==NULL)
		return 0;
	if(m_trfobj!=NULL){
		delete m_trfobj;
		m_trfobj=NULL;
	}
	if(!AllocRcvObj())
		return FALSE;
	m_trfobj->init(m_selsite,theApp.m_logpath);

	return TRUE;
}
//接收
BOOL CRcvThd::Receive()
{
	int ret;
//	TRACE("receive\n");
	if(m_trfobj==NULL)
		return TRUE;
	memset(m_rcv_buffer,0,sizeof(m_rcv_buffer));
	
	//接收
	ret=m_trfobj->Receive((long *)m_rcv_buffer);
	if(ret==-1)
	{
		//reconnect
		m_trfobj->ReConnection();
		return FALSE;
	}
	if(ret==0 || ret==2)
		return FALSE;
	
	//插入数据
	short * p=(short *)m_trfobj->m_dataframe.szData;
	short * p1=(short *)m_rcv_buffer;
		
	CHANNEL * pchn=m_pchannels;
	LOCALREVBLK * pblk=m_selsite->m_plocal;
	while(pblk!=NULL)
	{
		if(pblk->local_id==m_trfobj->m_dataframe.nChId)
			break;
		for(int i=0;i<pblk->chn_sum && pchn!=NULL;i++)
			pchn=pchn->lpnext;
		pblk=pblk->lpnext;
	}
	if(pchn==NULL || pblk==NULL)
		return FALSE;

	if(p1[1]!=0x55CC && p1[1]!=0x55BB && p1[1]!=0x55AA && p1[1]!=0x5555 && p1[1]!=0x55DD 
		 && p1[1]!=0x5501 &&  p1[1]!=0x5502 && p1[1]!=0x55dd)
	{
		//参数帧存入参数接收队列
		memcpy((char *)pchn->cmdblk.p_rev->ds,m_rcv_buffer,p[2]+6);
		pchn->cmdblk.p_rev->flg=pchn->cmdblk.st;
		pchn->cmdblk.st++;
		if(pchn->cmdblk.st==(int)0x7fffffff)
			pchn->cmdblk.st=0;
		pchn->cmdblk.p_rev=pchn->cmdblk.p_rev->pn;
	//	TRACE("%x %d\n",pchn->cmdblk.p_rev,pchn->cmdblk.p_rev->flg);
	//	TRACE("cmd=%x %d \n",p1[1],p1[2]);

		return TRUE;
	}

	//处理波形数据
	int num=1;
	if(p1[1]==0x55bb)//1HZ采样数据
		num=10;

	for(int n=0;n<num;n++)
	{
		//波形数据帧存入数据接收队列
		memcpy((char *)pchn->revblk.p_rev->ds,m_rcv_buffer,4);
		memcpy(((char *)pchn->revblk.p_rev->ds+4),m_rcv_buffer+4+m_trfobj->m_dataframe.samp*4*pblk->chn_sum*n,
				m_trfobj->m_dataframe.samp*pblk->chn_sum*4);

		pchn->revblk.p_rev->time=m_trfobj->m_dataframe.time+n;
		pchn->revblk.p_rev->flg=pchn->revblk.st;
		pchn->revblk.st++;
		if(pchn->revblk.st==(int)0x7fffffff)
			pchn->revblk.st=0;
		pchn->revblk.p_rev->cali_flag=m_trfobj->m_dataframe.sFrId.ZipFlag;

		pchn->revblk.p_rev=pchn->revblk.p_rev->pn;
	}
	pchn->samp=pblk->samp;
	
	//theApp.m_current_time=m_trfobj->m_dataframe.time+num-1;

	return TRUE;
}


//发送数据到采集器
BOOL CRcvThd::Send(char *cmd,int cmd_len)
{
	if(m_trfobj==NULL)
		return FALSE;
	return m_trfobj->Send(cmd,cmd_len);
	
}


//分配接收对象
BOOL CRcvThd::AllocRcvObj()
{
	if(m_selsite==NULL)
		return FALSE;
	try{
		if(m_trfobj!=NULL)
			delete m_trfobj;
		if(m_selsite->m_site.protocol==1)
			m_trfobj=(CCommonRcv *)new CNetRcv();//网络通信
		else if(m_selsite->m_site.protocol==2) {m_trfobj=(CCommonRcv *)new CNetRcv();}
			//m_trfobj=(CCommonRcv *)new CListenRcv();//主动接收
		else //m_trfobj=(CCommonRcv *)new CComRcv();//串口
		{
				m_trfobj=(CCommonRcv *)new CNetRcv();
		}

	}catch(...){
		return FALSE;
	}
	return TRUE;
}
void CRcvThd::FreeRcvObj()
{
	if(m_trfobj!=NULL){
		delete m_trfobj;
		m_trfobj=NULL;
	}
}

int CRcvThd::IsConnected()
{
	if(m_trfobj==NULL)
		return FALSE;
	return m_trfobj->IsConnected();
}