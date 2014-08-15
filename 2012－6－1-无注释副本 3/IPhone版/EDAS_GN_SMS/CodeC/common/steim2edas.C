
#include <stdlib.h>
#include <stdio.h>
/*update 2004-7-1
 ∏¸∏ƒπÃ∂®µƒ3Õ®µ¿—πÀıŒ™∏˘æ›≤Œ ˝comp(∑÷œÚ)»∑∂® ˝æ›≥Ã∂»
 2006-7-1
 ≤…—˘¬ 1hz£∫0x55aaµƒ÷°≥§=18
 */
#include <string.h>
#include <string.h>
#include <math.h>

#include "steim2edas.h"

CSteim2 :: CSteim2() {
	st2fr.flag  = 0x55ce;
	dafra.flag  = 0x55aa;
}

CSteim2 :: ~CSteim2() {
}

int CSteim2 :: ZipASeries(long *data_in,char *data_out,int samp)
{
	long  dif[1510];
	int   rec_len;
	int   k;
	int   max,min;
	
	memset(dif,0,6040);
	int i = 0;
	for(int i=1;i<samp;i++) {
		dif[i-1] = data_in[i]-data_in[i-1];
	}
	
	memset((char *)&subFra,0,64);
	rec_len = 0;
	k       = 1;
	subFra.data[0].d32b = data_in[0];
	for(i=0;i<samp;) {
		max = dif[i];
		min = dif[i];
		if(dif[i+1] > max) max = dif[i+1];
		if(dif[i+1] < min) min = dif[i+1];
		if(max > 16383 || min < -16384)  { // 30 bits.
			SetIdBit(k,2);
			subFra.data[k].d30b.dnib  = 1;
			subFra.data[k].d30b.v     = dif[i]&0x3fffffff;
			i ++;
			k ++;
			goto check;
		}
		
		if(dif[i+2] > max) max = dif[i+2];
		if(dif[i+2] < min) min = dif[i+2];
		if(max > 511 || min < -512) {
			SetIdBit(k,2);
			subFra.data[k].d15b.dnib  = 2;
			subFra.data[k].d15b.v0    = dif[i] & 0x7fff;
			subFra.data[k].d15b.v1    = dif[i+1] & 0x7fff;
			k ++;
			i += 2;
			goto check;
		} 
		
		if(dif[i+3] > max) max = dif[i+3];
		if(dif[i+3] < min) min = dif[i+3];
		if(max > 127 || min < -128) {  // 10 bits.
			SetIdBit(k,2);
			subFra.data[k].d10b.dnib  = 3;
			subFra.data[k].d10b.v0 = dif[i] & 0x3ff;
			subFra.data[k].d10b.v1 = dif[i+1] & 0x3ff;
			subFra.data[k].d10b.v2 = dif[i+2] & 0x3ff;
			i += 3;
			k ++;
			goto check; 
		}
		
		if(dif[i+4] > max) max = dif[i+4];
		if(dif[i+4] < min) min = dif[i+4];
		if(max >31 || min < -32) {    // 8 bits.
			SetIdBit(k,1);
			subFra.data[k].d08b.v0  = dif[i] & 0xff;
			subFra.data[k].d08b.v1  = dif[i+1] & 0xff;
			subFra.data[k].d08b.v2  = dif[i+2] & 0xff;
			subFra.data[k].d08b.v3  = dif[i+3] & 0xff;
			i += 4;
			k ++;
			goto check;
		}
		
		if(dif[i+5] > max) max = dif[i+5];
		if(dif[i+5] < min) min = dif[i+5];
		if(max > 15 || min < -16) {   // 6 bits.
			SetIdBit(k,3);
			subFra.data[k].d06b.dnib  = 0;
			subFra.data[k].d06b.v0 = dif[i] & 0x3f;
			subFra.data[k].d06b.v1 = dif[i+1] & 0x3f;
			subFra.data[k].d06b.v2 = dif[i+2] & 0x3f;
			subFra.data[k].d06b.v3 = dif[i+3] & 0x3f;
			subFra.data[k].d06b.v4 = dif[i+4] & 0x3f;
			i += 5;
			k ++;
			goto check;
		}
		
		if(dif[i+6] > max) max = dif[i+6];
		if(dif[i+6] < min) min = dif[i+6];
		if(max > 7 || min < -8)  {  // 5 bits.
			SetIdBit(k,3);
			subFra.data[k].d05b.dnib  = 1;
			subFra.data[k].d05b.v0 = dif[i] & 0x1f;
			subFra.data[k].d05b.v1 = dif[i+1] & 0x1f;
			subFra.data[k].d05b.v2 = dif[i+2] & 0x1f;
			subFra.data[k].d05b.v3 = dif[i+3] & 0x1f;
			subFra.data[k].d05b.v4 = dif[i+4] & 0x1f;
			subFra.data[k].d05b.v5 = dif[i+5] & 0x1f;
			
			i += 6;
			k ++;
			goto check;
		}
		
		SetIdBit(k,3);
		subFra.data[k].d04b.dnib  = 2;
		subFra.data[k].d04b.v0    = dif[i] & 0xf;
		subFra.data[k].d04b.v1    = dif[i+1] & 0xf;
		subFra.data[k].d04b.v2    = dif[i+2] & 0xf;
		subFra.data[k].d04b.v3    = dif[i+3] & 0xf;
		subFra.data[k].d04b.v4    = dif[i+4] & 0xf;
		subFra.data[k].d04b.v5    = dif[i+5] & 0xf;
		subFra.data[k].d04b.v6    = dif[i+6] & 0xf;
		i += 7;
		k ++;
		
	check:
		if(k >= 15) {
			memcpy(&data_out[rec_len],(char *)&subFra,64);
			memset((char *)&subFra,0,64);
			k        = 0;
			rec_len += 64;
		}
    }
	
	if(k != 0) {
		memcpy(&data_out[rec_len],(char *)&subFra,(k+1)*4);
		rec_len  += (k+1)*4;
	}
	
	return rec_len;
}

