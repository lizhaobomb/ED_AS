/************************************************************************************/
/*功能：定义通用数据采集器快速数据服务高速通道压缩数据帧和连续波形文件的压缩数据帧
 /*作者：陈阳
 /*create date: 2009-9-4
 /*update
 /*		date			action				common
 2009-9-29		add					UncmpLEdasfrm解压缩低速通道数据帧
 edit				UncmpEdasfrm改名为UncmpHEdasfrm，修改：增加输出相位
 2010-5-6		edit				UncmpHEdasfrm:输入flag==1 且samp>1: 自动计算rec_len=max(10,samp/5)
 
 /*company：geodevice
 /***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edasunpackdata.h"
//#include "calc_crc16.h"

int CEDASUNPackData::UncmpFileblk(char *pin,int in_len,int samp,int comp,long * time,int * rec_len,int * cali_flag,long * outdata,int  out_len)
{
	if(pin==NULL || samp<=0 || samp>500 || in_len<=0 || comp<1 || comp>3 || out_len<=0 || outdata==NULL
	   || time==NULL || rec_len==NULL || cali_flag==NULL)
		return 0;
	
	CIn inbits;
	int i ,len;
	int offset[3];
	CEdasUncmp edas;
	
	long *	pd=(long *)pin;
	
	*time=pd[1];
	
	//初始化
	inbits.Init(&pin[8]); 
	
	//数据长度
	*rec_len=inbits.InputBits(4);
	len=(*rec_len)*samp;//每个通道的记录长度
	//输出缓冲区长度不够
	if(out_len<len*comp*4)
		return 0;
	
	//标定标志
	*cali_flag=inbits.InputBits(4);
	
	//通道的起始位置
	offset[0]=14;
 	for(i=1;i<comp;i++)
		offset[i]=inbits.InputBits(20)+14;
	
	for(i=0;i<comp;i++)
	{
		//pin定位到通道压缩数据的起始位置
		if(!edas.UncompressData(&pin[offset[i]],len, &outdata[len*i]))
			return 0;
	}	 
	
	return 1;
}

int CEDASUNPackData::UncmpHEdasfrm(char *pin,int rec_len,int flag,long * second,int * frac_sec,int * samp,int * phase,int * chn_id,int * cali_flag,char * outdata,int out_len)
{
	if(pin==NULL || outdata==NULL || rec_len<=0 || out_len<=0 || second==NULL || frac_sec==NULL || samp==NULL || chn_id==NULL || cali_flag==NULL || phase==NULL
	   || out_len<rec_len*4)
		return 0;
	
	CIn inbits;
  	CEdasUncmp edas;
	
	long *	pd=(long *)pin;
	
	*second=pd[0];
	
	//初始化
	inbits.Init(&pin[4]); 
	
	//第一个样点的秒内偏移样点
	*frac_sec=inbits.InputBits(12);
	
	//采样率标志
	int sid=inbits.InputBits(4)-1;
	if(sid<0 || sid>8)
		return 0;
	int ds[]={1,10,20,50,100,125,200,500};
	*samp=ds[sid];
	//相位
	int phid=inbits.InputBits(1);
	*phase=phid+1;
	
	//通道代码
	*chn_id=inbits.InputBits(3);
	
	//标定标志
	*cali_flag=inbits.InputBits(4);
	
	if(flag==1 && *samp>1)
		//rec_len=max(10,*samp/5);
		rec_len = 10 > *samp/5 ? 10 : *samp/5;
	//pin定位到通道压缩数据的起始位置
 	return edas.UncompressData(&pin[7],rec_len,(long *)outdata);
}

int CEDASUNPackData::UncmpLEdasfrm(char *pin, long * second,int * chn_sum,int *  chn_id,int *  cali_flag,char * outdata,int out_len)
{
	if(pin==NULL || outdata==NULL ||   out_len<=0 || second==NULL ||  chn_sum==NULL || chn_id==NULL || cali_flag==NULL )
		return 0;
	
	CIn inbits;
	
	long *	pd=(long *)pin;
	long * opd=(long *)outdata;
	long value;
	int num;
	
	*second=pd[0];
	
	//初始化
	inbits.Init(&pin[4]); 
	
	//通道总数
	*chn_sum=inbits.InputBits(4);
	if(*chn_sum<=0 || *chn_sum>6)
		return 0;
	//通道代码
	int i = 0;
	for(i=0;i<*chn_sum;i++)
		chn_id[i]=inbits.InputBits(3)+6;
	//标定标记
	for( i=0;i<*chn_sum;i++)
		cali_flag[i]=inbits.InputBits(4);
	num=(*chn_sum)*7+4;//bits
	if(num%8!=0)
		num=num/8+1+4;
	else num=num/8+4;
	
	inbits.Init(&pin[num]); 
	
	for( i=0;i<*chn_sum;i++)
 		opd[i]=inbits.InputBits(32);
	
	return 1;
}
