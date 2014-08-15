// sitemagDoc.cpp : implementation of the CSitemagDoc class
 


//#include "sitemag.h"

#include "sitemagDoc.h"
#include "CommonDef.h"
#include "sitemag.h"


/*
#include "sitemagdlg.h"
#include "savemodedlg.h"
#include "datacore.h"
#include "gaindlg.h"
#include "hpdlg.h"
#include "ipdlg.h"
#include "ipucomdlg.h"
#include "pseudodlg.h"
#include "pulsedlg.h"
#include "senspardlg.h"
#include "selsensdlg.h"
#include "smpdlg.h"
#include "stnpardlg.h"
#include "strongdlg.h"
#include "trigautodlg.h"
#include "sinedlg.h"
#include "heartbeatdlg.h"
#include "dasoffdlg.h"
#include "mainfrm.h"
#include "datasrvcfg.h"
#include "showmsg.h"
#include "determinedlg.h"
#include "ComctlDlg.h"
#include "timezonedlg.h"
#include "systmdlg.h"
#include "backruledlg.h"
#include "ftpdlg.h"
#include "instpardlg.h"
#include "SetSyssmpDlg.h"
#include "CALDelayDlg.h"
#include "NetDataSrvDlg.h"
#include "senssigdlg.h"
#include "evtrecdlg.h"
 */
#include "list"
#include "GlobalData.h"

extern  CSitemagApp theApp;


char sens_id[]={'A','B','C','D','E','F','G','H','I','J','K','L'};//,'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
#define MAXSENSNUM sizeof(sens_id)

void GetLabel(std::vector<string> & slabel,int *unit,float * factor)
{
	FILE * fp;
	char file[256],buf[20];
	int n;
 	char label[1024];
//	int size=slabel.size();
	int size = 13;
	sprintf(file,"%s／cfg／channel.cfg",theApp.m_taskpath);

	fp=fopen(file,"r");
	if(fp==NULL)
	{
		for(n=0;n<size;n++)
		{
			sprintf(label,"CH%d",n+1);
			slabel.push_back(string(label));
			if(n<6)
			{
				unit[n]=0;
				factor[n]=1.f;
			}else if(n>=6 && n<12)
			{
				unit[n]=1;
				factor[n]=1.f;
			}else
			{
				unit[n]=4;
				factor[n]=10000000;
			}
		}
		return;
	}

 	n=0;
//	while(!feof(fp) && n<size)
	while(!feof(fp))
	{
		strcpy(buf,"");
		fscanf(fp,"%s %d %f\n",buf,&unit[n],&factor[n]);
		if(strlen(buf)>0){
			//slabel[n]=buf;
			slabel.push_back(string(buf));
		 	n++;
		}
	}
	fclose(fp);
	for(;n<size;n++)
	{
		sprintf(label,"CH%d",n+1);
		slabel.push_back(string(label));
		if(n<6)
		{
			unit[n]=0;
			factor[n]=1.f;
		}else if(n>=6 && n<12)
		{
			unit[n]=1;
			factor[n]=1.f;
		}else
		{
			unit[n]=4;
			factor[n]=10000000;
		}
	}
}

void* CreateRcvThread(void * pthd)
{
	
	((CRcvThd *)pthd)->main();
	return NULL;
}



/////////////////////////////////////////////////////////////////////////////
// CSitemagDoc construction/destruction

CSitemagDoc::CSitemagDoc()
{
	// TODO: add one-time construction code here
	m_bshow=FALSE;
	m_selsite=NULL;
	m_pchannels=NULL;
	m_save_status=FALSE;

	//m_bctn_mode=theApp.GetProfileInt("CFG","SAVE_MODE",0);
	//m_btxt_file=theApp.GetProfileInt("CFG","SAVE_TXT_FILE",0);
	m_bctn_mode= 0;
	m_btxt_file = 0;

	m_rec_stat=0;
	m_thread=NULL;
	AllocThd();
	m_btip=0;
	m_badmin=0;
//	m_constatusdlg=NULL;
	m_bview_heartbeat=TRUE;

	m_bshow_gps=TRUE;

	m_begin_con=-1;
	m_bmsg_clr=TRUE;
	m_savefile_len=3600;
	m_savefile_begin = time(NULL);

	m_pre_evt=m_aft_evt=10;
	
	m_update_id=0;
	//add by chenyang
	m_pcur_chn=NULL;
	m_old_msg="";
	m_msg="";
	m_bshow_allmsg=FALSE;
	m_sensid=0;
	
	//OnNewDocument();
	 
}

CSitemagDoc::~CSitemagDoc()
{
	CloseCon();
	FreeChannel();
	FreeSitelst();
	FreeThd();


}

BOOL CSitemagDoc::OnNewDocument()
{
	memset(&m_das,0,sizeof(m_das));
	m_das.stnpar.chn_sum=3;
	m_das.stnpar.id=0;
	strcpy(m_das.das.ip.ip,"0.0.0.0");
	strcpy(m_das.das.ip.gw,"0.0.0.0");
	strcpy(m_das.das.ip.mask,"255.255.255.0");

	//m_exe_path=theApp.GetProfileString("Config",_T("CAL_EXE"),"");
	m_exe_path = "";

	LoadSiteinfo();
	m_bshow=TRUE;

	//∂¡º‡≤‚µÁ—π„–÷µ
	FILE * fp;
	char name[256];

	sprintf(name,"%s/cfg/volmon.cfg",theApp.m_taskpath);
	fp=fopen(name,"r");
	if(fp==NULL)
		return TRUE;
	fscanf(fp,"%*s %d %d %f\n",&m_bsysvolmon,&m_sysvolad,&m_sysvol);
	fscanf(fp,"%*s %d %f\n",&m_bsensoffvolmon,&m_sensoffvol);
	fclose(fp);
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSitemagDoc commands
//∑÷≈‰ƒ⁄¥Ê
BOOL CSitemagDoc::AllocateChannel(){
	LOCALREVBLK * pblk;
	CHANNEL *tail,*pt;
	int chnid;
	char label[][10]={"垂直","东西","北南"};
	if(m_selsite==NULL)
		return FALSE;

	if(m_pchannels!=NULL)
		FreeChannel();
 

	try{
		chnid=0;
		pblk=m_selsite->m_plocal;

		for(int j=0;j<m_selsite->m_site.chn_sum;j++)//√ø∏ˆµÿ’º∆
		{
			for(int n=0;n<pblk->chn_sum;n++)//∑÷≈‰√ø∏ˆÕ®µ¿µƒƒ⁄¥Ê
			{
				tail=new CHANNEL;
				tail->point=NULL;
				if(n==0){
					//∑÷≈‰√ø∏ˆÕ®µ¿µƒ≤®–Œ ˝æ›Ω” ’∂”¡–
					if(!AllocRevblk(&tail->revblk,(MAX_SMP_RATE+100)*pblk->chn_sum))
						return 0;
					//∑÷≈‰√ø∏ˆµÿ’º∆µƒ≤Œ ˝Ω” ’∂”¡–£¨÷ª±£¥Ê‘⁄µÿ’º∆µƒµ⁄“ª∏ˆÕ®µ¿
					if(!AllocRevblk(&tail->cmdblk,250))
						return 0;
				}else tail->revblk.p_dat=tail->revblk.p_dat1=tail->cmdblk.p_dat=tail->cmdblk.p_dat1=NULL;

				tail->chn_id=chnid;
				strcpy(tail->stn,pblk->obs_name);
				sprintf(tail->chn,"%d",chnid+1);

				sprintf(tail->chn_label,"%s-%d-%s",tail->stn,j+1,label[n] );

				tail->bView=tail->bShow=1;
				tail->cnt=0;
				tail->data_cnt=0;
				tail->max_posi=0;
				tail->position=0;
				tail->maxvalue=0;
				tail->minvalue=0;
				tail->offset=0;
				tail->dsp.max=1024;
				tail->dsp.min=-1024;
				tail->dsp.unit=0;
				tail->factor=10;
				tail->curtm=0;
				tail->cali_flag=0;

				//±£¥Ê≤®–Œ◊¯±Í÷µ£¨∂‘”⁄∂‡∏ˆ ˝æ›µƒx◊¯±Í‘⁄Õ¨“ªxŒª÷√«Èøˆ£∫‘⁄CSitemageView::dsp_data÷–Ωˆ±£¥ÊY◊¯±Íµƒ◊Ó¥Û£¨◊Ó–°÷µ
				//“Ú¥À£¨∑÷≈‰¥Û–°£∫>=∆¡ƒªœ‘ æ«¯œÒÀÿ*2
				tail->point=new POINT[8000];
				tail->lpnext=NULL;
				if(m_pchannels==NULL)
					m_pchannels=tail;
				else pt->lpnext=tail;
				pt=tail;
				chnid++;
			}
			pblk=pblk->lpnext;
		}
		
	}catch(...){
		return 0;
	}
	return 1;
}

int CSitemagDoc::AllocRevblk(REVBLKS *prev,long samp)
{

	long size=samp*sizeof(long);
	prev->st=0;
	try{
		prev->p_dat = new char[60*sizeof(struct SHDAT1)];
		prev->p_dat1= new char[60*size];
	}catch(...){
		return 0;
	}
	prev->dat = (struct SHDAT1 *)prev->p_dat;

	int i = 0;
	for( i=0;i<60;i++){
		prev->dat[i].flg = -1;
		prev->dat[i].check_sum = 1;
		prev->dat[i].time=-1;
		prev->dat[i].ds = (long *)(prev->p_dat1+i*size);
	}
	for(i=0;i<59;i++)
		prev->dat[i].pn = prev->dat+i+1;
	prev->dat[i].pn=prev->dat;

	for(i=1;i<60;i++)
		prev->dat[i].pb = prev->dat+i-1;
	prev->dat[0].pb = &prev->dat[59];
	prev->p_rev=prev->p_dsp=prev->wr_dat=prev->dat;

	return 1;
}

BOOL CSitemagDoc::AllocThd()
{
	try{
		m_thd=new CRcvThd;
	}catch(...){
		return FALSE;
	}
	return TRUE;
}

void CSitemagDoc::ChangeChannelFactor(int sens_id,float factor)
{
	CHANNEL * pt=m_pchannels;
	int i = 0;
	for(i=0;i<sens_id;i++)
		pt=pt->lpnext;
	if(pt==NULL)
		return;
	for(i=0;i<3 && pt!=NULL;i++){
		if(pt->dsp.unit==1)
			pt->factor=factor;
		pt=pt->lpnext;
	}
}



void CSitemagDoc::CloseCon()
{
	if(m_thread!=NULL)
	{
		//TerminateThread(m_thread,0);
		pthread_cancel(m_thread);
		m_thread=NULL;
		m_thd->CloseCon();
		m_selsite=NULL;
		//UpdateAllViews(NULL,ID_DISCON);
	}
}


//Ω®¡¢œﬂ≥Ã£¨¡¨Ω”≤…ºØ∆˜
BOOL CSitemagDoc::ConnectSite()
{
	if(m_selsite==NULL)
		return 0;
	CloseCon();

	if(m_thd==NULL)
	{
		if(!AllocThd())
			return FALSE;
	}

	m_thd->Init(m_selsite,m_pchannels);
	/*
	long threadid;
	m_thread=CreateThread((LPSECURITY_ATTRIBUTES)NULL,           // No security attributes. 
                (DWORD)0,                              // Use same stack size. 
                (LPTHREAD_START_ROUTINE)CreateRcvThread, // Thread procedure. 
                (LPVOID)m_thd,                   // Parameter to pass. 
                (DWORD)0,                              // Run immediately. 
                 (LPDWORD)&threadid);
	*/
	int err = pthread_create(&m_thread, NULL, CreateRcvThread, (void *)m_thd);

	if(err != 0){
		printf("Cant't Create Thread %s \n", strerror(err));
		m_thread = NULL;
	}

	//CString tip,fmt;
	//fmt.LoadString(IDS_CONSTATUS);
	//tip.Format(fmt,m_selsite->m_site.net_con.host);

	//ChangeStatusbar(2,tip);
	//UpdateAllViews(NULL,ID_CHECKCON);

	//((CMainFrame *)theApp.m_pMainWnd)->m_wndToolBar.m_edit_cal.SetWindowText(_T(""));

	time(&m_begin_con);


	return TRUE;
}
BOOL CSitemagDoc::IsConnected()
{
	if(m_thd==NULL || m_thread==NULL)
		return FALSE;
	return m_thd->IsConnected();
}

int CSitemagDoc::CheckConStat()
{
	if(m_selsite==NULL)
		return 1;

	//CString fmt;

	if(IsConnected()==1 || m_begin_con>=0)
		return 1;
	return 0;
	//fmt.LoadString(IDS_RECONSTR);
	//ChangeStatusbar(2,fmt);

}
int CSitemagDoc::ProcessCon()
{
	if(!m_thd->IsConnected()){
		long t;
		time(&t);
		if(t-m_begin_con>120)
		{
		//	info="";
			/*
			UpdateAllViews(NULL,ID_KILLTIMER);

			CString tip;
			tip.LoadString(IDS_CONTIMEOUT);
			AfxMessageBox(tip);
			tip="";
			ChangeStatusbar(2,tip);
			*/
			m_begin_con=-1;

			return -1;
		}
		return 0;
	}

	//∑¢ÀÕ«Î«Û≤…ºØ∆˜≤Œ ˝√¸¡Ó
	if(m_badmin)
		GetDASInfo();
		
	//CString tip;

	//tip.LoadString(IDS_CONSTATUS1);
	//	ChangeStatusbar(2,tip);
	m_begin_con=-1;

	//UpdateAllViews(NULL,ID_KILLTIMER);

	return 1;
}

void CSitemagDoc::ChangeStatusbar(int index,string info)
{
	/*
	CStatusBar *pBar=(CStatusBar *)theApp.m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	if(pBar==NULL)
		return;
	pBar->SetPaneText(index,info,TRUE);
	*/
}

void CSitemagDoc::FreeThd()
{
	if(m_thd!=NULL){
		try{
			delete m_thd;
		}catch(...)
		{
			return ;
		};
		m_thd=NULL;
	}
}
	
void CSitemagDoc::FreeChannel()
{
	CHANNEL *pt;
	while(m_pchannels!=NULL){
		pt=m_pchannels;
		m_pchannels=m_pchannels->lpnext;
		FreeRevblk(&pt->revblk);
		FreeRevblk(&pt->cmdblk);
		if(pt->point!=NULL)
			delete pt->point;
		delete pt;
	}
}

void CSitemagDoc::FreeRevblk(REVBLKS *prev)
{
	if(prev->p_dat!=NULL)
		delete prev->p_dat;
	if(prev->p_dat1!=NULL)
		delete prev->p_dat1;
	prev->p_dat=NULL;
	prev->p_dat1=NULL;
}


void CSitemagDoc::FreeSitelst()
{
	/*
	for(vector<CSitelinkObj *>::iterator iter = m_sitelst.begin();
		iter!= m_sitelst.end(); ++iter)
	{
		
		
	}
	 */
	while(!m_sitelst.empty()){
		
		CSitelinkObj * p=(CSitelinkObj *)m_sitelst.at(0);
		delete p->m_plocal;
		delete p;
		vector<CSitelinkObj *>::iterator iter = m_sitelst.begin();
		m_sitelst.erase(iter);
		
	}
}


void CSitemagDoc::GetDASInfo()
{
	m_bmsg_clr=FALSE;
	//m_msg.clearbuf();

	OnInqSite();
	OnInqNet();
	OnInqDatasrv();
	OnInqDas();
	//OnInqDas();
	//OnInqCal();
	//gps
	OnInqGps();

	OnInqEnv();


	//OnInqSenssig();
 
	m_bmsg_clr=TRUE;
}

BOOL CSitemagDoc::LoadSiteinfo()
{
	int chn_sum;
	char user[128],passwd[128],name[20],host[128];
	int  cmd_port;
	SITEMAG sitemag;
	LOCALREVBLK * plocal;
	char file[MAX_PATH];
 	FILE * fp;

	sprintf(file,"%s/cfg/sitelst.cfg",theApp.m_taskpath);
	TRACE("%s\n",file);
	fp=fopen(file,"r");
	if(fp==NULL)
		return FALSE;

	while(!feof(fp))
	{
		strcpy(name,"");
		strcpy(host,"");
		strcpy(user,"");
		strcpy(passwd,"");
		chn_sum=0;

		fscanf(fp,"%s %d %s %d %s %s\n",name,&chn_sum,host,&cmd_port,user,passwd);
		
		if(strlen(name)==0 || chn_sum==0)
			break;

		strcpy(sitemag.site,name);
		sitemag.chn_sum=chn_sum/3;
		sitemag.protocol=1;
		
		strcpy(sitemag.net_con.host,host);
		strcpy(sitemag.net_con.user,user);
		strcpy(sitemag.net_con.passwd,passwd);
		sitemag.net_con.cmd_port=cmd_port;
		
		
		try{
			int i = 0;
			plocal=new LOCALREVBLK[sitemag.chn_sum];
			for( i=0;i<sitemag.chn_sum;i++){
				plocal[i].chn_sum=3;
			
				plocal[i].local_id=i;
				strcpy(plocal[i].obs_name,name);
			}
			for(i=0;i<sitemag.chn_sum-1;i++)
				plocal[i].lpnext=&plocal[i+1];
			plocal[i].lpnext=NULL;

			m_sitelst.push_back((CSitelinkObj *)new CSitelinkObj(&sitemag,plocal));

		}catch(...){
			return FALSE;
		}

	}
	fclose(fp);
	return TRUE;
}

int CSitemagDoc::OnSelSite(int selIndex)
{
	/*
	CSitemagDlg dlg;
	POSITION pos;
	CSitelinkObj * psel;

	dlg.m_sitelst=&m_sitelst;
	dlg.m_pselsite=m_selsite;
	dlg.m_benable_connect=flag;
	if(m_thd!=NULL)
		dlg.m_isconnected=m_thd->IsConnected();
	else dlg.m_isconnected=FALSE;

	int ret=dlg.DoModal();
	 */
	/*
	if(dlg.m_selsite>=0 && dlg.m_selsite<m_sitelst.GetCount())
	{
		pos=m_sitelst.FindIndex(dlg.m_selsite);
		psel=(CSitelinkObj *)m_sitelst.GetAt(pos);
		if(m_selsite==psel && m_thd!=NULL && m_thd->IsConnected())
			return 0;
		//settitle
		SetTitle(psel->m_site.site);

		m_selsite=psel;
		if(m_selsite!=NULL)
		{
			if(m_selsite->m_site.protocol==1){
				if(lstrcmp(m_selsite->m_site.net_con.user,"root")==0)
					m_badmin=1;
				else m_badmin=0;
			}else m_badmin=1;
		}
		return dlg.m_ret;
	}
	 */
	 CSitelinkObj *psel=(CSitelinkObj *)siteDoc.m_sitelst.at(selIndex);
	if(m_selsite==psel && m_thd!=NULL && m_thd->IsConnected())
		return 0;
	//settitle
	//SetTitle(psel->m_site.site);
	
	m_selsite=psel;
	if(m_selsite!=NULL)
	{
		if(m_selsite->m_site.protocol==1){
			if(strcmp(m_selsite->m_site.net_con.user,"root")==0)
				m_badmin=1;
			else m_badmin=0;
		}else m_badmin=1;
	}
	
	return 2;
}

void CSitemagDoc::OnViewHeartbeat() 
{
	m_bview_heartbeat=!m_bview_heartbeat;
}

/*
void CSitemagDoc::OnUpdateViewHeartbeat(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->SetCheck(m_bview_heartbeat);
}
*/

void CSitemagDoc::OnRunSitemag(int selIndex) 
{
	// TODO: Add your command handler code here
	int ret=OnSelSite(selIndex);
	if(!ret)
		return;

	AllocateChannel();
	//UpdateAllViews(NULL,ID_CHANGESITE);

	CreateDir();

	//initialize m_das
	memset(&m_das,0,sizeof(m_das));
	m_das.stnpar.sens_num=m_selsite->m_site.chn_sum;
	m_das.stnpar.id=-99999.f;
	m_das.stnpar.lat=m_das.stnpar.lon=m_das.stnpar.alt=-99999.f;
	if(m_selsite!=NULL && m_selsite->m_site.protocol==1)
		strcpy(m_das.das.ip.ip,m_selsite->m_site.net_con.host);
	else strcpy(m_das.das.ip.ip,"0.0.0.0");
	strcpy(m_das.das.ip.gw,"0.0.0.0");
	strcpy(m_das.das.ip.mask,"255.255.255.0");
	m_rec_stat=0;

	if(ret==2)
		ConnectSite();
		
}

void CSitemagDoc::OnCfgSavemode() 
{
	/*
	// TODO: Add your command handler code here
	CSaveModeDlg dlg;
	CString path(theApp.m_evtdir);

	dlg.m_bctn_mode=m_bctn_mode;
	dlg.m_btxt_file=m_btxt_file;
	dlg.m_evtpath=theApp.m_evtdir;
	dlg.m_tm_len=m_savefile_len;
	dlg.m_tm_begin=(CTime::GetCurrentTime()).GetTime();

	if(dlg.DoModal()==IDCANCEL)
		return;
	m_bctn_mode=dlg.m_bctn_mode;
	m_btxt_file=dlg.m_btxt_file;
	theApp.m_evtdir=dlg.m_evtpath;
	m_savefile_begin=dlg.m_tm_begin;
	m_savefile_len=dlg.m_tm_len;

	theApp.WriteProfileInt("CFG","SAVE_MODE",m_bctn_mode);
	theApp.WriteProfileInt("CFG","SAVE_TXT_FILE",m_btxt_file);
	theApp.WriteProfileString("CFG","DATA_PATH",theApp.m_evtdir);
	if(path.CompareNoCase(theApp.m_evtdir)!=0)
		CreateDir();
	 */
}
  

void CSitemagDoc::CreateDir()
{
	
	CSitelinkObj * psite;
	int count;
	LOCALREVBLK * pblk;
	string path;

	//CreateDirectory(m_evtdir,NULL);
	mkdir(theApp.m_evtdir, S_IRUSR | S_IWUSR | S_IXUSR);
	count=m_sitelst.size();
	for(int i=0;i<count;i++)
	{
		//pos=m_sitelst.at(i);
		//psite=(CSitelinkObj *)m_sitelst.GetAt(pos);
		char temp[512];
		psite=(CSitelinkObj *)m_sitelst.at(i);
		//path = m_evtdir + "\\" +psite->m_site.site;
		sprintf(temp, "%s/%s", theApp.m_evtdir, psite->m_site.site);
		//CreateDirectory(path,NULL);
		mkdir(temp, S_IRUSR | S_IWUSR | S_IXUSR);
		pblk=psite->m_plocal;
		while(pblk!=NULL){
			char buff[512];
			sprintf(buff, "%s/%s/%02d",theApp.m_evtdir,psite->m_site.site,pblk->local_id);
			//CreateDirectory(path,NULL);
			mkdir(buff, S_IRUSR | S_IWUSR | S_IXUSR);
			pblk=pblk->lpnext;
		}
	}
}
/*
void CSitemagDoc::OnUpdateCfgSavemode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_save_status==0);
}
 */

void CSitemagDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
	//CFileException e;
	CHANNEL * pchan;
	LOCALREVBLK * plocal;

	//save real time data to file
	if(m_save_status==0)//start save
	{
		string inf,fmt,s1,s;
		//fmt.LoadString(IDS_SAVEALERT);
		s="";
		plocal=m_selsite->m_plocal;
		while(plocal!=NULL)
		{
			char temp[512];
			sprintf(temp, "%s/cfg/%s%02d.cfg ",theApp.m_taskpath,m_selsite->m_site.site,plocal->local_id);
			//s1.Format("%s\\cfg\\%s%02d.cfg ",theApp.m_taskpath,m_selsite->m_site.site,plocal->local_id);
			s1 = temp;
			s+=s1;
			plocal=plocal->lpnext;
		}
		/*
		inf.Format(fmt,s);
		if(AfxMessageBox(inf,MB_YESNO)==IDNO)
			return;
		 */
		LoadStnpar();
		
		if(m_bctn_mode!=2)
		{
			//¡¨–¯¥Ê≈Ã
			plocal=m_selsite->m_plocal;
			pchan=m_pchannels;
			while(pchan!=NULL)
			{
				pchan->revblk.wr_dat=pchan->revblk.p_rev->pb;
				if(pchan->revblk.wr_dat->time<0)
					return;
				m_save_file.SetEvtTime(pchan->revblk.wr_dat->time,plocal->local_id);

				pchan->wr_cnt=0;
				for(int j=0;j<plocal->chn_sum;j++)
					pchan=pchan->lpnext;
				plocal=plocal->lpnext;
			}
		

		}else{
			plocal=m_selsite->m_plocal;
			pchan=m_pchannels;
			while(pchan!=NULL)
			{
				pchan->revblk.wr_dat=pchan->revblk.p_rev->pb;
			//	if(pchan->revblk.wr_dat->time<0)
			//		return;
				m_save_file.SetEvtTime(m_savefile_begin,plocal->local_id);

				pchan->wr_cnt=0;
				for(int j=0;j<plocal->chn_sum;j++)
					pchan=pchan->lpnext;
				plocal=plocal->lpnext;
			}
		}
		//m_save_file.m_flname.SetSize(m_selsite->m_site.chn_sum);
		int i = 0;
		for(i=0;i<m_selsite->m_site.chn_sum;i++)
		{
			if(!CreateDataFile(i))
				return;
		}
 		m_save_status=1;
		//MessageBeep(MB_OK);
	}else{
		//Õ£÷π¥Ê≈Ã
		int i = 0;
		for(i=0;i<m_selsite->m_site.chn_sum;i++)
		{
			CloseDataFile(i);
			m_save_status=0;
		}
	}		
}

