//
//  QueryinfoViewController.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-2-23.
//  Copyright 2012 北京. All rights reserved.
//

#import "QueryinfoViewController.h"
#include "GlobalData.h"

@implementation QueryinfoViewController

@synthesize u_message_view;
@synthesize m_timer;

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/
 
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	
	//m_timer=[NSTimer scheduledTimerWithTimeInterval:1.0f target:self selector:@selector(ontimer_msg:) userInfo:nil repeats:YES];	
	//[m_timer invalidate];
}

- (void) showMsg {
	
  	if(siteDoc.m_bshow_allmsg == TRUE) {
		//u_message_view.text=[[NSString alloc] initWithUTF8String: temp];;
		u_message_view.text= [[NSString alloc] initWithUTF8String: siteDoc.m_old_msg.c_str()];
	}
	else {
		u_message_view.text= [[NSString alloc] initWithUTF8String: siteDoc.m_msg.c_str()];
	}
}
 
- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	m_timer=[NSTimer scheduledTimerWithTimeInterval:1.0f target:self selector:@selector(ontimer_msg:) userInfo:nil repeats:YES];
	//u_message_view.text = [[NSString alloc] initWithUTF8String: siteDoc.m_old_msg.c_str()];
	[self showMsg];
	//[m_timer fire];
}
  
- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
	[m_timer invalidate];
	//[m_timer release];
	u_message_view.text = @"";
	//[m_timer invalidate];
}

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation) toInterfaceOrientation {
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}
 

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
	u_message_view=nil;
	[m_timer invalidate];
}
-(IBAction) SaveMessage: (id)sender
{
	siteDoc.SaveMessage();
}
-(IBAction) ClearMessage: (id)sender{
	siteDoc.m_old_msg="";
	siteDoc.m_msg="";
	u_message_view.text = [[NSString alloc] initWithString: @""];
}

-(IBAction) Refresh: (id)sender{
	if(siteDoc.m_bshow_allmsg)
		u_message_view.text=[[NSString alloc] initWithUTF8String: siteDoc.m_old_msg.c_str()];	
	else u_message_view.text=[[NSString alloc] initWithUTF8String: siteDoc.m_msg.c_str()];
}

-(IBAction) Close: (id)sender{
	[self.parentViewController dismissModalViewControllerAnimated:YES];
}

-(void) ontimer_msg : (NSTimer *)timer{
	//printf("siteDoc.m_bshow_allmsg = %d ,time = %s\n", siteDoc.m_bshow_allmsg, siteDoc.m_msg.c_str());
	//char temp[100000];
	//sprintf(temp, "%s", siteDoc.m_msg.c_str());
	//printf("siteDoc.m_bshow_allmsg = %d ,time = %s\n", siteDoc.m_bshow_allmsg, temp);
	[self showMsg];
}

	
- (void)dealloc {
    [super dealloc];
}


@end