EDAFRAME* CSteim2 :: UnzipSteim2(STEIM2FRA *f_st2,int comp)
{
	char     *cptr,p_chk;
	char      d_att;
	short    *sptr,chk_sum;
	long      data[1510];
	int       k,samp;
	
	cptr  = (char *)&f_st2->sen_id;
	
	chk_sum = 0;
	sptr    = &f_st2->sen_id;
	int i =0;
	for(i=0;i<(f_st2->fra_len+6)/2;i++) 
		chk_sum += sptr[i]; 
	if(chk_sum != 0) 
		return NULL;
	
	d_att   = (f_st2->d_att>>4) &0xf;
	
	switch(d_att) {
		case 0: samp  = 1;
			break;
		case 1: samp  = 10;
			break;
		case 2: samp  = 20;
			break;
		case 3: samp  = 50;
			break;
		case 4: samp  = 100;
			break;
		case 5: samp=125;
			break;
		case 6: samp  = 200;
			break;
		case 7: samp  = 500;
			break;
	}
	
	memset(dafra.data,0,sizeof(dafra.data));
	UnzipASeries(&f_st2->data[0],data,samp*comp);
	
	memcpy((char *)&dafra.sen_id,(char *)&f_st2->sen_id,6);
	dafra.ep_sec[0]=(short)(f_st2->epsec &0xffff);
	dafra.ep_sec[1]=(short)((f_st2->epsec>>16)&0xffff);
	dafra.d_att=sptr[6] &0xf;
	
	if(samp==1 || samp==125)
		dafra.fra_len=samp*3*comp+9;
	else   dafra.fra_len = samp*3*comp+8;
	k = 0;
	for(int j=0;j<samp*comp;j++) {
		memcpy((char *)&dafra.data[k],(char *)&data[j],3);
		k += 3;
	}
	
	cptr  = (char *)&dafra.sen_id;
	p_chk = 0;
	
	
	for( i=0;i<11;i++)
		p_chk -= cptr[i];
	
	dafra.p_chk = p_chk;
	
	chk_sum = 0;
	sptr    = &dafra.sen_id;
	for( i=0;i<(dafra.fra_len+4)/2;i++) {
		chk_sum -= sptr[i]; }
	
	memcpy(&dafra.data[dafra.fra_len-8],(char *)&chk_sum,2);
	dafra.flag=0x55AA;
	return &dafra;
}     

