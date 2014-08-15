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
	//1-œ‘ æ
	BOOL m_bshow;
	//Ω” ’
//	BOOL Receive();
	//∑¢ÀÕ
//	BOOL Send();
	
	void ProcessMessage(char * pdata,string & info);
	//÷ÿ–¬∆Ù∂Ø ˝æ›≤…ºØ∆˜
	void ResetSys();
	//1-¡¨Ω”◊¥Ã¨
	BOOL IsConnected();
	//÷ÿ–¬¡¨Ω”
	void ReConnect();
	//check network connection , return 1-connected 0- disconnected -1: disconnected and timeout
	int ProcessCon();
	//save parameter message
	void SaveMessage();
	//Õ®µ¿
	CHANNEL * m_pchannels;
	//µ±«∞¥¶¿ÌÃ®’æ
	CSitelinkObj * m_selsite;
	//±£¥ÊŒƒº˛±Í÷æ
	BOOL m_save_status;

	DASCMDSTRUC m_das;
	//1--‘⁄…Ë÷√∫Û≤Œ ˝–Ë“™÷ÿ∆Ù ˝≤…∫Û…˙–ß£¨0-Œﬁ–Ë÷ÿ∆Ù
	BOOL m_btip;
	//0-≤ªœ‘ ægpsΩ·π˚
	BOOL m_bshow_gps;
	float m_lat,m_lon,m_alt;

	//1-º‡≤‚œµÕ≥µÁ—π
	int m_bsysvolmon;
	//œµÕ≥µÁ—π¡¨Ω”µƒ∏®÷˙Õ®µ¿£¨¥”0ø™ º
	int m_sysvolad;
	//œµÕ≥µÁ—π±®æØ÷µ
	float m_sysvol;
	//1-º‡≤‚¡„ŒªµÁ—π
	int m_bsensoffvolmon;
	//¡„ŒªµÁ—π±®æØ÷µ
	float m_sensoffvol;	
public:
	// ˝æ›Õ®–≈œﬂ≥Ã
	CRcvThd * m_thd;
	
	int m_selSite;
	//CObList m_sitelst;
	vector<CSitelinkObj*> m_sitelst;

	BOOL m_badmin;

	//Œƒº˛ƒ£ Ω 1-¡¨–¯¥Ê£¨0-∑÷–° ±¥Ê, 2-∂® ±¥Ê≈Ã
	int m_bctn_mode;
	//∂® ±¥Ê≈Ã ±µƒ∆ º ±º‰ 
	long m_savefile_begin;
	//∂® ±¥Ê≈Ã ±µƒ√ø∏ˆŒƒº˛≥§∂»£¨µ•Œª£∫∑÷
	int m_savefile_len;

	//1-∑÷Ω‚≥…µ•Õ®µ¿asciiŒƒº˛
	BOOL m_btxt_file;
	//±£¥ÊŒƒº˛
	CSaveEVTArray m_save_file;
	//ª∫¥Ê¥” ˝æ›≤…ºØ∆˜µ√µΩµƒÃ®’æ≤Œ ˝
	//STN_PAR m_stnpar[40];
	//1-“—∆Ù∂Ø∆¨∂œº«¬º◊¥Ã¨£¨0-Õ£÷πº«¬º
	int m_rec_stat;

	pthread_t m_thread;
	//¡¨Ω”◊¥Ã¨∂‘ª∞øÚ
