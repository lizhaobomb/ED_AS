// CommonRcv.cpp: implementation of the CCommonRcv class.
//
//////////////////////////////////////////////////////////////////////

#include "sitemag.h"
#include "CommonRcv.h"
#include "inbits.h"
#include "codetab.h"
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommonRcv::CCommonRcv()
{
	m_dataframe.szData=NULL;
	m_elapse=-1;
	m_bdatarestore=0;
	m_bconnected=FALSE;
}

CCommonRcv::~CCommonRcv()
{
	FreeMemory();
}
//////////////////////////////////////////////////////////////////////////////

BOOL CCommonRcv::AllocateMemory()
{
	
	try{
		m_dataframe.szData=new char[MAXFRAMESIZE];//max 3 comp/local channel
	
	}catch (...) {
		
		printf("Meneny Erro");
		return FALSE;
	};
	return TRUE;
}
void CCommonRcv::FreeMemory()
{
	if(m_dataframe.szData!=NULL){
		delete m_dataframe.szData;
		m_dataframe.szData=NULL;
	}

}
//////////////////////////decode///////////////////////////////////////////
BOOL CCommonRcv::ExpandCodeSingle(char  *uncmpdata,char *pcmpdata)
{
	int cindex1,cindex2[20];
	long samp;
	int num,k;
	CIn inbits;
	long difs[MAX_SMP_RATE];
    short *pt1=(short *)uncmpdata;
	short *pt=(short *)pcmpdata;
    char *cmpdata=&pcmpdata[11];

    pt1[0]=pt[0];
	pt1[1]=0x5501;
	pt1[3]=pt[3];
	pt1[4]=pt[4];
	pt1[5]=pt[5];

	inbits.Init(cmpdata);
	samp=(long)inbits.InputBits(12);
	pt1[2]=8+samp*3;
	num=samp/10;

	cindex1=(int)inbits.InputBits(4);
	int i= 0;
	for(i=0;i<num;i++)
	{
		cindex2[i]=(int)inbits.InputBits(2);
		cindex2[i]=codetable[cindex1][cindex2[i]];
	}
	//difs=new long[samp];
	difs[0]=inbits.InputBits(24);
	difs[0]=inbits.Convert_to_data(difs[0],24);
	for(i=1;i<10;i++)
	{
		difs[i]=inbits.InputBits(cindex2[0]);
		difs[i]=inbits.Convert_to_data(difs[i],cindex2[0]);
	}
	
	for(int j=1;j<num;j++)
	{
		for(i=0;i<10;i++)
		{
			k=i+j*10;
			difs[k]=inbits.InputBits(cindex2[j]);
			difs[k]=inbits.Convert_to_data(difs[k],cindex2[j]);
		}
	}
//	TRACE("%d\n",pt[5]&0x1f);
	for(i=1;i<samp;i++){
		difs[i]+=difs[i-1];
	}
	for(i=0;i<samp;i++)
	{
		uncmpdata[12+i*3]=difs[i] & 0xff;
		uncmpdata[12+i*3+1]=(difs[i]>>8) & 0xff;
		uncmpdata[12+i*3+2]=(difs[i]>>16) & 0xff;
	}
	//delete difs;
	return 1;
}