STEIM2FRA* CSteim2 :: PackSteim2(int sen_id,long ep_sec,int d_att,long *data,int samp,int comp)
{
	char    *ptr;
	short   *sptr,chk_sum;
	
	st2fr.sen_id     = sen_id;
	st2fr.fra_len    = 8;
	st2fr.d_att      = d_att;
	switch(samp) {
		case 1:  st2fr.d_att  |= 0x00;
			break;
		case 10: st2fr.d_att  |= 0x10;
			break;
		case 20: st2fr.d_att  |= 0x20;
			break;
		case 50: st2fr.d_att  |= 0x30;
			break;
		case 100:st2fr.d_att  |= 0x40;
			break;
		case 125:st2fr.d_att  |= 0x50;
			break;
		case 200:st2fr.d_att  |= 0x60;
			break;
		case 500:st2fr.d_att  |= 0x70;
			break;
	}
	
	memcpy((char *)st2fr.epsec,(char *)&ep_sec,4);
	memset(st2fr.data,0,4560);
	ptr            = &st2fr.data[0];
	st2fr.fra_len += ZipASeries(data,ptr,samp*comp);
	
	ptr = (char *)&st2fr.sen_id;
	st2fr.p_chk = 0;
	int i = 0;
	for( i=0;i<11;i++)
		st2fr.p_chk -= ptr[i];
	
	sptr = &st2fr.sen_id;
	chk_sum = 0;
	for( i=0;i<(st2fr.fra_len+4)/2;i++) {
		chk_sum -= sptr[i];  }
	
	memcpy((char *)&st2fr.data[st2fr.fra_len-8],(char *)&chk_sum,2);
	
	return &st2fr;
}

int CSteim2 :: UnpackSteim2(STEIM2FRA* f_st2,int *sen_id,long *ep_sec,int *d_att,long *data,int *samp,int comp)
{
	char     *cptr,p_chk;
	short    *sptr,chk_sum;
    
	cptr  = (char *)&f_st2->sen_id;
	p_chk = 0;
	int i = 0;
	for(i=0;i<12;i++)
		p_chk -= cptr[i];
	if(p_chk != 0) return -1;
	
	chk_sum = 0;
	sptr    = &f_st2->sen_id;
	for( i=0;i<(f_st2->fra_len+6)/2;i++) {
		chk_sum += sptr[i]; }
	if(chk_sum != 0) return -1;
	
	*sen_id  =   f_st2->sen_id;
	memcpy((char *)ep_sec,(char *)f_st2->epsec,4);
	
	*d_att   = f_st2->d_att;
	*d_att  &= 0xf0;
	*d_att  /= 16;
	switch(*d_att) {
		case 0: *samp  = 1;
			break;
		case 1: *samp  = 10;
			break;
		case 2: *samp  = 20;
			break;
		case 3: *samp  = 50;
			break;
		case 4: *samp  = 100;
			break;
		case 5: *samp  = 125;
			break;
		case 6: *samp  = 200;
			break;
		case 7: *samp  = 500;
			break;
	}
	
	*d_att  = f_st2->d_att&0x0f;
	
	UnzipBSeries(&f_st2->data[0],data,(*samp)*comp); 
	// UnzipASeries(&f_st2->data[0],data,(*samp)*comp); 
	
	return 0;
}

