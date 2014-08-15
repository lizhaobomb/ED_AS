#include "stdafx.h"
#include "ccondatasrv.h"

CConDataSrv::~CConDataSrv()
{
	CloseSrv();
}

int CConDataSrv::ConnectCmdSrv(char * host,int port,char *user,char *password,int bcenter)
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
	if(cmd==108)
		return 1;
	else if(cmd!=100){
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

	if(!ReadSockResponse(m_cfd,buf,sizeof(buf),"\r\n",108))
	{
		m_error.Format("The received response code 108");
		CloseSocket(m_cfd);
		m_cfd=-1;
		return -1;
	}

	return m_cfd;
}

int CConDataSrv::ConnectDataSrv(char * host,int port)
{
	char tmp[1024];
	m_dfd=Connect(host,port);
	if(m_dfd<0){
		m_dfd=-1;
		return 0;
	}
	if(!ReadSockResponse(m_cfd,tmp,sizeof(tmp),"\r\n",104))
	{
		m_error.Format("The received response code 104");
		CloseSocket(m_dfd);
		m_dfd=-1;
		return -1;
	}
	return m_dfd;
}

void CConDataSrv::CloseSrv()
{
	if(m_dfd!=-1){
		CloseSocket(m_dfd);
		m_dfd=-1;
	}
	if(m_cfd!=-1){
		CloseSocket(m_cfd);
		m_cfd=-1;
	}
}

int CConDataSrv::AskData(char * cmd)
{
	char buf[1024];

	if(!ReadSockResponse(m_cfd,buf,sizeof(buf),"\r\n",109))
	{
		m_error.Format("The received response code 109");
		return 0;
	}

	strcpy(buf,"RTS ON\r\n");

	if(!write_all(m_cfd,buf,lstrlen(buf)))
	{
		m_error.Format("Can not write RTS ON to server");
		return 0;
	}

	if(!ReadSockResponse(m_cfd,buf,sizeof(buf),"\r\n",110))
	{
		m_error.Format("The received response code 110");
		return 0;
	}

	return 1;
}
