#include "stdafx.h"
#include "CConDXSrvV6.h"

CConDXSrvV6::~CConDXSrvV6()
{
	CloseSrv();
}

int CConDXSrvV6::ConnectCmdSrv(char * host,int port,char *user,char *password,int bcenter,char *askdata)
{
	char buf[1024];
	int cmd;
	int data_port;

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

	if(!ReadSockResponse(m_cfd,buf,sizeof(buf),"\r\n",107))
	{
		m_error.Format("Cannot pass user authentication");
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}
	
	CString s(buf);
	int index=s.Find("108");
	if(index>=0)
	{
		s=s.Right(s.GetLength()-index);
		wsprintf(buf,"%s",s);
	}else if(!ReadSockResponse(m_cfd,buf,sizeof(buf),"\r\n",108))
	{
		m_error.Format("Cannot receive data port");
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}
	sscanf(buf,"%*d %d %*s",&data_port);
	
	if(!ConnectDataport(host,data_port))
	{
		m_error.Format("Cannot connect to data port %d",data_port);
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
BOOL CConDXSrvV6::ConnectDataport(char * host,int data_port)
{
	m_dfd=Connect(host,data_port);
	if(m_dfd<=0)
	{
		m_dfd=-1;
		return FALSE;
	}
	return TRUE;
}

void CConDXSrvV6::CloseSrv()
{
	if(m_cfd!=-1){
		CloseSocket(m_cfd);
		m_cfd=-1;
	}
	if(m_dfd!=-1){
		CloseSocket(m_dfd);
		m_dfd=-1;
	}
}

int CConDXSrvV6::AskData(char * askdata)
{
	char buf[1024];
	wsprintf(buf,"%s\r\n",askdata);
	if(!write_all(m_cfd,buf,lstrlen(buf)))
	{
		m_error.Format("Can not write ask data command to server");
		return 0;
	}

	if(!ReadSockResponse(m_cfd,buf,sizeof(buf),"\r\n",110))
	{
		m_error.Format("The received response code 110");
		return 0;
	}

	return 1;
}
BOOL CConDXSrvV6::Send(int fd,char *data,int data_len)
{
//	char sync4[]={(char)0xbf,(char)0x13,(char)0x97,(char)0x74};
	struct timeval timeout;

	timeout.tv_sec=5;
	timeout.tv_usec=0;

	if(!IsWriteable(&fd,1,&timeout))
		return FALSE;
	if(write_all(fd,data,data_len)<data_len)
		return FALSE;
	return TRUE;
}
