/* CSaveEVTArray.cpp : 数据存盘基类，提供存盘函数和取台站参数、仪器参数函数
  作者：陈阳 2004.12.2
*/
#include "CommonDef.h"
#include "CSaveEVTArray.h"
#include "datacore.h"
#include "response.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

CSaveEVTArray::CSaveEVTArray()
{
}


CSaveEVTArray::~CSaveEVTArray()
{
}


BOOL CSaveEVTArray::CopyDataToFl(int channel)
{
	
	m_file[channel]=fopen(m_flname[channel],"r+");
	if(m_file[channel]==NULL)
		return FALSE;
	
	fseek(m_file[channel],0,SEEK_SET);
	
	fwrite(&m_evthead[channel],EVTHEADLEN,1,m_file[channel]);
	
	fclose(m_file[channel]);
	
	return TRUE;

}

//写数据到文件
int CSaveEVTArray::WriteData(short *pdata,int channel)
{
	if(m_stnpar[channel].samp==1)
		return WriteToDataBuf1(pdata,channel);
	return WriteToDataBufO(pdata,channel);
	
}


void CSaveEVTArray::SetEvtTime(long time,int channel)

{
	
	m_evthead[channel].data_begin= time;
	
	struct tm * ctm;
	
	ctm=localtime(&m_evthead[channel].data_begin);
	
	
	
	
	
	m_evthead[channel].data_start.year=ctm->tm_year+1900;
	
	m_evthead[channel].data_start.mon=ctm->tm_mon+1;
	
	m_evthead[channel].data_start.day=ctm->tm_mday;
	
	m_evthead[channel].data_start.hour=ctm->tm_hour;
	
	m_evthead[channel].data_start.min=ctm->tm_min;
	
	m_evthead[channel].data_start.sec=ctm->tm_sec;  
	
	m_wrsec[channel]=m_evthead[channel].data_begin;
	
	m_evthead[channel].rec_length=0;



}

