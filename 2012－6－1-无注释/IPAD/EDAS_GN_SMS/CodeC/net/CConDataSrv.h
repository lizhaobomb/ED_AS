#ifndef __CCONDATASRV_H__
#define __CCONDATASRV_H__

#include "csocketapi.h"
//�������ݽ�����������
class CConDataSrv : public CSocketAPI
{
public:
	CConDataSrv(){
		m_cfd=-1;
		m_dfd=-1;
		m_error="";
	}
	//�����������ر�����
	~CConDataSrv();
	//���������������host:������ip��ַ��port:�˿ڣ�user:�û�����password���bcenter:1-���Ľڵ�
	//����: >0:��������������� -1:����ʧ�ܣ�ͨ��GetLastError��ѯʧ����Ϣ
	int ConnectCmdSrv(char * host,int port,char *user,char *password,int bcenter);

	//�������ݷ�������host:������ip��ַ��port:�˿ڣ�����: >0:���ݷ����������� -1:����ʧ�ܣ�ͨ��GetLastError��ѯʧ����Ϣ
	int ConnectDataSrv(char * host,int port);

	//�رյ���������������ݷ�����������
	void CloseSrv();

	//��������������������������cmd:����������أ�1:����ɹ���0��ʧ�ܣ�ͨ��GetLastError��ѯʧ����Ϣ
	int AskData(char * cmd);

	//ȡ���µĴ�����Ϣ�����أ�������Ϣ
	CString & GetLastError();
private:
	//��������������֣���ʼ��Ϊ0
	int m_cfd;
	//���ݷ����������֣���ʼ��Ϊ0
	int m_dfd;
	//���´�����Ϣ
	CString m_error;
};

#endif