// sitemag.cpp : Defines the class behaviors for the application.
//


#include "sitemag.h"
#include "Message.h"
#include "jtime.h"

//#include "sitemagDoc.h"
//#include "sitemagView.h"


CSitemagApp::CSitemagApp()
{
	m_sptype=NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSitemagApp object

/////////////////////////////////////////////////////////////////////////////
// CSitemagApp initialization

BOOL CSitemagApp::InitInstance()
{
	///Users/guanmofeng/Library/Application Support/iPhone Simulator/4.3/Applications/299441E3-AF32-4658-8196-4CD10B2A803E/Documents/cfg/sitelst.cfg
	//GetCurrentDirectory(MAX_PATH,m_taskpath);
	getcwd(m_taskpath, sizeof(m_taskpath) - 1);
	sprintf(m_logpath,"%s/log",m_taskpath);
	
	
	mkdir(m_logpath, S_IRUSR | S_IWUSR | S_IXUSR);
	//CreateDirectory(m_logpath,NULL);
	
	//CString s=GetProfileString("CFG","DATA_PATH","");

	//if(s.IsEmpty())
		//m_evtdir.Format("%s\\data",m_taskpath);
	//else m_evtdir=s;
	sprintf(m_evtdir, "%s/data",m_taskpath);
	mkdir(m_evtdir, S_IRUSR | S_IWUSR | S_IXUSR);
	//CreateDirectory(m_evtdir,NULL);
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	LoadandInitCXID();


	return TRUE;
}

BOOL CSitemagApp::InitInstance(char *home)
{
	
	//GetCurrentDirectory(MAX_PATH,m_taskpath);
	//getcwd(m_taskpath, sizeof(m_taskpath) - 1);
	sprintf(m_taskpath, "%s", home);
	sprintf(m_logpath,"%s/log",m_taskpath);
	
	
	mkdir(m_logpath, S_IRUSR | S_IWUSR | S_IXUSR);
	//CreateDirectory(m_logpath,NULL);
	
	//CString s=GetProfileString("CFG","DATA_PATH","");
	
	//if(s.IsEmpty())
	//m_evtdir.Format("%s\\data",m_taskpath);
	//else m_evtdir=s;
	sprintf(m_evtdir, "%s/data",m_taskpath);
	mkdir(m_evtdir, S_IRUSR | S_IWUSR | S_IXUSR);
	
	char cfgdir[1024];
	sprintf(cfgdir, "%s/cfg",m_taskpath);
	mkdir(cfgdir, S_IRUSR | S_IWUSR | S_IXUSR);
	
	LoadandInitCXID();
	
	
	return TRUE;
}

// CSitemagApp message handlers

int CSitemagApp::CheckTime(long time,long show_value)
{
	/*check the tm1, tm1 > current time , return value =1. tm1 < current time,return value=-1.
	tm1 = current time,return value=0
	CheckTime(time,show value): if( showvalue!=return value ), show messagebox*/
	int return_value;
	if(time > m_current_time)
		return_value=1;
	else if(time==m_current_time)
		return_value=0;
	else return_value=-1;

	if(return_value ==show_value)
		return return_value;
	
	string inf;
	if(show_value==1)
		inf = "输入的启动时间必须晚于当前时间";
	else if(show_value==-1)
		inf = "输入时间必须早于当前时间";
	else if(show_value==0)
		inf = "输入时间必须是当前时间";
	//MessageShow(inf);
	return return_value;
}
//long CSitemagApp::CalcStarttime(int method,int mon,int day,int hour,int min)

long CSitemagApp::CalcStarttime(short tm_method, short tm_start1, short tm_start2, short tm_start3, short tm_start4)
//long CalcStarttime(short tm_method, short tm_start1, short tm_start2, short tm_start3, short tm_start4)
{
	int fd = -1;
	int flag = 301;
	long g_time_now = 0;
	long g_time_for = 0;
	struct tm *ptm_now, tm_now;
	struct tm *ptm_for, tm_for;
	
	/*
	if( (fd = open(CM_GEO_GPS, O_RDWR|O_NDELAY)) < 0 ) {
		printf("open /dev/geo_gps erro \n");
		return -1;
	}
	if( ioctl( fd, flag, &g_time_now) == -1 ) {printf("read /dev/geo_gps erro \n"); return -1;}
	close(fd);
	
	ptm_now = localtime(&g_time_now);
	memcpy(&tm_now, ptm_now, sizeof(struct tm));
	*/
	
	 time(&g_time_now);
	 ptm_now = localtime(&g_time_now);
	 memcpy(&tm_now, ptm_now, sizeof(struct tm));
	 memcpy(&tm_for, ptm_now, sizeof(struct tm));
	 //printf_time(&tm_now);
	 //*/
	int wk = 0;
	long h1 = 0, h2 = 0 ;
	
	TIME jtime;
	switch(tm_method) {
		case 0 :
			
			tm_for.tm_year = tm_now.tm_year;
			tm_for.tm_mon = tm_now.tm_mon;
			tm_for.tm_mday = tm_now.tm_mday;
			tm_for.tm_hour = tm_start3;
			tm_for.tm_min = tm_start4;
			tm_for.tm_sec = 0;
			g_time_for = mktime(&tm_for);
			/*
			jtime.year =  tm_now.tm_year;
			jtime.mon = tm_now.tm_mon;
			jtime.day = tm_now.tm_mday;
			jtime.hour = tm_start3;
			jtime.min = tm_start4;
			jtime.sec = 0;
			g_time_for = julian(jtime);
			if(g_time_for < g_time_now) g_time_for += 86400;
			 */
			break;
		case 1 : 
			
			tm_for.tm_year = tm_now.tm_year;
			tm_for.tm_mon = tm_now.tm_mon;
			tm_for.tm_mday = tm_now.tm_mday;
			tm_for.tm_hour = tm_start3;
			tm_for.tm_min = tm_start4;
			tm_for.tm_sec = 0;
			g_time_for = mktime(&tm_for);
			wk =  tm_for.tm_wday;
			if(wk==0) wk=7;
			
			if(wk < tm_start2) g_time_for =(tm_start2 -wk) * 86400 + g_time_for;
			else if(wk == tm_start2)
			{
				h1 = tm_start3 * 3600 + tm_start4 * 60;
				h2 = tm_now.tm_hour * 3600 + tm_now.tm_min * 60;
				if( h1 <= h2 )  g_time_for = ( 7 -( wk - tm_start2 ) * 86400) + g_time_for;
			}else
				g_time_for = (7-(wk-tm_start2)) * 86400 + g_time_for;
			
			break;
		case 2 : 
			
			
			tm_for.tm_year = tm_now.tm_year;
			tm_for.tm_mon = tm_now.tm_mon;
			tm_for.tm_mday = tm_start2;
			tm_for.tm_hour = tm_start3;
			tm_for.tm_min = tm_start4;
			tm_for.tm_sec = 0;
			g_time_for = mktime(&tm_for);
			
			if(g_time_for < g_time_now)
			{
				
				tm_for.tm_mon ++ ;
				if(tm_for.tm_mon == 12)
				{
					tm_for.tm_mon = 0;
					tm_for.tm_year ++;
				}
				g_time_for = mktime(&tm_for);
				
			}
			 
			/*
			jtime.year =  tm_now.tm_year;
			jtime.mon = tm_now.tm_mon;
			jtime.day = tm_start2;
			jtime.hour = tm_start3;
			jtime.min = tm_start4;
			jtime.sec = 0;
			g_time_for = julian(jtime);
			
			if(g_time_for < g_time_now)
			{
				
				tm_for.tm_mon ++ ;
				if(jtime.mon == 12)
				{
					jtime.mon = 0;
					jtime.year ++;
				}
				g_time_for = julian(jtime);
				
			}
			*/
			break;
		case 3 :
			
			
			tm_for.tm_year = tm_now.tm_year;
			tm_for.tm_mon = tm_start1;
			tm_for.tm_mday = tm_start2;
			tm_for.tm_hour = tm_start3;
			tm_for.tm_min = tm_start4;
			tm_for.tm_sec = 0;
			g_time_for = mktime(&tm_for);
			
			if(g_time_for < g_time_now)
			{
				tm_for.tm_year ++;
				g_time_for = mktime(&tm_for);
			}
			 
			/*
			jtime.year =  tm_now.tm_year;
			jtime.mon = tm_start1;
			jtime.day = tm_start2;
			jtime.hour = tm_start3;
			jtime.min = tm_start4;
			jtime.sec = 0;
			
			g_time_for = julian(jtime);
			
			if(g_time_for < g_time_now)
			{
				jtime.year ++;
				g_time_for = julian(jtime);
			}
			*/
			break;
		default:
			
			tm_for.tm_year = tm_now.tm_year;
			tm_for.tm_mon = tm_now.tm_mon;
			tm_for.tm_mday = tm_now.tm_mday;
			tm_for.tm_hour = tm_now.tm_hour;
			tm_for.tm_min = tm_start4;
			tm_for.tm_sec = 0;
			g_time_for = mktime(&tm_for);
			if(tm_start4 < tm_now.tm_min)
				g_time_for = g_time_for + 3600;
			 
			
			/*
			jtime.year =  tm_now.tm_year;
			jtime.mon = tm_now.tm_mon;
			jtime.day = tm_now.tm_mday;
			jtime.hour = tm_now.tm_hour;;
			jtime.min = tm_start4;
			jtime.sec = 0;
			g_time_for = julian(jtime);
			g_time_for = mktime(&tm_for);
			if(tm_start4 < tm_now.tm_min)
				g_time_for = g_time_for + 3600;
			 */
	}
	
	return g_time_for ;
}


/*
long CSitemagApp::CalcStarttime(int method,int mon,int day,int hour,int min)
{
	//CTime t=CTime::GetCurrentTime();
	time_t t = time(NULL);
	struct tm * t_tm = localtime(&t);
	long start;
	if(method==0)//day
	{
		struct tm ctm;
		ctm.tm_year=t_tm->tm_year;
		ctm.tm_mon=t_tm->tm_mon;
		ctm.tm_mday=t_tm->tm_mday;
		ctm.tm_hour=hour;
		ctm.tm_min=min;
		ctm.tm_sec=0;
		
		long t1=mktime(&ctm);
		
		//CTime t1(t.GetYear(),t.GetMonth(),t.GetDay(),hour,min,0);
		if(t1 < t) {start = t1+86400; }
		else{start=t1;}
		
		//if(t1.GetTime()<t.GetTime())
		//	start=t1.GetTime()+86400;
		//else start=t1.GetTime();
	}else if(method==1)//week
	{
		struct tm ctm;
		ctm.tm_year=t_tm->tm_year;
		ctm.tm_mon=t_tm->tm_mon;
		ctm.tm_mday=t_tm->tm_mday;
		ctm.tm_hour=hour;
		ctm.tm_min=min;
		ctm.tm_sec=0;
		
		long t1=mktime(&ctm);
		
		//CTime t1(t.GetYear(),t.GetMonth(),t.GetDay(),hour,min,0);
		int wk=t_tm->tm_wday;
		if(wk==1)
			wk=7;
		else wk--;
		if(wk<day)
			//start=(day-wk)*86400+t1.GetTime();
			start=(day-wk)*86400+t1;
		else if(wk==day){
			long h1=hour*3600+min*60;
			//long h2=t.GetHour()*3600+t.GetMinute()*60;
			long h2=t_tm->tm_hour*3600+t_tm->tm_min*60;
			if(h1<=h2)
				//start=(7-(wk-day))*86400+t1.GetTime();
				start=(7-(wk-day))*86400+t1;
			else //start=t1.GetTime();
				start=t1;
		}else //start=(7-(wk-day))*86400+t1.GetTime();
			start=(7-(wk-day))*86400+t1;
	}else if(method==2)//mon
	{
		struct tm ctm;
		ctm.tm_year=t_tm->tm_year;
		ctm.tm_mon=t_tm->tm_mon;
		ctm.tm_mday=day;
		ctm.tm_hour=hour;
		ctm.tm_min=min;
		ctm.tm_sec=0;
		
		long t1=mktime(&ctm);
		
		//CTime t1(t.GetYear(),t.GetMonth(),day,hour,min,0);
		//if(t1.GetTime()<t.GetTime())
		if(t1<t)	
		{
			//switch to next month
			//int yr=t.GetYear();
			//int m=t.GetMonth();
			int yr=t_tm->tm_year;
			int m=t_tm->tm_mon;
			
			m++;
			if(m>12)
			{
				m=1;
				yr++;
			}
			
			struct tm ctm1;
			ctm1.tm_year=yr;
			ctm1.tm_mon=m;
			ctm1.tm_mday=day;
			ctm1.tm_hour=hour;
			ctm1.tm_min=min;
			ctm1.tm_sec=0;
			
			long t2=mktime(&ctm1);
			
			//CTime t2(yr,m,day,hour,min,0);
			start=t2;
		}else //start=t1.GetTime();
			start=t1;

	}else if(method==3)//year
	{
		struct tm ctm;
		ctm.tm_year=t_tm->tm_year;
		ctm.tm_mon=mon;
		ctm.tm_mday=day;
		ctm.tm_hour=hour;
		ctm.tm_min=min;
		ctm.tm_sec=0;
		long t1=mktime(&ctm);
		
		//CTime t1(t.GetYear(),mon,day,hour,min,0);
		//if(t1.GetTime()<t.GetTime())
		if(t1<t)	
		{
			int yr=t_tm->tm_year;
			yr++;
			
			struct tm ctm1;
			ctm1.tm_year=yr;
			ctm1.tm_mon=mon;
			ctm1.tm_mday=day;
			ctm1.tm_hour=hour;
			ctm1.tm_min=min;
			ctm1.tm_sec=0;
			
			long t2=mktime(&ctm1);
			
			//CTime t2(yr,mon,day,hour,min,0);
			start=t2;
		}else //start=t1.GetTime();
			start=t1;

	}else {
		
		
		struct tm ctm;
		ctm.tm_year=t_tm->tm_year;
		ctm.tm_mon=t_tm->tm_mon;
		ctm.tm_mday=t_tm->tm_mday;
		ctm.tm_hour=t_tm->tm_hour;
		ctm.tm_min=min;
		ctm.tm_sec=0;
		long t1=mktime(&ctm);
		
		//CTime t1(t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),min,0);
		
		//if(min< t.GetMinute())
		if(min < t_tm->tm_min)
			start=t1+3600;
		else start=t1;
	}
	printf("long start cal time = %ld",  start);
	return start;
}

*/

void CSitemagApp::LoadandInitCXID()
{
	/*	
	 4
	 111 短周期观测 3 0
	 1115 短周期观测垂直向
	 1112 短周期观测东西向
	 1111 短周期观测北南向
	 114 宽频带观测 3 1
	 1145 宽频带观测垂直向
	 1142 宽频带观测东西向
	 1141 宽频带观测北南向
	 115 甚宽频带观测 3 2
	 1155 甚宽频带观测垂直向
	 1152 甚宽频带观测东西向
	 1151 甚宽频带观测北南向
	 117 强震观测 3 2
	 1175 强震观测垂直向
	 1172 强震观测东西向
	 1171 强震观测北南向
	 */
	
	m_total_type = 4;
	m_sptype=new CXTYPE[m_total_type];
	
	if(m_sptype==NULL)
		return;
	int i = 0 ;
	//NSLog(@"-----CSitemagApp::LoadandInitCXID()");
	sscanf("111 短周期观测 3 0","%s %s %d %d", m_sptype[i].id, m_sptype[i].name,&m_sptype[i].comp,&m_sptype[i].ssmode);
	sscanf("1115 短周期观测垂直向","%s %s",m_sptype[i].cxid[0], m_sptype[i].cxname[0]);
	sscanf("1112 短周期观测东西向","%s %s",m_sptype[i].cxid[1], m_sptype[i].cxname[1]);
	sscanf("1111 短周期观测北南向","%s %s",m_sptype[i].cxid[2], m_sptype[i].cxname[2]);
	
	i = 1 ;
	sscanf("114 宽频带观测 3 1","%s %s %d %d", m_sptype[i].id, m_sptype[i].name,&m_sptype[i].comp,&m_sptype[i].ssmode);
	sscanf("1145 宽频带观测垂直向","%s %s",m_sptype[i].cxid[0], m_sptype[i].cxname[0]);
	sscanf("1142 宽频带观测东西向","%s %s",m_sptype[i].cxid[1], m_sptype[i].cxname[1]);
	sscanf("1141 宽频带观测北南向","%s %s",m_sptype[i].cxid[2], m_sptype[i].cxname[2]);
	
	i = 2 ;
	sscanf("115 甚宽频带观测 3 2","%s %s %d %d", m_sptype[i].id, m_sptype[i].name,&m_sptype[i].comp,&m_sptype[i].ssmode);
	sscanf("1155 甚宽频带观测垂直向","%s %s",m_sptype[i].cxid[0], m_sptype[i].cxname[0]);
	sscanf("1152 甚宽频带观测东西向","%s %s",m_sptype[i].cxid[1], m_sptype[i].cxname[1]);
	sscanf("1151 甚宽频带观测北南向","%s %s",m_sptype[i].cxid[2], m_sptype[i].cxname[2]);
	
	i = 3 ;
	sscanf("117 强震观测 3 2","%s %s %d %d", m_sptype[i].id, m_sptype[i].name,&m_sptype[i].comp,&m_sptype[i].ssmode);
	sscanf("1175 强震观测垂直向","%s %s",m_sptype[i].cxid[0], m_sptype[i].cxname[0]);
	sscanf("1172 强震观测东西向","%s %s",m_sptype[i].cxid[1], m_sptype[i].cxname[1]);
	sscanf("1171 强震观测北南向","%s %s",m_sptype[i].cxid[2], m_sptype[i].cxname[2]);
}
/*
void CSitemagApp::LoadandInitCXID()
{
	FILE *f;
	//CFileException e;
	string s; 
	char buff[1500];
	sprintf(buff, "%s/cfg/sptype.cfg", m_taskpath);
	//s.Format("%s\\cfg\\sptype.cfg", m_taskpath);
	//s = buff;
	if((f = fopen(buff, "r")) == NULL ) {return;}
	
	//if(!f.Open(s,CFile::modeRead|CFile::shareDenyNone|CFile::typeText,&e))
	//	return;
	   if(fgets(buff, 1499, f) != NULL){
		   m_total_type=atol(buff);
	   }else return;
	   
	//if(f.ReadString(s))
	//	m_total_type=atol(s);
	//else return;

	if(m_total_type<=0)
		return;

	m_sptype=new CXTYPE[m_total_type];
	if(m_sptype==NULL)
		return;
	s="";
	int i;
	for(i=0;i<m_total_type;i++)
	{
		//if( !f.ReadString(s))
		//	break;
		if(fgets(buff, 1499, f) == NULL) break;
	
		sscanf(buff,"%s %s %d %d", m_sptype[i].id, m_sptype[i].name,&m_sptype[i].comp,&m_sptype[i].ssmode);
		for(int j=0;j<m_sptype[i].comp && j<8;j++){
			if(fgets(buff, 1499, f) == NULL) break;
			//if (!f.ReadString(s))
			//	break;
			sscanf(buff,"%s %s",m_sptype[i].cxid[j], m_sptype[i].cxname[j]);
		}
		buff[0]='\0';
	}
}*/
 
int CSitemagApp::GetCXSID(int type, const char *cxname,string & cxid)
{
	if(type>=m_total_type || m_sptype==NULL)
		return 0;
	for(int i=0;i<m_sptype[type].comp;i++)
	{
		if(strcmp(m_sptype[type].cxname[i],cxname)==0)
		{
			cxid=m_sptype[type].cxid[i];
			return 1;
		}
	}
	cxid="";
	return 0;
}
int CSitemagApp::GetCXName( char *cxid, string & cxname)
{
	for(int j=0;j<m_total_type;j++)
	{
		if(GetCXName(j,cxid,cxname)==1)
			return 1;
	}
	cxname="";
	
	return 0;
}
int CSitemagApp::GetCXName(int type,char *cxid,string & cxname)
{
	if(type>=m_total_type || m_sptype==NULL)
		return 0;
	for(int i=0;i<m_sptype[type].comp;i++)
	{
		if(strcmp(m_sptype[type].cxid[i],cxid)==0)
		{
			cxname=m_sptype[type].cxname[i];
			return 1;
		}
	}
	cxname="";
	return 0;
}
int CSitemagApp::GetCXID(int smode)
{
	if(m_sptype==NULL)
		return 0;
	int i = 0;
	for(i=0;i<m_total_type;i++)
	{
		if(m_sptype[i].ssmode==smode)
			break;
	}
	if(i==m_total_type)
		return 0;
	return i;
}

int CSitemagApp::GetCXID(char *  id)
{
	if(m_sptype==NULL)
		return 0;
	int i = 0;
	for(i=0;i<m_total_type;i++)
	{
		if(strcmp(m_sptype[i].id,id)==0)
			break;
	}
	if(i==m_total_type)
		return 0;
	return i;
}
int CSitemagApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_sptype!=NULL){
		delete m_sptype;
		m_sptype=NULL;
	}
	return TRUE;
}