BOOL CSaveEVTArray::WriteToDataBuf1(short * pdata,int channel)
{
	
	struct tm ctm;
	ctm.tm_year=m_evthead[channel].data_start.year-1900;
	ctm.tm_mon=m_evthead[channel].data_start.mon-1;
	ctm.tm_mday=m_evthead[channel].data_start.day;
	ctm.tm_hour=m_evthead[channel].data_start.hour;
	ctm.tm_min=ctm.tm_sec=0;
	
	long tsec=mktime(&ctm);
	long sec=pdata[3] & 0xffff;
	
	sec |= (int)(pdata[4]) << 16;
	
	long slen=10;
	int oldhead,oldsec;
	
	oldsec=m_wrsec[channel];
	oldhead=m_evthead[channel].rec_length;
	
	fseek(m_file[channel],0,SEEK_END);
	
	long dur;
	int i;
	int size;
	if(m_stnpar[channel].wlen==24)
		
		size=4;
	
	else size=m_stnpar[channel].wlen/8;
	
	size *=m_stnpar[channel].samp*m_stnpar[channel].comp;
	
	
	
	char *data=(char *)malloc(size);
	
	if(data==NULL)
		
		return FALSE;
	
	
	
	if(sec<m_wrsec[channel])
		
	{
		
		dur=sec-m_wrsec[channel];
		
		//	int flen=m_file[channel].GetLength();
		
		int flen=ftell(m_file[0]);
		
		
		
		if(abs(dur)*(size+4) > flen)
			
			return FALSE;
		
		fseek(m_file[channel],dur*(size+4),SEEK_CUR);
		
		/*	TRY
		 
		 m_file[channel].Seek(dur*(size+4),CFile::current);
		 
		 CATCH(CFileException,e)
		 
		 return FALSE;
		 
		 END_CATCH;
		 
		 */
		
		if(sec+10>=m_wrsec[channel])
			
		{
			
			dur=sec+10-m_wrsec[channel]+1;
			
			m_evthead[channel].rec_length+=dur;
			
			m_wrsec[channel]=sec+10;
			
		}
		
	}else if(sec==m_wrsec[channel])
		
	{
		
		m_wrsec[channel]=sec+10;
		
		m_evthead[channel].rec_length+=slen;
		
	}else{
		
		dur=sec-m_wrsec[channel];
		
		memset(data,0,size);
		
		long flag=1;
		
		for(i=0;i<dur;i++)
			
		{
			
			if(fwrite(&flag,4,1,m_file[channel])<1)
				
			{
				
				m_evthead[channel].rec_length=oldhead;
				
				m_wrsec[channel]=oldsec;
				
				delete data;
				
				return FALSE;
				
			}
			
			if(fwrite(data,size,1,m_file[channel])<1)
				
			{
				
				m_evthead[channel].rec_length=oldhead;
				
				m_wrsec[channel]=oldsec;
				
				delete data;
				
				return FALSE;
				
			}
			
		}
		
		oldhead=m_evthead[channel].rec_length+=dur+slen;
		oldsec=m_wrsec[channel]=sec+10;
		
	}
	
	int block;
	
	long length=pdata[2];
	
	if(m_stnpar[channel].wlen==16)
		
		block=2*10*m_stnpar[channel].samp*m_stnpar[channel].comp+8;
	
	else block=3*10*m_stnpar[channel].samp*m_stnpar[channel].comp+8;
	
	for(i=sec;i<sec+10;i++)
		
	{
		
		if(block!=length)
			
		{
			
			long flag=1;
			
			if(fwrite(&flag,4,1,m_file[channel])<1)
				
			{
				
				m_evthead[channel].rec_length=oldhead;
				
				m_wrsec[channel]=oldsec;
				
				delete data;
				
				return FALSE;
				
			}
			
			memset(data,0,size);
			
		}else{
			
			char *ds=(char *)pdata;	
			
			int ch_s=12;
			
			long value;
			
			long flag;
			
			
			
			if(pdata[1]==0x55CC)
				
				flag=(int)((pdata[5]>>6)&0x3);
			
			else flag=(int)(pdata[5]&0x0f);
			
			if(flag!=0)
				
			{
				
				if(flag==1)
					
					flag=4;
				
				else if(flag==2)
					
					flag=6;
				
				else if(flag==3)
					
					flag=2;
				
				else flag=1;
				
			}
			
			if(fwrite(&flag,4,1,m_file[channel])<1)
				
			{
				
				m_evthead[channel].rec_length=oldhead;
				
				m_wrsec[channel]=oldsec;
				
				delete data;
				
				return FALSE;
				
			}
			
			if(m_stnpar[channel].wlen==24 || m_stnpar[channel].wlen==32){
				
				for(int j=0;j<m_stnpar[channel].comp;j++)
					
				{
					
					ch_s=3*j*10*m_stnpar[channel].samp+12;
					
					value=(ds[ch_s+(i-sec)*3+2]*65536)&0xffff0000;
					
					value|=(ds[ch_s+(i-sec)*3+1]&0xff)<<8;
					
					value|=ds[ch_s+(i-sec)*3]&0xff;
					
					::SetSampleData(j,m_stnpar[channel].wlen,data,value);
					
				}
				
			}else memcpy((short *)data,&pdata[6],size);
			
		}
		
		if(fwrite(data,size,1,m_file[channel])<1)
			
		{
			
			m_evthead[channel].rec_length=oldhead;
			
			m_wrsec[channel]=oldsec;
			
			delete data;
			
			return FALSE;
			
		}
		
	}
	
	delete data;
	
	return TRUE;
	
}


BOOL CSaveEVTArray::WriteToDataBufO(short *pdata,int channel)