//	CConstatusDlg *	m_constatusdlg;
	string m_exe_path;
	//1-œ‘ æheartbeat
	BOOL m_bview_heartbeat;
	long m_begin_con;

 	//1-«Â≥˝œ‘ æª∫≥Â«¯
	BOOL m_bmsg_clr;

	// ¬º˛º«¬º≤Œ ˝
	int m_pre_evt,m_aft_evt;
	BOOL be_wr;
    
	CHANNEL * m_pcur_chn;//current processed channel
	string m_old_msg;//old message before clear;
	string m_msg;//current message
	BOOL m_bshow_allmsg;//TRUE-show m_old_msg,FALSE-show m_msg
	
	int m_sensid;//the sensor id to be send message to, used at SensSet
    
    BOOL m_offset;//offset or not
 public:
	//∑÷≈‰m_pchannel
	BOOL AllocateChannel();
	int AllocRevblk(REVBLKS *prev,long samp);
	//∑÷≈‰m_thd
	BOOL AllocThd();
	// Õ∑≈m_thd
	void FreeThd();

	// Õ∑≈m_pchannel
	void FreeChannel();
	void FreeRevblk(REVBLKS *prev);

	//¡¨Ω”Ã®’æ
	BOOL ConnectSite();
	//πÿ±’¡¨Ω”
	void CloseCon();
	
	void ChangeStatusbar(int index,string info);
	//◊∞»ÎÃ®’æ◊ ¡œ
	BOOL LoadSiteinfo();
	//Ω®¡¢ ˝æ›Œƒº˛ƒø¬º
	void CreateDir();
	// Õ∑≈Ã®’æ±Ì
	void FreeSitelst();
	//Ã®’æπ‹¿Ì£¨flag:1 enable connection button
	int OnSelSite(int selIndex);
	//◊∞‘ÿÃ®’æ≤Œ ˝
	void LoadStnpar();
	BOOL SetStnpar(STN_PAR *spara,char * site,int local_id);
	BOOL SetChnPar(CHA_PAR *par,char * site,int local_id,int total,int comp);
	//Ω®¡¢ ˝æ›Œƒº˛£¨channel:Õ®µ¿¥˙¬Î
	BOOL CreateDataFile(int channel);
	//πÿ±’Œƒº˛ channel:Õ®µ¿¥˙¬Î
	void CloseDataFile(int channel);
	//◊™ªª≥…∑÷µ¿µƒasccŒƒº˛ channel:Õ®µ¿¥˙¬Î
	BOOL ConvertToASC(int channel);
	//–¥»ÎŒƒº˛≥§∂» channel:Õ®µ¿¥˙¬Î
	BOOL CopyDataToFl(int channel);
	//∑¢ÀÕ«Î«Û≤…ºØ∆˜≤Œ ˝√¸¡Ó
	void GetDASInfo();
	//≤È—ØÃ®’æ–≈œ¢¿‡ µÿ’º∆≤Œ ˝
	void SiteInquery(unsigned short cmd,int id);
	//…Ë÷√µÿ’ ˝æ›≤…ºØ∆˜,µØ≥ˆ∂‘ª∞øÚ—°‘ÒÕ®µ¿
	void SensSet(unsigned short cmd,string & tip);
	//≤È—Ø◊¥Ã¨¥´ ‰º‰∏Ù
	void OnInqHeartInterval();
	//¥¶¿Ì∑¢ÀÕ¥ÌŒÛ
	void SendErr();

	/*¥¶¿Ì∑µªÿ÷µ£∫pdata: ’µΩµƒ ˝æ›£¨info:∑µªÿµƒœ‘ æ–≈œ¢
	infoŒ™ø’‘Ú≤ªµØ–≈œ¢øÚœ‘ æ*/
	//Ã®’æ–≈œ¢
	void OnStnpar(char * pdata,string & info);
	//µÿ’º∆–≈œ¢
	void OnSenspar(char * pdata,string & info);
	//“«∆˜œÏ”¶
	void OnInstpar(char * pdata,string & info);

	//≤…—˘¬ 
	void OnSmprate(char * pdata,string & info);
	//∏ﬂÕ®