/*
void CSitemagDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected());
	UINT id;
	CString s;

	if(!m_save_status)
		id=IDS_SAVEFILE;
	else id=IDS_STOPSAVE;
	s.LoadString(id);
	pCmdUI->SetText(s);
	if(id==IDS_SAVEFILE)
		pCmdUI->SetRadio(0);
	else pCmdUI->SetRadio(1);
	
}
*/
BOOL CSitemagDoc::CreateDataFile(int channel)
{
	/*
	CFileException e;
	m_save_file.m_flname[channel].Format("%s\\%s\\%02d\\%4d%02d%02d%02d%02d%02d%02d.dat",
				theApp.m_evtdir,m_selsite->m_site.site,channel,m_save_file.m_evthead[channel].data_start.year,m_save_file.m_evthead[channel].data_start.mon,
				m_save_file.m_evthead[channel].data_start.day,m_save_file.m_evthead[channel].data_start.hour,
				m_save_file.m_evthead[channel].data_start.min,m_save_file.m_evthead[channel].data_start.sec,channel);

	if(!m_save_file.m_file[channel].Open(m_save_file.m_flname[channel],CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite,&e))
	{
		CString err,fmt;
		fmt.LoadString(IDS_SAVEERR);
		err.Format(fmt,m_save_file.m_flname[channel]);
		AfxMessageBox(err);
		return FALSE;
	}
			
	m_save_file.m_file[channel].SeekToBegin();
	m_save_file.m_file[channel].Write(&m_save_file.m_evthead[channel],EVTHEADLEN);
	m_save_file.m_file[channel].Write(&m_save_file.m_stnpar[channel],STNLEN);
	int j = 0;
	for( j=0;j<m_save_file.m_stnpar[channel].comp;j++)
		m_save_file.m_file[channel].Write(&m_save_file.m_chnpar[channel][j],CHALEN);
	return TRUE;
	 */
	return TRUE;
}
void CSitemagDoc::CloseDataFile(int channel)
{	/*
	m_save_file.m_file[channel].Close();
	CopyDataToFl(channel);
	if(m_btxt_file)
		ConvertToASC(channel);
	 */
}
BOOL CSitemagDoc::CopyDataToFl(int channel)
{
	/*
	CFileException e;
	if(!m_save_file.m_file[channel].Open(m_save_file.m_flname[channel],CFile::modeReadWrite,&e))
		return FALSE;

	m_save_file.m_file[channel].SeekToBegin();
	m_save_file.m_file[channel].Write(&m_save_file.m_evthead[channel],EVTHEADLEN);
	m_save_file.m_file[channel].Abort();
	return TRUE;
	 */
	return TRUE;
}

BOOL CSitemagDoc::ConvertToASC(int channel)
{
	/*
	CFile f;
	FILE ***fp;
	NETEVT_HEAD evthead;
	STN_PAR *spara;
	int length,index;
	CString fl(m_save_file.m_flname[channel]),filename;
	char **data;
	CFileException e;

	if(!f.Open(m_save_file.m_flname[channel],CFile::modeRead|CFile::shareDenyNone,&e))
		return 0;

	index=fl.ReverseFind('.');
	fl=fl.Left(index);

	f.Read(&evthead,EVTHEADLEN);       
	length=EVTHEADLEN;

	fp=(FILE ***)new FILE**[evthead.netpar.stn_sum];
	spara=new STN_PAR[evthead.netpar.stn_sum];

	for(int i=0;i<evthead.netpar.stn_sum;i++)
	{
		f.Read(&spara[i],STNLEN);
		if(spara[i].wlen==24)
			spara[i].wlen=32;
		f.Seek(CHALEN*spara[i].comp,CFile::current);
		length+=STNLEN+CHALEN*spara[i].comp;
		fp[i]=(FILE **)new FILE *[spara[i].comp];
	}
	char cp[][3]={"ud","ew","ns"};
	for(i=0;i<evthead.netpar.stn_sum;i++)
	{
		for(int j=0;j<spara[i].comp;j++)
		{
			filename.Format("%s.%02d%c",fl,spara[i].no,cp[j][0]);
			fp[i][j]=fopen(filename,"w");
			if(fp[i][j]==NULL)
			{
				delete fp;
				delete spara;
				return 0;
			}
		}
	}
	
	data=new char *[evthead.netpar.stn_sum];
	for(i=0;i<evthead.netpar.stn_sum;i++)
		data[i]=new char[spara[i].samp*spara[i].wlen/8];

	for(int k=0;k<evthead.rec_length;k++)
	{
		for(int i=0;i<evthead.netpar.stn_sum;i++)
		{
			f.Seek(4,CFile::current);
			for(int j=0;j<spara[i].comp;j++)
			{
				f.Read(data[i],spara[i].samp*spara[i].wlen/8);
				for(int n=0;n<spara[i].samp;n++)
					fprintf(fp[i][j],"%d\n",::GetSampleData(n,spara[i].wlen,data[i]));
			}
		}
	}	   

	f.Close();
	for(i=0;i<evthead.netpar.stn_sum;i++)
	{
		for(int j=0;j<spara[i].comp;j++)
			fclose(fp[i][j]);
	}
	delete spara;
	for(i=0;i<evthead.netpar.stn_sum;i++)
	{
		delete data[i];
		delete fp[i];
	}
	delete fp;
	delete data;
	return TRUE;
	 */
	return TRUE;
}
//¥Ê≈Ã
void CSitemagDoc::SaveData()
{
//	TRACE("save\n");
	if(m_save_status==0)
		return;

	//CTime time;
	int year,mon,day,hour,min,sec;
	string s,s1;
	char buff[1500];

	CHANNEL *pchan=m_pchannels;
	LOCALREVBLK *plocal=m_selsite->m_plocal;
	int i = 0;
	for(i=0;i<m_selsite->m_site.chn_sum;i++)
	{
//		while(pchan->revblk.wr_dat->pn != NULL)
		while(pchan->revblk.wr_dat->flg > pchan->revblk.wr_dat->pb->flg)
		{
			if(pchan->revblk.wr_dat->time<m_save_file.m_evthead[i].data_begin)
			{
				pchan->revblk.wr_dat = pchan->revblk.wr_dat->pn;
				continue;
			}

			if(m_bctn_mode==0 || m_bctn_mode==2)
			{
				//time=pchan->revblk.wr_dat->time;
				struct tm * t = localtime(&pchan->revblk.wr_dat->time);
				year = t->tm_year;
				mon = t->tm_mon;
				day = t->tm_wday;
				hour = t->tm_hour;
				min = t->tm_min;
				sec = t->tm_sec;
				/*
				min=time.GetMinute();
				sec=time.GetSecond();
				day=time.GetDay();
				hour=time.GetHour();
				mon=time.GetMonth();
				year=time.GetYear();
			*/
				if( (m_bctn_mode==0 && ((sec==0 && min==0) || hour!=m_save_file.m_evthead[i].data_start.hour
					||mon!=m_save_file.m_evthead[i].data_start.mon || 
					day!=m_save_file.m_evthead[i].data_start.day || 
					year!=m_save_file.m_evthead[i].data_start.year)) || 	//∑÷–° ±¥Ê≈Ã
					(m_bctn_mode==2 && (pchan->revblk.wr_dat->time-m_save_file.m_evthead[i].data_begin >= m_savefile_len)) )//∂® ±¥Ê≈Ã
				{
					CloseDataFile(i);
					m_save_file.SetEvtTime(pchan->revblk.wr_dat->time,i);
					if(!CreateDataFile(i))
						return;
					pchan->wr_cnt=0;

				}
			}
			m_save_file.WriteDatablock((char *)&pchan->revblk.wr_dat->ds[1],pchan->revblk.wr_dat->time,pchan->revblk.wr_dat->cali_flag,i);
			pchan->wr_cnt++;
			pchan->revblk.wr_dat = pchan->revblk.wr_dat->pn;

		}
		for(int j=0;j<plocal->chn_sum;j++)
			pchan=pchan->lpnext;
	}
	//s.LoadString(IDS_SAVECNT);
	s =  "存盘：";
	pchan=m_pchannels;
	for(i=0;i<m_selsite->m_site.chn_sum;i++)
	{
		//s1.Format("%d ",pchan->wr_cnt);
		sprintf(buff, "%d ",pchan->wr_cnt);
		s1 = buff;
		s+=s1;
		for(int j=0;j<plocal->chn_sum;j++)
			pchan=pchan->lpnext;
	}

	ChangeStatusbar(1,s);


}
void CSitemagDoc::LoadStnpar()
{
	LOCALREVBLK * plocal;
	CHANNEL * pchan;
	char compname[][10]={"U-D","E-W","N-S"};

	plocal=m_selsite->m_plocal;
	pchan=m_pchannels;
	for(int i=0;i<m_selsite->m_site.chn_sum;i++)
	{
		m_save_file.m_evthead[i].netpar.stn_sum=1;
		strcpy(m_save_file.m_evthead[i].netpar.NetName,m_selsite->m_site.site);
		strcpy(m_save_file.m_evthead[i].EvtFlag,"digital event");
		strcpy(m_save_file.m_evthead[i].hostype,"PC");
		m_save_file.m_evthead[i].rectype=0;
		m_save_file.m_evthead[i].process=0;
		m_save_file.m_evthead[i].rec_length=0;
	
		if(!SetStnpar(&m_save_file.m_stnpar[i],m_selsite->m_site.site,plocal->local_id))
		{
			m_save_file.m_stnpar[i].oritype=1;
			m_save_file.m_stnpar[i].veloc=2;
			m_save_file.m_stnpar[i].weight=1;
			strcpy(m_save_file.m_stnpar[i].name,m_selsite->m_site.site);
		}
		m_save_file.m_stnpar[i].comp=plocal->chn_sum;
		m_save_file.m_stnpar[i].wlen=32;
		m_save_file.m_stnpar[i].samp=plocal->samp;
		if(m_das.stnpar.id!=-99999.f)
			m_save_file.m_stnpar[i].no=m_das.stnpar.id;
		m_save_file.m_stnpar[i].azimuth=m_save_file.m_stnpar[i].incident=(float)0;;
		m_save_file.m_stnpar[i].ClkErr=0;

		if(m_das.stnpar.lat!=-99999.f){
			m_save_file.m_evthead[i].netpar.centlat=m_das.stnpar.lat;
			m_save_file.m_evthead[i].netpar.centlon=m_das.stnpar.lon;
			m_save_file.m_evthead[i].netpar.centalt=m_das.stnpar.alt;

			m_save_file.m_stnpar[i].longitude=m_das.stnpar.lon;
			m_save_file.m_stnpar[i].latitude=m_das.stnpar.lat;
			m_save_file.m_stnpar[i].altitude=m_das.stnpar.alt;
		}
		int j = 0;
		for(j=0;j<plocal->chn_sum;j++)
		{
			if(!SetChnPar(&m_save_file.m_chnpar[i][j],m_selsite->m_site.site,plocal->local_id,plocal->chn_sum,j))
			{
				m_save_file.m_chnpar[i][j].no=j;
				m_save_file.m_chnpar[i][j].factor=(float)1.0;
				m_save_file.m_chnpar[i][j].respstep=0;
				strcpy(m_save_file.m_chnpar[i][j].name,compname[j]);
				memset(m_save_file.m_chnpar[i][j].resp,0,sizeof(m_save_file.m_chnpar[i][j].resp));
			}
		}
		for(j=0;j<plocal->chn_sum;j++)
			pchan=pchan->lpnext;
		plocal=plocal->lpnext;
	}
}

BOOL CSitemagDoc::SetStnpar(STN_PAR *spara,char * site,int local_id)
{
	char name[512];
	FILE * tf;
	char buf[1000];
	int weight,veloc,oritype,ssmode;

	sprintf(name, "%s/cfg/%s%02d.cfg",theApp.m_taskpath,site,local_id);

	tf=fopen(name,"r");
	if(tf==NULL)
		return FALSE;

	fscanf(tf,"%d %s\n",&spara->no,spara->name);
	fscanf(tf,"%f %f %f\n",&spara->longitude,&spara->latitude,&spara->altitude);
	fscanf(tf,"%s %d %d\n",spara->seismometer,&oritype,&ssmode);
	fscanf(tf,"%s %d %f\n",spara->DASType,&spara->wlen,&spara->VLtd);
	fscanf(tf,"%s",spara->tfin.tf.TranIns);
	fscanf(tf,"%d\n",&spara->tfin.tf.TranMode);
	fscanf(tf,"%s",spara->ClkType);
	fscanf(tf,"%d %f\n",&spara->tfin.tf.TimerMode,&spara->ClkErr);
	fscanf(tf,"%d\n",&weight);
	fscanf(tf,"%d\n",&veloc);
	fscanf(tf,"%d\n",&spara->samp);
	fscanf(tf,"%d\n",&spara->comp);
	spara->weight=weight;
	spara->veloc=veloc;
	spara->oritype=oritype;
	spara->ssmode=ssmode;
	fclose(tf);
	return TRUE;
}

BOOL CSitemagDoc::SetChnPar(CHA_PAR *par,char * site,int local_id,int total,int comp)	
{
	char name[512];
	char buf[1000];
	FILE * tf;
	char compname[]={'U','E','N'};

	sprintf(name,"%s/cfg/%s%02d.cfg",theApp.m_taskpath,site,local_id);
	tf=fopen(name,"r");
	if(tf==NULL)
		return FALSE;
	int i=0;
	for(i=0;i<10;i++)
		fgets(buf,1000,tf);

	for(i=0;i<total;i++)
	{
		fscanf(tf,"%s %f\n",buf,&par->factor);
		if(buf[0]==compname[comp])
		{
			int type,num;
			strcpy(par->name,buf);
			par->no=comp;
			fscanf(tf,"%d %d\n",&type,&num);
			if(type==1)
			{
				RESFAP resp;
				par->respstep=1;
				resp.type=FAPTYPE;
				resp.ntrip=num;
				memcpy(par->resp,&resp,8);
				resp.fap=(FAP *)new FAP[resp.ntrip];
				int n = 0;
				for(n=0;n<num;n++)
				{
					fscanf(tf,"%f",&resp.fap[n].freq);
					if(resp.fap[n].freq<0)
						resp.fap[n].freq=(float)-1/resp.fap[n].freq;
					else resp.fap[n].freq=(float)resp.fap[n].freq;
					resp.fap[n].phase=(float)0;
				}
	
				for(n=0;n<num;n++)
					fscanf(tf,"%f",&resp.fap[n].amp);
				
				for(n=0;n<num;n++)
					memcpy(&par->resp[8+n*12],&resp.fap[n],12);
				delete resp.fap;
				break;	
			}
			
		}else{
			fgets(buf,1000,tf);
			fgets(buf,1000,tf);
			fgets(buf,1000,tf);
		}
	}
	fclose(tf);
	return TRUE;
}        

