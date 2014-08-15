#ifndef __EVTSHARE_H__
#define __EVTSHARE_H__

typedef struct tagSTATION_DATA{
	long flag;
	char *data;
	struct tagSTATION_DATA *lpnext;
}STATION_DATA;

typedef struct tagMULDATABUF{
	long wr_time;//the time of write to buffer,-1: free
	long data_time;//the time of data
	STATION_DATA *sdata;
	struct tagMULDATABUF * lpnext,*lplast;
}MULDATABUF;

#endif