STEIM2FRA* CSteim2 :: ZipSteim2(EDAFRAME* f_eda,int comp) {
	char      *cptr,p_chk;
	short     *sptr,chk_sum;
	long       data[1510];
	int        samp;
	STEIM2FRA *p_frz;
	
	cptr  = (char *)&f_eda->sen_id;
	p_chk = 0;
	int i = 0;
	for(i=0;i<12;i++)
		p_chk += cptr[i];
	if(p_chk != 0) return NULL;
	
	chk_sum = 0;
	sptr    = &f_eda->sen_id;
	for( i=0;i<(f_eda->fra_len+6)/2;i++) {
		chk_sum += sptr[i]; }
	if(chk_sum != 0) return NULL;
	
	if(125*3*comp+9==f_eda->fra_len)
		samp=125;
	else if(3*comp+9==f_eda->fra_len)
		samp=1;
	else   samp = (f_eda->fra_len-8)/(3*comp);
	
	memset(data,0,6040);
	for( i=0;i<comp;i++) { 
		for(int j=0;j<samp;j++) {
			memcpy((char *)&data[samp*i+j],&f_eda->data[(samp*i+j)*3],3);
			if(data[samp*i+j] & 0x800000) data[samp*i+j] |= 0xff000000;
		}
	}
	
	p_frz = PackSteim2(f_eda->sen_id,*((long *)f_eda->ep_sec),f_eda->d_att,data,samp,comp);
	return p_frz;
}

inline void CSteim2 :: SetIdBit(int k,int flag) {
	switch(k) {
		case 0: subFra.id.c0  = flag;
            break;
		case 1: subFra.id.c1  = flag;
            break; 
		case 2: subFra.id.c2  = flag;
            break; 
		case 3: subFra.id.c3  = flag;
            break;
		case 4: subFra.id.c4  = flag;
            break; 
		case 5: subFra.id.c5  = flag;
            break; 
		case 6: subFra.id.c6  = flag;
            break; 
		case 7: subFra.id.c7  = flag;
            break;
		case 8: subFra.id.c8  = flag;
            break; 
		case 9: subFra.id.c9  = flag;
            break; 
		case 10:subFra.id.c10 = flag;
            break; 
		case 11:subFra.id.c11 = flag;
            break;
		case 12:subFra.id.c12 = flag;
            break; 
		case 13:subFra.id.c13 = flag;
            break; 
		case 14:subFra.id.c14 = flag;
            break; 
    }
}