BOOL CCommonRcv::ExpandCode(short *pexpand,short *pcompress,int cmd)
{
	int cindex1,cindex2[100];
	int num,index;
	CIn inbits;
	long tflg;
	unsigned long dtime;//time code
	long cflag;//calbration flag;
	char *cmpdata;
	char *uncmpdata;
	long samp;
	long c2index,c3index;
	short *pt;
	char *expand=(char *)pexpand;
	char *compress=(char *)pcompress;
	int k;
	long difs[MAX_SMP_RATE];

	tflg=(compress[10]>>5)&0x1;	//flag byte
	cflag=compress[10] >>6;

	pt=(short *)&compress[6];
	dtime=pt[0]&0xffff;
	dtime|=(long)(pt[1] & 0xffff ) <<16;
	index=11;
	
	cmpdata=&compress[index];
	inbits.Init(cmpdata);//locate the compress buffer point
	samp=(long)inbits.InputBits(12);
	c2index=(long)inbits.InputBits(12)+index;
	c3index=(long)inbits.InputBits(12)+index;
	if(cmd==0x55CC)
		num=samp/10;
	else{
		num=1;
		samp=10;
	}
	if(samp<=0 || samp>500)
		return FALSE;
	//difs=new long[samp];

	//get the first channel
	uncmpdata=&expand[12];//set the expand-data-buffer point
	cindex1=(int)inbits.InputBits(4);//get the no.1 compress index
	if(cindex1<0 || cindex1>15) //cindex1 must be 0-15
	{
//		delete difs;
		return FALSE;
	}
	int i = 0;
	for(i=0;i<num;i++)//get the no.2 compress index
	{
		cindex2[i]=(int)inbits.InputBits(2);
		if(cindex2[i]<0 || cindex2[i]>3)//cindex2 must be 0-3
		{
		//	delete difs;
			return FALSE;
		}
		cindex2[i]=codetable[cindex1][cindex2[i]];
	}
	//get the first sample data
	difs[0]=inbits.InputBits(24);
	difs[0]=inbits.Convert_to_data(difs[0],24);//process the sign bit
	//get the compressed code,and uncompress 
	for(i=1;i<10;i++)
	{
		difs[i]=inbits.InputBits(cindex2[0]);
		difs[i]=inbits.Convert_to_data(difs[i],cindex2[0]);
	}
	int j = 0;
	for(j=1;j<num;j++)
	{
		for(i=0;i<10;i++)
		{
			k=i+j*10;
			difs[k]=inbits.InputBits(cindex2[j]);
			difs[k]=inbits.Convert_to_data(difs[k],cindex2[j]);
		}
	}
	for(i=1;i<samp;i++)
		difs[i]+=difs[i-1];
//transfer to expand-data-buffer: 3bytes/sample
	for(i=0;i<samp;i++)
	{
		k=i*3;
		uncmpdata[k]=difs[i] & 0xff;
		uncmpdata[k+1]=(difs[i]>>8) & 0xff;
		uncmpdata[k+2]=(difs[i]>>16) & 0xff;
	}

//get the second channel
	cmpdata=&compress[c2index];
	inbits.Init(cmpdata);//relocate compress buffer point
	uncmpdata=&expand[12+samp*3];
	cindex1=(int)inbits.InputBits(4);
	if(cindex1<0 || cindex1>15)
	{
	//	delete difs;
		return FALSE;
	}
	for(i=0;i<num;i++)
	{
		cindex2[i]=(int)inbits.InputBits(2);
		if(cindex2[i]<0 || cindex2[i]>3)
		{
	//		delete difs;
			return FALSE;
		}
		cindex2[i]=codetable[cindex1][cindex2[i]];
	}
	difs[0]=inbits.InputBits(24);
	difs[0]=inbits.Convert_to_data(difs[0],24);

	for(i=1;i<10;i++)
	{
		difs[i]=(long)inbits.InputBits(cindex2[0]);
		difs[i]=inbits.Convert_to_data(difs[i],cindex2[0]);
	}
	for(j=1;j<num;j++)
	{
		for(i=0;i<10;i++)
		{
			k=i+j*10;
			difs[k]=(long)inbits.InputBits(cindex2[j]);
			difs[k]=inbits.Convert_to_data(difs[k],cindex2[j]);
		}
	}
	for(i=1;i<samp;i++)
		difs[i]+=difs[i-1];
	for(i=0;i<samp;i++)
	{
		k=i*3;
		uncmpdata[k]=difs[i] & 0xff;
		uncmpdata[k+1]=(difs[i]>>8) & 0xff;
		uncmpdata[k+2]=(difs[i]>>16) & 0xff;
	}
//get the third channel
	cmpdata=&compress[c3index];
	inbits.Init(cmpdata);//relocate the compress buffer point
	uncmpdata=&expand[12+samp*6];
	cindex1=(int)inbits.InputBits(4);
	if(cindex1<0 || cindex1>15)
	{
	//	delete difs;
		return FALSE;
	}
	for( i=0;i<num;i++)
	{
		cindex2[i]=(int)inbits.InputBits(2);
		if(cindex2[i]<0 || cindex2[i]>3)
		{
	//		delete difs;
			return FALSE;
		}
		cindex2[i]=codetable[cindex1][cindex2[i]];
	}
	difs[0]=inbits.InputBits(24);
	difs[0]=inbits.Convert_to_data(difs[0],24);


	for(i=1;i<10;i++)
	{
		difs[i]=(long)inbits.InputBits(cindex2[0]);
		difs[i]=inbits.Convert_to_data(difs[i],cindex2[0]);
	}
	for( j=1;j<num;j++)
	{
		for(i=0;i<10;i++)
		{
			k=i+j*10;
			difs[k]=(long)inbits.InputBits(cindex2[j]);
			difs[k]=inbits.Convert_to_data(difs[k],cindex2[j]);
		}
	}
	for(i=1;i<samp;i++)
		difs[i]+=difs[i-1];
	for(i=0;i<samp;i++)
	{
		k=i*3;
		uncmpdata[k]=difs[i] & 0xff;
		uncmpdata[k+1]=(difs[i]>>8) & 0xff;
		uncmpdata[k+2]=(difs[i]>>16) & 0xff;
	}
	////
//	delete difs;
	pexpand[0]=pcompress[0];//get the siteid and cmd
	pexpand[1]=pcompress[1];
	pexpand[2]=8+samp*9;
	index=12+9*samp;
	if(samp==125)
	{
		pexpand[2]++;
		index++;
	}
	pexpand[3]=(short)(dtime&0xffff);
	pexpand[4]=(short)((dtime>>16)&0xffff);
	expand[10]=compress[10];
	expand[11]=0;
	for(i=0;i<11;i++)//head chk_sum
		expand[11]-=expand[i];
		
	pexpand[index/2]=0;
	for(i=0;i<index;i++)
		pexpand[index]-=pexpand[i];
	return TRUE;
}

