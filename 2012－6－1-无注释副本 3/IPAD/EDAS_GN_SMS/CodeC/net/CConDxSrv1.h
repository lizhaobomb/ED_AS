#ifndef __CConDXSrv_H__
#define __CConDXSrv_H__

#include "csocketapi.h"
//�������ݽ�����������
class CConDXSrv : public CSocketAPI
{
public:
	CConDXSrv(){
		m_cfd=m_dfd=-1;
		m_error="";
	}
	//�����������ر�����
	~CConDXSrv();
	//���ӷ�������host:������ip��ַ��port:�˿ڣ�user:�û�����password���bcenter:1-���Ľڵ�,askdata: ������������
	//����: >0:��������������� -1:����ʧ�ܣ�ͨ��GetLastError��ѯʧ����Ϣ
	int ConnectCmdSrv(char * host,int port,char *user,char *password,int bcenter,char *askdata);
	//��������,fd: ������ data:Ҫ���͵����� data_len:data����,1-���ͳɹ�,0-ʧ��
	BOOL Send(int fd,char *data,int data_len);

	//�رյ������������ݷ�����������
	void CloseSrv();
	
	//ȡ���ݷ�����������
	int GetDatafd(){return m_dfd;}
	//ȡ���µĴ�����Ϣ�����أ�������Ϣ
	CString & GetLastError();
private:
	//�����������֣���ʼ��Ϊ-1
	int m_cfd;
	//���ݷ����������֣���ʼ��Ϊ-1
	int m_dfd;
	//���´�����Ϣ
	CString m_error;

	//����������������������askdata:����������أ�1:����ɹ���0��ʧ�ܣ�ͨ��GetLastError��ѯʧ����Ϣ
	int AskData(char * askdata);
	//�������ݷ�����
	BOOL ConnectDataport(char * host,int data_port);
};

#endif