{
	
	long sec=pdata[3] & 0xffff;
	
	sec |= (int)(pdata[4]) << 16;
	
	
	
	long dur;
	
	int i;
	
	int size;
	
	
	
	struct tm ctm;
	
	ctm.tm_year=m_evthead[channel].data_start.year-1900;
	
	ctm.tm_mon=m_evthead[channel].data_start.mon-1;
	
	ctm.tm_mday=m_evthead[channel].data_start.day;
	
	ctm.tm_hour=m_evthead[channel].data_start.hour;
	
	ctm.tm_min=ctm.tm_sec=0;
	
	
	
	long tsec=mktime(&ctm);
	
	/*
	 
	 CTime t(m_evthead[channel].data_start.year,m_evthead[channel].data_start.mon,m_evthead[channel].data_start.day,
	 
	 m_evthead[channel].data_start.hour,0,0);
	 
	 long tsec=t.GetTime();
	 
	 */
	
	if(m_stnpar[channel].wlen==24)
		
		size=4;
	
	else size=m_stnpar[channel].wlen/8;
	
	size *=m_stnpar[channel].samp*m_stnpar[channel].comp;
	
	
	
	char * data=(char *)malloc(size);
	
	if(data==NULL)
		
		return FALSE; 
	
	
	
	int oldhead,oldsec;
	
	
	
	oldsec=m_wrsec[channel];
	
	oldhead=m_evthead[channel].rec_length;
	
	
	
	/*	TRY
	 
	 m_file[channel].SeekToEnd();
	 
	 CATCH(CFileException,e)
	 
	 return FALSE;
	 
	 END_CATCH;
	 
	 */
	
	fseek(m_file[channel],0,SEEK_END);
	
	
	
	if(sec<m_wrsec[channel])
		
	{
		
		//	int flen=m_file[channel].GetLength();
		
		int flen=ftell(m_file[0]);
		
		
		
		dur=sec-m_wrsec[channel];
		
		if(abs(dur)*(size+4) > flen)
			
			return FALSE;
		
		
		
		fseek(m_file[channel],dur*(size+4),SEEK_CUR);
		
		/*
		 
		 TRY
		 
		 m_file[channel].Seek(dur*(size+4),CFile::current);
		 
		 CATCH(CFileException,e)
		 
		 return FALSE;
		 
		 END_CATCH;*/
		
		
		
	}else if(sec==m_wrsec[channel])
		
	{
		
		m_wrsec[channel]++;
		
		m_evthead[channel].rec_length++;
		
	}else{
		
		long flag=1;
		
		dur=sec-m_wrsec[channel];
		
		
		
		memset(data,0,size);
		
		for(int j=0;j<dur;j++)
			
		{
			
			if(fwrite(&flag,4,1,m_file[channel])<1)
				
			{
				
				m_evthead[channel].rec_length=oldhead;
				
				m_wrsec[channel]=oldsec;
				
				delete data;
				
				return FALSE;
				
			}
			
			if(fwrite( data,size,1,m_file[channel])<1)
				
			{
				
				m_evthead[channel].rec_length=oldhead;
				
				m_wrsec[channel]=oldsec;
				
				delete data;
				
				return FALSE;
				
			}
			
		}
		
		/*		TRY{
		 
		 for(int j=0;j<dur;j++)
		 
		 {
		 
		 m_file[channel].Write(&flag,4);
		 
		 m_file[channel].Write(data,size);
		 
		 }
		 
		 }CATCH(CFileException,e){
		 
		 m_evthead[channel].rec_length=oldhead;
		 
		 m_wrsec[channel]=oldsec;
		 
		 delete data;
		 
		 return FALSE;
		 
		 }END_CATCH;*/
		
		oldhead=m_evthead[channel].rec_length+=dur+1;
		
		oldsec=m_wrsec[channel]=sec+1;
		
		
		
	}
	
	int block;
	
	if(m_stnpar[channel].wlen!=16){
		
		if(m_stnpar[channel].samp==125)
			
			block=3*m_stnpar[channel].samp*m_stnpar[channel].comp+9;
		
		else block=3*m_stnpar[channel].samp*m_stnpar[channel].comp+8;
		
	}else block=2*m_stnpar[channel].samp*m_stnpar[channel].comp+8;
	
	
	
	long length=pdata[2];
	
	if(length!=block)
		
	{
		
		long flag=1;
		
		if(fwrite(&flag,4,1,m_file[channel])<1)
			
		{
			
			m_evthead[channel].rec_length=oldhead;
			
			m_wrsec[channel]=oldsec;
			
			delete data;
			
			return FALSE;
			
		}
		
		/*TRY{
		 
		 m_file[channel].Write(&flag,4);
		 
		 }CATCH(CFileException,e){
		 
		 m_evthead[channel].rec_length=oldhead;
		 
		 m_wrsec[channel]=oldsec;
		 
		 delete data;
		 
		 return FALSE;
		 
		 }END_CATCH;*/
		
		memset(data,0,size);
		
	}else{
		
		long value;
		
		int ch_s=12;
		
		char *ds=(char *)pdata;
		
		long flag;
		
		if(pdata[1]==0x55CC)
			
			flag=(int)((pdata[5]>>6)&0x3);
		
		else flag=(int)(pdata[5]&0x0f);
		
		if(flag!=0)
			
		{
			
			if(flag==1)
				
				flag=4;
			
			else if(flag==2)
				
				flag=6;
			
			else if(flag==3)
				
				flag=2;
			
			else flag=1;
			
		}
		
		/*	TRY
		 
		 m_file[channel].Write(&flag,4);
		 
		 CATCH(CFileException,e){
		 
		 m_evthead[channel].rec_length=oldhead;
		 
		 m_wrsec[channel]=oldsec;
		 
		 delete data;
		 
		 return FALSE;
		 
		 }END_CATCH;
		 
		 */
		
		if(fwrite(&flag,4,1,m_file[channel])<1)
			
		{
			
			m_evthead[channel].rec_length=oldhead;
			
			m_wrsec[channel]=oldsec;
			
			delete data;
			
			return FALSE;
			
		}
		
		
		
		if(m_stnpar[channel].wlen!=16){
			for(i=0;i<m_stnpar[channel].samp*m_stnpar[channel].comp;i++)
			{
				
				value=(ds[ch_s+i*3+2]*65536)&0xffff0000;
				value|=(ds[ch_s+i*3+1]&0xff)<<8;
				value|=ds[ch_s+i*3]&0xff;
				
				::SetSampleData(i,m_stnpar[channel].wlen,data,value);
				
			}
			
		}else memcpy((short *)data,&pdata[6],size);
		
	}	
	
	if(fwrite( data,size,1,m_file[channel])<1)
	{
		m_evthead[channel].rec_length=oldhead;
		m_wrsec[channel]=oldsec;
		delete data;
		return FALSE;
	}

	delete data;

	return TRUE;

}



