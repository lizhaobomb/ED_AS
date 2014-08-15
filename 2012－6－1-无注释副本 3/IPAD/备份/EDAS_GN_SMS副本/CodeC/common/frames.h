/*edas数据包头段通用定义
data frame structure */
#ifndef  _edas_frame_h
#define  _edas_frame_h

typedef struct  {
  short      SiteId;
  short      FrType;
  short      nFrLen;
  char       ZipFlag;
  char       ChkSum;//check sum of FrameID
} FrameID;

struct WaveDataFrame {
  FrameID      sFrId;
  long		   time;
  short        nChId;
  short		   samp;
  char        *szData;
  short        ChkSum; //check sum of nChId and data
};
#define FRAMEHEADSIZE sizeof(FrameID)
#endif
