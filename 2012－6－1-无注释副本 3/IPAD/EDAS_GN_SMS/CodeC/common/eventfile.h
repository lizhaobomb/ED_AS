//事件文件格式 加union支持测震，强震两种格式
#ifndef __EVENTFILE_H
#define __EVENTFILE_H

#define UD 0
#define EW 1
#define NS 2

#define DISPLACEMENT 0
#define VELOCITY 1
#define ACCELERATION 2

#define INTEG 0x00000001
#define DERIV 0x00000002
#define RESCO 0x00000004
#define ROTAT 0x00000008
#define FILTR 0x00000010
#define BIASC 0x00000020//mean
#define LINER 0x00000040//lineregress
#define ADDINS 0x00000080
#define REVISE 0x00000100

///#define GPS 0
#define BPM 1

typedef struct
{
	short year,mon,day;
	short hour,min,sec;
}WD_TIME;

typedef struct 
{
	double seis_start;
	float latitude;
	float longitude;
	float depth;
	float Ml,Md,Ms,Mb;
	char loacte[12];
}SEIS_PAR;

typedef struct
{
	char opr_name[12];
	long opr_id;
	long loadtime;
	WD_TIME savetime;
}SIGN_PAR;
	
typedef struct
{
	SEIS_PAR seispar;
	SIGN_PAR signup;
}SEIS_SIGNUP;

typedef struct 
{
	char NetName[80];
	long stn_sum;
	float centlat;
	float centlon;
	float centalt;
	
}NETPAR;

typedef struct
{
    char EvtFlag[16];
	char hostype[16];
	long rectype;
	long process;
    NETPAR netpar;
	char seis_par[120];
    long data_begin;
    WD_TIME data_start;
    long rec_length;
}NETEVT_HEAD;
typedef struct
{
	char	name[12];
	int		no;
	float	factor;
	int		respstep;
	char	resp[12288];
}CHA_PAR;
typedef struct
{
	short	TranMode;
	short   TimerMode;
	float	ins_damp,ins_freq,ins_hfreq;
}TRFINSPAR;

typedef struct{
	short	TranMode;
	char	TranIns[12];
	short   TimerMode;
}TRANSPAR;

typedef union {
		TRFINSPAR tin;
		TRANSPAR tf;
}TFUNION ;
typedef struct 
{
    long	no;
    char	name[20];
	char	DASType[10];
	short   wlen;
	float   VLtd;
    long	samp;
    long	comp;
	TFUNION tfin;
	char	ClkType[12];
	float	ClkErr;
    float	latitude;
    float   longitude;
    float	altitude;
	float   azimuth,incident;
    short   weight;
    short   veloc;
	int		oritype;
    char    seismometer[8];
	long    ssmode;
}STN_PAR;     
#define EVTHEADLEN sizeof(NETEVT_HEAD)
//#define EVTHEADLEN 276
#define STNLEN sizeof(STN_PAR)
//#define STNLEN 124
#define CHALEN sizeof(CHA_PAR)


#endif