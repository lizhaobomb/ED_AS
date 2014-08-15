#ifndef __SAVEEVTARRAY_H__
#define __SAVEEVTARRAY_H__

//保存数据文件  20034/3/8

#include "eventfile.h"
#include "response.h"
#include "rcvstruc.h"
#include <stdio.h>

#define SAVEEVTARRAYSIZE 40
class CSaveEVTArray{
public:
	SITEREVBLK *m_revblk;//数据区
	//CFile m_file[SAVEEVTARRAYSIZE];
	FILE ** m_file;
	char ** m_flname;
	//CStringArray m_flname;
	NETEVT_HEAD m_evthead[ SAVEEVTARRAYSIZE];
	//NETEVT_HEAD *m_evthead;
	STN_PAR m_stnpar[ SAVEEVTARRAYSIZE];
	CHA_PAR m_chnpar[ SAVEEVTARRAYSIZE][3];
	
	//STN_PAR *m_stnpar;
	//CHA_PAR **m_chnpar;
	
	long m_wrsec[SAVEEVTARRAYSIZE];
	

public:
	CSaveEVTArray();
	~CSaveEVTArray();


	BOOL CopyDataToFl(int channel);
	void SetEvtTime(long time,int channel);

	BOOL WriteData(short *pdata,int channel);
	BOOL WriteDatablock(char * lpdata,long sec,long dflag,int channel);

private:

	BOOL WriteToDataBuf1(short * pdata,int channel);
	BOOL WriteToDataBufO(short *pdata,int channel);


};
#endif