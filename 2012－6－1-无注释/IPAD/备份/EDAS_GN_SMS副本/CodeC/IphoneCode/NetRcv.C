// NetRcv.cpp: implementation of the CNetRcv class.
//
//////////////////////////////////////////////////////////////////////


#include "sitemag.h"
#include "NetRcv.h"
#include "DebugDef.h"
#include "CommonDef.h"

int g_x=1;
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetRcv::CNetRcv()
{
	m_connect.cfd=m_connect.dfd=-1;

}

CNetRcv::~CNetRcv()
{
	CloseConnection();
}


BOOL CNetRcv::init(CSitelinkObj * psite,char * logpath)
{
	m_sitelink=psite;
	
	memcpy(&m_connect,&m_sitelink->m_site.net_con,sizeof(m_connect));
	m_connect.cfd=m_connect.dfd=-1;

	strcpy(m_logfile.m_logpath,logpath);

	if(!AllocateMemory())
		return FALSE;
	ReConnection();
	return TRUE;
}

////////////////////////////connect to data server//////////////////////
BOOL CNetRcv::ConnectToCmdServer()
{
	char buf[1000];
	int cmd;

	m_connect.cfd=m_sock.Connect(m_connect.host,m_connect.cmd_port);
	if(m_connect.cfd<0){
		m_connect.cfd=-1;
		return 0;
	}
	if(!m_sock.ReadSockString(m_connect.cfd,buf,sizeof(buf),"\r\n"))
		return 0;
	sscanf(buf,"%d",&cmd);
	if(cmd!=100)
		return 0;
	sprintf(buf,"%s\r\n",m_connect.user);
	if(!m_sock.write_all(m_connect.cfd,buf, strlen(buf)))
		return 0;

	if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",102))
		return 0;

	sprintf(buf,"%s\r\n",m_connect.passwd);
	if(!m_sock.write_all(m_connect.cfd,buf,strlen(buf)))
		return 0;
	if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",107)){
	//	m_logfile.WriteTolog(buf,"err");
		return 0;
	}

	std::string s(buf);
	int index=s.find("108");
	if(index>=0)
	{
		s = s.substr(index);
		sprintf(buf,"%s",s.c_str());
	}else if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",108)){
