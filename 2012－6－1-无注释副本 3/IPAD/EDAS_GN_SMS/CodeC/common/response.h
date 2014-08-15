/*response.h*/
#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#define PAZTYPE 0
#define FAPTYPE 1
#define	FIRTYPE 2

typedef struct ecomplex{
	float real,image;
}ecomplex;

typedef struct{
	float freq,amp,phase;
}FAP;

typedef struct{
	long type;
	float ao;
	long nop;
	ecomplex *pole;
	long noz;
	ecomplex *zero;
}RESPAZ;

typedef struct{
	long type;
	char name[20];
	long noc;
	float *coe;
}RESFIR;

typedef struct{
	long type;
	long ntrip;
	FAP *fap;
}RESFAP;

typedef union {
	long	type;
	RESPAZ  ResPaz;
	RESFAP	ResFap;
	RESFIR  ResFir;
}RESPONSE;
#endif

