#include "stdafx.h"
#include "CConDXSrv.h"

CConDXSrv::~CConDXSrv()
{
	CloseSrv();
}

int CConDXSrv::ConnectCmdSrv(char * host,int port,char *user,char *password,int bcenter,char *askdata)
{
	char buf[1024];
	int cmd;

	m_cfd=Connect(host,port);
	if(m_cfd<0){
		m_error.Format("Connect to %s %d error!",host,port);
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}
	if(bcenter)
		return m_cfd;

	if(!ReadSockString(m_cfd,buf,sizeof(buf),"\r\n")){
		m_error.Format("Can not read response code!");
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}
		
	sscanf(buf,"%d",&cmd);
	if(cmd!=100){
		m_error.Format("The received response code is %d",cmd);
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}

	sprintf(buf,"%s\r\n",user);
	if(!write_all(m_cfd,buf,lstrlen(buf)))
	{
		m_error.Format("Can not write user to server");
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}
	
	if(!ReadSockResponse(m_cfd,buf,sizeof(buf),"\r\n",102))
	{
		m_error.Format("The received response code 102");
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}

	sprintf(buf,"%s\r\n",password);
	if(!write_all(m_cfd,buf,lstrlen(buf)))
	{
		m_error.Format("Can not write password to server");
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}

	if(!ReadSockResponse(m_cfd,buf,sizeof(buf),"\r\n",104))
	{
		m_error.Format("The received response code 104");
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}
	if(AskData(askdata))
		return m_cfd;
	return -1;
}


void CConDXSrv::CloseSrv()
{
	if(m_cfd!=-1){
		CloseSocket(m_cfd);
		m_cfd=-1;
	}
}

int CConDXSrv::AskData(char * askdata)
{
	char buf[1024];
	wsprintf(buf,"%s\r\n",askdata);
	if(!write_all(m_cfd,buf,lstrlen(buf)))
	{
		m_error.Format("Can not write ask data command to server");
		return 0;
	}

	if(!ReadSockResponse(m_cfd,buf,sizeof(buf),"\r\n",108))
	{
		m_error.Format("The received response code 108");
		return 0;
	}

	return 1;
}
BOOL CConDXSrv::Send(char *data,int data_len)
{
	char sync4[]={(char)0xbf,(char)0x13,(char)0x97,(char)0x74};
	struct timeval timeout;

	timeout.tv_sec=5;
	timeout.tv_usec=0;

	if(!IsWriteable(m_cfd,&timeout))
		return FALSE;

	if(write_all(m_cfd,sync4,4)<4)
		return FALSE;
	if(!IsWriteable(m_cfd,&timeout))
		return FALSE;
	if(write_all(m_cfd,data,data_len)<data_len)
		return FALSE;
	return TRUE;
}
