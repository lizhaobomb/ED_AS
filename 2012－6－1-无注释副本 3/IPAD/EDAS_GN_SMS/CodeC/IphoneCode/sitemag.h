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

//测项代码
typedef struct tagCXTYPE{
	char name[100];//名称
	char id[12];//测项代码
	int comp;//测项分量总数
	char cxid[8][8];//测项分量代码
	char cxname[8][100];//测项分量名称
	int ssmode;//类型,0-测震短周期,1-测震宽,2-测震甚宽带,3-加速度,4-前兆
}CXTYPE;



class CSitemagApp
{
public:
	CSitemagApp();
	char m_taskpath[MAX_PATH];
	char m_logpath[MAX_PATH];
	char m_evtdir[MAX_PATH];
	char m_tiptext[MAX_PATH];
		//当前时间
	long m_current_time;
	int CheckTime(long time,long show_value);
	//long CalcStarttime(int method,int mon,int day,int hour,int min);
	long CalcStarttime(short tm_method, short tm_start1, short tm_start2, short tm_start3, short tm_start4);

	CXTYPE *m_sptype;//测项代码
	int m_total_type;//测项代码总数
	//读测项代码文件,初始化m_combo_type,m_combo_ch1cxid(ch2cxid,ch3xcid)
	void LoadandInitCXID();

	//根据smode获取测项代码序号,smode: 0-短周期,1-宽带,2-甚宽带,3-加速度,4-前兆,返回: 测项代码在m_sptype中序号,如果没有找到,则返回0
	int GetCXID(int smode);
  
	//根据测项代码 id获取测项代码序号,返回: 测项代码在m_sptype中序号,如果没有找到,则返回0
	int GetCXID(char *  id);
	
	//根据测项分量代码取测项分量名称,type: m_sptype[type], cxid: 项分量代码,cxname: 测项分量名称
	//返回:1-找到,0-没找到
	int GetCXName(int type,char* cxid, string &cxname);
	
	//根据测项分量代码取测项分量名称 , cxid: 项分量代码,cxname: 测项分量名称
	//返回:1-找到,0-没找到
	int GetCXName( char* cxid,string &cxname);

	//根据测项分量名称取测项分量代码,type: m_sptype[type], cxid: 项分量代码,cxname: 测项分量名称
	//返回:1-找到,0-没找到
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
