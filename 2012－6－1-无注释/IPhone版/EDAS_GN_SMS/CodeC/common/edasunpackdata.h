#ifndef __CEDASUNPACKDATA_H__
#define __CEDASUNPACKDATA_H__

/************************************************************************************/
/*功能：定义通用数据采集器快速数据服务高速通道压缩数据帧和连续波形文件的压缩数据帧的解码
/*作者：陈阳
/*create date: 2009-9-21
/*update
/*		date			action				common
		2009-9-29		add					UncmpLEdasfrm解压缩低速通道数据帧
						edit				UncmpEdasfrm改名为UncmpHEdasfrm，修改：增加输出相位
		2010-5-6		edit				UncmpHEdasfrm:输入flag==1 且samp>1: 自动计算rec_len=max(10,samp/5)
/*company：geodevice
/***********************************************************************************/


#include "edasuncmp.h"

class CEDASUNPackData
{
public:
	CEDASUNPackData()
	{
	}
	
	~CEDASUNPackData()
	{
	}
 	//解压缩数据生成连续波形文件数据帧
	//输入:
	//	pin: 压缩数据，格式: 连续波形文件数据帧
	//  in_len: pin存储的数据长度,单位:字节
	//  samp: 采样率
	//  comp: 通道总数
 	//  out_len:  outdata最大长度,单位:字节 
	//输出:
	//	outdata: 解压缩后的波形数据,按长整数存储,全部通道. 数据按照: [通道1全部数据], [通道2全部数据], [通道3全部数据]
	//	rec_len: 数据帧中数据记录长度,单位: 秒
 	//  cali_flag: 标定标记（0：普通数据，1：脉冲标定，2：正弦标定，3：随机标定）
	//  time:时间码
	//返回: 1-成功,0-失败
	int UncmpFileblk(char *pin,int in_len,int samp,int comp,long * time,int * rec_len,int * cali_flag,long * outdata,int  out_len);

	//解压缩数据生成快速数据服务高速通道压缩数据帧的数据部分,前提:  压缩前的记录长度已知
	//输入:
	//	pin: 压缩数据，格式: 快速数据服务高速通道压缩数据帧,从时间码开始
	//  rec_len:  压缩前的记录长度,单位:个
	//	flag: flag==1 且 采样率>1时, 自动计算rec_len, rec_len=max(10,samp/5)
 	//  out_len:  outdata最大长度,单位:字节 
 	//输出:
	//	outdata: 解压缩后的波形数据,按长整数存储
	//	second:  时间码
	//	frac_sec: 第一个样点的秒内偏移样点
	//	samp: 采样率
	//	phase: 相位
	//	chn_id: 通道代码,从1开始
 	//  cali_flag: 标定标记（0：普通数据，1：脉冲标定，2：正弦标定，3：随机标定）
 	//返回: 1-成功,0-失败
 	int UncmpHEdasfrm(char *pin,int rec_len,int flag,long * second,int * frac_sec,int * samp,int * phase,int * chn_id,int * cali_flag,char * outdata,int out_len);
  
	//解压缩数据生成快速数据服务低速通道压缩数据帧的数据部分 
	//输入:
	//	pin: 压缩数据，格式: 快速数据服务高速通道压缩数据帧,从时间码开始
  	//  out_len:  outdata最大长度,单位:字节 
 	//输出:
	//	outdata: 解压缩后的波形数据,按长整数存储
	//	second:  时间码
	//  chn_sum：通道总数
  	//	chn_id: 通道代码数组，最大数组长度6，从7开始
 	//  cali_flag: 标定标记数组，最大数组长度6（标定标记为0：普通数据，1：脉冲标定，2：正弦标定，3：随机标定）
 	//返回: 1-成功,0-失败
 	int UncmpLEdasfrm(char *pin, long * second,int * chn_sum,int *  chn_id,int *  cali_flag,char * outdata,int out_len);

};

#endif  
