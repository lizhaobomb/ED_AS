// sitemagDoc.h : interface of the CSitemagDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITEMAGDOC_H__E7C240F5_EEB8_4563_888E_8FF1F71B3FFB__INCLUDED_)
#define AFX_SITEMAGDOC_H__E7C240F5_EEB8_4563_888E_8FF1F71B3FFB__INCLUDED_

#include "netrcv.h"
#include "csaveevtarray.h"
#include "cmdascmd.h"
#include "rcvthd.h"
#include "CommonDef.h"
#include "SitelinkObj.h"


//#include "constatusdlg.h"
class CSitemagDoc
{
public: // create from serialization only
	CSitemagDoc();

// Attributes
public:
	//1-settimer -1: nomessage
	int m_update_id;
	//1-显示
	BOOL m_bshow;
	//接收
//	BOOL Receive();
	//发送
//	BOOL Send();
	
	void ProcessMessage(char * pdata,string & info);
	//重新启动数据采集器
	void ResetSys();
	//1-连接状态
	BOOL IsConnected();
	//重新连接
	void ReConnect();
	//check network connection , return 1-connected 0- disconnected -1: disconnected and timeout
	int ProcessCon();
	//save parameter message
	void SaveMessage();
	//通道
	CHANNEL * m_pchannels;
	//当前处理台站
	CSitelinkObj * m_selsite;
	//保存文件标志
	BOOL m_save_status;

	DASCMDSTRUC m_das;
	//1--在设置后参数需要重启数采后生效，0-无需重启
	BOOL m_btip;
	//0-不显示gps结果
	BOOL m_bshow_gps;
	float m_lat,m_lon,m_alt;

	//1-监测系统电压
	int m_bsysvolmon;
	//系统电压连接的辅助通道，从0开始
	int m_sysvolad;
	//系统电压报警值
	float m_sysvol;
	//1-监测零位电压
	int m_bsensoffvolmon;
	//零位电压报警值
	float m_sensoffvol;	
public:
	//数据通信线程
	CRcvThd * m_thd;
	
	int m_selSite;
	//CObList m_sitelst;
	vector<CSitelinkObj*> m_sitelst;

	BOOL m_badmin;

	//文件模式 1-连续存，0-分小时存, 2-定时存盘
	int m_bctn_mode;
	//定时存盘时的起始时间 
	long m_savefile_begin;
	//定时存盘时的每个文件长度，单位：分
	int m_savefile_len;

	//1-分解成单通道ascii文件
	BOOL m_btxt_file;
	//保存文件
	CSaveEVTArray m_save_file;
	//缓存从数据采集器得到的台站参数
	//STN_PAR m_stnpar[40];
	//1-已启动片断记录状态，0-停止记录
	int m_rec_stat;

	pthread_t m_thread;
	//连接状态对话框
//	CConstatusDlg *	m_constatusdlg;
	string m_exe_path;
	//1-显示heartbeat
	BOOL m_bview_heartbeat;
	long m_begin_con;

 	//1-清除显示缓冲区
	BOOL m_bmsg_clr;

	//事件记录参数
	int m_pre_evt,m_aft_evt;
	
	CHANNEL * m_pcur_chn;//current processed channel
	string m_old_msg;//old message before clear;
	string m_msg;//current message
	BOOL m_bshow_allmsg;//TRUE-show m_old_msg,FALSE-show m_msg
	
	int m_sensid;//the sensor id to be send message to, used at SensSet
 public:
	//分配m_pchannel
	BOOL AllocateChannel();
	int AllocRevblk(REVBLKS *prev,long samp);
	//分配m_thd
	BOOL AllocThd();
	//释放m_thd
	void FreeThd();

	//释放m_pchannel
	void FreeChannel();
	void FreeRevblk(REVBLKS *prev);

	//连接台站
	BOOL ConnectSite();
	//关闭连接
	void CloseCon();
	
	void ChangeStatusbar(int index,string info);
	//装入台站资料
	BOOL LoadSiteinfo();
	//建立数据文件目录
	void CreateDir();
	//释放台站表
	void FreeSitelst();
	//台站管理，flag:1 enable connection button
	int OnSelSite(int selIndex);
	//装载台站参数
	void LoadStnpar();
	BOOL SetStnpar(STN_PAR *spara,char * site,int local_id);
	BOOL SetChnPar(CHA_PAR *par,char * site,int local_id,int total,int comp);
	//建立数据文件，channel:通道代码
	BOOL CreateDataFile(int channel);
	//关闭文件 channel:通道代码
	void CloseDataFile(int channel);
	//转换成分道的ascc文件 channel:通道代码
	BOOL ConvertToASC(int channel);
	//写入文件长度 channel:通道代码
	BOOL CopyDataToFl(int channel);
	//发送请求采集器参数命令
	void GetDASInfo();
	//查询台站信息类 地震计参数
	void SiteInquery(unsigned short cmd,int id);
	//设置地震数据采集器,弹出对话框选择通道
	void SensSet(unsigned short cmd,string & tip);
	//查询状态传输间隔
	void OnInqHeartInterval();
	//处理发送错误
	void SendErr();

