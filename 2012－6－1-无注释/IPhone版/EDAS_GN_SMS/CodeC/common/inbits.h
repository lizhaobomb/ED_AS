#ifndef __INBITS_H__
#define __INBITS_H__

class CIn
{
	public :
		int rack;
		int mark;
		int readbytes;
		char *inbytes;
		CIn(){};
		~CIn(){};
		unsigned long InputBits(int);
		long Convert_to_data(long,int);
		void Init(char * pinbytes){
					rack=0;
					mark=0x01;
					readbytes=0;
					inbytes=pinbytes;
					}
};
#endif