int CSteim2 :: UnzipASeries(char *data_in,long *data_out,int samp) {
	SubFrame *p_sub;
	int   k,m;
	int   ck;
	long  dif[8];
	int j;
	p_sub = (SubFrame *)data_in;
	
	data_out[0] = p_sub->data[0].d32b;
	k  = 1,  m  = 1;
	for(;;) {
		for(int i=m;i<15;i++) {
			ck = GetIdBit(p_sub->id,i);
			switch(ck) {
				case 0: break;   // other kind of data, do not care. 
				case 1: dif[0] = p_sub->data[i].d08b.v0;
					dif[1] = p_sub->data[i].d08b.v1;
					dif[2] = p_sub->data[i].d08b.v2;
					dif[3] = p_sub->data[i].d08b.v3;
					for( j=0;j<4;j++) {
						data_out[k+j] = dif[j]+data_out[k+j-1];
					}
					k += 4;
					break;
				case 2: switch(p_sub->data[i].d04b.dnib) {
					case 1: if(p_sub->data[i].d30b.v & 0x20000000)
						data_out[k] = (p_sub->data[i].d30b.v|0xc0000000)+data_out[k-1];
					else
						data_out[k] = p_sub->data[i].d30b.v+data_out[k-1];
						k ++;
						break;
					case 2: if(p_sub->data[i].d15b.v0 & 0x4000)
						data_out[k]   = (p_sub->data[i].d15b.v0|0xffff8000)+data_out[k-1];
					else
						data_out[k]   = p_sub->data[i].d15b.v0+data_out[k-1];
						if(p_sub->data[i].d15b.v1 & 0x4000)
							data_out[k+1] = (p_sub->data[i].d15b.v1|0xffff8000)+data_out[k];
						else
							data_out[k+1] = p_sub->data[i].d15b.v1+data_out[k];
						k += 2;
						break;
					case 3: if(p_sub->data[i].d10b.v0 & 0x200) 
						data_out[k]   = (p_sub->data[i].d10b.v0|0xfffffc00)+data_out[k-1];
					else
						data_out[k]   = p_sub->data[i].d10b.v0+data_out[k-1];
						if(p_sub->data[i].d10b.v1 & 0x200) 
							data_out[k+1] = (p_sub->data[i].d10b.v1|0xfffffc00)+data_out[k];
						else
							data_out[k+1] = p_sub->data[i].d10b.v1+data_out[k];
						if(p_sub->data[i].d10b.v2 & 0x200) 
							data_out[k+2] = (p_sub->data[i].d10b.v2|0xfffffc00)+data_out[k+1];
						else 
							data_out[k+2] = p_sub->data[i].d10b.v2+data_out[k+1];
						k += 3;
						break;
				}
					break;
				case 3: switch(p_sub->data[i].d04b.dnib) {
					case 0: dif[0] = p_sub->data[i].d06b.v0;
						dif[1] = p_sub->data[i].d06b.v1;
						dif[2] = p_sub->data[i].d06b.v2;
						dif[3] = p_sub->data[i].d06b.v3;
						dif[4] = p_sub->data[i].d06b.v4;
						for( j=0;j<5;j++) {
                            if(dif[j] & 0x20) dif[j] |= 0xffffffc0;
                            data_out[k+j] = dif[j]+data_out[k+j-1];
						}
						k += 5;
						break;
					case 1: dif[0] = p_sub->data[i].d05b.v0;
						dif[1] = p_sub->data[i].d05b.v1;
						dif[2] = p_sub->data[i].d05b.v2;
						dif[3] = p_sub->data[i].d05b.v3;
						dif[4] = p_sub->data[i].d05b.v4;
						dif[5] = p_sub->data[i].d05b.v5;
						for( j=0;j<6;j++) {
                            if(dif[j] & 0x10) dif[j] |= 0xffffffe0;
                            data_out[k+j] = dif[j]+data_out[k+j-1];
						}
						k += 6;
						break;
					case 2: dif[0] = p_sub->data[i].d04b.v0;
						dif[1] = p_sub->data[i].d04b.v1;
						dif[2] = p_sub->data[i].d04b.v2;
						dif[3] = p_sub->data[i].d04b.v3;
						dif[4] = p_sub->data[i].d04b.v4;
						dif[5] = p_sub->data[i].d04b.v5;
						dif[6] = p_sub->data[i].d04b.v6;
						for( j=0;j<7;j++) {
                            if(dif[j] & 0x8) dif[j] |= 0xfffffff0;
                            data_out[k+j] = dif[j]+data_out[k+j-1];
						}
						k += 7;
						break;
				}
			}
			if(k>=samp) return 0;
		}
		m     = 0;
		p_sub ++;
	}
	return 1;
}

inline int CSteim2 :: GetIdBit(STEIM2_C id,int i)
{
	switch(i) {
		case 0:  return id.c0;
		case 1:  return id.c1;
		case 2:  return id.c2;
		case 3:  return id.c3;
		case 4:  return id.c4;
		case 5:  return id.c5;
		case 6:  return id.c6;
		case 7:  return id.c7;
		case 8:  return id.c8;
		case 9:  return id.c9;
		case 10: return id.c10;
		case 11: return id.c11;
		case 12: return id.c12;
		case 13: return id.c13;
		case 14: return id.c14;
    }
}