void CSitemagDoc::OnRunConnect() 
{
	// TODO: Add your command handler code here
	if(m_thd==NULL)
		ConnectSite();
	else if(!m_thd->IsConnected())
		ConnectSite();
	else CloseCon();

}
void CSitemagDoc::ReConnect()
{
	if(m_thd!=NULL && m_thd->IsConnected())
		CloseCon();
	m_badmin=0;
	ConnectSite();
	m_badmin=1;
}

//void CSitemagDoc::OnUpdateRunConnect(CCmdUI* pCmdUI) 
//{
//	// TODO: Add your command update UI handler code here
//	UINT id;
//	CString s;
//	if(m_begin_con>=0)
//		return;
//	if(m_selsite==NULL)
//		pCmdUI->Enable(FALSE);
//	else pCmdUI->Enable(TRUE);
//	if(m_thd==NULL)
//		id=IDS_CONSTR;
//	else if(m_thd->IsConnected())
//		id=IDS_DISCONSTR;
//	else id=IDS_CONSTR;
//
//	s.LoadString(id);
//	pCmdUI->SetText(s);
//
//	if(id==IDS_DISCONSTR)
//		pCmdUI->SetRadio(1);
//	else pCmdUI->SetRadio(0);
//}

void CSitemagDoc::SendErr()
{
	//CString err;
//	//err.LoadString(IDS_SENDERR);
//	AfxMessageBox(err);
	TRACE("Send errr");
}

void CSitemagDoc::ResetSys()
{
	char buf[20];
	short *p=(short *)buf;

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	p[2]=0;
	p[3]=0x6078;
	p[4]=4;
	p[5]=0;
	p[6]=0;
	for(int i=2;i<6;i++)
		p[6]-=p[i];
	if(!m_thd->Send(buf,14))
		SendErr();

}

//////////////////////////////¥¶¿Ì ’µΩµƒ–≈œ¢//////////////////////


void CSitemagDoc::ProcessMessage(char * pdata,string & info)
{
	unsigned short *p=(unsigned short *)pdata;
	TRACE("cmd=%x\n",p[1]);
	switch(p[1])
	{
	case 0x7000:
		OnStnpar(pdata,info);
		break;
	case 0x7001:
		OnSenspar(pdata,info);
		break;
	case 0x7002:
		OnInstpar(pdata,info);
		break;
	case 0x7003:
	case 0x9003:
		OnSmprate(pdata,info);
		break;
//	case 0x2003:
//		OnHp(pdata,info);
//		break;
	case 0x7005:
	case 0x9005:
		OnGain(pdata,info);
		break;
	case 0x7006:
		OnDasoff(pdata,info);
		break;
	case 0x7030:
	case 0x7031:
	case 0x7035:
		OnNetsrv(p[1],pdata,info);
		break;
	case 0x7032:
		OnDataSrv(pdata,info);
		break;
	case 0x7033:
		OnIPUCom(pdata,info);
		break;
	case 0x7034:
		OnComctl(pdata,info);
		break;
	case 0x7020:
		OnCalPulse(pdata,info);
		break;
	case 0x7021:
		OnCalSine(pdata,info);
		break;
	case 0x7022:
		OnCalPseudo(pdata,info);
		break;
	case 0x7023:
		OnCalStrong(pdata,info);
		break;
	case 0x7024:
	case 0x7025:
	case 0x7026:
	case 0x7027:
		OnStartcal(pdata,info);
		break;
	case 0x7028://new
		OnStopcal(pdata,info);
		break;
	case 0x7029://new
		OnCalDelay(pdata,info);
		break;
	case 0x7050:
	case 0x9050:
		OnCfgSensoff(pdata,info);
		break;
	case 0x7051:
	case 0x9051:
		OnStopSensoff(pdata,info);
		break;
	case 0x7052:
		OnCmgLock(pdata,info);
		break;
	case 0x9053://new
		OnSensAdjStat(pdata,info);
		break;
	case 0x7054://new
		OnSensSig(pdata,info);
		break;
	case 0x7040:
		OnTrigauto(pdata,info);
		break;
	case 0x7041:
		OnDetevt(pdata,info);
		break;
	case 0x7060:
		OnHeartbeat(pdata,info);
		break;
	case 0x7061:
		OnVOL(pdata,info);
		break;
	case 0x7062:
		OnTEMP(pdata,info);
		break;
	case 0x7063:
		OnClkerr(pdata,info);
		break;
	case 0x7064:
		OnBatcharge(pdata,info);
		break;
	case 0x7065:
		OnGpsStat(pdata,info);
		break;
	case 0x7066:
		OnGetHeartbeat(pdata,info);
		break;
	case 0x7067:
		OnHdStat(pdata,info);
		break;
	case 0x7068:
		OnGPS(pdata,info);
		break;
	case 0x7069:
		OnGetSensoff(pdata,info);
		break;
	case 0x7070:
		OnWrctl(pdata,info);
		break;
	case 0x7071:
		OnClearSD(pdata,info);
		break;
	case 0x7072:
		OnBackupRule(pdata,info);
		break;
	case 0x7073:
		OnIp(pdata,info);
		break;
	case 0x7074:
		OnSysTm(pdata,info);
		break;
	case 0x7075:
		OnTimezone(pdata,info);
		break;
	case 0x7076:
		OnGpsInt(pdata,info);
		break;
	case 0x7077:
		OnResettoDefault(info);
		break;
	case 0x7078:
		OnResetSys(info);
		break;
	case 0x7079:
		OnStopSys(info);
		break;
	case 0x707e:
		OnEvtrec(pdata,info);
		break;
	case 0x90f0:
		OnEvtTrig(pdata,info);
		break;
	case 0x90f1:
		OnEvtDetrig(pdata,info);
		break;
	case 0x9000:
		OnBroadcast(pdata,info);
		break;
	case 0x90f2:
		OnLd(pdata,info);
		break;
	case 0x90f3:
		OnZaiping(pdata,info);
		break;

	}
}
//return message
void CSitemagDoc::OnLd(char * pdata,string & info)
{
	CM_SMALDFRM frm;
	const char *fmt = "\r\n触发时间: %d-%d-%d %d-%d-%.1f 烈度：%.1f\r\n垂直向PGA：%.1f(ga)l，东西向PGA：%.1f(gal)，北南向PGA：%.1f(gal)\r\n卓越周期：%.1f(Hz)，持续时间：%.1f秒\r\n1Hz反应谱：%.1f(gal)，2Hz反应谱：%.1f(gal)，5Hz反应谱：%.1f(gal)，8Hz反应谱：%.1f(gal)\r\n";
	char buff[1500];
	float sec;

	memcpy(&frm,pdata,sizeof(frm));
	struct tm * t = localtime(&frm.trig_time);	

	//CTime t( frm.trig_time);
	sec=(float)frm.frac_time/1000;
	
	sprintf(buff, fmt, t->tm_year,t->tm_mon,t->tm_wday,t->tm_hour,t->tm_min,t->tm_sec+sec,
			frm.ld,frm.pga_u,frm.pga_e,frm.pga_n,frm.td,frm.fp,frm.rsp_1,frm.rsp_2,frm.rsp_5,frm.rsp_8);	
	
	//info.Format(fmt,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond()+sec,
	//	frm.ld,frm.pga_u,frm.pga_e,frm.pga_n,frm.td,frm.fp,frm.rsp_1,frm.rsp_2,frm.rsp_5,frm.rsp_8);
	info = buff;
}
void CSitemagDoc::OnZaiping(char * pdata,string & info)
{
	CM_TAQPDFRM frm;
	const char *fmt = "\r\n%s台 触发时间: %d-%d-%d %d-%d-%.1f tao_c=%.2f PGD=%.1f(cm))\r\n";
	char buff[1500];
	float sec;

	memcpy(&frm,pdata,sizeof(frm));
	
	//fmt.LoadString(IDS_TAOCPDFMT);

	//CTime t( frm.trig_time);
	struct tm * t = localtime(&frm.trig_time);	
	sec=(float)frm.frac_time/1000;
	sprintf(buff,frm.name,t->tm_year,t->tm_mon,t->tm_wday,t->tm_hour,t->tm_min,t->tm_sec+sec,
			frm.tao_c,frm.pd);
	
	//info.Format(buff, fmt,frm.name,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond()+sec,
	//	frm.tao_c,frm.pd);
	info = buff;

}

void CSitemagDoc::OnStnpar(char * pdata,string & info)
{
//	const char *fmt="\r\n台号: %d 台站名: %s 台名缩写: %s\r\n台网标志: %s\r\n地震计总数: %d 采集通道总数: %d\r\n纬度: %.4f度 经度: %.4f度 高程: %.3f米\r\n起用日期: %s\r\n软件版本: %s\r\n";
    const char *fmt="\r\n台号: %d 台站名: %s 台名缩写: %s\r\n台网标志: %s\r\n纬度: %.4f度 经度: %.4f度 高程: %.3f米\r\n起用日期: %s\r\n软件版本: %s\r\n";
	char buff[1500];
	CM_STNPARFRM frm;
	int bchange;

	memcpy((char *)&frm,pdata,sizeof(frm));

	m_das.stnpar.id=frm.id;
	strcpy(m_das.stnpar.name,frm.name);
	strcpy(m_das.stnpar.abbr,frm.abbr);

	if(m_das.stnpar.sens_num!=frm.sens_sum)
		bchange=1;
	else bchange=0;

	m_das.stnpar.sens_num=frm.sens_sum;
	m_das.stnpar.chn_sum=frm.chn_sum;

	m_das.stnpar.lat=(float)frm.lat/3600000;
	m_das.stnpar.lon=(float)frm.lon/3600000;
	m_das.stnpar.alt=(float)frm.alt/100;
	strcpy(m_das.stnpar.run_date,frm.run_date);
	strcpy(m_das.stnpar.netid,frm.netid);
	strcpy(m_das.stnpar.version,frm.version);

	//fmt.LoadString(IDS_STNPARSTR);
	
	sprintf(buff, fmt,m_das.stnpar.id,m_das.stnpar.name,m_das.stnpar.abbr,m_das.stnpar.netid,
			 m_das.stnpar.lat,m_das.stnpar.lon,m_das.stnpar.alt,m_das.stnpar.run_date,m_das.stnpar.version);
			
	info = buff;
	
	//printf("%s \n", buff);
	/*
	info.Format(fmt,m_das.stnpar.id,m_das.stnpar.name,m_das.stnpar.abbr,m_das.stnpar.netid,m_das.stnpar.sens_num,m_das.stnpar.chn_sum,
		m_das.stnpar.lat,m_das.stnpar.lon,m_das.stnpar.alt,m_das.stnpar.run_date,m_das.stnpar.version);
	if(frm.sens_sum<=0)
	{
		CString s;
		s.LoadString(IDS_WARNRESETCFG);
		AfxMessageBox(s);
	}else if(bchange)
	{
		CString s;
		s.LoadString(IDS_RESTARTSYSSTR);
		AfxMessageBox(s);
	}
	 */
}

void CSitemagDoc::OnSenspar(char * pdata,string & info)
{
	string fmt,sr,dasstr,cxstr,s;
	char buff[2000];
	CM_SENSPARFRM frm;
	//UINT id;
 
	memcpy((char *)&frm,pdata,sizeof(frm));

	//fmt.LoadString(IDS_SENSPARSTR);
//	fmt = "\r\n地震计%c参数:\r\n型号:  %s\r\n分向数:  %d %s\r\n测项代码: %s\r\n记录类型:  %s\r\n标准类型: %s\r\n带宽: %s\r\n基岩: %s 埋深: %d厘米\r\n仪器响应文件:%s\r\n供货商: %s\r\n产品序号: %s\r\n";
    fmt = "\r\n加速度计参数:\r\n方位角:%f\r\n埋深: %d厘米\r\n岩基特性值: %s\r\n仪器响应文件:%s\r\n产品序号: %s\r\n";
	
	info="";

	if(frm.rectype[0]=='D')
		sr = "位移";
	else if(frm.rectype[0]=='A')
		sr ="加速度";
	else sr="速度";
	
	//sr.LoadString(id);
	
	dasstr="";
	cxstr="";
	int i = 0;
	for(i=0;i<frm.comp;i++)
	{
		//s.Format("%d ",frm.das_id[i]);
		sprintf(buff, "%d ",frm.das_id[i]);
		s = buff;
		dasstr+=s;
		theApp.GetCXName(frm.cxid[i],s);
		cxstr+=s;
		cxstr+=" ";
	}
	//cxstr = "";
	//info.Format(fmt,sens_id[frm.head.sens_id],frm.name,frm.comp,dasstr,cxstr,sr,frm.type,frm.bandstr,frm.rock,frm.depth,
	//		frm.rspfile,frm.provider,frm.series);
	sprintf(buff, fmt.c_str(),frm.azim,frm.depth,frm.rock,
			frm.rspfile,frm.series);
	info = buff;
	//info = "";
	//printf("-----------------------------\n");
	//printf("pppp=%s \n", buff);
	
	if(frm.head.sens_id<0 || frm.head.sens_id>=12)
		return;
	memcpy(&m_das.sens[frm.head.sens_id].sens_par,&frm.name,sizeof(frm)-4);
}

void CSitemagDoc::OnInstpar(char * pdata,string & info)
{
	string s1,s2,s,fmt;
	char buff[1500];
	CM_INSTPARFRM * frm=(CM_INSTPARFRM*)pdata;
	if(frm->head.sens_id>=0 && frm->head.sens_id<MAXSENSNUM)
	{
		memset(&m_das.sens[frm->head.sens_id].inst,0,sizeof(CM_INSTPARFRM));
		memcpy(&m_das.sens[frm->head.sens_id].inst,pdata,sizeof(CM_INSTPARFRM));
		fmt =  "\r\n加速度计 仪器响应：\r\n垂直向阻尼：%.1f 东西向阻尼：%.1f 北南向阻尼：%.1f\r\n垂直向自振周期：%.1f 东西向自振周期：%.1f 北南向自振周期(Hz)：%.1f\r\n垂直向灵敏度：%.1f 东西向灵敏度：%.1f 北南向灵敏度：%.1f\r\n归一化值：%e\r\n极点数：%d\r\n极点：\r\n%s零点数：%d\r\n零点：\r\n%s\r\n ";
		
		//fmt.LoadString(IDS_INSTTR);
		s1="";
		int i = 0;
		for(i=0;i<frm->pnum;i++)
		{
			sprintf(buff, "%e %e\r\n",frm->pole[i].real,frm->pole[i].image);
			s = buff;
			//s.Format("%e %e\r\n",frm->pole[i].real,frm->pole[i].image);
			s1+=s;
		}
		s2="";
		for(i=0;i<frm->znum;i++)
		{
			sprintf(buff, "%e %e\r\n",frm->zero[i].real,frm->zero[i].image);
			s = buff;
			//s.Format("%e %e\r\n",frm->zero[i].real,frm->zero[i].image);
			s2+=s;
		}
		sprintf(buff,fmt.c_str(),frm->ud_damp,frm->ew_damp,frm->ns_damp,frm->ud_freq,frm->ew_freq,frm->ns_freq,frm->ud,frm->ew,frm->ns,frm->ao,frm->pnum,s1.c_str(),frm->znum,s2.c_str());
		info = buff;
		//info.Format(fmt,sens_id[frm->head.sens_id],frm->ud_damp,frm->ew_damp,frm->ns_damp,frm->ud_freq,frm->ew_freq,frm->ns_freq,frm->ud,frm->ew,frm->ns,frm->ao,frm->pnum,s1,frm->znum,s2);
	}
}


void CSitemagDoc::OnSmprate(char * pdata,string & info)
{
	string fmt1,fmt2,s,s1;
	char buff[1500];
	int id ;
	int samp[]={1,10,20,50,100,125,200,500};

	CM_SMPRATEFRM frm;

	memcpy((char *)&frm,pdata,sizeof(frm));
//	frm.samp_id-=1;
	if(frm.samp_id<=0 || frm.samp_id>8)
	{
		//fmt1.LoadString(IDS_UNKNOWNSMP);
		fmt1 = "返回不可识别的采样率代码";
		//AfxMessageBox(fmt1);
		return;
	}

	if(frm.phase_id==1)
		//fmt1.LoadString(IDS_LINEARPSTR);
		fmt1 = "线性相位";
	else if(frm.phase_id==2)
		//fmt1.LoadString(IDS_MINPSTR);
		fmt1 = "最小相位";
	else {
		//fmt1.LoadString(IDS_UNKNOWNPHA);
		fmt1 = "返回不可识别的采样滤波器代码";
		//AfxMessageBox(fmt1);
		return;
	}
 	id=frm.head.sens_id;
 	if(id>=0 && id<MAXSENSNUM)
	{
		m_das.sens[id].smpstat[frm.samp_id-1].samp=samp[frm.samp_id-1];
		m_das.sens[id].smpstat[frm.samp_id-1].phase=frm.phase_id;
		m_das.sens[id].smpstat[frm.samp_id-1].stat=frm.be_ad;
		if(frm.be_syssamp){
			m_das.sens[id].samp=samp[frm.samp_id-1];
			m_das.sens[id].phase=frm.phase_id;
		}
	}

	if(frm.be_syssamp)
	{
		//fmt2.LoadString(IDS_SMPSTR2);
		fmt2 =  "\r\n加速度计：系统采样率%d %s\r\n";
		sprintf(buff, fmt2.c_str(),samp[frm.samp_id-1],fmt1.c_str() );
		//info.Format(fmt2,sens_id[id],samp[frm.samp_id-1],fmt1 ); 
		info = buff;
	}
		
}

void CSitemagDoc::OnGain(char * pdata,string & info)
{
	string fmt,fmt1;
	char buff[1500];
	CM_GAINFRM frm;
	float factor;

	memcpy((char *)&frm,pdata,sizeof(frm));

	if(frm.gain_id <0 || frm.gain_id>3)
	{
		//fmt.LoadString(IDS_UNKNOWNGAIN);
		//AfxMessageBox(fmt);
		fmt = "不可识别的量程代码";
		return;
	}
	if(frm.head.sens_id>=0 && frm.head.sens_id<MAXSENSNUM)
		m_das.sens[frm.head.sens_id].gain_id=frm.gain_id;

	//fmt.LoadString(IDS_GAINSTR);
	fmt = "\r\n加速度计%c：量程%s\r\n";
	
	if(frm.gain_id==0){
		fmt1="-10V ~ 10V";
		factor=10.f;
	}else if(frm.gain_id==1){
		fmt1="-20V ~ 20V";
		factor=20.f;
	}else if(frm.gain_id==2){
		fmt1="-5V ~ 5V";
		factor=5.f;
	}else if(frm.gain_id==3){
		fmt1="-2.5V ~ 2.5V";
		factor=2.5;
	}
	
	//info.Format(fmt,sens_id[frm.head.sens_id],fmt1);
	sprintf(buff, fmt.c_str(), sens_id[frm.head.sens_id],fmt1.c_str());
	ChangeChannelFactor(frm.head.sens_id,factor);
	info = buff;
}


