#ifndef __CCONDATASRV_H__
#define __CCONDATASRV_H__

#include "csocketapi.h"
//连接数据交换服务器类
class CConDataSrv : public CSocketAPI
{
public:
	CConDataSrv(){
		m_cfd=-1;
		m_dfd=-1;
		m_error="";
	}
	//析构函数：关闭连接
	~CConDataSrv();
	//连接命令服务器，host:服务器ip地址，port:端口，user:用户名，password口令，bcenter:1-中心节点
	//返回: >0:命令服务器连接字 -1:连接失败，通过GetLastError查询失败信息
	int ConnectCmdSrv(char * host,int port,char *user,char *password,int bcenter);

	//连接数据服务器，host:服务器ip地址，port:端口，返回: >0:数据服务器连接字 -1:连接失败，通过GetLastError查询失败信息
	int ConnectDataSrv(char * host,int port);

	//关闭到命令服务器和数据服务器的连接
	void CloseSrv();

	//向命令服务器发送数据请求命令，cmd:请求命令，返回：1:请求成功，0：失败，通过GetLastError查询失败信息
	int AskData(char * cmd);

	//取最新的错误信息，返回：错误信息
	CString & GetLastError();
private:
	//命令服务器连接字，初始化为0
	int m_cfd;
	//数据服务器连接字，初始化为0
	int m_dfd;
	//最新错误信息
	CString m_error;
};

#endif