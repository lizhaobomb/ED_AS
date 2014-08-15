//
//  DSPCTL.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-2-24.
//  Copyright 2012 北京. All rights reserved.
//


@interface CDSPCTL : NSObject {
@public 
	int scantime;
	int voffset;//verscroll offset
	int hoffset;//hscroll offset
	int channelnum;//channelnum
	int channeltype;//channel time
	int  maxrange_Hscroll,maxrange_Vscroll;//scroll range
	int iHscrollPos,iVscrollPos;//scorll position
	int totalchannel,channel_range;//number of channels , 
}
@property int scantime;
@property int voffset;//verscroll offset
@property int hoffset;//hscroll offset
@property int channelnum;//channelnum
@property  int channeltype;//channel time
@property  int  maxrange_Hscroll;
@property int maxrange_Vscroll;//scroll range
@property int iHscrollPos;
@property int iVscrollPos;//scorll position
@property  int totalchannel;
@property int channel_range;//number of channels , 

@end