//		m_logfile.WriteTolog(buf,"err");
		return 0;
	}
	sscanf(buf,"%*d %d %*s",&m_connect.data_port);
	return 1;
}
BOOL CNetRcv::ConnectDataServer()
{
	char tmp[1000];
//	printf("connect data server\n");
	m_connect.dfd=m_sock.Connect(m_connect.host,m_connect.data_port);
	if(m_connect.dfd<0){
		m_logfile.WriteTolog("Can not connect to data server","err");
		m_connect.dfd=-1;
		return 0;
	}
	//printf("read 104\n");
	if(!m_sock.ReadSockResponse(m_connect.cfd,tmp,sizeof(tmp),"\r\n",104))
	{
		m_logfile.WriteTolog(tmp,"err");
		return 0;
	}
	char *str=strstr(tmp,"109");
	if(str!=NULL)
		m_b109=TRUE;
	else m_b109=FALSE;
	return 1;
}
BOOL CNetRcv::AskData()
{
	char buf[1000];

	if(!m_b109){
		if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",109))
			return 0;
	}
//	strcpy(buf,"PKG ON *.*.* 220000 300\r\n");
 	strcpy(buf,"RTS ON\r\n");

	if(!m_sock.write_all(m_connect.cfd,buf,strlen(buf)))
		return 0;

 	if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",110))
	 	return 0;
//	if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",120))
//	 	return 0;
	if(strstr(buf,"109")==NULL)
	{
		if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",109))
			return 0;
	}
	

	strcpy(buf,"RSTAT ON\r\n");

	if(!m_sock.write_all(m_connect.cfd,buf,strlen(buf)))
		return 0;

	if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",130))
		return 0;

	if(strstr(buf,"109")==NULL)
	{
		if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",109))
			return 0;
	}

	strcpy(buf,"BROADCAST ON\r\n");

	if(!m_sock.write_all(m_connect.cfd,buf,strlen(buf)))
		return 0;

	if(!m_sock.ReadSockResponse(m_connect.cfd,buf,sizeof(buf),"\r\n",133))
		return 0;

	m_bconnected=TRUE;
	return 1;
}
BOOL CNetRcv::Connection()
{
	m_bconnected=FALSE;

	if(m_connect.cfd!=-1){
		m_sock.CloseSocket(m_connect.cfd);
		m_connect.cfd=-1;
	}
	if(m_connect.dfd!=-1){
		m_sock.CloseSocket(m_connect.dfd);
		m_connect.dfd=-1;
	}
	if(!ConnectToCmdServer())
		return FALSE;
	
	if(!ConnectDataServer()){
		m_sock.CloseSocket(m_connect.dfd);
		m_sock.CloseSocket(m_connect.cfd);
		m_connect.dfd=m_connect.cfd=-1;
		return FALSE;
	}
	if(AskData()==0)
	{
		m_bconnected=FALSE;
		return 0;
	}
	return 1;

}
void CNetRcv::ReConnection()
{
	char err[256];
	//CString s;

	while(1){
		if(Connection())
			break;
		sprintf(err,"connect %s error!",m_connect.host);
		m_logfile.WriteTolog(err,"err");

		if(m_connect.cfd!=-1){
			m_sock.CloseSocket(m_connect.cfd);
			m_connect.cfd=-1;
		}
		if(m_connect.dfd!=-1){
			m_sock.CloseSocket(m_connect.dfd);
			m_connect.dfd=-1;
		}
		usleep(30000);
	}
//	AfxMessageBox("Connect");
}
void CNetRcv::CloseConnection()
{
	if(m_connect.cfd!=-1)
	{
		m_sock.CloseSocket(m_connect.cfd);
		m_connect.cfd=-1;
	}

	if(m_connect.dfd!=-1)
	{
		m_sock.CloseSocket(m_connect.dfd);
		m_connect.dfd=-1;
	}
}
BOOL CNetRcv::IsConnected()
{
	if(m_connect.cfd>0 && m_connect.dfd>0 && m_bconnected)
		return TRUE;
	return FALSE;
}
/////////////////recevie data///////////////////////////////////////////
//如果大于1分钟收不到数据，认为数据中断
int CNetRcv::CheckInterrupt()
{
	long ctime;
	
	if(m_elapse==-2)
		return 0;
	time(&ctime);
	if(m_elapse==-1)
	{
		m_elapse=ctime;
		return 0;
	}
	if(ctime-m_elapse >= 60){
		m_logfile.WriteTolog("Data interrupt!","err");
		m_elapse=-2;//already alarm
		return 1;
	}
	return 0;
}
int CNetRcv::CheckSync()
{
	static char sync4[]={(char)0xbf,(char)0x13,(char)0x97,(char)0x74};

	if(strncmp(m_sync,sync4,4)==0){
		if(m_elapse==-2){
			m_logfile.WriteTolog("Data restore!","err");
			m_bdatarestore=1;
		}
		m_elapse=-1;//每收到一个同步字，中断时间标志复位
		return 1;
	}
	m_sync[0] = m_sync[1];
	m_sync[1] = m_sync[2];
	m_sync[2] = m_sync[3];
	if(CheckInterrupt())//计算是否长时间不能收到同步字
		return -1;
	return 0;
}

int CNetRcv::ReceiveSync()
{
	struct timeval timeout;
	int ret1;
	timeout.tv_sec=120;
	timeout.tv_usec=0;
	while(1){
		int ret=m_sock.IsReadable(m_connect.dfd,&timeout);
		if(ret>0){
			if(m_sock.read_all(m_connect.dfd,&m_sync[3],1)==1){
				ret1=CheckSync();
				if(ret1==1)
					return 1;//receive sync
				if(ret1==-1){//data interrupt
					TRACE("sync ret=-1\n");
					return -1;
				}else{
					ret=0;
				}
			}else {//network error
				TRACE("%x %x %x %x\n",m_sync[0],m_sync[1],m_sync[2],m_sync[3]);
				TRACE("snyc net err\n");
				return -1;
			}
		}else return -1;
	}
	return 1;

}