void CCommonRcv::StorePackData()
{
	short * p,*pr;
	p=(short *)m_dataframe.szData;
	pr=(short *)m_rcv_data;

	LOCALREVBLK * pblk=m_sitelink->m_plocal;
	while(pblk!=NULL)
	{
		if(pblk->local_id==m_dataframe.nChId)
			break;
		pblk=pblk->lpnext;
	}
	if(pblk==NULL)
		return;
	
	pr[0]=p[0];
	pr[1]=p[1];

	if(p[1]==0x5555)
		SetData(m_dataframe.szData+12,&m_rcv_data[1],3,16);
	else if(p[1]==0x55BB || p[1]==0x55DD){
		char buf[400];
		for(int j=0;j<10;j++){
			buf[0]=m_dataframe.szData[12+j];
			buf[1]=m_dataframe.szData[12+j+1];
			buf[2]=m_dataframe.szData[12+j+2];
			buf[3]=m_dataframe.szData[12+j+30];
			buf[4]=m_dataframe.szData[12+j+31];
			buf[5]=m_dataframe.szData[12+j+32];
			buf[6]=m_dataframe.szData[12+j+60];
			buf[7]=m_dataframe.szData[12+j+61];
			buf[8]=m_dataframe.szData[12+j+62];

			SetData(buf,&m_rcv_data[1+j*m_dataframe.samp*3],3,32);
		}
	}else if(p[1]==0x55AA || p[1]==0x55CC)
		SetData(m_dataframe.szData+12,&m_rcv_data[1],3,32);
	else if(p[1]==0x5501 && p[1]==0x5502)
		SetData(m_dataframe.szData+12,&m_rcv_data[1],1,32);

}

void CCommonRcv::SetData(char *data,long * rcvdata,int comp,int wlen)
{
	int n;

	if(wlen!=16){
		long value;
		for( n=0;n<m_dataframe.samp*comp;n++)
		{
			value=(data[n*3+2]*65536)&0xffff0000;
			value|=(data[n*3+1]&0xff)<<8;
			value|=data[n*3]&0xff;
			rcvdata[n]=value;
		}
	}else {//16bit
		short value;
		for( n=0;n<m_dataframe.samp*comp;n++)
		{
			value=(data[n*2+1]&0xff)<<8;
			value|=data[n*2]&0xff;
			rcvdata[n]=value;
		}
	}
	

}

void CCommonRcv::SetCmd()
{
	short * p=(short *)m_dataframe.szData;
	if(p[2]+6 > MAXFRAMESIZE)
		return;
	m_dataframe.nChId=0;
	char * pdat=(char *)m_rcv_data;
	memcpy(pdat,m_dataframe.szData,p[2]+6);
}