void CSitemagDoc::OnDasoff(char *pdata,string & info)
{
	string fmt ;
	char buff[1500];
	CM_DASOFFFRM frm;
	std::vector<string> label;
	float factor[13];
	int unit[13];
	int n;
	LOCALREVBLK * pblk;

	//label.SetSize(13);
 
	GetLabel(label,unit,factor);

	memcpy((char *)&frm,pdata,sizeof(frm));

	n=0;
	pblk=m_selsite->m_plocal;
	int j = 0;
	for(j=0;j<frm.head.sens_id;j++)
	{
		n+=pblk->chn_sum;
		pblk=pblk->lpnext;
	}
	fmt = "\r\n设置的加速度计%c的零点修正值为：\r\n%s(count)：%d\r\n%s(count)：%d\r\n%s(count)：%d\r\n";
	//fmt.LoadString(IDS_DASOFFSTR2);
	//info.Format(fmt,sens_id[frm.head.sens_id], label[n],frm.ud,label[n+1],frm.ew,label[n+2],frm.ns);
	sprintf(buff, fmt.c_str(), sens_id[frm.head.sens_id], label.at(n).c_str(),frm.ud,label.at(n + 1).c_str(),frm.ew,label.at(n + 2).c_str(),frm.ns);
	info = buff;
}

void CSitemagDoc::OnNetsrv(short cmd,char * pdata,string & info)
{
	CM_NETSRVCTLFRM * net=(CM_NETSRVCTLFRM *)pdata;

	string fmt;
	char buff[1500];
	if(cmd==0x7030){
		//fmt.LoadString(IDS_RSNETSRVFMT);
		fmt = "\r\n实时网络数据服务器 控制端口: %d 起始数据端口: %d 最大连接数: %d 网络超时控制: %d秒\r\n";
		m_das.das.rts_netsrv.port=net->port;
		m_das.das.rts_netsrv.max_con=net->max_con;
		m_das.das.rts_netsrv.tm_ctl=net->tm_ctl;
		m_das.das.rts_netsrv.dataport=net->dataport;
	}else if(cmd==0x7031){
		//fmt.LoadString(IDS_QKNETSRVFMT);
		fmt = "\r\n快速网络数据服务器 控制端口: %d 起始数据端口: %d 最大连接数: %d 网络超时控制: %d秒\r\n";
		m_das.das.qk_netsrv.port=net->port;
		m_das.das.qk_netsrv.max_con=net->max_con;
		m_das.das.qk_netsrv.tm_ctl=net->tm_ctl;
		m_das.das.qk_netsrv.dataport=net->dataport;
	}else return ;

	//info.Format(fmt,net->port,net->dataport,net->max_con,net->tm_ctl);
	sprintf(buff, fmt.c_str(), net->port,net->dataport,net->max_con,net->tm_ctl);
	info = buff;

}

void CSitemagDoc::OnDataSrv(char * pdata,string & info)
{
	CM_CLIENTCONFRM frm;
	string fmt,s1,s2,s,s3,s4;
	char buff[1500];
	
	memcpy(&frm,pdata,sizeof(frm));
	info="";
	for(int i=0;i<4;i++)
	{
		if(frm.client[i].bon)
			//s1.LoadString(IDS_DATASRVONSTR);
			s1 = "启动发送服务";
		else //s1.LoadString(IDS_DATASRVOFFSTR);
			s1 =  "关闭发送服务";
		if(frm.client[i].broot)
			//s2.LoadString(IDS_DATASRVUSRSTR2);
			s2 = "管理员用户";
		else //s2.LoadString(IDS_DATASRVUSRSTR1);
			s2 = "普通用户";
		
		if(frm.client[i].be_broad==1)
			//s3.LoadString(IDS_BEBROAD);
			s3 = "发送广播帧";
		else //s3.LoadString(IDS_NOBROAD);
			s3 = "不发送广播帧";

		if(frm.client[i].be_stat==1)
			//s4.LoadString(IDS_BESTAT);
			s4 = "发送运行状态帧";
		else //s4.LoadString(IDS_NOSTAT);
			s4 =  "不发送运行状态信息";

		if(frm.client[i].bevent==0){
			//fmt.LoadString(IDS_DATASRVFRM);
			fmt = "接收服务器%d: %s 传送实时数据流  服务器地址: %s 端口: %d %s 加速度计: %s %s %s 超时控制: %d秒\r\n";
			sprintf(buff, fmt.c_str(),i+1,s1.c_str(),frm.client[i].host,frm.client[i].port,s2.c_str(),frm.client[i].sens,s3.c_str(),s4.c_str(),frm.client[i].tm_ctl);
			//s.Format(fmt,i+1,s1,frm.client[i].host,frm.client[i].port,s2,frm.client[i].sens,s3,s4,frm.client[i].tm_ctl);
			s = buff;
		}else if(frm.client[i].bevent==1){
			//fmt.LoadString(IDS_DATASRVFRM2);
			//s.Format(fmt,i+1,s1,frm.client[i].host,frm.client[i].port,s2,frm.client[i].sens,frm.client[i].pre_evt,frm.client[i].aft_evt,s3,s4,frm.client[i].tm_ctl);
			fmt = "接收服务器%d: %s 传送事件数据  服务器地址: %s 端口: %d %s 加速度计: %s 传送事件前%d秒数据 传送事件后%d秒数据 %s %s 超时控制: %d秒\r\n";
			sprintf(buff, fmt.c_str(),i+1,s1.c_str(),frm.client[i].host,frm.client[i].port,s2.c_str(),frm.client[i].sens,frm.client[i].pre_evt,frm.client[i].aft_evt,s3.c_str(),s4.c_str(),frm.client[i].tm_ctl);
			s = buff;
		}else {
			//fmt.LoadString(IDS_DATASRVFRM3);
			//s.Format(fmt,i+1,s1,frm.client[i].host,frm.client[i].port,s2,frm.client[i].sens,s3,s4,frm.client[i].tm_ctl);
			fmt =  "接收服务器%d: %s 传送烈度数据  服务器地址: %s 端口: %d %s 加速度计: %s %s %s 超时控制: %d秒\r\n";
			sprintf(buff, fmt.c_str(), i+1,s1.c_str(),frm.client[i].host,frm.client[i].port,s2.c_str(),frm.client[i].sens,s3.c_str(),s4.c_str(),frm.client[i].tm_ctl);
			s = buff;
		}

		info+=s;
		m_das.das.data_srv[i].id=frm.client[i].id;
		m_das.das.data_srv[i].bon=frm.client[i].bon;
		m_das.das.data_srv[i].broot=frm.client[i].broot;
		m_das.das.data_srv[i].bevent=frm.client[i].bevent;
		m_das.das.data_srv[i].aft_evt=frm.client[i].aft_evt;
		m_das.das.data_srv[i].pre_evt=frm.client[i].pre_evt;
		m_das.das.data_srv[i].port=frm.client[i].port;
		m_das.das.data_srv[i].be_broad=frm.client[i].be_broad;
		m_das.das.data_srv[i].be_stat=frm.client[i].be_stat;
		strcpy(m_das.das.data_srv[i].host,frm.client[i].host);
		strcpy(m_das.das.data_srv[i].sens,frm.client[i].sens);
		m_das.das.data_srv[i].tm_ctl=frm.client[i].tm_ctl;
	}

}


void CSitemagDoc::OnIPUCom(char * pdata,string & info)
{
	string fmt,s;
	char buff[1500];
	CM_COMBAUDFRM frm;

	memcpy((char *)&frm,pdata,sizeof(frm));

	//fmt.LoadString(IDS_IPUCOMSTR);
	fmt = "\r\n与数据采集器通信的波特率为%d\r\n";
	m_das.das.comctl.baud=frm.baud;
	sprintf(buff, fmt.c_str(),m_das.das.comctl.baud);
	info = buff;
	//info.Format(fmt,m_das.das.comctl.baud);
}

void CSitemagDoc::OnComctl(char * pdata,string & info)
{
	string s1,s2,s3,s4;
	CM_COMCTLFRM frm;

	memcpy(&frm,pdata,sizeof(CM_COMCTLFRM));
	m_das.das.comctl.back=frm.back;
	m_das.das.comctl.binit=frm.send;
	m_das.das.comctl.be_broad=frm.be_broad;
	m_das.das.comctl.be_stat=frm.be_stat;

	if(m_das.das.comctl.back)
	{
		s1 = "应答方式";
		//s1.LoadString(IDS_COMCTLFMT1);
	}
	else //s1.LoadString(IDS_COMCTLFMT2);
		s1 = "非应答方式";

	if(m_das.das.comctl.binit)
		//s2.LoadString(IDS_COMCTLFMT3);
		s2 = "系统启动后直接发送波形";
	else //s2.LoadString(IDS_COMCTLFMT4);
		s2 =  "等待请求后发送";

	if(m_das.das.comctl.be_broad)
		//s3.LoadString(IDS_BEBROAD);
		s3 = "发送广播帧";
	else //s3.LoadString(IDS_NOBROAD);
		s3 = "不发送广播帧";

	if(m_das.das.comctl.be_stat)
		//s4.LoadString(IDS_BESTAT);
		s4 = "发送运行状态帧";
	else //s4.LoadString(IDS_NOSTAT);
		s4 = "不发送运行状态信息";

	info=s1+" "+s2+" "+s3+" "+s4;
	info+="\r\n";
}


