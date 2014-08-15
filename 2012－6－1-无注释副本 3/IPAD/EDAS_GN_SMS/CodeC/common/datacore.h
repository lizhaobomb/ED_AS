#ifndef _datacore_h

#define _datacore_h



#include "response.h"



#ifndef PI

#define PI 3.1415926

#endif 



long GetSampleData(int ,int ,void *);

void  SetSampleData(int ,int ,void *,int);

double  GetSampleDataf(int ,int ,void *);

void  SetSampleDataf(int ,int ,void *,double);

double  response1(ecomplex * pole,ecomplex *zero,int nop,int noz,float freq);



#endif
