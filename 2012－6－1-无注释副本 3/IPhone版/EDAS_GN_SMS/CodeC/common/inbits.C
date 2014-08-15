
#include "inbits.h"
 

//retrive bits from LSB
unsigned long CIn::InputBits(int bit_count)
{
	unsigned long mask,end;
	unsigned long return_value;

	mask=1L;
	end=1L<<(bit_count-1);
	return_value=0;
	while(end!=0)
	{
		if(mark==0x1)
		{
			rack=inbytes[readbytes];
			readbytes++;
		}
		if(rack & mark)
			return_value |=mask;
		mask<<=1;
		mark<<=1;
		end>>=1;
		if(mark==0x100)
			mark=0x1;
	}
	return(return_value);
}
long CIn::Convert_to_data(long data,int ni)
{
	long temp;  
	long T;
	char d1,d2,d3;
	temp=data>>(ni-1);
	
	if(temp)
	{
	//	d1=data & 0xff;
	//	d2=(data >> 8) &0xff;
	//	d3=(data >> 16) & 0xff;
	//	data=(d3*65536) & 0xffff0000;
	//	data+=(d2 & 0xff)<<8;
	//	data+=d1 & 0xff;
		T=1<<ni;
		for(int i=0;i<32-ni;i++)
		{
			data|=T;
			T<<=1;
		}
	}/*else{
		d1=data & 0xff;
		d2=(data >> 8) &0xff;
		d3=(data >> 16) & 0xff;
		data=(d3*65536) & 0xffff0000;
		data+=(d2 & 0xff)<<8;
		data+=d1 & 0xff;
	}*/
	return data;
}
