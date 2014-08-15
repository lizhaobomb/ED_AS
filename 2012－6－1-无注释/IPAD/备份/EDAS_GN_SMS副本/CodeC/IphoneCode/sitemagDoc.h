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
	//1-��ʾ
	BOOL m_bshow;
	//����
//	BOOL Receive();
	//����
//	BOOL Send();
	
	void ProcessMessage(char * pdata,string & info);
	//�����������ݲɼ���
	void ResetSys();
	//1-����״̬
	BOOL IsConnected();
	//��������
	void ReConnect();
	//check network connection , return 1-connected 0- disconnected -1: disconnected and timeout
	int ProcessCon();
	//save parameter message
	void SaveMessage();
	//ͨ��
	CHANNEL * m_pchannels;
	//��ǰ����̨վ
	CSitelinkObj * m_selsite;
	//�����ļ���־
	BOOL m_save_status;

	DASCMDSTRUC m_das;
	//1--�����ú������Ҫ�������ɺ���Ч��0-��������
	BOOL m_btip;
	//0-����ʾgps���
	BOOL m_bshow_gps;
	float m_lat,m_lon,m_alt;

	//1-���ϵͳ��ѹ
	int m_bsysvolmon;
	//ϵͳ��ѹ���ӵĸ���ͨ������0��ʼ
	int m_sysvolad;
	//ϵͳ��ѹ����ֵ
	float m_sysvol;
	//1-�����λ��ѹ
	int m_bsensoffvolmon;
	//��λ��ѹ����ֵ
	float m_sensoffvol;	
public:
	//����ͨ���߳�
	CRcvThd * m_thd;
	
	int m_selSite;
	//CObList m_sitelst;
	vector<CSitelinkObj*> m_sitelst;

	BOOL m_badmin;

	//�ļ�ģʽ 1-�����棬0-��Сʱ��, 2-��ʱ����
	int m_bctn_mode;
	//��ʱ����ʱ����ʼʱ�� 
	long m_savefile_begin;
	//��ʱ����ʱ��ÿ���ļ����ȣ���λ����
	int m_savefile_len;

	//1-�ֽ�ɵ�ͨ��ascii�ļ�
	BOOL m_btxt_file;
	//�����ļ�
	CSaveEVTArray m_save_file;
	//��������ݲɼ����õ���̨վ����
	//STN_PAR m_stnpar[40];
	//1-������Ƭ�ϼ�¼״̬��0-ֹͣ��¼
	int m_rec_stat;

	pthread_t m_thread;
	//����״̬�Ի���
//	CConstatusDlg *	m_constatusdlg;
	string m_exe_path;
	//1-��ʾheartbeat
	BOOL m_bview_heartbeat;
	long m_begin_con;

 	//1-�����ʾ������
	BOOL m_bmsg_clr;

	//�¼���¼����
	int m_pre_evt,m_aft_evt;
	
	CHANNEL * m_pcur_chn;//current processed channel
	string m_old_msg;//old message before clear;
	string m_msg;//current message
	BOOL m_bshow_allmsg;//TRUE-show m_old_msg,FALSE-show m_msg
	
	int m_sensid;//the sensor id to be send message to, used at SensSet
 public:
	//����m_pchannel
	BOOL AllocateChannel();
	int AllocRevblk(REVBLKS *prev,long samp);
	//����m_thd
	BOOL AllocThd();
	//�ͷ�m_thd
	void FreeThd();

	//�ͷ�m_pchannel
	void FreeChannel();
	void FreeRevblk(REVBLKS *prev);

	//����̨վ
	BOOL ConnectSite();
	//�ر�����
	void CloseCon();
	
	void ChangeStatusbar(int index,string info);
	//װ��̨վ����
	BOOL LoadSiteinfo();
	//���������ļ�Ŀ¼
	void CreateDir();
	//�ͷ�̨վ��
	void FreeSitelst();
	//̨վ����flag:1 enable connection button
	int OnSelSite(int selIndex);
	//װ��̨վ����
	void LoadStnpar();
	BOOL SetStnpar(STN_PAR *spara,char * site,int local_id);
	BOOL SetChnPar(CHA_PAR *par,char * site,int local_id,int total,int comp);
	//���������ļ���channel:ͨ������
	BOOL CreateDataFile(int channel);
	//�ر��ļ� channel:ͨ������
	void CloseDataFile(int channel);
	//ת���ɷֵ���ascc�ļ� channel:ͨ������
	BOOL ConvertToASC(int channel);
	//д���ļ����� channel:ͨ������
	BOOL CopyDataToFl(int channel);
	//��������ɼ�����������
	void GetDASInfo();
	//��ѯ̨վ��Ϣ�� ����Ʋ���
	void SiteInquery(unsigned short cmd,int id);
	//���õ������ݲɼ���,�����Ի���ѡ��ͨ��
	void SensSet(unsigned short cmd,string & tip);
	//��ѯ״̬������
	void OnInqHeartInterval();
	//�����ʹ���
	void SendErr();

	/*������ֵ��pdata:�յ������ݣ�info:���ص���ʾ��Ϣ
	infoΪ���򲻵���Ϣ����ʾ*/
	//̨վ��Ϣ
	void OnStnpar(char * pdata,string & info);
	//�������Ϣ
	void OnSenspar(char * pdata,string & info);
	//������Ӧ
	void OnInstpar(char * pdata,string & info);

	//������
	void OnSmprate(char * pdata,string & info);
	//��ͨ