BOOL CNetRcv::ReadData()
{
	short *p=(short *)m_dataframe.szData;
	int chk_sum,ret;
	struct timeval timeout;

	timeout.tv_sec=120;
	timeout.tv_usec=0;

	//receive Frame
	ret=m_sock.IsReadable(m_connect.dfd,&timeout);
	if(ret<=0)
		return 0;
/*	ret=m_sock.read_all(m_connect.dfd,buf,10);
	if(ret<=0)
		return 0;
	memcpy(m_dataframe.szData,&buf[4],6);
	*/
	ret=m_sock.read_all(m_connect.dfd,m_dataframe.szData,6);
	if(ret<=0)
		return 0;
	
	if(p[2]>MAXFRAMESIZE-6 || p[2]<=0){
	//	m_logfile.WriteTolog("The data length error!","err");
		return -1;
	}

	//receive data
	ret=m_sock.IsReadable(m_connect.dfd,&timeout);
	if(ret<=0)
		return 0;

	ret=m_sock.read_all(m_connect.dfd,&m_dataframe.szData[6],p[2]);
	if(ret <= 0 )
		return 0;

	chk_sum=0;
	for(int i=0;i<(p[2]+6)/2;i++)
		chk_sum+=p[i];
	chk_sum =chk_sum &0xffff;
	if(chk_sum!=0){
		if(p[1]==0x55AA || p[1]==0x55dd){
			long time=p[4] &0xffff;
			time |=(p[5] << 16);
			Resend(time,1,-1);
		}
		TRACE("CHECK ERR\n");
	//	m_logfile.WriteTolog("check data error","err");
		return -1;
	}
	return 1;
}
int CNetRcv::Receive(long * prcv_data)
{
	short * p;
	int ret;
	p=(short *)m_dataframe.szData;
	
	m_rcv_data=prcv_data;

	ret=ReceiveSync();
	if(ret==-1)
		return -1;
	else if(ret==0)
		return 0;

	ret=ReadData();
	if(ret==0)
	{
		if(	CheckInterrupt())
			return -1;
		return 0;
	}else if(ret==-1)
		return 0;
						
	//oldcal=m_dataframe.sFrId.ZipFlag;
	ret=DecodeData();
	if(ret==1)
	{
		StorePackData();//不分道
	}else if(ret==2)
		SetCmd();
	return 1;
}
///////////////////decode data////////////////////////////////////////////

int CNetRcv::DecodeData()
{
	short * p=(short *)m_dataframe.szData;
	char data[MAX_SMP_RATE*9+40];
//	TRACE("%x\n",p[1] );
	if(p[1]!=0x55dd && p[1]!=0x55AA && p[1]!=0x55BB)
		return 2;//not data frame

	m_dataframe.sFrId.SiteId=p[0];

	if(p[1]==0x55dd){
		STEIM2FRA f_st;
		int comp;
		
		memcpy((char *)&f_st,m_dataframe.szData,14);
		memcpy((char *)&f_st.data,&m_dataframe.szData[14],p[2]-8);

		comp=(p[6]>>8) & 0xf;
		if(comp!=1 && comp!=3)
			comp=3;
	
		EDAFRAME * fra=m_steim.UnzipSteim2(&f_st,comp);
		if(fra==NULL)
			return 0;
			
		memcpy(m_dataframe.szData,fra,fra->fra_len+6);
	}

	m_dataframe.nChId=p[0];
	

	//get time
	m_dataframe.time=p[3]&0xffff;
	m_dataframe.time|=(p[4] &0xffff)<<16;

	//get samp	and cal_flag
	if(p[2]==125*9+9)		
		m_dataframe.samp=125;
	else if(p[1]!=0x55BB && p[1]!=0x55DD)
		m_dataframe.samp = (p[2]-8)/9;
	else m_dataframe.samp=1;
	//cal_flag
	 m_dataframe.sFrId.ZipFlag = p[5] & 0x0f;
//	 TRACE("cal_flag=%d\n",m_dataframe.sFrId.ZipFlag);

	if(m_dataframe.sFrId.ZipFlag==1)
		m_dataframe.sFrId.ZipFlag=4;
	else if(m_dataframe.sFrId.ZipFlag==2)
		m_dataframe.sFrId.ZipFlag=6;
	else if(m_dataframe.sFrId.ZipFlag==3)
		m_dataframe.sFrId.ZipFlag=2;
    
	SendResponse(m_dataframe.time,m_dataframe.nChId);

	if(m_dataframe.nChId>=m_sitelink->m_site.chn_sum || m_dataframe.nChId<0)
	{
		char err[200];
		sprintf(err,"The bad channel id: %d!",m_dataframe.nChId);
//		m_logfile.WriteTolog(err,"err");
		return 0;
	}
	if(m_dataframe.samp>MAX_SMP_RATE || m_dataframe.samp<=0)
	{
		char err[200];
		sprintf(err,"The No.%d channel's sample rate(%d) error, check parameter file!",m_dataframe.nChId,m_dataframe.samp);
	//	m_logfile.WriteTolog(err,"err");
		return 0;
	}

 /*	if(g_x==1){
		Resend(m_dataframe.time-10,60,m_dataframe.nChId);
		g_x=2;
	}
*/
	LOCALREVBLK * pblk=m_sitelink->m_plocal;
	for(int i=0;i<m_dataframe.nChId && pblk!=NULL;i++)
		pblk=pblk->lpnext;
	if(pblk==NULL)
		return 0;
	pblk->samp=m_dataframe.samp;
//	TRACE("%d %d %d\n",m_dataframe.nChId,m_dataframe.samp,m_dataframe.time);
	return 1;
}	