void CSitemagDoc::OnCalPulse(char * pdata,string & info)
{
	string fmt,fmt1,s,s1;
	int id;
	//CTime t;
	char buff[1500];
	CM_PULSEFRM frm;

	memcpy((char *)&frm,pdata,sizeof(frm));
	if(frm.time<0)
		return;
	if(frm.amptype==0)
		//fmt.LoadString(IDS_PULSESTR1);
		fmt = "\r\n地震计%c脉冲标定参数：\r\n%s\r\n第一次启动时间：%04d-%02d-%02d %02d:%02d:%02d\r\n定时方式：%s %s\r\n脉冲幅度：%d(cnt)\r\n脉冲宽度：%d秒\r\n";
	else //fmt.LoadString(IDS_PULSESTR2);
		fmt = "\r\n地震计%c脉冲标定参数：\r\n%s\r\n第一次启动时间：%04d-%02d-%02d %02d:%02d:%02d\r\n定时方式：%s %s\r\n脉冲幅度：%d(uA)\r\n脉冲宽度：%d秒\r\n";
	if(frm.head.sens_id>=0 && frm.head.sens_id<MAXSENSNUM)
	{
		id=frm.head.sens_id;
		m_das.sens[id].pulse.btimer=frm.btimer;
		m_das.sens[id].pulse.amp=frm.amp;
		m_das.sens[id].pulse.dur=frm.dur;
		m_das.sens[id].pulse.time=frm.time;
		m_das.sens[id].pulse.tm_start1=frm.tm_start1;
		m_das.sens[id].pulse.tm_start2=frm.tm_start2;
		m_das.sens[id].pulse.tm_start3=frm.tm_start3;
		m_das.sens[id].pulse.tm_start4=frm.tm_start4;
		m_das.sens[id].pulse.method=frm.method;
		m_das.sens[id].pulse.amptype=frm.amptype;

	}
	if(frm.btimer==1)
		//fmt1.LoadString(IDS_ENABLESTR1);
		fmt1 = "允许定时启动";
	else //fmt1.LoadString(IDS_DISABLESTR1);
		fmt1 = "不允许定时启动";

	//UINT ids[]={IDS_TMMETHOD1,IDS_TMMETHOD2,IDS_TMMETHOD3,IDS_TMMETHOD4,IDS_TMMETHOD5};
	string ids[]={"每日", "每星期","每月","每年","小时"};
	if(frm.method>=0 && frm.method<=4)
		//s.LoadString(ids[frm.method]);
		s = ids[frm.method];
	else s="";

	if(frm.method==0)//every day
	{
		//s1.Format("%d:%d",frm.tm_start1,frm.tm_start2);
		sprintf(buff, "%d:%d",frm.tm_start1,frm.tm_start2);
		s1 = buff;
	}
	else if(frm.method==1)
	{
		//√ø–«∆⁄ 
		//UINT sid[]={IDS_DATE1,IDS_DATE2,IDS_DATE3,IDS_DATE4,IDS_DATE5,IDS_DATE6,IDS_DATE7};
		string sid[]={"星期一","星期二", "星期三","星期四","星期五","星期六","星期日"};
		string s2 = sid[frm.tm_start1-1];
		//s2.LoadString(sid[frm.tm_start1-1]);
		//s1.Format("%s %d:%d",s2,frm.tm_start2,frm.tm_start3);
		sprintf(buff, "%s %d:%d",s2.c_str(),frm.tm_start2,frm.tm_start3);
		s1 = buff;
		

	}else if(frm.method==2)//every month
	{
		//s1.Format("%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3);
		sprintf(buff,"%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3);
		s1 = buff;
	}
	else if(frm.method==3)
	{
		//s1.Format("%d-%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3,frm.tm_start4);
		sprintf(buff,"%d-%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3,frm.tm_start4);
		s1 = buff;
	}
	else {
		//s1.Format("%d",frm.tm_start1);
		sprintf(buff,"%d",frm.tm_start1);
		s1 = buff;
	}


	//t=frm.time;
	struct tm * t = localtime(&frm.time);
	
	//info.Format(fmt,sens_id[id],fmt1,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),
	//	s,s1,frm.amp,(unsigned short)(0.1f*frm.dur));
	sprintf(buff, fmt.c_str(),sens_id[id],fmt1.c_str(),t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,
				s.c_str(),s1.c_str(),frm.amp,(unsigned short)(0.1f*frm.dur));
	
	info= buff;

}
void CSitemagDoc::OnCalSine(char * pdata,string & info)
{
	string fmt,fmt1,s,s1;
	char buff[1500];
	int id;
	//CTime t;
	CM_SINEFRM frm;
	short * p;
	memcpy((char *)&frm,pdata,sizeof(frm));
	if(frm.time<0)
		return;
	if(frm.amptype==0)
		//fmt.LoadString(IDS_SINESTR1);
		fmt = "\r\n地震计%c正弦标定参数：\r\n%s\r\n第一次启动时间：%04d-%02d-%02d %02d:%02d:%02d\r\n定时方式：%s %s\r\n频点数：%d\r\n周期数\t频率/周期\t衰减因子\r\n";
	else //fmt.LoadString(IDS_SINESTR2);
		fmt =  "\r\n地震计%c正弦标定参数：\r\n%s\r\n第一次启动时间：%04d-%02d-%02d %02d:%02d:%02d\r\n定时方式：%s %s\r\n频点数：%d\r\n周期数\t频率/周期\t幅度值(uA)\r\n";
	if(frm.head.sens_id>=0 && frm.head.sens_id<MAXSENSNUM)
	{
		id=frm.head.sens_id;
		m_das.sens[id].sines.btimer=frm.btimer;
		m_das.sens[id].sines.prd_num=frm.prd_num;
		m_das.sens[id].sines.time=frm.time;
		m_das.sens[id].sines.tm_start1=frm.tm_start1;
		m_das.sens[id].sines.tm_start2=frm.tm_start2;
		m_das.sens[id].sines.tm_start3=frm.tm_start3;
		m_das.sens[id].sines.tm_start4=frm.tm_start4;
		m_das.sens[id].sines.method=frm.method;
		m_das.sens[id].sines.amptype=frm.amptype;
		p=(short *)&((char *)&frm)[28];
		int j=0;
		for(int i=0;i<frm.prd_num;i++){
			m_das.sens[id].sines.sine[i].cycle=p[j];
			j++;
			m_das.sens[id].sines.sine[i].prd=p[j];
			j++;
			m_das.sens[id].sines.sine[i].aten=p[j];
			j++;
			
		}
	}
	if(frm.btimer==1)
		//fmt1.LoadString(IDS_ENABLESTR1);
		fmt1 =  "允许定时启动";
	else //fmt1.LoadString(IDS_DISABLESTR1);
		fmt1 = "不允许定时启动";
	struct tm * t = localtime(&frm.time);
	//t=frm.time;

	//UINT ids[]={IDS_TMMETHOD1,IDS_TMMETHOD2,IDS_TMMETHOD3,IDS_TMMETHOD4,IDS_TMMETHOD5};
	string ids[]={"每日", "每星期","每月","每年","小时"};
	
	if(frm.method>=0 && frm.method<=4)
		//s.LoadString(ids[frm.method]);
		s = ids[frm.method];
	else s="";

	if(frm.method==0)//every day
	{
		//s1.Format("%d:%d",frm.tm_start1,frm.tm_start2);
		sprintf(buff, "%d:%d",frm.tm_start1,frm.tm_start2);
		s1 = buff;
	}
	else if(frm.method==1){
		//√ø–«∆⁄ 
		//UINT sid[]={IDS_DATE1,IDS_DATE2,IDS_DATE3,IDS_DATE4,IDS_DATE5,IDS_DATE6,IDS_DATE7};
		string sid[]={"星期一","星期二", "星期三","星期四","星期五","星期六","星期日"};
		string s2=sid[frm.tm_start1-1];
		//s2.LoadString(sid[frm.tm_start1-1]);
		//s1.Format("%s %d:%d",s2,frm.tm_start2,frm.tm_start3);
		
		sprintf(buff, "%s %d:%d",s2.c_str(),frm.tm_start2,frm.tm_start3);
		s1 = buff;
		
	}else if(frm.method==2)//every month
	{
		//s1.Format("%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3);
		sprintf(buff, "%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3);
		s1 = buff;
	}
	else if(frm.method==3) {
		//s1.Format("%d-%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3,frm.tm_start4);
		sprintf(buff,"%d-%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3,frm.tm_start4);
		s1 = buff;
	}
	else { //s1.Format("%d",frm.tm_start1);
		sprintf(buff,"%d",frm.tm_start1);
		s1 = buff;
	}


	//info.Format(fmt,sens_id[id],fmt1,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),
	//	s,s1,frm.prd_num);

	sprintf(buff, fmt.c_str(), sens_id[id],fmt1.c_str(),t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec, 
				s.c_str(),s1.c_str(),frm.prd_num);
	info = buff;
	
	for(int i=0;i<frm.prd_num;i++){
		//fmt1.Format("%6d\t%9d\t%8d\r\n",frm.sine[i].cycle,(int)(frm.sine[i].prd*.1),frm.sine[i].aten);
		sprintf(buff, "%6d\t%9d\t%8d\r\n",frm.sine[i].cycle,(int)(frm.sine[i].prd*.1),frm.sine[i].aten);
		fmt1 = buff;
		info+=fmt1;
	}
}

void CSitemagDoc::OnCalPseudo(char * pdata,string & info)
{
	string fmt,fmt1,fmt2,s,s1;
	char buff[1500];
	int id;
	//CTime t;
	CM_PSEUDOFRM frm;
	char sflag[][10]={"bin10","bin10r","bin11","bin11r"};

	memcpy((char *)&frm,pdata,sizeof(frm));
	if(frm.time<0)
		return;
	if(frm.amptype==0)
		//fmt.LoadString(IDS_PSEUDOSTR1);
		fmt = "\r\n地震计%c随机码标定参数：\r\n%s\r\n第一次启动时间：%04d-%02d-%02d %02d:%02d:%02d\r\n定时方式：%s %s\r\n脉冲幅度：%d(cnt)\r\n码元宽度：%d毫秒\r\n信号：%s\r\n重复次数：%d次\r\n";
	else //fmt.LoadString(IDS_PSEUDOSTR2);
		fmt = "\r\n地震计%c随机码标定参数：\r\n%s\r\n第一次启动时间：%04d-%02d-%02d %02d:%02d:%02d\r\n定时方式：%s %s\r\n脉冲幅度：%d(uA)\r\n码元宽度：%d毫秒\r\n信号：%s\r\n重复次数：%d次\r\n";

	if(frm.head.sens_id>=0 && frm.head.sens_id<MAXSENSNUM)
	{
		id=frm.head.sens_id;
		m_das.sens[id].pseudo.btimer=frm.btimer;
		m_das.sens[id].pseudo.amp=frm.amp;
		m_das.sens[id].pseudo.time=frm.time;
		m_das.sens[id].pseudo.dur=frm.dur;

		m_das.sens[id].pseudo.tm_start1=frm.tm_start1;
		m_das.sens[id].pseudo.tm_start2=frm.tm_start2;
		m_das.sens[id].pseudo.tm_start3=frm.tm_start3;
		m_das.sens[id].pseudo.tm_start4=frm.tm_start4;
		m_das.sens[id].pseudo.method=frm.method;

		m_das.sens[id].pseudo.rep_num=frm.rep_num;
		m_das.sens[id].pseudo.signal_flag=frm.signal_flag;

		m_das.sens[id].pseudo.amptype=frm.amptype;
	}
	if(frm.btimer==1)
		//fmt1.LoadString(IDS_ENABLESTR1);
		fmt1 = "允许定时启动";
	else //fmt1.LoadString(IDS_DISABLESTR1);
		fmt1 =  "不允许定时启动";

	struct tm * t = localtime(&frm.time);
	//t=frm.time;
	
	if(frm.signal_flag>=2 && frm.signal_flag<=5)
		fmt2=sflag[frm.signal_flag-2];
	else fmt2="Unknown flag";

	//UINT ids[]={IDS_TMMETHOD1,IDS_TMMETHOD2,IDS_TMMETHOD3,IDS_TMMETHOD4,IDS_TMMETHOD5};
	string ids[]={"每日", "每星期","每月","每年","小时"};
	
	if(frm.method>=0 && frm.method<=4)
		//s.LoadString(ids[frm.method]);
		s = ids[frm.method];
	else s="";


	if(frm.method==0)//every day
	{
		sprintf(buff, "%d:%d",frm.tm_start1,frm.tm_start2);
		//s1.Format("%d:%d",frm.tm_start1,frm.tm_start2);
		s1 = buff;
	}
	else if(frm.method==1)
	{
		//√ø–«∆⁄ 
		//UINT sid[]={IDS_DATE1,IDS_DATE2,IDS_DATE3,IDS_DATE4,IDS_DATE5,IDS_DATE6,IDS_DATE7};
		string sid[]={"星期一","星期二", "星期三","星期四","星期五","星期六","星期日"};
		string s2 = sid[frm.tm_start1-1];
		
		//s2.LoadString(sid[frm.tm_start1-1]);
		sprintf(buff, "%s %d:%d",s2.c_str(),frm.tm_start2,frm.tm_start3);
		s1 = buff;
		//s1.Format("%s %d:%d",s2,frm.tm_start2,frm.tm_start3);

	}else if(frm.method==2)//every month
	{
		//s1.Format("%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3);
		sprintf(buff, "%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3);
		s1 = buff;
	}
	else if(frm.method==3)
	{
		//s1.Format("%d-%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3,frm.tm_start4);
		sprintf(buff, "%d-%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3,frm.tm_start4);
		s1 = buff;
	}
	else { 
		//s1.Format("%d",frm.tm_start1);
		sprintf(buff,"%d",frm.tm_start1);
		s1 = buff;
	}


	//info.Format(fmt,sens_id[id],fmt1,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),
	//			s,s1,frm.amp,frm.dur,fmt2,frm.rep_num);
	
	sprintf(buff, fmt.c_str(),sens_id[id],fmt1.c_str(),t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,
				s.c_str(),s1.c_str(),frm.amp,frm.dur,fmt2.c_str(),frm.rep_num);
	info = buff;
}

void CSitemagDoc::OnCalStrong(char * pdata,string & info)
{
	string fmt,fmt1;
	char buff[1500];
	int id;
	//CTime t;
	CM_STRONGFRM frm;

	memcpy((char *)&frm,pdata,sizeof(frm));
	if(frm.time<0)
		return;

	//fmt.LoadString(IDS_STRONGSTR);
	fmt = "\r\n加速度计 强震标定参数：\r\n%s\r\n第一次启动时间：%04d-%02d-%02d %02d:%02d:%02d\r\n定时方式：%s %s\r\n";
	if(frm.head.sens_id>=0 && frm.head.sens_id<MAXSENSNUM)
	{
		id=frm.head.sens_id;
		m_das.sens[id].strong.btimer=frm.btimer;
		m_das.sens[id].strong.time=frm.time;
		
		m_das.sens[id].strong.tm_start1=frm.tm_start1;
		m_das.sens[id].strong.tm_start2=frm.tm_start2;
		m_das.sens[id].strong.tm_start3=frm.tm_start3;
		m_das.sens[id].strong.tm_start4=frm.tm_start4;
		m_das.sens[id].strong.method=frm.method;

	}
	if(frm.btimer==1)
		//fmt1.LoadString(IDS_ENABLESTR1);
		fmt1 = "允许定时启动";
	else //fmt1.LoadString(IDS_DISABLESTR1);]
		fmt1 =  "不允许定时启动";
	
	
	//t=frm.time;
	struct tm * t = localtime(&frm.time);
	string s,s1;
	//UINT ids[]={IDS_TMMETHOD1,IDS_TMMETHOD2,IDS_TMMETHOD3,IDS_TMMETHOD4,IDS_TMMETHOD5};
	string ids[]={"每日","每星期","每月", "每年","小时"};
	if(frm.method>=0 && frm.method<=4)
		//s.LoadString(ids[frm.method]);
		s = ids[frm.method];
	else s="";

	if(frm.method==0) {
		sprintf(buff, "%d:%d",frm.tm_start1,frm.tm_start2);
		s1 = buff;
		//s1.Format("%d:%d",frm.tm_start1,frm.tm_start2);
	}
	else if(frm.method==1)	
	{
		//√ø–«∆⁄ 
		//UINT sid[]={IDS_DATE1,IDS_DATE2,IDS_DATE3,IDS_DATE4,IDS_DATE5,IDS_DATE6,IDS_DATE7};
		string sid[]={"星期一","星期二", "星期三","星期四","星期五","星期六","星期日"};
		string s2 = sid[frm.tm_start1-1];
		//s2.LoadString(sid[frm.tm_start1-1]);
		sprintf(buff,"%s %d:%d",s2.c_str(),frm.tm_start2,frm.tm_start3);
		//s1.Format("%s %d:%d",s2,frm.tm_start2,frm.tm_start3);
		s1 = buff;
	}else if(frm.method==2) {
		//s1.Format("%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3);
		sprintf(buff,"%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3);
		s1 = buff;
	}
	else if(frm.method==3) //every year
	{
		//s1.Format("%d-%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3,frm.tm_start4);
		sprintf(buff,"%d-%d %d:%d",frm.tm_start1,frm.tm_start2,frm.tm_start3,frm.tm_start4);
		s1 = buff;
	}
	else
	{
		//s1.Format("%d",frm.tm_start1);
		sprintf(buff,"%d",frm.tm_start1);
		s1 = buff;
	}

	sprintf(buff,fmt.c_str(),fmt1.c_str(),t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,
			s.c_str(),s1.c_str());
	//info.Format(fmt,sens_id[id],fmt1,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),
	//	s,s1);
	info = buff;

}

void CSitemagDoc::OnStartcal(char * pdata,string & info)
{
	short * p=(short *)pdata;
	if(p[1]==0x7024)
		//info.LoadString(IDS_RCVSTARTPULSE);
		info = "已收到启动脉冲标定命令";
	else if(p[1]==0x7025)
		//info.LoadString(IDS_RCVSTARTSINE);
		info = "已收到启动正弦标定命令";
	else if(p[1]==0x7026)
		//info.LoadString(IDS_RCVSTARTPSEUDO);
		info = "已收到启动伪随机码标定命令";
	else if(p[1]==0x7027)
		//info.LoadString(IDS_RCVSTARTSTRONG);
		info ="已收到启动强震标定命令";
	else {
		info="";
		return;
	}
	//AfxMessageBox(info);
	info="";
	
}

void CSitemagDoc::OnStopcal(char * pdata,string & info)
{
	//info.LoadString(IDS_STOPCALISTR);
	//AfxMessageBox(info);
	info="已收到停止标定命令";
}

void CSitemagDoc::OnCalDelay(char * pdata,string & info)
{
	const char *fmt = "地震计%c的标定等待时间：%d秒\r\n";
	char buff[1500];
	CM_CALIWAITFRM * frm=(CM_CALIWAITFRM *)pdata;
	if(frm->head.sens_id>=0 && frm->head.sens_id<MAXSENSNUM)
		m_das.sens[frm->head.sens_id].cal_delay=frm->time;
	//fmt.LoadString(IDS_CALIDELAYFMT);
	sprintf(buff, fmt,sens_id[frm->head.sens_id],frm->time);
	//info.Format(fmt,sens_id[frm->head.sens_id],frm->time);
	info = buff;
}

void CSitemagDoc::OnCfgSensoff(char * pdata,string & info)
{
	//const char *fmt = "已发出调整地震计%c机械零位命令";
	const char *fmt = "已启动地震计%c调零";
	char buff[1500];
	unsigned short * p=(unsigned short *)pdata;

	//fmt.LoadString(IDS_SENSCMDSTR);
	//info.Format(fmt,sens_id[p[0]]);
	sprintf(buff,fmt, sens_id[p[0]]);
	//AfxMessageBox(info);
	info=buff;
}

void CSitemagDoc::OnStopSensoff(char * pdata,string & info)
{
	unsigned short * p=(unsigned short *)pdata;
	string fmt;
	char buff[1500];
	if(p[1]==0x7051)
		//fmt.LoadString(IDS_STOPSENADJSTR1);
		fmt ="已停止地震计%c调零";
	else 
		fmt = "已停止地震计%c调零";
		//fmt.LoadString(IDS_STOPSENADJSTR2);
	//info.Format(fmt,sens_id[p[0]]);
	sprintf(buff, fmt.c_str(),sens_id[p[0]]);
	//AfxMessageBox(info);
	//info="";
	info=buff;
}

void CSitemagDoc::OnSensAdjStat(char * pdata,string & info)
{
	unsigned short * p=(unsigned short *)pdata;
	char buff[1500];
	const char *fmt = "地震计%s已处于调零状态\r\n";
 	//fmt.LoadString(IDS_INSENSADJ);
	sprintf(buff, fmt,sens_id[p[0]]);
	//info.Format(fmt,sens_id[p[0]]);
	info = buff;
}

void CSitemagDoc::OnCmgLock(char *pdata,string & info)
{
	string fmt;
	char buff[1500];
	CM_SENSCTLFRM frm;
	
	memcpy((char *)&frm,pdata,sizeof(frm));
	
	if(frm.bctl==0)//unlock
		//fmt.LoadString(IDS_CMGLOCKONSTR);
		fmt = "地震计%c开摆\r\n";
	else //fmt.LoadString(IDS_CMGLOCKOFFSTR);
		fmt = "地震计%c锁摆\r\n";

	//info.Format(fmt,sens_id[frm.head.sens_id]);
	sprintf(buff, fmt.c_str(), sens_id[frm.head.sens_id]);
	//AfxMessageBox(info);
	info=buff;

}

void CSitemagDoc::OnTrigauto(char * pdata,string & info)
{
	string fmt,fmt1,s,s1,s2,s3;
	char buff[1500];
	CM_AUTOTRIGFRM frm;
	int num,id;
	std::vector<string> label;
	int unit[3];
	float factor[3];

	//label.SetSize(3);
	GetLabel(label,unit,factor);

	memcpy((char *)&frm,pdata,sizeof(frm));

	//fmt.LoadString(IDS_TRIGAUTOSTR);
	fmt = "\r\n地震计%c\r\n自动触发参数：";
	num=(frm.head.length-4)/sizeof(AUTOTRIG);

	if(frm.head.sens_id>=0 && frm.head.sens_id<MAXSENSNUM)
	{
		id=frm.head.sens_id;
		for(int i=0;i<num && i<40;i++)
			memcpy((char *)&m_das.sens[id].trig_auto[i],(char *)&frm.autotrig[i],sizeof(AUTOTRIG));
		m_das.sens[id].trig_num=num;
	}
	sprintf(buff, fmt.c_str(), sens_id[frm.head.sens_id]);
	//info.Format(fmt,sens_id[frm.head.sens_id]);
	info = buff;
	int i = 0;
	for( i=0;i<num;i++)
	{
		if(frm.autotrig[i].method==1){
			//s1.LoadString(IDS_TRIGMETHOD1);
			s1 = "STA/LTA";
			//fmt1.LoadString(IDS_TRIGAUTOSTR2);
			fmt1 = "\r\n触发器%d：\r\n%s\r\n长窗长度：%.1f秒 短窗长度：%.1f秒  验证窗长度：%.1f秒 \r\n触发阈值：%d(db )验证阈值：%d(db)\r\n结束窗长度：%.1f秒 结束阈值：%d(db)\r\n%s投票数: %d %s投票数: %d %s投票数: %d 触发票数: %d\r\n最大触发长度：%d秒\r\n";
			
			sprintf(buff, fmt1.c_str(), frm.autotrig[i].trig_id,s1.c_str(),frm.autotrig[i].lwin*.1f,frm.autotrig[i].swin*.1f,
					frm.autotrig[i].vwin*.1f,frm.autotrig[i].td,frm.autotrig[i].vd,frm.autotrig[i].ewin*.1f,frm.autotrig[i].te,
					label.at(0).c_str(),frm.autotrig[i].ud_w,label.at(1).c_str(),frm.autotrig[i].ew_w,label.at(2).c_str(),frm.autotrig[i].ns_w,frm.autotrig[i].total_w,
					frm.autotrig[i].trig_len);
			s = buff;
			//s.Format(fmt1,frm.autotrig[i].trig_id,s1,frm.autotrig[i].lwin*.1f,frm.autotrig[i].swin*.1f,
			//	frm.autotrig[i].vwin*.1f,frm.autotrig[i].td,frm.autotrig[i].vd,frm.autotrig[i].ewin*.1f,frm.autotrig[i].te,
			//	label[0],frm.autotrig[i].ud_w,label[1],frm.autotrig[i].ew_w,label[2],frm.autotrig[i].ns_w,frm.autotrig[i].total_w,
			//	frm.autotrig[i].trig_len);
	
		}else if(frm.autotrig[i].method==0){
			//s1.LoadString(IDS_TRIGMETHOD2);
			//fmt1.LoadString(IDS_TRIGAUTOSTR3);
			s1 = "电平触发";
			fmt1 = "\r\n触发器%d：\r\n%s\r\n触发阈值：%d(cm/s/s)结束阈值：%d(cm/s/s)\r\n%s投票数: %d %s投票数: %d %s投票数: %d 触发票数: %d\r\n最大触发长度：%d秒\r\n";
			sprintf(buff, fmt1.c_str(),frm.autotrig[i].trig_id,s1.c_str(),
					frm.autotrig[i].ld,frm.autotrig[i].le,
					label.at(0).c_str(),frm.autotrig[i].ud_w,label.at(1).c_str(),frm.autotrig[i].ew_w,label.at(2).c_str(),frm.autotrig[i].ns_w,frm.autotrig[i].total_w,
					frm.autotrig[i].trig_len);
			s = buff;
			//s.Format(fmt1,frm.autotrig[i].trig_id,s1,
			//	frm.autotrig[i].ld,frm.autotrig[i].le,
			//	label[0],frm.autotrig[i].ud_w,label[1],frm.autotrig[i].ew_w,label[2],frm.autotrig[i].ns_w,frm.autotrig[i].total_w,
			//	frm.autotrig[i].trig_len);
		}else {
			//s1.LoadString(IDS_TRIGMETHOD3);
			//fmt1.LoadString(IDS_TRIGAUTOSTR4);
			s1 = "STA/LTA+电平触发";
			fmt1 = "\r\n触发器%d：\r\n%s\r\n长窗长度：%.1f秒 短窗长度：%.1f秒  验证窗长度：%.1f秒 \r\n触发阈值：%d(db) 验证阈值：%d(db)\r\n结束窗长度：%.1f秒 结束阈值：%d(db)\r\n电平触发阈值：%d(cm/s/s) 电平触发结束阈值：%d(cm/s/s)\r\n%s投票数: %d %s投票数: %d %s投票数: %d 触发票数: %d\r\n最大触发长度：%d秒\r\n";
			sprintf(buff, fmt1.c_str(),frm.autotrig[i].trig_id,s1.c_str(),frm.autotrig[i].lwin*.1f,frm.autotrig[i].swin*.1f,
					frm.autotrig[i].vwin*.1f,frm.autotrig[i].td,frm.autotrig[i].vd,frm.autotrig[i].ewin*.1f,frm.autotrig[i].te,
					frm.autotrig[i].ld,frm.autotrig[i].le,
					label.at(0).c_str(),frm.autotrig[i].ud_w,label.at(1).c_str(),frm.autotrig[i].ew_w,label.at(2).c_str(),frm.autotrig[i].ns_w,frm.autotrig[i].total_w,
					frm.autotrig[i].trig_len);
			s = buff;
			//s.Format(fmt1,frm.autotrig[i].trig_id,s1,frm.autotrig[i].lwin*.1f,frm.autotrig[i].swin*.1f,
			//	frm.autotrig[i].vwin*.1f,frm.autotrig[i].td,frm.autotrig[i].vd,frm.autotrig[i].ewin*.1f,frm.autotrig[i].te,
			//	frm.autotrig[i].ld,frm.autotrig[i].le,
			//	label[0],frm.autotrig[i].ud_w,label[1],frm.autotrig[i].ew_w,label[2],frm.autotrig[i].ns_w,frm.autotrig[i].total_w,
			//	frm.autotrig[i].trig_len);

		}
		info+=s;
	}


}

void CSitemagDoc::OnDetevt(char * pdata,string & info)
{
	CM_DETERMINEVTFRM frm;
	string fmt,s;
	char buff[1500];
	memcpy(&frm,pdata,sizeof(CM_DETERMINEVTFRM));
	m_das.das.determine.trig_num=frm.trig_num;
	m_das.das.determine.trig_win=frm.trig_win;
	m_das.das.determine.end_num=frm.end_num;
	m_das.das.determine.method=frm.method;

	if(frm.method==0){
		//info.LoadString(IDS_DETEVTMETHOD0);
		info = "各地震计独立判定";
		return;
	}else //s.LoadString(IDS_DETEVTMETHOD1);
		s = "全部地震计进行综合判定";
	//fmt.LoadString(IDS_DETEVTFMT);
	fmt = "%s 触发地震计数: %d 触发时间窗（秒）: %d 结束地震计数: %d\r\n";
	//info.Format(fmt,s,frm.trig_num,frm.trig_win,frm.end_num);
	sprintf(buff, fmt.c_str(),s.c_str(),frm.trig_num,frm.trig_win,frm.end_num);
	info= buff;
}

void CSitemagDoc::OnHeartbeat(char * pdata,string & info)
{
	string fmt;
	short * pd=(short *)pdata;
	char buff[1500];
	//fmt.LoadString(IDS_HEARTINTVALSTR);
	fmt = "\r\n系统每%d秒自动传输设备状态\r\n";
	//info.Format(fmt,pd[4]);
	sprintf(buff,fmt.c_str(),pd[4]);
	info = buff;
	m_das.heart_interval=pd[4];
	m_btip=0;
}


void CSitemagDoc::OnVOL(char * pdata,string & info)
{
	string fmt,fmt1;
	CM_VOLFRM frm;
	char buff[1500];
	memcpy((char *)&frm,pdata,sizeof(frm));

	m_das.das.vol.in_v=(float)frm.in_vol*0.1f;
	m_das.das.vol.out_v=(float)frm.out_vol*0.1f;
	m_das.das.vol.stor_v=(float)frm.stor_vol*0.1f;

	//fmt.LoadString(IDS_VOLSTR);
	fmt = "\r\n外部电压: %.2f伏\r\n网络供电电压: %.2f伏\r\n";
	//info.Format(fmt,m_das.das.vol.out_v,m_das.das.vol.in_v,m_das.das.vol.stor_v);
	sprintf(buff, fmt.c_str(),m_das.das.vol.out_v,m_das.das.vol.in_v);
    info = buff;
}

void CSitemagDoc::OnTEMP(char * pdata,string & info)
{
	string fmt,fmt1;
	CM_TEMPFRM frm;
	char buff[1500];
	
	memcpy((char *)&frm,pdata,sizeof(frm));
	m_das.das.temperature.temperature=frm.temperature*.1f;
	//fmt.LoadString(IDS_TEMPSTR);
	fmt = "\r\n机箱温度：%.1f摄氏度\r\n";
	sprintf(buff,fmt.c_str(),(float)frm.temperature/10.f);
	//info.Format(fmt,(float)frm.temperature/10.f);
	info = buff;
}


void CSitemagDoc::OnClkerr(char * pdata,string & info)
{
	string fmt,fmt1;
	CM_CLKERRFRM frm;
	char buff[1500];
	memcpy((char *)&frm,pdata,sizeof(frm));
	m_das.das.clk_err.time_err=frm.clk_err;
	m_das.das.clk_err.freq_err=frm.freq_err*.01f;
	
	//fmt.LoadString(IDS_CLKERRSTR);
	//info.Format(fmt,frm.clk_err,(float)frm.freq_err/100.f);
	fmt = "\r\n时间误差：%d微秒 频率误差：%.2f PPB\r\n";
	sprintf(buff, fmt.c_str(),frm.clk_err,(float)frm.freq_err/100.f);
	info = buff;
}

void CSitemagDoc::OnGpsStat(char * pdata,string & info)
{
	CM_GPSSTATFRM * frm=(CM_GPSSTATFRM *)pdata;

	if(frm->bgps)
		//info.LoadString(IDS_GPSCLKON);
		info = "GPS正在对钟\r\n";
	else //info.LoadString(IDS_GPSCLKOFF);
		info = "\r\nGPS对钟结束\r\n";
}

void CSitemagDoc::OnBatcharge(char * pdata,string & info)
{
	CM_BATCHARGEFRM * pfrm=(CM_BATCHARGEFRM *)pdata;
	char buff[1500];
	if(pfrm->bstop==1){
		OnBatchargeStop(pdata,info);
		return;
	}
	CM_BATCHARGEFRM frm;
	string fmt;
	
	memcpy((char *)&frm,pdata,sizeof(frm));
	m_das.das.bat.time=frm.time;
	m_das.das.bat.vol=(float)frm.vol/10.f;

	if(m_das.das.bat.time<=0)
		info="";
	else 
	{
		//CTime t(frm.time);
		struct tm * t = localtime(&frm.time);
		
		//fmt.LoadString(IDS_BATCHARGE);
		fmt = "最后一次启动电池充电时间：%d-%d-%d %d:%d:%d，充电前电池电压：%.1f伏\r\n";
		sprintf(buff, fmt.c_str(),t->tm_year,t->tm_mon,t->tm_wday,t->tm_hour,t->tm_min,t->tm_sec, m_das.das.bat.vol);
		//info.Format(fmt,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),m_das.das.bat.vol);
		info = buff;
	}
}

void CSitemagDoc::OnBatchargeStop(char * pdata,string & info)
{
	CM_BATCHARGEFRM frm;
	string fmt;
	char buff[1500];
	memcpy((char *)&frm,pdata,sizeof(frm));
	m_das.das.sbat.time=frm.time;
	m_das.das.sbat.vol=(float)frm.vol/10.f;

	if(m_das.das.sbat.time<=0)
		info="";
	else {
		//CTime t(frm.time);
		struct tm * t = localtime(&frm.time);
		//fmt.LoadString(IDS_BATCHARGESTOP);
		fmt = "最后一次电池充电结束时间：%d-%d-%d %d:%d:%d，充电后电池电压：%.1f伏\r\n";
		//info.Format(fmt,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),m_das.das.sbat.vol);
		sprintf(buff, fmt.c_str(),t->tm_year,t->tm_mon,t->tm_wday,t->tm_hour,t->tm_min,t->tm_sec,m_das.das.sbat.vol);
	}
}
 

void CSitemagDoc::OnGetHeartbeat(char *pdata,string & info)
{
	string fmt,s;
	char buff[1500];
	CM_HEARTBEATFRM frm;

	memcpy((char *)&frm,pdata,sizeof(frm));
	m_das.das.vol.in_v=frm.in_vol*.1f;
	m_das.das.vol.out_v=frm.out_vol*.1f;
	m_das.das.vol.stor_v=frm.stor_vol*.1f;
	m_das.das.temperature.temperature=frm.temperature*.1f;
	m_das.das.clk_err.freq_err=frm.freq_err ;
	m_das.das.clk_err.time_err=frm.clk_err;

	if(!m_bview_heartbeat){
		info="";
		return;
	}
//	fmt.LoadString(IDS_VOLSTR);
//	s.Format(fmt,(float)frm.out_vol/10.f,(float)frm.in_vol/10.f,(float)frm.stor_vol/10.f);
//	info=s;
	info="";
	//fmt.LoadString(IDS_TEMPSTR);
	fmt = "\r\n机箱温度：%.1f摄氏度\r\n";
	//s.Format(fmt,(float)frm.temperature/10.f);	
	sprintf(buff, fmt.c_str(),(float)frm.temperature/10.f);	
	s = buff;
	info+=s;

	//fmt.LoadString(IDS_CLKERRSTR);
	fmt = "\r\n时间误差：%d微秒 频率误差：%.2f PPB\r\n";
	sprintf(buff, fmt.c_str(), frm.clk_err,(float)frm.freq_err );
	//s.Format(fmt,frm.clk_err,(float)frm.freq_err );
	s = buff;
	info+=s;

//	fmt.LoadString(IDS_OFFSETSTR);
//	s.Format(fmt,frm.ud_offset,frm.ew_offset,frm.ns_offset);
//	info+=s;

	if(frm.buf_total!=0.f){
		//fmt.LoadString(IDS_BUFSIZEFMT2);
		fmt = "存储器总空间: %.2f(MB)，可用空间: %.2f(MB)，占用: %.1f%%\r\n";
		float rate=(float)100*(frm.buf_total-frm.buf_free)/frm.buf_total;
		//s.Format(fmt,frm.buf_total,frm.buf_free,rate);
		sprintf(buff,fmt.c_str(),frm.buf_total/1024.0,frm.buf_free/1024.0,rate);
		s = buff;
	}else {
		//fmt.LoadString(IDS_BUFSIZEFMT1);
		//s.Format(fmt,frm.buf_total,frm.buf_free);
		fmt = "存储器总空间: %.2f(MB)，可用空间: %.2f(MB)\r\n";
		sprintf(buff, fmt.c_str(), frm.buf_total/1024.0,frm.buf_free/1024.0);
		s = buff;
	}
	info+=s;

}

void CSitemagDoc::OnHdStat(char * pdata,string & info)
{
	CM_HDSTATFRM frm;
	string fmt,s;
	char buff[1500];
	//CTime t;

	memcpy(&frm,pdata,sizeof(CM_HDSTATFRM));
	
	//fmt.LoadString(IDS_SDSIZEFMT);
	//info.Format(fmt,frm.id,frm.total,frm.free,(float)(frm.total-frm.free)*100/frm.total);
	
	fmt = "卡%d总空间%d(KB)，空闲空间%d(KB)，占用%.1f%%\r\n";
	sprintf(buff,fmt.c_str(),frm.id,frm.total,frm.free,(float)(frm.total-frm.free)*100/frm.total);
	info = buff;
	
	if(frm.be_curmass){
		//fmt.LoadString(IDS_CURMASSSTR);
		fmt = "为正在写入的存储器\r\n";
		info+=fmt;
	}
	if(frm.io_error>0)
	{
		//fmt.LoadString(IDS_IOERRTM);
		fmt = "\r\n最后一次读写存储器失败的时间是：%d:%d:%d %d:%d:%d\r\n";
		struct tm * t = localtime(&frm.io_error);
		
		fmt = "最后一次电池充电结束时间：%d-%d-%d %d:%d:%d，充电后电池电压：%.1f伏\r\n";
		//info.Format(fmt,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),m_das.das.sbat.vol);
		sprintf(buff, fmt.c_str(),t->tm_year,t->tm_mon,t->tm_wday,t->tm_hour,t->tm_min,t->tm_sec);
		s = buff;
		//t=frm.io_error;
		//s.Format(fmt,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
		info+=s;
	}
	if(frm.id==1 || frm.id==2){
		m_das.das.sd_stat[frm.id-1].total=frm.total;//◊‹ø’º‰,µ•Œª:Kb
		m_das.das.sd_stat[frm.id-1].free=frm.free;//ø…”√ø’º‰,µ•Œª:Kb
 		m_das.das.sd_stat[frm.id-1].be_curmass=frm.be_curmass;//1-’˝‘⁄–¥µƒ¥Ê¥¢ø®
		m_das.das.sd_stat[frm.id-1].io_error=frm.io_error;//◊Ó∫Û1¥ŒI/O¥ÌŒÛ
		m_das.das.sd_stat[frm.id-1].flag=frm.id;//ø®∫≈
	}
 
}

void CSitemagDoc::OnGPS(char * pdata,string & info)
{
	string fmt,fmt1,s,s1,s2;
	CM_IRIGFRM frm;
	float lat,lon,alt;
	string stat;
	char buff[1500];
	
	memcpy((char *)&frm,pdata,sizeof(frm));

	if(!m_bshow_gps)
	{
		m_lat=(float)frm.lat/3600000;
		m_lon=(float)frm.lon/3600000;
		m_alt=(float)frm.alt/10;
	
		m_bshow_gps=TRUE;
		info="";
		return;
	}
 
	//fmt.LoadString(IDS_GPSSTR2);
	fmt = "\r\nGPS信息：\r\nGPS对钟时间：%d-%d-%d %d:%d:%d\r\n纬度: %.4f 经度: %.4f 高程: %.1f米\r\n";
	//CTime t(frm.time);	
	
	lat=(float)frm.lat/3600000;
	lon=(float)frm.lon/3600000;
	alt=(float)frm.alt/10;
	
	struct tm * t = localtime(&frm.time);
	
	//info.Format(fmt,t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),
	//	 lat,lon,alt  );
	sprintf(buff, fmt.c_str(),t->tm_year + 1900,t->tm_mon + 1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec, lat,lon,alt);
	info = buff;
	/*
	m_das.das.gps.alt=alt;
	m_das.das.gps.lat=lat;
	m_das.das.gps.lon=lon;
	m_das.das.gps.dop=frm.dop;
	m_das.das.gps.time=frm.time;
	 */
}

void CSitemagDoc::OnSensSig(char * pdata,string & info)
{
	string fmt,s,s1,s2,sl;
	char buff[1500];
	CM_SENSSIGCTLFRM * frm;
	int data,i;

	frm=(CM_SENSSIGCTLFRM *)pdata;
	if(frm->head.sens_id<0 || frm->head.sens_id>MAXSENSNUM)
		return;
	
	m_das.sens[frm->head.sens_id].flag=frm->flag;
	m_das.sens[frm->head.sens_id].adj_sig=frm->adj_sig;
	m_das.sens[frm->head.sens_id].cal_sig=frm->cal_sig;
	m_das.sens[frm->head.sens_id].lock_sig=frm->lock_sig;
	m_das.sens[frm->head.sens_id].unlock_sig=frm->unlock_sig;
	m_das.sens[frm->head.sens_id].stopcal_sig =frm->stopcal_sig ;
	
	m_das.sens[frm->head.sens_id].adj_sig_init=frm->adj_sig_init;
	m_das.sens[frm->head.sens_id].cal_sig_init=frm->cal_sig_init;
	m_das.sens[frm->head.sens_id].lock_sig_init=frm->lock_sig_init;
	m_das.sens[frm->head.sens_id].unlock_sig_init=frm->unlock_sig_init;
	m_das.sens[frm->head.sens_id].stopcal_sig_init=frm->stopcal_sig_init;

 	if(frm->flag==0){
		//fmt.LoadString(IDS_SIGMODE_NOSIGSTR);
		fmt = "\r\n地震计%c无控制信号\r\n";
		//info.Format(fmt,sens_id[frm->head.sens_id]);
		sprintf(buff, fmt.c_str(),sens_id[frm->head.sens_id]);
		info = buff;
	}else if(frm->flag==2)
	{
		//fmt.LoadString(IDS_SIGMODE_BBVSSIG);
		//info.Format(fmt,sens_id[frm->head.sens_id]);
		fmt = "\r\n地震计%c使用BBVS-60/120控制方式\r\n";
		sprintf(buff,fmt.c_str(),sens_id[frm->head.sens_id]);
		info = buff;
	}else{
		//fmt.LoadString(IDS_SIGMODE_LOCKSIG);
		fmt = "\r\n地震计%c锁摆 初始值：%s 控制信号： 启动延迟%d秒，第一个脉冲宽度%d秒， 波形定义：%s\r\n";
	 	s2="";
		data=(frm->lock_sig>>14)&0x3ff;
		for( i=0;i<10;i++)
		{
			if(((data >> (9-i))&0x1)==1)
				s2+="1";
			else s2+="0";
		}
		if(frm->lock_sig_init)
			//sl.LoadString(IDS_HIGHLEVEL);
			sl = "高电平";
		else //sl.LoadString(IDS_LOWLEVEL);
			sl = "低电平";
		//s.Format(fmt,sens_id[frm->head.sens_id],sl, ( frm->lock_sig & 0xfe)>>1,(frm->lock_sig >>24)&0x1f,s2);
		sprintf(buff,fmt.c_str(),sens_id[frm->head.sens_id],sl.c_str(), ( frm->lock_sig & 0xfe)>>1,(frm->lock_sig >>24)&0x1f,s2.c_str());
		s = buff;
		info+=s;
		
		//fmt.LoadString(IDS_SIGMODE_UNLOCKSIG);
		fmt="\r\n地震计%c开摆 初始值：%s 控制信号： 启动延迟%d秒，第一个脉冲宽度%d秒， 波形定义：%s\r\n";
	 
		s2="";
		data=(frm->unlock_sig>>14)&0x3ff;
		for( i=0;i<10;i++)
		{
			if(((data >> (9-i))&0x1)==1)
				s2+="1";
			else s2+="0";
		}
		if(frm->unlock_sig_init)
			//sl.LoadString(IDS_HIGHLEVEL);
			sl = "高电平";
		else //sl.LoadString(IDS_LOWLEVEL);
			sl = "低电平";
		//s.Format(fmt,sens_id[frm->head.sens_id],sl,( frm->unlock_sig &0xfe)>>1,(frm->unlock_sig >>24)&0x1f,s2);
		sprintf(buff, fmt.c_str(),sens_id[frm->head.sens_id],sl.c_str(),( frm->unlock_sig &0xfe)>>1,(frm->unlock_sig >>24)&0x1f,s2.c_str());
		s = buff;
		info+=s;

		//fmt.LoadString(IDS_SIGMODE_CALSIG);
		fmt = "\r\n地震计%c标定 初始值：%s 控制信号： 启动延迟%d秒，第一个脉冲宽度%d秒， 波形定义：%s\r\n";
		s2="";
		data=(frm->cal_sig>>14)&0x3ff;
		for( i=0;i<10;i++)
		{
			if(((data >> (9-i))&0x1)==1)
				s2+="1";
			else s2+="0";
		}
		if(frm->cal_sig_init)
			//sl.LoadString(IDS_HIGHLEVEL);
			sl =  "高电平";
		else //sl.LoadString(IDS_LOWLEVEL);
			sl = "低电平";
		//s.Format(fmt,sens_id[frm->head.sens_id],sl, ( frm->cal_sig &0xfe)>>1,(frm->cal_sig >>24)&0x1f,s2);
		sprintf(buff, fmt.c_str(),sens_id[frm->head.sens_id],sl.c_str(), ( frm->cal_sig &0xfe)>>1,(frm->cal_sig >>24)&0x1f,s2.c_str());
		s = buff;
		info+=s;

		//fmt.LoadString(IDS_SIGMODE_STOPCALSIG);
		fmt = "\r\n地震计%c停止标定控制信号： 启动延迟%d秒，第一个脉冲宽度%d秒， 波形定义：%s\r\n";
	 	s2="";
		data=(frm->stopcal_sig>>14)&0x3ff;
		for( i=0;i<10;i++)
		{
			if(((data >> (9-i))&0x1)==1)
				s2+="1";
			else s2+="0";
		}
		//s.Format(fmt,sens_id[frm->head.sens_id], ( frm->stopcal_sig  &0xfe)>>1,(frm->stopcal_sig >>24)&0x1f,s2);
		sprintf(buff, fmt.c_str(),sens_id[frm->head.sens_id], ( frm->stopcal_sig  &0xfe)>>1,(frm->stopcal_sig >>24)&0x1f,s2.c_str());
		s = buff;
		info+=s;

		fmt = "\r\n地震计%c调零 初始值：%s 控制信号： 启动延迟%d秒，第一个脉冲宽度%d秒， 波形定义：%s\r\n";
		//fmt.LoadString(IDS_SIGMODE_ADJSIG);
	 
		s2="";
		data=(frm->adj_sig>>14)&0x3ff;
		for( i=0;i<10;i++)
		{
			if(((data >> (9-i))&0x1)==1)
				s2+="1";
			else s2+="0";
		}
		if(frm->adj_sig_init)
			//sl.LoadString(IDS_HIGHLEVEL);
			sl = "高电平";
		else //sl.LoadString(IDS_LOWLEVEL);
			sl = "低电平";
		//s.Format(fmt,sens_id[frm->head.sens_id], sl,( frm->adj_sig &0xfe)>>1,(frm->adj_sig >>24)&0x1f,s2);
		sprintf(buff, fmt.c_str(),sens_id[frm->head.sens_id], sl.c_str(),( frm->adj_sig &0xfe)>>1,(frm->adj_sig >>24)&0x1f,s2.c_str());
		s = buff;
		info+=s;
	}
}

void CSitemagDoc::OnGetSensoff(char * pdata,string & info)
{
	string fmt;
	char buff[1500];
	CM_SENSROFFFRM frm;
	float off[3];

	memcpy((char *)&frm,pdata,sizeof(frm));
	
	//fmt.LoadString(IDS_SENSOFFSTR);
//	fmt="\r\n地震计%c的机械零位的监测值为：垂直向=%.3f毫伏 东西向=%.3f毫伏 北南向=%.3f毫伏\r\n实际值（BBVS-60/120地震计）为: 垂直向=%.3f毫伏 东西向=%.3f毫伏 北南向=%.3f毫伏\r\n";
    fmt="\r\n加速度计零点值：CH1=%.3f毫伏 CH2=%.3f毫伏 CH3=%.3f毫伏\r\n";
	//frm.ch1∂‘”¶ µº Ω”œﬂ3,frm.ch2∂‘”¶ µº Ω”œﬂ1,frm.ch3∂‘”¶ µº Ω”œﬂ2
 	off[1]=frm.ch1/33554.432;//mv 2.384uV/count
	off[2]=frm.ch2/33554.432;
	off[0]=frm.ch3/33554.432;
	//“‘10vŒ™¬˙∑˘º∆À„∞Ÿ∑÷±»,µ⁄“ª◊Èº‡≤‚÷µ£∫BBVSÀ•ºı6±∂£¨ µº ÷µ «’Ê µ÷µ=º‡≤‚÷µ*6£ª∆‰À˚µÿ’º∆£∫µ⁄“ª◊Èº‡≤‚÷µ
	if(frm.head.sens_id<MAXSENSNUM) {
		//info.Format(fmt,sens_id[frm.head.sens_id],off[0],off[1],off[2],off[0]*6,off[1]*6,off[2]*6);
		sprintf(buff,fmt.c_str(),off[1],off[2],off[0]);
		info = buff;
	}
	
	
	if(m_bsensoffvolmon && (frm.ch1>= m_sensoffvol || frm.ch2>= m_sensoffvol || frm.ch3>= m_sensoffvol ))
	{
		//CString warn;
		//warn.LoadString(IDS_SYSVOLWARNSTR);
		info+= "警告：系统电压值超过阈值！！";
	}
}

void CSitemagDoc::OnWrctl(char * pdata,string & info)
{
	short * p=(short *)pdata;
	m_das.bwr=p[4];
	if(p[4]==1)
		//info.LoadString(IDS_PERMITWR);
		info =  "\r\n允许写存储器\r\n";
	else //info.LoadString(IDS_FORBIDDENWR);
		info = "\r\n停止写存储器\r\n";
}

void CSitemagDoc::OnClearSD(char * pdata,string & info)
{
	string fmt; 
	char buff[1500];
	short * p=(short *)pdata;

	//fmt.LoadString(IDS_CLEARSD);
	fmt = "已经收到清空卡%d命令";
	
	sprintf(buff, fmt.c_str(), p[4]);
	info = buff;
	//info.Format(fmt,p[4]);
	//AfxMessageBox(info);
	info="";
}

void CSitemagDoc::OnBackupRule(char *pdata,string &info) 
{
	// TODO: Add your command handler code here
	CM_BACKRULEFRM frm;
	string fmt1,fmt2;
	char buff[1500];
	memcpy((char *)&frm,pdata,sizeof(frm));
	
 	m_das.das.ctn_ttl=frm.ctn_ttl;

	info="";
	if(frm.ctn_ttl==0){
		//fmt1.LoadString(IDS_BACKUPSTR4);
		fmt1 =  "\r\n按占用空间维护存储器，占用空间超过95%时删除连续波形，\r\n直到占用空间小于80%";
		info+="\r\n"+fmt1;
	}else if(frm.ctn_ttl>0){
		//fmt1.LoadString(IDS_BACKUPSTR5);
		fmt1 =  "\r\n按时间维护存储器,占用空间超过95%%时,保存%d天连续文件";
		//fmt2.Format(fmt1,frm.ctn_ttl);
		sprintf(buff, fmt1.c_str(), frm.ctn_ttl);
		fmt2 = buff;
		info+="\r\n"+fmt2;
	}else //info.LoadString(IDS_BACKUPSTR3);
		info =  "\r\n不做存储器空间维护";
	info+="\r\n";
}

void CSitemagDoc::OnIp(char * pdata,string	& info)
{
	string fmt,fmt1;
	CM_IPGWFRM frm;
	char buff[1500];

	memcpy((char *)&frm,pdata,sizeof(frm));
	//fmt.LoadString(IDS_IPSTR);
	fmt = "\r\n网络地址：%s\r\n子网掩码：%s\r\n网关：%s\r\n";
	sprintf(buff, fmt.c_str(),frm.ip,frm.netmask,frm.gate);
	//info.Format(fmt,frm.ip,frm.netmask,frm.gate);
	info = buff;
	strcpy(m_das.das.ip.ip,frm.ip);
	strcpy(m_das.das.ip.gw,frm.gate);
	strcpy(m_das.das.ip.mask,frm.netmask);

}
void CSitemagDoc::OnSysTm(char * pdata,string & info)
{
	info = "已经设置系统时间";
	//info.LoadString(IDS_SYSTIMESTR);
	//AfxMessageBox(info);
	info="";
}

void CSitemagDoc::OnTimezone(char * pdata,string & info)
{
	CM_TIMEZONEFRM frm;
	string fmt;
	char buff[1500];
	memcpy((char *)&frm,pdata,sizeof(frm));
	strcpy(m_das.das.timezone.zone,frm.zone);
	m_das.das.timezone.offset=frm.offset;

	if(frm.offset==0)
		//info.LoadString(IDS_TIMEZONESTR0);
		info = "\r\n当前系统时区为：GMT\r\n";
	else {
		if(frm.offset<0)
			//fmt.LoadString(IDS_TIMEZONESTR1);
			fmt = "\r\n当前系统时区为：（GMT+%d）\r\n";
		else //fmt.LoadString(IDS_TIMEZONESTR2);
			fmt = "\r\n当前系统时区为：（GMT%d）\r\n";
		sprintf(buff, fmt.c_str(), frm.offset*-1);
		//info.Format(fmt,frm.offset*-1);
		info = buff;
	}
}

 
void CSitemagDoc::OnGpsInt(char * pdata,string & info)
{
	CM_GPSINTFRM frm;
	string fmt;
	char buff[1500];
	memcpy((char *)&frm,pdata,sizeof(frm));
	m_das.das.gps_int=frm.interval;

	if(m_das.das.gps_int==-1)
		//info.LoadString(IDS_GPSINT2);
		info = "\r\nGPS连续对钟\r\n";
	else {
		//fmt.LoadString(IDS_GPSINT1);
		fmt =  "\r\nGPS对钟间隔：%d分钟\r\n";
		sprintf(buff, fmt.c_str(), frm.interval);
		//info.Format(fmt,frm.interval);
		info = buff;
	}

}

void CSitemagDoc::OnResettoDefault(string & info)
{
	info = "已经恢复出厂配置，新的配置需要在重新启动系统后生效。\n选择“是”立即重启系统，选择“否”以后重启系统。";
	//info.LoadString(IDS_RESETTODEFAULTSTR2);
	//if(AfxMessageBox(info,MB_YESNO)==IDYES)
	//return;
	ResetSys();
	info="";
}


void CSitemagDoc::OnResetSys(string & info)
{
	info = "系统将在三分钟之后重新启动！";
	//info.LoadString(IDS_RESETSYSSTR2);
	//AfxMessageBox(info);
	info="";
}

void CSitemagDoc::OnStopSys(string & info)
{
	info = "电池供电将在三分钟之后被切断！";
	//info.LoadString(IDS_STOPSYSSTR2);
	//AfxMessageBox(info);
	info="";
}

  
void CSitemagDoc::OnEvtDetrig(char * pdata,string & info)
{
	CM_EVTDETRIGFRM * dfrm=(CM_EVTDETRIGFRM *)pdata;
	string fmt;
	char buff[1500];
 	
	if(dfrm->head.sens_id>=0 && dfrm->head.sens_id<MAXSENSNUM && dfrm->trig_time>=0 && dfrm->detrig_time>=0)
	{
		//CTime t((long)dfrm->trig_time);
		//CTime dt((long)dfrm->detrig_time);
		
		struct tm * t = localtime(&dfrm->trig_time);
		struct tm * dt = localtime(&dfrm->detrig_time);
		fmt = "地震计%c事件触发结束，触发时间：%d-%d-%d %d:%d:%d.%d，结束触发时间：%d-%d-%d %d:%d:%d.%d\r\n";
		//fmt.LoadString(IDS_DETRIGEVTSTR);
		//info.Format(fmt,sens_id[dfrm->head.sens_id],t.GetYear(),t.GetMonth(),t.GetDay(),
		//	t.GetHour(),t.GetMinute(),t.GetSecond(),dfrm->trigfrac_sec,
		//	dt.GetYear(),dt.GetMonth(),dt.GetDay(),
		//	dt.GetHour(),dt.GetMinute(),dt.GetSecond(),dfrm->detrigfrac_sec);
		sprintf(buff, fmt.c_str(),sens_id[dfrm->head.sens_id],t->tm_year + 1900,t->tm_mon + 1,t->tm_wday,t->tm_hour,t->tm_min,t->tm_sec, 
				dfrm->trigfrac_sec,
					dt->tm_year+1900,dt->tm_mon + 1,dt->tm_wday,dt->tm_hour,dt->tm_min,dt->tm_sec,
				dfrm->detrigfrac_sec);
	}
}

void CSitemagDoc::OnEvtTrig(char * pdata,string & info)
{
 	string fmt;
	char buff[1500];
	CM_EVTTRIGFRM * frm =(CM_EVTTRIGFRM * )pdata;
 
	if(frm->head.sens_id>=0 && frm->head.sens_id<MAXSENSNUM)
	{
		//CTime t((long)frm->trig_time);
		struct tm * t = localtime(&frm->trig_time);
		fmt = "地震计%c事件触发，触发时间：%d-%d-%d %d:%d:%d.%d\r\n";
		//fmt.LoadString(IDS_TRIGEVTSTR);
		sprintf(buff, fmt.c_str(), sens_id[frm->head.sens_id],t->tm_year+1900,t->tm_mon + 1,t->tm_wday,t->tm_hour,t->tm_min,t->tm_sec,frm->frac_sec);
		info = buff;
		//info.Format(fmt,sens_id[frm->head.sens_id],t.GetYear(),t.GetMonth(),t.GetDay(),
		//	t.GetHour(),t.GetMinute(),t.GetSecond(),frm->frac_sec);
	}
}

void CSitemagDoc::OnEvtrecFile(char * pdata,string & info)
{
	CM_EVTRECFILE * frm=(CM_EVTRECFILE *)pdata;
	string fmt = "共%d条地震事件记录";
	char buff[1500];
	//fmt.LoadString(IDS_EVTRECFMT);
	//info.Format(fmt,frm->pre_evt,frm->aft_evt);
	sprintf(buff, fmt.c_str(), frm->file_sum);
    info = buff;
}

void CSitemagDoc::OnEvtrec(char * pdata,string & info)
{
	CM_EVTRECFRM * frm=(CM_EVTRECFRM *)pdata;
	string fmt = "事件记录参数：事件前记录%d秒，事件后记录%d秒\r\n";
	char buff[1500];
	//fmt.LoadString(IDS_EVTRECFMT);
	//info.Format(fmt,frm->pre_evt,frm->aft_evt);
	sprintf(buff, fmt.c_str(), frm->pre_evt,frm->aft_evt);
	m_pre_evt=frm->pre_evt;
	m_aft_evt=frm->aft_evt;
    info = buff;
}


void CSitemagDoc::OnBroadcast(char * pdata,string & info)
{
	CM_ERRORFRM * frm=(CM_ERRORFRM *)pdata;
//	1-≤…—˘¬ …Ë÷√£¨2-¡ø≥Ã…Ë÷√£¨3-∏ﬂÕ®¬À≤®∆˜…Ë÷√£¨4- ‰»Î–≈∫≈¡„µ„–£’˝£¨5-µÿ’º∆øÿ÷∆£¨6-…Ë÷√µÿ’º∆¡„µ„,7-Õ‚≤ø¥Ê¥¢ø’º‰¬˙£¨8-DOCø’º‰¬˙£¨9-¥Ê¥¢∆˜I/O¥Ì 
	//UINT ids[]={IDS_CFGSMPERR,IDS_CFGGAINERR,IDS_CFGHPERR,IDS_CFGDASOFFERR,IDS_CTLSENSERR,IDS_CFGSENSOFFERR,IDS_HDFULLERR,IDS_DOCFULLERR,IDS_HDIOERR};
 	string ids[]={"出现设置采样率错误！","出现设置量程错误！","出现设置高通滤波器错误！","出现设置输入信号校正值错误！",
		"出现控制地震计错误！","出现调整地震计零位错误！", "存储器空间超过95%!!!\r\n如果设置的存储器维护方式为""不维护""存储器,数据采集器将停止内部记录.\r\n如果设置的维护方式为""按空间维护""或者""按时间维护"",数据采集器将开始删除旧数据,直到存储器可用空间超过70%",
		"存储器占用空间超过80%","存储器读写错误！"};
	if(frm->error<=0 || frm->error>sizeof(ids)/4)
		info="";
	else //info.LoadString(ids[frm->error-1]);
		info = ids[frm->error-1];
	info+="\r\n";
}
  

////////////////inquery/////////////////////////////////////////////////

void CSitemagDoc::SiteInquery(unsigned short cmd,int id)
{
	
	char buf[128];
	short * p=(short *)buf;
	unsigned short * ps=(unsigned short *)buf;

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	p[2]=id;
	ps[3]=cmd;
	p[4]=4;
	p[5]=0;
	p[6]=0;
	for(int i=2;i<6;i++)
		p[6]-=p[i];
	if(!m_thd->Send(buf,14))
		SendErr();
}

void CSitemagDoc::SensSet(unsigned short cmd,string & tip)
{
	/*
	CSelsensDlg dlg;

	dlg.m_das=&m_das;
	dlg.m_tip=tip;
	if(dlg.DoModal()==IDCANCEL)
		return;
*/
	char buf[20];
	short *p=(short *)buf;
	unsigned short* ps=(unsigned short *)buf;

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	p[2]=m_sensid;
	ps[3]=cmd;
	p[4]=4;
	p[5]=0;
	p[6]=0;
	for(int i=2;i<6;i++)
		p[6]-=p[i];
	if(!m_thd->Send(buf,14))
		SendErr();
	 
}

void CSitemagDoc::OnInqDas() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();

	//sample rate
	int i = 0;
	for(i=0;i<m_das.stnpar.sens_num;i++)
		OnInqSmp(i);

	//hp
	//for( i=0;i<m_das.stnpar.sens_num;i++)
		//SiteInquery(0x4003,i);

	//gain
	//for( i=0;i<m_das.stnpar.sens_num;i++)
	//	SiteInquery(0x8005,i);
 
}


void CSitemagDoc::OnInqTrig() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	for(int i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8040,i);	
	SiteInquery(0x8041,0);
}


void CSitemagDoc::OnInqCal() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	int i = 0;
	for(i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8020,i);//pulse
	for( i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8021,i);//sine
	for( i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8022,i);//pseudo
	for( i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8023,i);//pseudo
	for( i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8029,i);//cal delay
}


void CSitemagDoc::OnSetGps()
{
	m_bshow_gps=FALSE;
	SiteInquery(0x8068,0);

}

void CSitemagDoc::OnInqGps() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	SiteInquery(0x8068,0);
	SiteInquery(0x8076,0);
//	SiteInquery(0x8065,0);

}

void CSitemagDoc::OnInqSens() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	for(int i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8001,i);


}

void CSitemagDoc::OnInqInst() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	for(int i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8002,i);
		
}

void CSitemagDoc::OnInqSenssig() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	for(int i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8054,i);
	
}

