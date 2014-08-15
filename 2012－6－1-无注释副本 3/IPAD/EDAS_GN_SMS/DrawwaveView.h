//
//  DrawwaveView.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-2-23.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "CDSPCTL.h"
#define FONTSIZE 12.0f
@interface DrawwaveView : UIView <UIActionSheetDelegate>{
	CDSPCTL * m_dspctl;//display parameters
	int m_real_time_status;//true-show realtime wave
	CGRect m_rect;//screen size
	CGRect m_rectangle ;//drawwave rectangel size
	CGContextRef m_context;
	int m_xChar,m_yChar;//font size
	int m_origin_y;//toolbar heigh
	//true- redraw
	BOOL m_redraw_status;
	//TRUE-show channel info tip
	BOOL m_blabel;	
	BOOL  m_bclear;//TRUE-clear the screen ,FALSE- not clear
	CGColorRef m_grid_color,m_bk_color,m_line_color;
	
	UIButton * m_stn_btn[13];
	int m_orient;//0-landscape,1-portraint
	
	UILabel * u_time_info;
	UILabel * u_samp_info;
	UILabel * u_save_info;
	UIToolbar * u_statusbar;
	
	
	
}
@property(nonatomic,retain) UIToolbar * u_statusbar;
@property(nonatomic,retain) IBOutlet UILabel *u_samp_info;
@property(nonatomic,retain) IBOutlet UILabel * u_save_info;
@property(nonatomic, retain) IBOutlet UILabel *u_time_info;
@property(nonatomic, retain)  CDSPCTL *  m_dspctl;//display parameters
@property  int  m_real_time_status;//true-show realtime wave
@property CGRect m_rect;//screen size
@property CGRect m_rectangle ;//drawwave rectange-(void) GetDataRange: (long *) data;
//@property CGContextRef m_context;
@property BOOL m_bclear;
@property BOOL m_redraw_status;
@property int m_origin_y;
@property int m_orient;

-(void) GetDataRange: (long *) data;
-(void)OnRunView: (int)bview;
-(void) InitDsp;
-(void) init_draw;
-(void) InitSize : (CGRect )rect;
-(void) setContextRef: (CGContextRef) context;
-(void)CalChannelRange;
-(void) dsp_data: (long *)pin : (int)l;
-(void) clear_eara: (CGRect *)rect: (int)posi : (int)step;
-(void)  DrawWave;
-(void)ReDraw;
-(void) ResetPos;
-(void) OnData: (int)start : (int)chn_sum;
-(void) Showlabel : (int)x: (int)y : (NSString *) labels;
-(void) CreateStnButton  :(int)l : (int)y : (NSString *) title;
-(void) CreateStatusbar;
@end
