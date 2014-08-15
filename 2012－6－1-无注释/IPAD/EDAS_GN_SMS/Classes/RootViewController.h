//
//  RootViewController.h
//
//  Created by guan mofeng on 11-12-9.
//  Copyright 2011 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "SetDsp1ViewController.h"
#import "ModalViewController.h"
#import "QueryMenuViewController.h"
#import "RunMenuViewController.h"
#import "SetMenuViewController.h"
//#import "DrawrtwaveViewController.h"
#import "QueryinfoViewController.h"
#import "DrawwaveView.h"



static DrawwaveView *uU_drawview;


@interface RootViewController : UIViewController<UIAlertViewDelegate> {
	
	RunMenuViewController *runMenuViewController;
	SetMenuViewController *setMenuViewController;
	QueryMenuViewController *queryMenuViewController;
	ModalViewController *modalViewController;
	//	DrawrtwaveViewController *drawwaveViewController;
	QueryinfoViewController * queryinfoViewController;
	DrawwaveView * u_drawview;//draw lines and timespan
	UILabel * u_cal_info;
	
	SetDsp1ViewController *setDsp1ViewContoller;
	
}


@property(nonatomic,retain) IBOutlet UILabel * u_cal_info;
@property (nonatomic, retain) SetMenuViewController *setMenuViewController;
@property (nonatomic, retain) QueryMenuViewController *queryMenuViewController;
@property (nonatomic, retain) RunMenuViewController *runMenuViewController;
@property (nonatomic, retain) ModalViewController *modalViewController;
@property (nonatomic, retain) SetDsp1ViewController *setDsp1ViewContoller;
@property (nonatomic,retain) QueryinfoViewController *queryinfoViewController;
@property (nonatomic,retain) DrawwaveView * u_drawview;

//+(DrawwaveView) gu_drawview;//draw lines and timespan

-(IBAction) OnScantime: (id)sender;
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

-(void) showDsp1ViewController :(NSString *) labetext;

+ (RootViewController*) U_RootViewController;

@end

