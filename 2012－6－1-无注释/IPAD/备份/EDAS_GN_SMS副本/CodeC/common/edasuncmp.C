/************************************************************************************/
/*功能：定义EDAS数据压缩算法解压缩部分
/*作者：陈阳
/*create date: 2009-9-4
/*company：geodevice
/***********************************************************************************/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "geo_codetab.h"
#include "edasuncmp.h"

int CEdasUncmp::UncompressData(char * pin,int rec_len,long * out_data)
{
	int cindex1,cindex2[MAXCINDEX2LEN];
	long  difs[500*MAXCMPDATALEN];
	int num ;
	CIn inbits;
	int i,j,k;
	long * pd=(long *)pin;
	
	num=rec_len/10;
	if(rec_len%10!=0)
		num++;

	//初始化
	inbits.Init(pin); 

	//第一个样点
	difs[0]=inbits.InputBits(32);
	
	//第一级索引
 	cindex1=(int)inbits.InputBits(5); 
	if(cindex1<0 || cindex1>=NUMOFONEIDX)  
 		return 0;

	//第二级索引
	for(i=0;i<num;i++) 
	{
		cindex2[i]=(int)inbits.InputBits(2);
		if(cindex2[i]<0 || cindex2[i]>3)//cindex2 must be 0-3
			return 0;
	 
		cindex2[i]=geo_codetab[cindex1][cindex2[i]];
	}
	//取压缩数据
	j=1;
	i=1;
	k=0;
 	for( k=0;k<num;k++)
	{
		for(;j<10 && i<rec_len;j++)
		{
 			difs[i]=inbits.InputBits(cindex2[k]);
			difs[i]=inbits.Convert_to_data(difs[i],cindex2[k]);
			i++;
		}
	
		j=0;
	}
	if(cindex1==NUMOFONEIDX-1)
	{
		//32位存储,没有做压缩,直接拷贝到输出缓冲区
		memcpy(out_data,difs,rec_len*4);
		return 1;
	}
	//还原查分值
	for(i=1;i<rec_len;i++)
		difs[i]+=difs[i-1];

	memcpy(out_data,difs,rec_len*4);
 
	return 1; 
}