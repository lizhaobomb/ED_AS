#include "SineCal.h"
#include <stdio.h>

void saveFeqToFile (char * fileName, char *content)
{
	FILE * fp;
	fp=fopen(fileName,"r");
	if(fp!=NULL) { fclose(fp); return;}
	fclose(fp);
	fp=fopen(fileName, "w+");
	if(fp==NULL) 
		return;
	fprintf(fp,"%s",content);
	fclose(fp);		
}

void initSaveFeqToFile(char *home)
{
	char temp[1000];
	// 短周期地震计 sine_short.cal
	sprintf(temp,"%s/cfg/%s",home, "sine_short.cal");

	char *c1 = "15\n\
	5	    -10	      1\n\
	8	     -5	      1\n\
	15	     -2	      1\n\
	30	      1	      1\n\
	60	      2	      1\n\
	150	      5	      1\n\
	300	     10	      1\n\
	450	     15	      1\n\
	540	     18	      1\n\
	600	     20	      1\n\
	660	     22	      1\n\
	690	     23	      1\n\
	990	     33	      1\n\
	1260	     42	      1\n\
	1320	     44	      1\n";
	saveFeqToFile(temp, c1);
	
	
	// 宽频带地震计  sine_band.cal
	sprintf(temp,"%s/cfg/%s", home, "sine_band.cal");
	
	char *c2 = "21\n\
	3    -100     250\n\
	3     -70     250\n\
	3     -50     250\n\
	3     -30     250\n\
	4     -20     250\n\
	4     -15     250\n\
	5     -10     190\n\
	8      -5     100\n\
	15      -2      40\n\
	30       1      20\n\
	60       2      10\n\
	150       5       4\n\
	300      10       2\n\
	450      15       1\n\
	540      18       1\n\
	600      20       1\n\
	660      22       1\n\
	690      23       1\n\
	990      33       1\n\
	1260      42       1\n\
	1320      44       1\n";
	
	saveFeqToFile(temp, c2);
	
	
	// 甚宽频带地震计  sine_vband.cal
	sprintf(temp,"%s/cfg/%s", home, "sine_vband.cal");
	char *c3 = "3\n\
	2 -120 1000\n\
	50 -10 100\n\
	500 1 100\n";
	saveFeqToFile(temp, c3);
	
	// BBVS-60_50HZ sine_bbvs60_50.cal
	sprintf(temp,"%s/cfg/%s", home, "sine_bbvs60_50.cal");
	char *c4 = "13\n\
	2 -200 320\n\
	2 -100 480\n\
	3 -60 400\n\
	4 -40 320\n\
	5 -20 160\n\
	10 -10 80\n\
	100 1 8\n\
	150 5 2\n\
	270 9 2\n\
	450 15 2\n\
	570 19 2\n\
	660 22 2\n\
	690 23 2\n";
	saveFeqToFile(temp, c4);
	
	//BBVS-60_100HZ sine_bbvs60_100.cal
	sprintf(temp,"%s/cfg/%s", home, "sine_bbvs60_100.cal");
	char *c5 = "13\n\
	2 -200 320\n\
	2 -100 480\n\
	3 -60 400\n\
	4 -40 320\n\
	5 -20 160\n\
	10 -10 80\n\
	100 1 8\n\
	150 5 2\n\
	270 9 2\n\
	570 19 2\n\
	1170 39 2\n\
	1260 42 2\n\
	1320 44 2\n";
	saveFeqToFile(temp, c5);
	
	// BBVS-60_200HZ sine_bbvs60_200.cal
	sprintf(temp,"%s/cfg/%s", home, "sine_bbvs60_200.cal");
	char *c6 = "18\n\
	2 -200 320\n\
	2 -100 480\n\
	3 -60 400\n\
	4 -40 320\n\
	5 -20 160\n\
	10 -10 80\n\
	100 1 8\n\
	150 5 2\n\
	270 9 2\n\
	570 19 2\n\
	990 33 2\n\
	1170 39 2\n\
	1320 44 2\n\
	1470 49 2\n\
	1560 52 2\n\
	1740 58 2\n\
	1920 64 2\n\
	2400 80 2\n";
	saveFeqToFile(temp, c6);
	
	
	// BBVS-120_50HZ sine_bbvs120_50.cal
	sprintf(temp,"%s/cfg/%s", home, "sine_bbvs120_50.cal");
	char *c7 = "13\n\
	2 -200 320\n\
	2 -100 480\n\
	3 -60 400\n\
	4 -40 320\n\
	5 -20 160\n\
	10 -10 80\n\
	100 1 8\n\
	150 5 2\n\
	270 9 2\n\
	450 15 2\n\
	570 19 2\n\
	660 22 2\n\
	690 23 2\n";
	saveFeqToFile(temp, c7);
	
	
	 //BBVS-120_100HZ sine_bbvs120_100.cal
	sprintf(temp,"%s/cfg/%s", home, "sine_bbvs120_100.cal");
	char *c8 = "13\n\
	2 -200 320\n\
	2 -100 480\n\
	3 -60 400\n\
	4 -40 320\n\
	5 -20 160\n\
	10 -10 80\n\
	100 1 8\n\
	150 5 2\n\
	270 9 2\n\
	570 19 2\n\
	1170 39 2\n\
	1260 42 2\n\
	1320 44 2\n";
	saveFeqToFile(temp, c8);
	
	 //BBVS-120_200HZ sine_bbvs120_200.cal
	sprintf(temp,"%s/cfg/%s", home, "sine_bbvs120_200.cal");
	//char *c9 = "0\n";
	saveFeqToFile(temp, c6);
	
}