// sitemag.h : main header file for the SITEMAG application
//

#if !defined(AFX_SITEMAG_H__3213023B_CE4E_4447_87AA_2C6C8593C3F9__INCLUDED_)
#define AFX_SITEMAG_H__3213023B_CE4E_4447_87AA_2C6C8593C3F9__INCLUDED_

#include "CommonDef.h"
#include "sitemagstruc.h"
#include "cmdascmd.h"
#include "cmdasframe.h"

/////////////////////////////////////////////////////////////////////////////
// CSitemagApp:
// See sitemag.cpp for the implementation of this class
//

//�������
typedef struct tagCXTYPE{
	char name[100];//����
	char id[12];//�������
	int comp;//�����������
	char cxid[8][8];//�����������
	char cxname[8][100];//�����������
	int ssmode;//����,0-���������,1-�����,2-���������,3-���ٶ�,4-ǰ��
}CXTYPE;



class CSitemagApp
{
public:
	CSitemagApp();
	char m_taskpath[MAX_PATH];
	char m_logpath[MAX_PATH];
	char m_evtdir[MAX_PATH];
	char m_tiptext[MAX_PATH];
		//��ǰʱ��
	long m_current_time;
	int CheckTime(long time,long show_value);
	//long CalcStarttime(int method,int mon,int day,int hour,int min);
	long CalcStarttime(short tm_method, short tm_start1, short tm_start2, short tm_start3, short tm_start4);

	CXTYPE *m_sptype;//�������
	int m_total_type;//�����������
	//����������ļ�,��ʼ��m_combo_type,m_combo_ch1cxid(ch2cxid,ch3xcid)
	void LoadandInitCXID();

	//����smode��ȡ����������,smode: 0-������,1-���,2-�����,3-���ٶ�,4-ǰ��,����: ���������m_sptype�����,���û���ҵ�,�򷵻�0
	int GetCXID(int smode);
  
	//���ݲ������ id��ȡ����������,����: ���������m_sptype�����,���û���ҵ�,�򷵻�0
	int GetCXID(char *  id);
	
	//���ݲ����������ȡ�����������,type: m_sptype[type], cxid: ���������,cxname: �����������
	//����:1-�ҵ�,0-û�ҵ�
	int GetCXName(int type,char* cxid, string &cxname);
	
	//���ݲ����������ȡ����������� , cxid: ���������,cxname: �����������
	//����:1-�ҵ�,0-û�ҵ�
	int GetCXName( char* cxid,string &cxname);

	//���ݲ����������ȡ�����������,type: m_sptype[type], cxid: ���������,cxname: �����������
	//����:1-�ҵ�,0-û�ҵ�
	int GetCXSID(int type,const char* cxname,string & cxid);

// Overrides
	public:
	virtual BOOL InitInstance();
	BOOL InitInstance(char *home);
	virtual int ExitInstance();

};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SITEMAG_H__3213023B_CE4E_4447_87AA_2C6C8593C3F9__INCLUDED_)