/////////////////////////////////send command////////////////////////////////////

void CNetRcv::Resend(long time,int length,int chn_id)
{
	char buf[200];
	char chn[][10]={"VBB","SMA","VLP"};

/*	if(chn_id>=0 && chn_id<3)
		sprintf(buf,"PKG ON *.*.%s %d %d\r\n",chn[chn_id],time,length);
	else sprintf(buf,"PKG ON *.*.* %d %d\r\n",time,length);
	*/
	short *pbuf;
	
	pbuf = (short *)buf;
	pbuf[0] = 0x13bf;
	pbuf[1] = 0x7497;
	pbuf[2] = chn_id;
	pbuf[3] = 0x1010;
	pbuf[4]=10;
	pbuf[5]=0;

	pbuf[6] = time & 0xffff;
	pbuf[7] = (time >> 16)&0xffff;

	pbuf[8] = length;

	pbuf[9]=0;
	for(int i=2;i<9;i++)
		pbuf[9]-=pbuf[i];

	struct timeval timeout;
	timeout.tv_sec=60;
	timeout.tv_usec=0;

	if(!m_sock.IsWriteable(m_connect.dfd,&timeout))
		return;
//	m_sock.write_all(m_connect.cfd,buf,lstrlen(buf));
	m_sock.write_all(m_connect.dfd,buf,20);

	TRACE("resend");
}

BOOL CNetRcv::Send(char * pdata,int len)
{
	struct timeval timeout;
	timeout.tv_sec=60;
	timeout.tv_usec=0;

	if(!m_sock.IsWriteable(m_connect.cfd,&timeout))
		return FALSE;
	int ret=m_sock.write_all(m_connect.dfd,pdata,len);
	if(ret<len)
		return FALSE;
	return TRUE;
}

void CNetRcv::SendResponse(long time,int chn_id)
{
	char buf[128];
	short * p=(short * )buf;
//	long * p1=(long *)&p[6];

	buf[0]=(char)0xbf;
	buf[1]=(char)0x13;
	buf[2]=(char)0x97;
	buf[3]=(char)0x74;
	p[2]=chn_id;
	p[3]=0x7777;
	p[4]=8;
	p[5]=0;
	p[6]=time & 0xffff;
	p[7]=(time >> 16)&0xffff;
	p[8]=0;
	for(int i=2;i<8;i++)
		p[8]-=p[i];
	
	struct timeval timeout;
	timeout.tv_sec=60;
	timeout.tv_usec=0;

 	if(!m_sock.IsWriteable(m_connect.dfd,&timeout))
 		return ;
 	int ret=m_sock.write_all(m_connect.dfd,buf,18);
 //	TRACE("send response time=%d id=%d bytes=%d\n",time,chn_id,ret);

}