void CSitemagDoc::OnInqSensoff() 
{
	// TODO: Add your command handler code here
	string s("");

	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	for(int i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8069,i);
	 
}
void CSitemagDoc::OnInqSite() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr){
	//	m_msg.clearbuf();
	//	m_bmsg_clr=FALSE;
	//}

	SiteInquery(0x8000,0);
	OnInqSens();
	m_bmsg_clr=TRUE;
}


void CSitemagDoc::OnInqInoff()
{
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	for(int i=0;i<m_das.stnpar.sens_num;i++)
		SiteInquery(0x8006,i);
}

void CSitemagDoc::OnInqBiaoDing(){
    SiteInquery(0x8023, 0);
}

void CSitemagDoc::OnInqDataService(){
    SiteInquery(0x8030, 0);
    SiteInquery(0x8031, 0);
    SiteInquery(0x8032, 0);
}

void CSitemagDoc::OnInqEvtRec(){
    SiteInquery(0x807e, 0);
    SiteInquery(0x807f, 0);
}

void CSitemagDoc::OnInqEnv() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr){
	//	m_msg.clearbuf();
	//	m_bmsg_clr=FALSE;
	//}
    SiteInquery(0x8061,0);
	SiteInquery(0x8066,0);
    SiteInquery(0x8065,0);
    SiteInquery(0x8068,0);
//	OnInqInoff();
//	OnInqBatcharge();
	m_bmsg_clr=TRUE;
}