//	void OnHp(char * pdata,CString & info);
	//¡ø≥Ã
	void OnGain(char * pdata,string & info);
	// ˝≤…¡„∆´
	void OnDasoff(char * pdata,string & info);
	
	//¬ˆ≥Â±Í∂®
	void OnCalPulse(char * pdata,string & info);
	//’˝œ“±Í∂®
	void OnCalSine(char * pdata,string & info);
	//ÀÊª˙¬Î
	void OnCalPseudo(char * pdata,string & info);
	//«ø’
	void OnCalStrong(char * pdata,string & info);
	//±Í∂®µ»¥˝ ±º‰
	void OnCalDelay(char * pdata,string & info);
	// ’µΩ∆Ù∂Ø±Í∂®√¸¡Ó
	void OnStartcal(char * pdata,string & info);
	// ’µΩÕ£÷π±Í∂®√¸¡Ó
	void OnStopcal(char * pdata,string & info);

	//µ˜’˚µÿ’º∆ª˙–µ¡„µ„
	void OnCfgSensoff(char *pdata,string & info);
	//Õ£÷πµÿ’º∆µ˜¡„
	void OnStopSensoff(char *pdata,string & info);
	//µÿ’º∆µ˜¡„◊¥Ã¨
	void OnSensAdjStat(char * pdata,string &info);
	//À¯∞⁄
	void OnCmgLock(char *pdata,string & info);

	// µ ± ˝æ›∑˛ŒÒ,øÏÀŸ ˝æ›∑˛ŒÒ,∑« µ ± ˝æ›∑˛ŒÒ
	void OnNetsrv(short cmd,char * pdata,string &info);
	//÷˜∂Ø∑¢ÀÕøÕªß–≈œ¢
	void OnDataSrv(char * pdata,string & info);
	//ip¥Æø⁄
	void OnIPUCom(char * pdata,string & info);
	//¥Æø⁄∑¢ÀÕ∑Ω Ω
	void OnComctl(char * pdata,string & info);

	//◊‘∂Ø¥•∑¢
	void OnTrigauto(char * pdata,string & info);
	//◊€∫œ≈–∂®
	void OnDetevt(char * pdata,string & info);
	// ¬º˛¥•∑¢
	void OnEvtTrig(char * pdata,string & info);
    
    void OnEvtrecFile(char * pdata, string & info);
	//¥•∑¢Ω· ¯
	void OnEvtDetrig(char * pdata,string & info);

	//–ƒÃ¯º‰∏Ù
	void OnHeartbeat(char * pdata,string & info);
	//µÁ—π
	void OnVOL(char * pdata,string & info);
	//Œ¬∂»
	void OnTEMP(char * pdata,string & info);
	//÷”≤Ó
	void OnClkerr(char * pdata,string & info);
	//µÁ≥ÿ≥‰µÁ ±º‰
	void OnBatcharge(char * pdata,string & info);
	//µÁ≥ÿÕ£÷π≥‰µÁ ±º‰
	void OnBatchargeStop(char * pdata,string & info);
	//GPS◊¥Ã¨
	void OnGpsStat(char * pdata,string & info);
	//–ƒÃ¯–≈œ¢
	void OnGetHeartbeat(char *pdata,string & info);
	//¥Ê¥¢∆˜◊¥Ã¨
	void OnHdStat(char *pdata,string & info);
	//gps
	void OnGPS(char * pdata,string & info);
	//≤È—Øµÿ’º∆ª˙–µ¡„µ„
	void OnGetSensoff(char *pdata,string & info);
	//≤È—Øµÿ’º∆–≈∫≈
	void OnSensSig(char * pdata,string & info);

	//º«¬ºøÿ÷∆
	void OnWrctl(char * pdata,string & info);
	//«Âø’¥Ê¥¢∆˜
	void OnClearSD(char * pdata,string & info);
	//Œƒº˛±∏∑›≤ﬂ¬‘
	void OnBackupRule(char * pdata,string & info);
	//Õ¯¬Áµÿ÷∑
	void OnIp(char * pdata,string & info);
	// ±«¯
	void OnTimezone(char * pdata,string & info);
	//œµÕ≥ ±º‰
	void OnSysTm(char * pdata,string & info);
	//GPS ±º‰º‰∏Ù
	void OnGpsInt(char * pdata,string & info);
	//œµÕ≥÷ÿ∆Ù
	void OnResetSys(string & info);
	//ª÷∏¥≥ˆ≥ß…Ë÷√
	void OnResettoDefault(string & info);
	//πÿ±’œµÕ≥
	void OnStopSys(string & info);

	//¡“∂»
	void OnLd(char * pdata,string & info);

	//‘÷∫¶∆¿π¿
	void OnZaiping(char * pdata,string & info);

	//π„≤•÷°œÏ”¶
	void OnBroadcast(char * pdata,string & info);


	void ChangeChannelFactor(int sens_id,float factor);

	//≤È—Ø ‰»Î–≈∫≈¡„Œª
	void OnInqInoff();

	//≤È—Øµÿ’º∆µƒ≤…—˘¬ 
	void OnInqSmp(int sensid);

	// ¬º˛º«¬º
	void OnEvtrec(char * pdata,string & info);

// Operations
public:
	//±£¥Ê≤®–Œ
	void SaveData();
	//return 1-connected 0-disconnected
	int CheckConStat();
	//≤È—Øgps±ª…Ë÷√Ã®’æ≤Œ ˝µ˜”√
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
     void OnInqBiaoDing();
     void OnInqDataService();
     void OnInqEvtRec();
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
