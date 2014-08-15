#ifndef __CConDXSrv_H__
#define __CConDXSrv_H__

#include "csocketapi.h"
//连接数据交换服务器类
class CConDXSrv : public CSocketAPI
{
public:
	CConDXSrv(){
		m_cfd=-1;
		m_error="";
	}
	//析构函数：关闭连接
	~CConDXSrv();
	//连接服务器，host:服务器ip地址，port:端口，user:用户名，password口令，bcenter:1-中心节点,askdata: 请求数据命令
	//返回: >0:命令服务器连接字 -1:连接失败，通过GetLastError查询失败信息
	int ConnectCmdSrv(char * host,int port,char *user,char *password,int bcenter,char *askdata);
	//发送数据,data:要发送的数据 data_len:data长度,1-发送成功,0-失败
	BOOL Send(char *data,int data_len);

	//关闭到服务器和数据服务器的连接
	void CloseSrv();

	//取最新的错误信息，返回：错误信息
	CString & GetLastError();
private:
	//服务器连接字，初始化为0
	int m_cfd;
	//最新错误信息
	CString m_error;

	//向服务器发送数据请求命令，askdata:请求命令，返回：1:请求成功，0：失败，通过GetLastError查询失败信息
	int AskData(char * askdata);

};

#endif