void CSitemagDoc::OnInqNet() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	SiteInquery(0x8073,0);	
}

void CSitemagDoc::OnInqHeartInterval()
{
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	SiteInquery(0x8060,0);
}
void CSitemagDoc::OnInqDatasrv() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	SiteInquery(0x8030,0);
	SiteInquery(0x8031,0);
	SiteInquery(0x8032,0);
//	SiteInquery(0x8033,0);
//	SiteInquery(0x8034,0);
//	SiteInquery(0x8035,0);

	
}

void CSitemagDoc::OnInqSmp(int sensid)
{
	char buf[40];
	CM_SMPRATEFRM * frm;
	int smp[]={1,2,3,4,5,7,8};
	short *p=(short *)&buf[4];
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	frm=(CM_SMPRATEFRM *)&buf[4];
	frm->head.cmd=0x8003;
	frm->head.sens_id=sensid;
	frm->head.reserved=0;
	frm->head.length=8;
	for(int i=1;i<=2;i++)
	{
		frm->phase_id=i;
		for(int j=0;j<7;j++)
		{
			frm->samp_id=smp[j];
			
			frm->chk_sum=0;
			for(int k=0;k<(frm->head.length+6)/2-1;k++)
				frm->chk_sum-=p[k];

			if(!m_thd->Send(buf,frm->head.length+10)){
				SendErr();
				return;
			}
		}
	}
}

void CSitemagDoc::OnInqBackup() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();
	SiteInquery(0x8072,0);
	SiteInquery(0x807e,0);
}
void CSitemagDoc::OnInqHdstat() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();

	
    SiteInquery(0x8065, 0);
    SiteInquery(0x8068, 0);
    //SiteInquery(0x8067,0);
	//SiteInquery(0x8070,0);

}

void CSitemagDoc::OnInqTimezone() 
{
	// TODO: Add your command handler code here
	//if(m_bmsg_clr)
	//	m_msg.clearbuf();

	SiteInquery(0x8075,0);	
}
 /*
void CSitemagDoc::OnUpdateInqCal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() );
	
}
 
void CSitemagDoc::OnUpdateInqDas(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected()  );
	
}

void CSitemagDoc::OnUpdateInqGps(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected()  );
	
}
 
void CSitemagDoc::OnUpdateInqSenssig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected()  );
	
}


void CSitemagDoc::OnUpdateInqSens(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected()  );
	
}

void CSitemagDoc::OnUpdateInqTrig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() );
	
}


void CSitemagDoc::OnUpdateInqSite(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() );
	
}
 
void CSitemagDoc::OnUpdateInqSensoff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected()  );
	
}
 
void CSitemagDoc::OnUpdateInqNet(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() );
	
}

void CSitemagDoc::OnUpdateInqEnv(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected()  );

}

 void CSitemagDoc::OnUpdateInqDatasrv(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected()  );
	
}


void CSitemagDoc::OnUpdateInqInst(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() );
	
}


void CSitemagDoc::OnUpdateInqBackup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected()  );
	
}

void CSitemagDoc::OnUpdateInqTimezone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() );
	
}



void CSitemagDoc::OnUpdateInqHdstat(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() );
	
}
*/

///////////configure das
void CSitemagDoc::OnCfgSite() 
{
	/*
	CStnparDlg dlg;
	TRACE("cfg site\n");
	dlg.m_das=&m_das;
	dlg.m_abbr=m_das.stnpar.abbr;
	dlg.m_no=m_das.stnpar.id;
	dlg.m_lat=m_das.stnpar.lat;
	dlg.m_alt=m_das.stnpar.alt;
	dlg.m_lon=m_das.stnpar.lon;
	dlg.m_netid=m_das.stnpar.netid;
	dlg.m_stn=m_das.stnpar.name;
	dlg.m_pDoc=this;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	m_btip=dlg.m_btip;
	 */
}


void CSitemagDoc::OnCfgSens() 
{
	/*
	CSensparDlg dlg;
	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	m_btip=dlg.m_btip;	
	 */
}

void CSitemagDoc::OnCfgInst() 
{
	/*
	CInstparDlg dlg;

	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();	
	 */
}

void CSitemagDoc::OnCfgSamp() 
{
	/*
	CSetSyssmpDlg dlg;

	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
//	m_btip=1;
	 */

}

void CSitemagDoc::OnCfgSmpctl() 
{
	/*
	CSmpDlg dlg;

	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();	
	 */
}

