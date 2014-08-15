#ifndef _jtime_h
#define _jtime_h
#define isaleap(year) (((year%100 != 0) && (year%4 == 0)) || (year%400 == 0))

typedef struct tagTIME
{
	int year,mon,day,hour,min,sec;
}TIME;

long  julian (TIME gt);

#endif