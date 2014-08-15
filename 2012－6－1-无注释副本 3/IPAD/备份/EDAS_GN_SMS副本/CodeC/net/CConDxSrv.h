#ifndef __CConDXSrv_H__
#define __CConDXSrv_H__

#include "csocketapi.h"
//�������ݽ�����������
class CConDXSrv : public CSocketAPI
{
public:
	CConDXSrv(){
		m_cfd=-1;
		m_error="";
	}
	//�����������ر�����
	~CConDXSrv();
	//���ӷ�������host:������ip��ַ��port:�˿ڣ�user:�û�����password���bcenter:1-���Ľڵ�,askdata: ������������
	//����: >0:��������������� -1:����ʧ�ܣ�ͨ��GetLastError��ѯʧ����Ϣ
	int ConnectCmdSrv(char * host,int port,char *user,char *password,int bcenter,char *askdata);
	//��������,data:Ҫ���͵����� data_len:data����,1-���ͳɹ�,0-ʧ��
	BOOL Send(char *data,int data_len);

	//�رյ������������ݷ�����������
	void CloseSrv();

	//ȡ���µĴ�����Ϣ�����أ�������Ϣ
	CString & GetLastError();
private:
	//�����������֣���ʼ��Ϊ0
	int m_cfd;
	//���´�����Ϣ
	CString m_error;

	//����������������������askdata:����������أ�1:����ɹ���0��ʧ�ܣ�ͨ��GetLastError��ѯʧ����Ϣ
	int AskData(char * askdata);

};

#endif