void CSitemagDoc::OnCfgGain(char *m_cmd, int m_cmd_len) 
{
	
	//CGainDlg dlg;
	//dlg.m_das=&m_das;
	//if(dlg.DoModal()==IDCANCEL)
	//	return;
	if(!m_thd->Send(m_cmd,m_cmd_len))
		SendErr();
	m_btip=2;
	
	
/*
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	
	 */
}

void CSitemagDoc::OnCfgPseudocal() 
{
	/*
	CPseudoDlg dlg;

	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	 */
}

void CSitemagDoc::OnCfgPulsecal() 
{
	/*
	CPulseDlg dlg;

	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	 */
	
}



void CSitemagDoc::OnCfgSinecal() 
{
	/*
	CSineDlg dlg;
//	TRACE("sine cal\n");
	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	 */
	
}



void CSitemagDoc::OnCfgStrongcal() 
{
	/*
	CStrongDlg dlg;
	
	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	 */
}


void CSitemagDoc::OnCfgCalwait() 
{
	/*
	CCALDelayDlg dlg;

	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();	
	 */
}


void CSitemagDoc::OnCfgAutotrig() 
{
	/*
	CTrigautoDlg dlg;

	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
//	m_btip=1;
	 */
}

void CSitemagDoc::OnCfgDetevt() 
{
	/*
	CDetermineDlg dlg;
	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	m_btip=0;
	 */
}

void CSitemagDoc::OnCfgIp(char*m_cmd, int m_cmd_len) 
{
	/*
	CIPDlg dlg;
	
	dlg.m_gw=m_das.das.ip.gw;
	dlg.m_ip=m_das.das.ip.ip;
	dlg.m_mask=m_das.das.ip.mask;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
//	m_btip=1;
	*/
	if(!m_thd->Send(m_cmd,m_cmd_len))
		SendErr();
}

void CSitemagDoc::OnCfgDatasrv() 
{
	/*
	CDataSrvcfg dlg;
	for(int i=0;i<4;i++)
	{
		dlg.m_datasrv[i].id=m_das.das.data_srv[i].id;
		dlg.m_datasrv[i].bon=m_das.das.data_srv[i].bon;
		dlg.m_datasrv[i].broot=m_das.das.data_srv[i].broot;
		dlg.m_datasrv[i].bevent=m_das.das.data_srv[i].bevent;
		dlg.m_datasrv[i].aft_evt=m_das.das.data_srv[i].aft_evt;
		dlg.m_datasrv[i].pre_evt=m_das.das.data_srv[i].pre_evt;
		dlg.m_datasrv[i].port=m_das.das.data_srv[i].port;
		dlg.m_datasrv[i].tm_ctl=m_das.das.data_srv[i].tm_ctl;
		dlg.m_datasrv[i].be_broad=m_das.das.data_srv[i].be_broad;
		dlg.m_datasrv[i].be_stat=m_das.das.data_srv[i].be_stat;
		lstrcpy(dlg.m_datasrv[i].host,m_das.das.data_srv[i].host);
		lstrcpy(dlg.m_datasrv[i].sens,m_das.das.data_srv[i].sens);
	}
	dlg.m_sens_num=m_das.stnpar.sens_num;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
//	m_btip=1;
	
	*/
}

void CSitemagDoc::OnCfgRtssrv() 
{
	/*
	CNetDataSrvDlg dlg;
	dlg.m_maxcon=m_das.das.rts_netsrv.max_con;
	dlg.m_port=m_das.das.rts_netsrv.port;
	dlg.m_tmctl=m_das.das.rts_netsrv.tm_ctl;
	dlg.m_dataport=m_das.das.rts_netsrv.dataport;
	dlg.m_type=1;
	dlg.m_title.LoadString(IDS_SETRTSSRV);
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	m_btip=1;	
	 */
}
void CSitemagDoc::OnCfgQksrv() 
{
	/*
	CNetDataSrvDlg dlg;
	dlg.m_maxcon=m_das.das.qk_netsrv.max_con;
	dlg.m_port=m_das.das.qk_netsrv.port;
	dlg.m_tmctl=m_das.das.qk_netsrv.tm_ctl;
	dlg.m_dataport=m_das.das.qk_netsrv.dataport;
	dlg.m_type=2;
	dlg.m_title.LoadString(IDS_SETQKSSRV);
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	m_btip=1;	
	 */
}

void CSitemagDoc::OnCfgCom() 
{
	/*
	CIpucomDlg dlg;
	int baud[]={9600,19200,38400,57600,115200};
	
	
	for(int i=0;i<5;i++)
	{
		if(baud[i]==m_das.das.comctl.baud)
		{
			dlg.m_baud=i;
			break;
		}
	}
	dlg.m_samp=0;
	for(i=0;i<m_das.stnpar.sens_num;i++)
	{
		dlg.m_samp+=m_das.sens[i].samp;
	}
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	 */
}

void CSitemagDoc::OnCfgComctl() 
{
	/*
	CComctlDlg dlg;
	dlg.m_das=&m_das;
 	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	m_btip=0;	
	*/
}


void CSitemagDoc::OnCfgStartpseudo() 
{
	/*
	CString s;
	s.LoadString(IDS_TIP3);

	CSelsensDlg dlg;
	dlg.m_das=&m_das;
	dlg.m_tip=s;
	if(dlg.DoModal()==IDCANCEL)
		return;

	char buf[100];
	short *p=(short *)&buf[4];

	memset(buf,0,sizeof(buf));

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	CM_STARTCALFRM * frm;
	frm=(CM_STARTCALFRM *)&buf[4];

	frm->head.sens_id=dlg.m_sensid;
	frm->head.cmd=0x6026;
	frm->time=-1;
	frm->head.length=8;
	frm->chk_sum=0;
	for(int i=0;i<(frm->head.length+4)/2;i++)
		frm->chk_sum-=p[i];
	
	if(!m_thd->Send(buf,frm->head.length+10))
		SendErr();
	*/
}

void CSitemagDoc::OnCfgStartpulse() 
{
	/*
	CString s;
	s.LoadString(IDS_TIP1);
	CSelsensDlg dlg;
	dlg.m_das=&m_das;
	dlg.m_tip=s;
	if(dlg.DoModal()==IDCANCEL)
		return;

	char buf[100];
	short *p=(short *)&buf[4];

	memset(buf,0,sizeof(buf));

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	CM_STARTCALFRM * frm;
	frm=(CM_STARTCALFRM *)&buf[4];

	frm->head.sens_id=dlg.m_sensid;
	frm->head.cmd=0x6024;
	frm->time=-1;
	frm->head.length=8;
	frm->chk_sum=0;
	for(int i=0;i<(frm->head.length+4)/2;i++)
		frm->chk_sum-=p[i];
	
	if(!m_thd->Send(buf,frm->head.length+10))
		SendErr();
	 */
}

void CSitemagDoc::OnCfgStartsine() 
{
	/*
	CString s;
	s.LoadString(IDS_TIP2);
	CSelsensDlg dlg;
	dlg.m_das=&m_das;
	dlg.m_tip=s;
	if(dlg.DoModal()==IDCANCEL)
		return;

	char buf[100];
	short *p=(short *)&buf[4];

	memset(buf,0,sizeof(buf));

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	CM_STARTCALFRM * frm;
	frm=(CM_STARTCALFRM *)&buf[4];

	frm->head.sens_id=dlg.m_sensid;
	frm->head.cmd=0x6025;
	frm->time=-1;
	frm->head.length=8;
	frm->chk_sum=0;
	for(int i=0;i<(frm->head.length+4)/2;i++)
		frm->chk_sum-=p[i];
	
	if(!m_thd->Send(buf,frm->head.length+10))
		SendErr();
	 */
}

void CSitemagDoc::OnCfgStartstrong() 
{
	/*
	CString s;
	s.LoadString(IDS_TIP4);
	CSelsensDlg dlg;
	dlg.m_das=&m_das;
	dlg.m_tip=s;
	if(dlg.DoModal()==IDCANCEL)
		return;

	char buf[100];
	short *p=(short *)&buf[4];

	memset(buf,0,sizeof(buf));

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	CM_STARTCALFRM * frm;
	frm=(CM_STARTCALFRM *)&buf[4];

	frm->head.sens_id=dlg.m_sensid;
	frm->head.cmd=0x6027;
	frm->time=-1;
	frm->head.length=8;
	frm->chk_sum=0;
	for(int i=0;i<(frm->head.length+4)/2;i++)
		frm->chk_sum-=p[i];
	
	if(!m_thd->Send(buf,frm->head.length+10))
		SendErr();
	 */
}

void CSitemagDoc::OnCfgStopcal() 
{
	// TODO: Add your command handler code here
	SiteInquery(0x6028,0);
}

void CSitemagDoc::OnCfgDasoff() 
{
	/*
	CDasoffDlg dlg;
	dlg.m_sens_num=min(m_das.stnpar.sens_num,40);

	CHANNEL * pchn=m_pchannels;
	LOCALREVBLK * plocal=m_selsite->m_plocal;
	int i=0;
	while(plocal!=NULL)
	{
		for(int j=0;j<plocal->chn_sum;j++)
		{
			dlg.m_off[i][j]=pchn->offset;
			pchn=pchn->lpnext;
		}
		plocal=plocal->lpnext;
		i++;
	}
	if(dlg.DoModal()==IDCANCEL)
		return;
		
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	 */
}


void CSitemagDoc::OnCfgSensctlon() 
{
	// TODO: Add your command handler code here
	string s = "提示：是否启动地震计调零?\n如果启动调零，在选择要标定的地震计后，按“确定”按钮。";
	//s.LoadString(IDS_TIP8);
	SensSet(0x6050,s);		
}

void CSitemagDoc::OnCfgSensctloff() 
{
	// TODO: Add your command handler code here
	string s = "提示：是否停止地震计调零?\n如果停止调零，按“确定”按钮。";
	//s.LoadString(IDS_TIP15);
	SensSet(0x6051,s);		
}

void CSitemagDoc::OnCfgCmgsenson() 
{
	/*
	CString s;
	s.LoadString(IDS_TIP10);

	CSelsensDlg dlg;

	dlg.m_das=&m_das;
	dlg.m_tip=s;
	if(dlg.DoModal()==IDCANCEL)
		return;

	char buf[20];
	short *p=(short *)buf;

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	p[2]=dlg.m_sensid;
	p[3]=0x6052;
	p[4]=6;
	p[5]=0;
	p[6]=0;
	p[7]=0;
	for(int i=2;i<7;i++)
		p[7]-=p[i];
	if(!m_thd->Send(buf,16))
		SendErr();	
	 */
}

void CSitemagDoc::OnCfgCmgsensoff() 
{
	/*
	CString s;
	s.LoadString(IDS_TIP11);

	CSelsensDlg dlg;

	dlg.m_das=&m_das;
	dlg.m_tip=s;
	if(dlg.DoModal()==IDCANCEL)
		return;

	char buf[20];
	short *p=(short *)buf;

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	p[2]=dlg.m_sensid;
	p[3]=0x6052;
	p[4]=6;
	p[5]=0;
	p[6]=1;
	p[7]=0;
	for(int i=2;i<7;i++)
		p[7]-=p[i];
	if(!m_thd->Send(buf,16))
		SendErr();		
	 */
}


void CSitemagDoc::OnCfgSensctl() 
{
	/*
	CSenssigDlg dlg;

	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();	
	 */
}



void CSitemagDoc::OnCfgBackrule()
{
	/*
	CBackruleDlg dlg;
	dlg.m_das=&m_das;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	 */
}


void CSitemagDoc::OnCfgTimezone() 
{
	/*
	CTimeZoneDlg dlg;
	dlg.m_zoffset=m_das.das.timezone.offset;
//	dlg.m_zone=m_das.das.timezone.zone;

	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();	
	m_btip=1;
	 */

}

void CSitemagDoc::OnCfgSystime() 
{
	/*
	CSystmDlg dlg;
	dlg.m_date=m_pchannels->curtm;
	dlg.m_time=m_pchannels->curtm;
	if(dlg.DoModal()==IDCANCEL)
		return;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();	
	 */
}

void CSitemagDoc::OnCfgHear() 
{
	/*
	CHeartbeatDlg dlg;

	dlg.m_interval = m_das.heart_interval;
	if(dlg.DoModal()==IDCANCEL)
		return;
	m_das.heart_interval=dlg.m_interval;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	 */
}

void CSitemagDoc::OnCfgReset() 
{
	// TODO: Add your command handler code here
	string s = "运行该命令将使用出厂参数覆盖已设置的置的参数\r\n是否恢复出厂配置？选择“是”继续，选择“否”取消操作。";
	
	//s.LoadString(IDS_RESETTODEFAULTSTR);
	//if(AfxMessageBox(s,MB_YESNO)==IDNO)
		return;

	char buf[20];
	short *p=(short *)buf;

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	p[2]=0;
	p[3]=0x6077;
	p[4]=4;
	p[5]=0;
	p[6]=0;
	for(int i=2;i<6;i++)
		p[6]-=p[i];
	if(!m_thd->Send(buf,14))
		SendErr();	
}



void CSitemagDoc::OnCfgResetsys() 
{
	// TODO: Add your command handler code here
	string s = "是否重新启动系统？选择“是”继续，选择“否”取消操作！";
	
	//s.LoadString(IDS_RESETSYSSTR);
	//if(AfxMessageBox(s,MB_YESNO)==IDNO)
	//	return;

	char buf[20];
	short *p=(short *)buf;

	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;

	p[2]=0;
	p[3]=0x6078;
	p[4]=4;
	p[5]=0;
	p[6]=0;
	for(int i=2;i<6;i++)
		p[6]-=p[i];
	if(!m_thd->Send(buf,14))
		SendErr();
}

void CSitemagDoc::SaveMessage()
{
}

 /*
///////////////OnUpdate....////////////////////////////
void CSitemagDoc::OnUpdateCfgSite(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);

}

void CSitemagDoc::OnUpdateCfgSens(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgInst(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgSamp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
} 

void CSitemagDoc::OnUpdateCfgSmpctl(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgGain(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}


void CSitemagDoc::OnUpdateCfgDasoff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}


void CSitemagDoc::OnUpdateCfgPseudocal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgPulsecal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}



void CSitemagDoc::OnUpdateCfgSinecal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgStrongcal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgStartpseudo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgStartpulse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);

}

void CSitemagDoc::OnUpdateCfgStartsine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgStartstrong(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}


void CSitemagDoc::OnUpdateCfgCalwait(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgStopcal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}


void CSitemagDoc::OnUpdateCfgSensctloff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}


void CSitemagDoc::OnUpdateCfgSensctlon(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}


void CSitemagDoc::OnUpdateCfgCmgsensoff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgCmgsenson(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
}

void CSitemagDoc::OnUpdateCfgSensctl(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgRtssrv(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgDatasrv(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);

}
void CSitemagDoc::OnUpdateCfgQksrv(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}
void CSitemagDoc::OnUpdateCfgCom(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_selsite!=NULL && m_selsite->m_site.protocol!=0){
		pCmdUI->Enable(FALSE);
		return ;
	}
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
  
	
}

void CSitemagDoc::OnUpdateCfgComctl(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgAutotrig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
}
 

void CSitemagDoc::OnUpdateCfgDetevt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}


void CSitemagDoc::OnUpdateCfgHear(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgBackrule(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);

}

void CSitemagDoc::OnUpdateCfgIp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

 
void CSitemagDoc::OnUpdateCfgSystime(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);

}

void CSitemagDoc::OnUpdateCfgTimezone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgReset(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);
	
}

void CSitemagDoc::OnUpdateCfgResetsys(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);

}
 */

////////////////////¡¨Ω”±Í∂®º∆À„≥Ã–Ú/////////////////////////////////////



void CSitemagDoc::OnCompPath() 
{
	/*
	// TODO: Add your command handler code here
	BROWSEINFO bi;
    TCHAR szDir[MAX_PATH];
    LPITEMIDLIST pidl;

	wsprintf(szDir,"%s",m_exe_path);
  	memset(&bi,0,sizeof(bi));
    bi.hwndOwner = NULL;
    bi.pszDisplayName = szDir;
    bi.pidlRoot = 0;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX;

    pidl = SHBrowseForFolder(&bi);
    if (pidl) 
	{
		if (SHGetPathFromIDList(pidl,szDir)){
			m_exe_path=szDir;
			theApp.WriteProfileString("Config",_T("CAL_EXE"),m_exe_path);
		}
	}	
	 */
}

void CSitemagDoc::OnCompPga() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\sma_2400.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);	
	 */
}

void CSitemagDoc::OnCompPulse() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\pcal_302.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);	
	 */
}

void CSitemagDoc::OnCompSine() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\scal_301.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);	
	 */
}


void CSitemagDoc::OnCompSinepulse() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\scpc_311.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	 */
}


void CSitemagDoc::OnCompMulpulse() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\pcal_net2003.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);	
	 */
}
void CSitemagDoc::OnCompNoise() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\noise_psd01.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	 */
}

void CSitemagDoc::OnCompMulnoise() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\noise_psd011.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);	
	 */
}

void CSitemagDoc::OnCompFun() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\dt_150p.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);	
	 */
}

void CSitemagDoc::OnCompAccpulse() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\scal_bbas301.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);	
	 */
}

void CSitemagDoc::OnCompXcorr() 
{/*// TODO: Add your command handler code here
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\xcorr_001.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
  */
}

void CSitemagDoc::OnCompZpmap() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\zpk_grf01.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	 */
}

void CSitemagDoc::OnCompPrintmap() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\pnt_p.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	 */
}

void CSitemagDoc::OnCompSens() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\scal_sc01.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);	
	 */
}
void CSitemagDoc::OnCompCompm() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\scal_ratio1.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	*/
}
void CSitemagDoc::OnCompSensnoise() 
{
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\seis_pa_noise01.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	 */
}

void CSitemagDoc::OnCompZhendong() 
{
	// TODO: Add your command handler code here
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\sensitivity_2ch.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	 */
}
void CSitemagDoc::OnCompSys() 
{
	// TODO: Add your command handler code here
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\identification.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	 */
}


void CSitemagDoc::OnCompUsedef() 
{
	// TODO: Add your command handler code here
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\comp_1.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	*/
}


void CSitemagDoc::OnCompUsedef2() 
{
	// TODO: Add your command handler code here
	/*
	char lpszname[MAX_PATH];
	
	wsprintf(lpszname,"%s\\comp_2.exe",m_exe_path);

	CSize size(400,400);
	CreateChildWindow(lpszname,"",size);
	 */
}
/*
void CSitemagDoc::OnUpdateCompUsedef2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());

}

void CSitemagDoc::OnUpdateCompAccpulse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompCompm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompFun(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompMulnoise(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}


void CSitemagDoc::OnUpdateCompMulpulse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}


void CSitemagDoc::OnUpdateCompPrintmap(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());

}

void CSitemagDoc::OnUpdateCompSens(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompSensnoise(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompSys(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());

}

void CSitemagDoc::OnUpdateCompZpmap(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompNoise(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
}

void CSitemagDoc::OnUpdateCompPga(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompPulse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompSine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompSinepulse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompUsedef(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompXcorr(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}

void CSitemagDoc::OnUpdateCompZhendong(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_exe_path.IsEmpty());
	
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////


void CSitemagDoc::OnCfgEvtrec() 
{
	/*
	CEvtrecDlg dlg;
	dlg.m_pre_evt=m_pre_evt;
	dlg.m_aft_evt=m_aft_evt;
	if(dlg.DoModal()==IDCANCEL)
		return;
	m_pre_evt=dlg.m_pre_evt;
	m_aft_evt=dlg.m_aft_evt;
	if(!m_thd->Send(dlg.m_cmd,dlg.m_cmd_len))
		SendErr();
	 */
}
/*
void CSitemagDoc::OnUpdateCfgEvtrec(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_thd!=NULL && m_thd->IsConnected() && m_badmin);

}
*/
