#ifndef __CLOGFILE_H__

#define __CLOGFILE_H__

//日志文件



class CLogfile{
	
public:
	
	//根据m_logpath,ext和系统时间生成日志文件，写文件
	
	//inf: 日志内容，ext：日志文件扩展名
	
	int WriteTolog(char * inf,char *ext);
	
	//写日志到filename，filename:日志文件名，inf:日志内容
	
	int Write(char * filename,char * inf);
	
	//日志文件目录
	
	char m_logpath[256];
	
};



#endif

