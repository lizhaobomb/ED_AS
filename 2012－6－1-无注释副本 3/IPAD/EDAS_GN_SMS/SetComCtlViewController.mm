//
//  SetComCtlViewController.m
//
//  Created by guan mofeng on 12-3-14.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetComCtlViewController.h"
#include "GlobalData.h"

@implementation SetComCtlViewController

- (void)viewDidLoad
{
	
	//UIImage *img = [UIImage imageNamed:@"kk.png"];
	/*
	UITabBarItem *barItem = [[UITabBarItem alloc] initWithTitle:@"发送方式" image:nil tag:0];
	self.tabBarItem = barItem;
	[barItem release];
	 */
	
	int xW = 300;
	int yH = 200;
	
	checkBack =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 30+xW , 60+yH , 170 , 30 )];
	checkBack.label.text = @"发送应答" ;
	checkBack.value =[[ NSNumber alloc ] initWithInt:0 ];
	checkBack.style = CheckButtonStyleBox ;
	[ self.view addSubview :checkBack];
	[checkBack release];
	
	checkBinit =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 200+xW , 60+yH , 250 , 30 )];
	checkBinit.label.text = @"系统启动后直接发生" ;
	checkBinit.value =[[ NSNumber alloc ] initWithInt:0 ];
	checkBinit.style = CheckButtonStyleBox ;
	[ self.view addSubview :checkBinit];
	[checkBinit release];
	
	checkBebroad =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 30+xW , 90+yH , 170 , 30 )];
	checkBebroad.label.text = @"发送广播帧" ;
	checkBebroad.value =[[ NSNumber alloc ] initWithInt:0 ];
	checkBebroad.style = CheckButtonStyleBox ;
	[ self.view addSubview :checkBebroad];
	[checkBebroad release];
	
	checkBestat =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 200+xW , 90+yH , 250 , 30 )];
	checkBestat.label.text = @"发生运行状态帧" ;
	checkBestat.value =[[ NSNumber alloc ] initWithInt:0 ];
	checkBestat.style = CheckButtonStyleBox ;
	[ self.view addSubview :checkBestat];
	[checkBestat release];
	
	//UIImage *img = [UIImage imageNamed:@"kk.png"];
	//UITabBarItem *barItem = [[UITabBarItem alloc] initWithTitle:@"发送方式" image:nil tag:0];
	//self.tabBarItem = barItem;
	//[barItem release];
	
}


- (void)viewDidAppear:(BOOL)animated
{
	/*
	m_back=m_das->das.comctl.back;
	m_binit=m_das->das.comctl.binit;
	m_bebroad=m_das->das.comctl.be_broad;
	m_bestat=m_das->das.comctl.be_stat;
	 */
	[checkBack setChecked:siteDoc.m_das.das.comctl.back];
	[checkBinit setChecked:siteDoc.m_das.das.comctl.binit];
	[checkBebroad setChecked:siteDoc.m_das.das.comctl.be_broad];
	[checkBestat setChecked:siteDoc.m_das.das.comctl.be_stat];
}

- (IBAction)btnCancelClick:(id)sender {
     [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnOKClick:(id)sender {
	char m_cmd[1024];
	char m_cmd_len;
    short * p=(short *)m_cmd;
	m_cmd[0]=0xbf;
	m_cmd[1]=0x13;
	m_cmd[2]=0x97;
	m_cmd[3]=0x74;
	
	p[2]=0;
	p[3]=0x6034;
	p[4]=12;
	p[5]=0;
	p[6]=[checkBack isChecked];
	p[7]=[checkBinit isChecked];
	p[8]=[checkBebroad isChecked];
	p[9]=[checkBestat isChecked];
	p[10]=0;
	for(int i=2;i<10;i++)
		p[10]-=p[i];
	m_cmd_len=p[4]+10;
	
	if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置串口发送方式成功" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}
	
	
	
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortrait) || (toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown));
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}
@end
