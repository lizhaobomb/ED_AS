 
#ifndef __EDASUNCMP_H__
#define __EDASUNCMP_H__

/************************************************************************************/
/*功能：定义EDAS数据压缩算法解压缩部分
/*作者：陈阳
/*create date: 2009-9-4
/*company：geodevice
/***********************************************************************************/

#include "inbits.h"

#define MAXCMPDATALEN 5//5秒
#define MAXCINDEX2LEN MAXCMPDATALEN*500/10//二级索引最大长度
 
 
class CEdasUncmp
{
public:
	CEdasUncmp()
	{
	}
	
	~CEdasUncmp()
	{
	}
	
	//解压缩,前提: 原始被压缩数据长度已知
	//输入:
	//	pin: 压缩数据, 格式:
 		//FIRST		long	4		第一个样点
		//IDX1 		bit		5		第一级索引
		//IDX2		bit		2*N		第二级索引，每个索引占2bit，N定义见*1
		//CDATA1	bit		9*N1	从第2个样点到第9个样点的压缩数据，N1-样点有效字长，见*2
		//CDATA2	bit		10*N2	第10到20个样点的压缩数据，N2-样点有效字长 见*2
		//…			
		//CDATAN 	bit		最后10个样点的压缩数据 见*2
		//UNUSED	bit		填0补足双字节

	//  rec_len: 原始数据长度,单位:个
	//输出:
	//	outdata: 解压缩数据,每个数据按长整数存储
 	//返回: 1-成功,0-失败
	int UncompressData(char * pin,int rec_len,long * out_data);

 
};

#endif  