//	void OnHp(char * pdata,CString & info);
	//����
	void OnGain(char * pdata,string & info);
	//������ƫ
	void OnDasoff(char * pdata,string & info);
	
	//����궨
	void OnCalPulse(char * pdata,string & info);
	//���ұ궨
	void OnCalSine(char * pdata,string & info);
	//�����
	void OnCalPseudo(char * pdata,string & info);
	//ǿ��
	void OnCalStrong(char * pdata,string & info);
	//�궨�ȴ�ʱ��
	void OnCalDelay(char * pdata,string & info);
	//�յ������궨����
	void OnStartcal(char * pdata,string & info);
	//�յ�ֹͣ�궨����
	void OnStopcal(char * pdata,string & info);

	//��������ƻ�е���
	void OnCfgSensoff(char *pdata,string & info);
	//ֹͣ����Ƶ���
	void OnStopSensoff(char *pdata,string & info);
	//����Ƶ���״̬
	void OnSensAdjStat(char * pdata,string &info);
	//����
	void OnCmgLock(char *pdata,string & info);

	//ʵʱ���ݷ���,�������ݷ���,��ʵʱ���ݷ���
	void OnNetsrv(short cmd,char * pdata,string &info);
	//�������Ϳͻ���Ϣ
	void OnDataSrv(char * pdata,string & info);
	//ip����
	void OnIPUCom(char * pdata,string & info);
	//���ڷ��ͷ�ʽ
	void OnComctl(char * pdata,string & info);

	//�Զ�����
	void OnTrigauto(char * pdata,string & info);
	//�ۺ��ж�
	void OnDetevt(char * pdata,string & info);
	//�¼�����
	void OnEvtTrig(char * pdata,string & info);
	//��������
	void OnEvtDetrig(char * pdata,string & info);

	//�������
	void OnHeartbeat(char * pdata,string & info);
	//��ѹ
	void OnVOL(char * pdata,string & info);
	//�¶�
	void OnTEMP(char * pdata,string & info);
	//�Ӳ�
	void OnClkerr(char * pdata,string & info);
	//��س��ʱ��
	void OnBatcharge(char * pdata,string & info);
	//���ֹͣ���ʱ��
	void OnBatchargeStop(char * pdata,string & info);
	//GPS״̬
	void OnGpsStat(char * pdata,string & info);
	//������Ϣ
	void OnGetHeartbeat(char *pdata,string & info);
	//�洢��״̬
	void OnHdStat(char *pdata,string & info);
	//gps
	void OnGPS(char * pdata,string & info);
	//��ѯ����ƻ�е���
	void OnGetSensoff(char *pdata,string & info);
	//��ѯ������ź�
	void OnSensSig(char * pdata,string & info);

	//��¼����
	void OnWrctl(char * pdata,string & info);
	//��մ洢��
	void OnClearSD(char * pdata,string & info);
	//�ļ����ݲ���
	void OnBackupRule(char * pdata,string & info);
	//�����ַ
	void OnIp(char * pdata,string & info);
	//ʱ��
	void OnTimezone(char * pdata,string & info);
	//ϵͳʱ��
	void OnSysTm(char * pdata,string & info);
	//GPSʱ����
	void OnGpsInt(char * pdata,string & info);
	//ϵͳ����
	void OnResetSys(string & info);
	//�ָ���������
	void OnResettoDefault(string & info);
	//�ر�ϵͳ
	void OnStopSys(string & info);

	//�Ҷ�
	void OnLd(char * pdata,string & info);

	//�ֺ�����
	void OnZaiping(char * pdata,string & info);

	//�㲥֡��Ӧ
	void OnBroadcast(char * pdata,string & info);


	void ChangeChannelFactor(int sens_id,float factor);

	//��ѯ�����ź���λ
	void OnInqInoff();

	//��ѯ����ƵĲ�����
	void OnInqSmp(int sensid);

	//�¼���¼
	void OnEvtrec(char * pdata,string & info);

// Operations
public:
	//���沨��
	void SaveData();
	//return 1-connected 0-disconnected
	int CheckConStat();
	//��ѯgps������̨վ��������
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
