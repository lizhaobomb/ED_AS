//
//  DrawrtwaveViewController.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-2-27.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "DrawwaveView.h"

@interface DrawrtwaveViewController : UIViewController {
	//UILabel * u_time_info;//data time
	//	UILabel * u_samp_info;//sample rate and save data
	//UILabel * u_conn_info;//connection information
	//	int u_count;
	DrawwaveView * u_drawview;//draw lines and timespan
	
	
	//	CSitemagView siteview;//draw lines 
}
//@property (nonatomic, retain) IBOutlet UILabel * u_time_info;
//@property (nonatomic, retain) IBOutlet UILabel * u_samp_info;
//@property (nonatomic, retain) IBOutlet UILabel * u_conn_info;
@property (nonatomic,retain) IBOutlet DrawwaveView * u_drawview;
//@property int u_count;

-(IBAction) changeTimescan: (id)sender;
-(IBAction) OnViewUp: (id)sender;
-(IBAction) OnViewDown: (id)sender;
-(IBAction) OnViewCenter: (id)sender;
-(void) alertView: (UIAlertView *)alertView clickedButtonAtIndex: (int) index;
-(void) ontimer_display: (NSTimer *)timer; 
-(void) ontimer_savedata:(NSTimer *)timer;
-(void) ontimer_checkcon:(NSTimer *)timer;
-(void) DisplayData;
-(void) ShowCaliflag: (int) cali_flag : (int) local_id : (char *) calistr;
-(void) OnMessage;
-(void) OnCloseCon;

-(void) settimer;
-(void) ontimer_processcon: (NSTimer *)timer;

-(void) SetContext;

@end
