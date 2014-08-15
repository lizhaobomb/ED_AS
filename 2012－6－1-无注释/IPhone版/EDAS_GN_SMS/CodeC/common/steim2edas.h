#ifndef _STEIM_2
#define _STEIM_2

struct STEIM2_C {
    unsigned   c14: 2; 
    unsigned   c13: 2;
    unsigned   c12: 2;
    unsigned   c11: 2;
    unsigned   c10: 2;
    unsigned   c9:  2;
    unsigned   c8:  2;
    unsigned   c7:  2;
    unsigned   c6:  2;
    unsigned   c5:  2;
    unsigned   c4:  2;
    unsigned   c3:  2;
    unsigned   c2:  2;
    unsigned   c1:  2;
    unsigned   c0:  2;
    unsigned   cf:  2; // must be zero.
};

struct DATA_8 {
    char       v3;
    char       v2;
    char       v1;
    char       v0;
};

struct DATA_30 {         // ck = 10
    unsigned   v: 30; 
    unsigned   dnib: 2;  // 01
};

struct DATA_15 {         // ck = 10
    unsigned   v1: 15;
    unsigned   v0: 15;
    unsigned   dnib:  2;  // 10
};

struct DATA_10 {         // ck = 10
    unsigned   v2: 10;
    unsigned   v1: 10;
    unsigned   v0: 10;
    unsigned   dnib:  2;  // 11
};

struct DATA_6 {          // ck = 11;
    unsigned   v4: 6;
    unsigned   v3: 6;
    unsigned   v2: 6;
    unsigned   v1: 6;
    unsigned   v0: 6;
    unsigned   dnib:  2; // 00
};

struct DATA_5 {          // ck = 11;
    unsigned   v5: 5;
    unsigned   v4: 5;
    unsigned   v3: 5;
    unsigned   v2: 5;
    unsigned   v1: 5;
    unsigned   v0: 5;
    unsigned   dnib:  2; // 01
};

struct DATA_4 {          // ck = 11;
    unsigned   v6: 4;
    unsigned   v5: 4;
    unsigned   v4: 4;
    unsigned   v3: 4;
    unsigned   v2: 4;
    unsigned   v1: 4;
    unsigned   v0: 4;
    unsigned   res:2;
    unsigned   dnib:  2; // 10
};

struct SubFrame {
    STEIM2_C   id;
    union {
        long    d32b;
        DATA_8  d08b;
        DATA_30 d30b;
        DATA_15 d15b;
        DATA_10 d10b;
        DATA_6  d06b;
        DATA_5  d05b;
        DATA_4  d04b;
    } data[15];
};

struct STEIM2FRA {
	short     sen_id;
	short     flag;
	short     fra_len;
	short	 unused;
	long      epsec;
	char      d_att;
	char      p_chk;
	char      data[8000];
};

struct EDAFRAME {
	short    sen_id;
	short    flag;         // 55AAH
	short    fra_len;
	short    ep_sec[2];    // data time
	char     d_att;        // 1: pulse  2: sine
	char     p_chk;        // part check sum
	char     data[4600];   // sample data
};

class CSteim2  {
public:      
	CSteim2();
	~CSteim2();
	//samp : ≤…—˘¬ ,comp£∫∑÷œÚ ˝
    STEIM2FRA *PackSteim2(int sen_id,long ep_sec,int d_att,long *data,int samp,int comp);
	//samp : ≤…—˘¬ ,comp£∫∑÷œÚ ˝
    int        UnpackSteim2(STEIM2FRA *p_st2,int *sen_id,long *ep_sec,int *d_att,long *data,int *samp,int comp);
	//comp£∫∑÷œÚ ˝
    STEIM2FRA *ZipSteim2(EDAFRAME *d_fra,int comp);
	//comp£∫∑÷œÚ ˝
    EDAFRAME  *UnzipSteim2(STEIM2FRA *f_st2,int comp);
	//samp : data_in(input compresed data),data_out(output data)◊‹ ˝
	int         UnzipASeries(char *data_in,long *data_out,int samp);
	//samp : data_in,data_out◊‹ ˝
    int         UnzipBSeries(char *data_in,long *data_out,int samp);
	
private:
    EDAFRAME    dafra;
    STEIM2FRA   st2fr;
    SubFrame    subFra;
	//samp : data_in,data_out◊‹ ˝
    int         ZipASeries(long *data_in,char *data_out,int samp);
	inline void SetIdBit(int k,int flag);
    int         GetIdBit(STEIM2_C id,int i);
};

#endif