BOOL CSaveEVTArray::WriteDatablock(char * lpdata,long sec,long dflag,int channel)

{
	
	long dur;
	
	int size;
	
	char *data;
	
	
	
	//	CTime t(m_evthead[channel].data_start.year,m_evthead[channel].data_start.mon,m_evthead[channel].data_start.day,
	
	//			m_evthead[channel].data_start.hour,0,0);
	
	//	long tsec=t.GetTime();
	
	struct tm ctm;
	
	ctm.tm_year=m_evthead[channel].data_start.year-1900;
	
	ctm.tm_mon=m_evthead[channel].data_start.mon-1;
	
	ctm.tm_mday=m_evthead[channel].data_start.day;
	
	ctm.tm_hour=m_evthead[channel].data_start.hour;
	
	ctm.tm_min=ctm.tm_sec=0;
	
	
	
	long tsec=mktime(&ctm);
	
	
	
	if(m_stnpar[channel].wlen==24)
		
		size=4;
	
	else size=m_stnpar[channel].wlen/8;
	
	size *=m_stnpar[channel].samp*m_stnpar[channel].comp;
	
	
	
	int oldhead,oldsec;
	
	oldsec=m_wrsec[channel];
	
	oldhead=m_evthead[channel].rec_length;
	
	
	
	/*	TRY
	 
	 m_file[channel].SeekToEnd();
	 
	 CATCH(CFileException,e)
	 
	 return FALSE;
	 
	 END_CATCH;
	 
	 */
	
	fseek(m_file[channel],0,SEEK_END);
	
	
	
	if(sec<m_wrsec[channel])
		
	{
		
		//	int flen=m_file[channel].GetLength();
		
		int flen=ftell(m_file[0]);
		
		
		
		dur=sec-m_wrsec[channel];
		
		if(abs(dur)*(size+4) > flen)
			
			return FALSE;
		
		fseek(m_file[channel],dur*(size+4),SEEK_CUR);
		
		/*
		 
		 TRY
		 
		 m_file[channel].Seek(dur*(size+4),CFile::current);
		 
		 CATCH(CFileException,e)
		 
		 return FALSE;
		 
		 END_CATCH;*/
		
		
		
	}else if(sec==m_wrsec[channel])
		
	{
		
		m_wrsec[channel]++;
		
		m_evthead[channel].rec_length++;
		
	}else{
		
		long flag=1;
		
		dur=sec-m_wrsec[channel];
		
		
		
		/*TRY{
		 
		 
		 
		 data=malloc(size);
		 
		 memset(data,0,size);
		 
		 for(int j=0;j<dur;j++)
		 
		 {
		 
		 m_file[channel].Write(&flag,4);
		 
		 m_file[channel].Write(data,size);
		 
		 }
		 
		 }CATCH(CFileException,e){
		 
		 m_evthead[channel].rec_length=oldhead;
		 
		 m_wrsec[channel]=oldsec;
		 
		 delete data;
		 
		 return FALSE;
		 
		 }CATCH(CMemoryException,e)
		 
		 {
		 
		 m_evthead[channel].rec_length=oldhead;
		 
		 m_wrsec[channel]=oldsec;
		 
		 return FALSE;
		 
		 }END_CATCH;*/
		
		data=NULL;
		
		data=(char *)malloc(size);
		
		if(data==NULL)
			
		{
			
			m_evthead[channel].rec_length=oldhead;
			
			m_wrsec[channel]=oldsec;
			
			return FALSE;
			
		}
		
		memset(data,0,size);
		
		for(int j=0;j<dur;j++)
			
		{
			
			if(fwrite(&flag,4,1,m_file[channel])<1)
				
			{
				
				m_evthead[channel].rec_length=oldhead;
				
				m_wrsec[channel]=oldsec;
				
				delete data;
				
				return FALSE;
				
			}
			
			if(fwrite( data,size,1,m_file[channel])<1)
				
			{
				
				m_evthead[channel].rec_length=oldhead;
				
				m_wrsec[channel]=oldsec;
				
				delete data;
				
				return FALSE;
				
			}
			
		}
		
		
		
		delete data;
		
		oldhead=m_evthead[channel].rec_length+=dur+1;
		
		oldsec=m_wrsec[channel]=sec+1;
		
	}
	
	if(fwrite(&dflag,4,1,m_file[channel])<1)
		
	{
		
		m_evthead[channel].rec_length=oldhead;
		
		m_wrsec[channel]=oldsec;
		
		delete data;
		
		return FALSE;
		
	}
	
	if(fwrite(lpdata,size,1,m_file[channel])<1)
		
	{
		
		m_evthead[channel].rec_length=oldhead;
		
		m_wrsec[channel]=oldsec;
		
		delete data;
		
		return FALSE;
		
	}
	return TRUE;

}