	/*处理返回值：pdata:收到的数据，info:返回的显示信息
	info为空则不弹信息框显示*/
	//台站信息
	void OnStnpar(char * pdata,string & info);
	//地震计信息
	void OnSenspar(char * pdata,string & info);
	//仪器响应
	void OnInstpar(char * pdata,string & info);

	//采样率
	void OnSmprate(char * pdata,string & info);
	//高通
//	void OnHp(char * pdata,CString & info);
	//量程
	void OnGain(char * pdata,string & info);
	//数采零偏
	void OnDasoff(char * pdata,string & info);
	
	//脉冲标定
	void OnCalPulse(char * pdata,string & info);
	//正弦标定
	void OnCalSine(char * pdata,string & info);
	//随机码
	void OnCalPseudo(char * pdata,string & info);
	//强震
	void OnCalStrong(char * pdata,string & info);
	//标定等待时间
	void OnCalDelay(char * pdata,string & info);
	//收到启动标定命令
	void OnStartcal(char * pdata,string & info);
	//收到停止标定命令
	void OnStopcal(char * pdata,string & info);

	//调整地震计机械零点
	void OnCfgSensoff(char *pdata,string & info);
	//停止地震计调零
	void OnStopSensoff(char *pdata,string & info);
	//地震计调零状态
	void OnSensAdjStat(char * pdata,string &info);
	//锁摆
	void OnCmgLock(char *pdata,string & info);

	//实时数据服务,快速数据服务,非实时数据服务
	void OnNetsrv(short cmd,char * pdata,string &info);
	//主动发送客户信息
	void OnDataSrv(char * pdata,string & info);
	//ip串口
	void OnIPUCom(char * pdata,string & info);
	//串口发送方式
	void OnComctl(char * pdata,string & info);

	//自动触发
	void OnTrigauto(char * pdata,string & info);
	//综合判定
	void OnDetevt(char * pdata,string & info);
	//事件触发
	void OnEvtTrig(char * pdata,string & info);
	//触发结束
	void OnEvtDetrig(char * pdata,string & info);

	//心跳间隔
	void OnHeartbeat(char * pdata,string & info);
	//电压
	void OnVOL(char * pdata,string & info);
	//温度
	void OnTEMP(char * pdata,string & info);
	//钟差
	void OnClkerr(char * pdata,string & info);
	//电池充电时间
	void OnBatcharge(char * pdata,string & info);
	//电池停止充电时间
	void OnBatchargeStop(char * pdata,string & info);
	//GPS状态
	void OnGpsStat(char * pdata,string & info);
	//心跳信息
	void OnGetHeartbeat(char *pdata,string & info);
	//存储器状态
	void OnHdStat(char *pdata,string & info);
	//gps
	void OnGPS(char * pdata,string & info);
	//查询地震计机械零点
	void OnGetSensoff(char *pdata,string & info);
	//查询地震计信号
	void OnSensSig(char * pdata,string & info);

	//记录控制
	void OnWrctl(char * pdata,string & info);
	//清空存储器
	void OnClearSD(char * pdata,string & info);
	//文件备份策略
	void OnBackupRule(char * pdata,string & info);
	//网络地址
	void OnIp(char * pdata,string & info);
	//时区
	void OnTimezone(char * pdata,string & info);
	//系统时间
	void OnSysTm(char * pdata,string & info);
	//GPS时间间隔
	void OnGpsInt(char * pdata,string & info);
	//系统重启
	void OnResetSys(string & info);
	//恢复出厂设置
	void OnResettoDefault(string & info);
	//关闭系统
	void OnStopSys(string & info);

	//烈度
	void OnLd(char * pdata,string & info);

	//灾害评估
	void OnZaiping(char * pdata,string & info);

	//广播帧响应
	void OnBroadcast(char * pdata,string & info);


	void ChangeChannelFactor(int sens_id,float factor);

	//查询输入信号零位
	void OnInqInoff();

	//查询地震计的采样率
	void OnInqSmp(int sensid);