int CSteim2 :: UnzipBSeries(char *data_in,long *data_out,int samp) {
	int    k,m,j;
	int    ck;
	int    dnib;
	long   dif[8];
	long  *lptr;
	
	lptr        = (long *)(data_in+4);
	data_out[0] = lptr[0];
	k  = 1,  m  = 1;
	for(;;) {
		for(int i=m;i<15;i++) {
			ck =  0x3<<(28-i*2);
			ck =  (*((int *)data_in)) & ck;
			ck =  ck >> (28-i*2);
			ck &= 0x3;
			//printf("ck=%d %x\n",ck,*((int *)data_in));
			switch(ck) {
				case 0: break;   // other kind of data, do not care. 
				case 1: for( j=0;j<4;j++) {
					dif[j] = (lptr[i] & (0xff000000 >> j*8))>>(3-j)*8;
					if(dif[j] & 0x80)
						data_out[k+j] = (dif[j]|0xffffff00)+data_out[k+j-1];
					else
						data_out[k+j] = dif[j]+data_out[k+j-1];
				}
					k += 4;
					break;
				case 2: dnib  = (0xc0000000 & lptr[i])>>30;
					switch(dnib) {
						case 0: printf("wrong, ck=%d dnib=%d.\n",ck,dnib);
							break;
						case 1: dif[0] = lptr[i] & 0x3fffffff;
							if(dif[0] & 0x20000000)
								data_out[k] = (dif[0]|0xc0000000)+data_out[k-1];
							else
								data_out[k] = dif[0]+data_out[k-1];
							k ++;
							break;
						case 2: for( j=0;j<2;j++) {
                            dif[j]            = (lptr[i] & (0x3fff8000>>j*15)) >> (1-j)*15;
                            if(dif[j] & 0x4000)
								data_out[k+j]   = (dif[j]|0xffff8000)+data_out[k+j-1];
                            else
								data_out[k+j]   = dif[j]+data_out[k+j-1];
						}
							k += 2;
							break;
						case 3: for(j=0;j<3;j++) {
							dif[j]            = (lptr[i] & (0x3ff00000 >> j*10)) >> (2-j)*10;
							if(dif[j] & 0x200) 
								data_out[k+j]   = (dif[j]|0xfffffc00)+data_out[k+j-1];
							else
								data_out[k+j]   = dif[j]+data_out[k+j-1];
						}
							k += 3;
							break;
					}
					break;
				case 3: dnib  = (lptr[i] & 0xc0000000)>>30;
					switch(dnib) {
						case 0: for( j=0;j<5;j++) {
                            dif[j]  = (lptr[i] & (0x3f000000 >> j*6))>>(4-j)*6;
                            if(dif[j] & 0x20) dif[j] |= 0xffffffc0;
                            data_out[k+j] = dif[j]+data_out[k+j-1];
						}
							k += 5;
							break;
						case 1: for( j=0;j<6;j++) {
                            dif[j]  = (lptr[i] & (0x3e000000 >> j*5))>>(5-j)*5;
                            if(dif[j] & 0x10) dif[j] |= 0xffffffe0;
                            data_out[k+j]             = dif[j]+data_out[k+j-1];
						}
							k += 6;
							break;
						case 2: for( j=0;j<7;j++) {
                            dif[j]  = (lptr[i] & (0x0f000000 >> j*4))>>(6-j)*4;
                            if(dif[j] & 0x8) dif[j] |= 0xfffffff0;
                            data_out[k+j] = dif[j]+data_out[k+j-1];
						}
							k += 7;
							break;
						case 3: printf("wrong, ck=%d dnib=%d.\n",ck,dnib);
							break;
					}
			}
			if(k>=samp) return 0;
		}
		m         = 0;
		data_in  += 64;
		lptr      = (long *)(data_in+4);
	}
	return 1;
}
