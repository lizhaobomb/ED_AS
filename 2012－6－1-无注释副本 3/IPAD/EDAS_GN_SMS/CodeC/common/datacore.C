#include "datacore.h"

#include "complex.h"



long GetSampleData(int i,int wlen,void *data)

{     
	
	switch(wlen) {
			
		case   8: {
			
			char *lpdata=(char *)data;
			
			return lpdata[i];
			
		}
			
			//return (data->data8[i]);
			
		case  16:{
			
			short *lpdata=(short *)data;
			
			return lpdata[i];
			
		}
			
			//return (data->data16[i]);
			
		case  32:{
			
			long * lpdata=(long *)data;
			
			return lpdata[i];
			
		}
			
			//return (data->data32[i]);
			
	};
	
	return 0;
	
}



void  SetSampleData(int i,int wlen,void * data,int val)

{  
	
	switch(wlen) {
			
		case   8: {
			
			char * lpdata=(char *)data;
			
			//data->data8[i] = val;
			
			lpdata[i]=val;
			
			break;
			
		}
			
		case  16: {
			
		    short * lpdata=(short *)data;
			
			lpdata[i]=val;
			
			//data->data16[i] = val;
			
			break;
			
		}
			
		case 24:
			
		case  32: {
			
		    long * lpdata=(long *)data;
			
			lpdata[i]=val;
			
			//data->data32[i] = val;
			
			break;
			
		}
			
	};
	
}

double  GetSampleDataf(int i,int wlen,void *data)

{     
	
	switch(wlen) {
			
		case 24:
			
		case  32:{
			
			float * lpdata=(float *)data;
			
			return lpdata[i];
			
		}
			
		case 64: {
			
			double *lpdata=(double *)data;
			
			return lpdata[i];
			
		}
			
			//return (data->data32[i]);
			
	};
	
	return (double)0;
	
}



void  SetSampleDataf(int i,int wlen,void * data,double val)

{  
	
	switch(wlen) {
			
		case  32: {
			
		    float * lpdata=(float *)data;
			
			lpdata[i]=(float)val;
			
			//data->data32[i] = val;
			
			break;
			
		}
			
		case 64:{
			
			double * lpdata=(double *)data;
			
			lpdata[i]=val;
			
			break;
			
		}
			
	};
	
}



double  response1(ecomplex * pole,ecomplex *zero,int nop,int noz,float freq)

{
	
	int i;
	
	ecomplex rsp,jomg,c;
	
	
	
	rsp.real=1.f;
	
	rsp.image=0.f;
	
	
	
	jomg.real=0;
	
	jomg.image=2*PI*freq;
	
	
	
	for(i=0;i<noz;i++)
		
	{
		
		csub(c,jomg,zero[i]);
		
		cmult(rsp,rsp,c);
		
	}
	
	
	
	for(i=0;i<nop;i++)

	{

		csub(c,jomg,pole[i]);

		cdiv(rsp,rsp,c);

	}

	return cabs(rsp);

}



