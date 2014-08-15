#include <stdio.h>

#include <string.h>

#include <time.h>

#include "clogfile.h"



int CLogfile::WriteTolog(char * inf,char *ext)

{
	
	long t;
	
	struct tm * ctm;
	
	
	
	time(&t);
	
	ctm=localtime(&t);
	
	
	
	char filename[256];
	
	if( strlen(ext)==0)
		
		sprintf(filename,"%s/%04d%02d%02d.log",m_logpath,ctm->tm_year+1900,ctm->tm_mon+1,ctm->tm_mday,ext);
	
	else sprintf(filename,"%s/%04d%02d%02d.%s",m_logpath,ctm->tm_year+1900,ctm->tm_mon+1,ctm->tm_mday,ext);
	
	
	
	if(!Write(filename,inf))
		
		return 0;
	
	return 1;
	
}



int CLogfile::Write(char * filename,char * inf)

{
	
	FILE * fp;
	
	
	
	fp=fopen(filename,"r");
	
	if(fp==NULL)
		
		fp=fopen(filename,"w");
	
	else{
		
		fclose(fp);
		
		fp=fopen(filename,"ab");
		
		if(fp==NULL)
			
			return 0;
		
	}
	
 	long t;
	
	struct tm * ctm;
	
	char buf[200];
	
	
	
	time(&t);
	
	ctm=localtime(&t);
	
	fprintf(fp,"%d:%d:%d: %s\n",ctm->tm_hour,ctm->tm_min,ctm->tm_sec,inf);

	fclose(fp);

	return 1;

}