	//事件记录
	void OnEvtrec(char * pdata,string & info);

// Operations
public:
	//保存波形
	void SaveData();
	//return 1-connected 0-disconnected
	int CheckConStat();
	//查询gps被设置台站参数调用
	void OnSetGps();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSitemagDoc)
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSitemagDoc();

protected:

// Generated message map functions
public:
	//{{AFX_MSG(CSitemagDoc)
	 void OnRunSitemag(int selIndex);
	 void OnCfgSavemode();
	 //void OnUpdateCfgSavemode(CCmdUI* pCmdUI);
	 void OnRunConnect();
	 //void OnUpdateRunConnect(CCmdUI* pCmdUI);
	 void OnCfgSite();
	 void OnCfgSens();
	 void OnCfgInst();
	 void OnCfgSamp();
	 void OnCfgSmpctl();
	 void OnCfgGain(char *m_cmd, int m_cmd_len);
	 void OnCfgDasoff();
	 void OnCfgPseudocal();
	 void OnCfgPulsecal();
	 void OnCfgStrongcal();
	 void OnCfgSinecal();
 	 void OnCfgStartpseudo();
	 void OnCfgStartpulse();
	 void OnCfgStartsine();
	 void OnCfgStartstrong();
	 void OnCfgStopcal();
	 void OnCfgCalwait();
	 void OnCfgSensctlon();
	 void OnCfgSensctloff();
	 void OnCfgCmgsenson();
	 void OnCfgCmgsensoff();
	 void OnCfgRtssrv();
	 void OnCfgQksrv();
	 void OnCfgDatasrv();
	 void OnCfgCom();
	 void OnCfgComctl();
	 void OnCfgIp(char*m_cmd, int m_cmd_len);
	 void OnCfgAutotrig();
 	 void OnCfgDetevt();
	 void OnCfgHear();
	 void OnCfgBackrule();
	 void OnCfgTimezone();
	 void OnCfgSystime();
 	 void OnCfgResetsys();
	 void OnCfgReset();

	 //////////////////////////////////////////////////////
	 // UI to Data
	 //////////////////////////////////////////////////////
	/*
	 void OnUpdateCfgReset(CCmdUI* pCmdUI);
	 void OnUpdateCfgHear(CCmdUI* pCmdUI);
	 void OnUpdateCfgResetsys(CCmdUI* pCmdUI);
 	 void OnUpdateCfgAutotrig(CCmdUI* pCmdUI);
	 void OnUpdateCfgCom(CCmdUI* pCmdUI);
	 void OnUpdateCfgGain(CCmdUI* pCmdUI);
	 void OnUpdateCfgIp(CCmdUI* pCmdUI);
	 void OnUpdateCfgPseudocal(CCmdUI* pCmdUI);
	 void OnUpdateCfgPulsecal(CCmdUI* pCmdUI);
	 void OnUpdateCfgSamp(CCmdUI* pCmdUI);
	 void OnUpdateCfgSinecal(CCmdUI* pCmdUI);
	 void OnUpdateCfgSite(CCmdUI* pCmdUI);
	 void OnUpdateCfgStartpseudo(CCmdUI* pCmdUI);
	 void OnUpdateCfgStartpulse(CCmdUI* pCmdUI);
	 void OnUpdateCfgStartsine(CCmdUI* pCmdUI);
	 void OnUpdateCfgStartstrong(CCmdUI* pCmdUI);
	 void OnUpdateCfgStrongcal(CCmdUI* pCmdUI);
	 void OnUpdateCfgCmgsensoff(CCmdUI* pCmdUI);
	 void OnUpdateCfgCmgsenson(CCmdUI* pCmdUI);
	 void OnUpdateCfgDasoff(CCmdUI* pCmdUI);
	 void OnUpdateCfgDatasrv(CCmdUI* pCmdUI);
	 void OnUpdateCfgSens(CCmdUI* pCmdUI);
	 void OnUpdateCfgDetevt(CCmdUI* pCmdUI);
	 void OnUpdateCfgComctl(CCmdUI* pCmdUI);
	 void OnUpdateCfgSystime(CCmdUI* pCmdUI);
	 void OnUpdateCfgTimezone(CCmdUI* pCmdUI);
	 void OnUpdateCfgStopcal(CCmdUI* pCmdUI);
	 void OnUpdateCfgInst(CCmdUI* pCmdUI);
	 void OnUpdateCfgSmpctl(CCmdUI* pCmdUI);
	 void OnUpdateCfgCalwait(CCmdUI* pCmdUI);
	 void OnUpdateCfgRtssrv(CCmdUI* pCmdUI);
	 void OnUpdateCfgQksrv(CCmdUI* pCmdUI);
	 void OnUpdateCfgSensctloff(CCmdUI* pCmdUI);
	 void OnUpdateCfgSensctlon(CCmdUI* pCmdUI);
	 */
	 void OnInqDas();
	 void OnInqCal();
	 void OnInqGps();
	 void OnInqSens();
	 void OnInqSite();
	 void OnInqSensoff();
	 void OnInqNet();
	 void OnInqBackup();
	 void OnInqInst();
	 void OnInqTrig();
	 void OnInqDatasrv();
	 void OnInqTimezone();
	 void OnInqEnv();
	 void OnInqHdstat();
	/*
 	 void OnUpdateInqCal(CCmdUI* pCmdUI);
	 void OnUpdateInqDas(CCmdUI* pCmdUI);
	 void OnUpdateInqGps(CCmdUI* pCmdUI);
	 void OnUpdateInqSens(CCmdUI* pCmdUI);
	 void OnUpdateInqSite(CCmdUI* pCmdUI);
	 void OnUpdateInqSensoff(CCmdUI* pCmdUI);
	 void OnUpdateInqNet(CCmdUI* pCmdUI);
	 void OnUpdateInqEnv(CCmdUI* pCmdUI);
	 void OnUpdateInqHdstat(CCmdUI* pCmdUI);
	 void OnUpdateInqTimezone(CCmdUI* pCmdUI);
	 void OnUpdateInqBackup(CCmdUI* pCmdUI);
	 void OnUpdateInqInst(CCmdUI* pCmdUI);
	 void OnUpdateInqTrig(CCmdUI* pCmdUI);
	 void OnUpdateInqDatasrv(CCmdUI* pCmdUI);
	 */
	 void OnCompPulse();
	 void OnCompNoise();
	 void OnCompPath();
	 void OnCompPga();
	 void OnCompSine();
	 void OnCompSinepulse();
	 void OnCompUsedef();
	 void OnCompXcorr();
	 void OnCompZhendong();
	/*
	 void OnUpdateCompNoise(CCmdUI* pCmdUI);
	 void OnUpdateCompPga(CCmdUI* pCmdUI);
	 void OnUpdateCompPulse(CCmdUI* pCmdUI);
	 void OnUpdateCompSine(CCmdUI* pCmdUI);
	 void OnUpdateCompSinepulse(CCmdUI* pCmdUI);
	 void OnUpdateCompUsedef(CCmdUI* pCmdUI);
	 void OnUpdateCompXcorr(CCmdUI* pCmdUI);
	 void OnUpdateCompZhendong(CCmdUI* pCmdUI);
	 */
	 void OnCompAccpulse();
	 //void OnUpdateCompAccpulse(CCmdUI* pCmdUI);
	 void OnCompCompm();
	 //void OnUpdateCompCompm(CCmdUI* pCmdUI);
	 void OnCompFun();
	 //void OnUpdateCompFun(CCmdUI* pCmdUI);
	 void OnCompMulnoise();
	 //void OnUpdateCompMulnoise(CCmdUI* pCmdUI);
	 void OnCompMulpulse();
	 //void OnUpdateCompMulpulse(CCmdUI* pCmdUI);
	 void OnCompPrintmap();
	 //void OnUpdateCompPrintmap(CCmdUI* pCmdUI);
	 void OnCompSens();
	 //void OnUpdateCompSens(CCmdUI* pCmdUI);
	 //void OnUpdateCompSensnoise(CCmdUI* pCmdUI);
	 void OnCompSys();
	 //void OnUpdateCompSys(CCmdUI* pCmdUI);
	 void OnCompZpmap();
	 //void OnUpdateCompZpmap(CCmdUI* pCmdUI);
	 void OnCompSensnoise();
	 void OnCompUsedef2();
	 //void OnUpdateCompUsedef2(CCmdUI* pCmdUI);
	 void OnViewHeartbeat();
	 //void OnUpdateViewHeartbeat(CCmdUI* pCmdUI);
	 void OnFileSave();
	 //void OnUpdateFileSave(CCmdUI* pCmdUI);
	 void OnRunFtp();
	 //void OnUpdateRunFtp(CCmdUI* pCmdUI);
	 //void OnUpdateCfgBackrule(CCmdUI* pCmdUI);
	 void OnCfgSensctl();
	 //void OnUpdateCfgSensctl(CCmdUI* pCmdUI);
	 void OnInqSenssig();
	 //void OnUpdateInqSenssig(CCmdUI* pCmdUI);
	 void OnCfgEvtrec();
	 //void OnUpdateCfgEvtrec(CCmdUI* pCmdUI);
	
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_SITEMAGDOC_H__E7C240F5_EEB8_4563_888E_8FF1F71B3FFB__INCLUDED_)
