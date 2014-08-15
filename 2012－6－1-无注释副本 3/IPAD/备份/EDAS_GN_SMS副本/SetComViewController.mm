//
//  SetComViewController.m
//
//  Created by guan mofeng on 12-3-14.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetComViewController.h"
#include "GlobalData.h"

@implementation SetComViewController

- (void)viewDidLoad
{
	//siteDoc.m_sitelst.at(selIndex);
	int xW = 300;
	int yH = 190;
	
	radioGroup =[[ RadioGroup alloc ] init ];
	
	radio9600 =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 30+xW , 60+yH , 150 , 30 )];
	radio9600.label.text = @"9600" ;
	radio9600.value =[[ NSNumber alloc ] initWithInt:0 ];	
	radio9600.style = CheckButtonStyleRadio ;
	[radioGroup add :radio9600];
	[ self.view addSubview :radio9600];
	[radio9600 release];
	
	radio19200 =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 180+xW , 60+yH , 150 , 30 )];
	radio19200.label.text = @"19200" ;
	radio19200.value =[[ NSNumber alloc ] initWithInt:1 ];	
	radio19200.style = CheckButtonStyleRadio ;
	[radioGroup add :radio19200];
	[ self.view addSubview :radio19200];
	[radio19200 release];
	
	radio38400 =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 30+xW , 90+yH , 150 , 32 )];
	radio38400.label.text = @"38400" ;
	radio38400.value =[[ NSNumber alloc ] initWithInt:2 ];	
	radio38400.style = CheckButtonStyleRadio ;
	[radioGroup add :radio38400];
	[ self.view addSubview :radio38400];
	[radio38400 release];
	
	radio57600 =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 180+xW , 90+yH , 150 , 32 )];
	radio57600.label.text = @"57600" ;
	radio57600.value =[[ NSNumber alloc ] initWithInt:3 ];
	radio57600.style = CheckButtonStyleRadio ;
	[radioGroup add :radio57600];
	[ self.view addSubview :radio57600];
	[radio57600 release];
	
	radio115200 =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 30+xW , 120+yH , 150 , 32 )];
	radio115200.label.text = @"115200" ;
	radio115200.value =[[ NSNumber alloc ] initWithInt:4 ];
	radio115200.style = CheckButtonStyleRadio ;
	[radioGroup add :radio115200];
	[ self.view addSubview :radio115200];
	[radio115200 release];
	
	
	//UIImage *img = [UIImage imageNamed:@"kk.png"];
	UITabBarItem *barItem = [[UITabBarItem alloc] initWithTitle:@"波特率" image:nil tag:0];
	self.tabBarItem = barItem;
	[barItem release];
}


- (void)viewDidAppear:(BOOL)animated
{
	//CIpucomDlg dlg;
	//int baud[]={9600,19200,38400,57600,115200};
	int i = 0;
	int m_baud = 0;
	m_samp = 0;
	
	if(siteDoc.m_das.das.comctl.baud == 9600) [radioGroup checkButtonClicked:radio9600];
	else if(siteDoc.m_das.das.comctl.baud == 19200) [radioGroup checkButtonClicked:radio19200];
	else if(siteDoc.m_das.das.comctl.baud == 38400) [radioGroup checkButtonClicked:radio38400];
	else if(siteDoc.m_das.das.comctl.baud == 57600) [radioGroup checkButtonClicked:radio57600];
	else if(siteDoc.m_das.das.comctl.baud == 115200) [radioGroup checkButtonClicked:radio115200];
	else [radioGroup checkButtonClicked:radio19200];
	
	NSLog(@"siteDoc.m_das.das.comctl.baud = %d", siteDoc.m_das.das.comctl.baud);
	/*
	for(i=0;i<5;i++)
	{
		if(baud[i]==siteDoc.m_das.das.comctl.baud)
		{
			m_baud=i;
			break;
		}
	}
	*/
	m_samp=0;
	for(i=0;i<siteDoc.m_das.stnpar.sens_num;i++)
	{
		m_samp+=siteDoc.m_das.sens[i].samp;
	}
	
	
}


- (IBAction)btnCancelClick:(id)sender {
     [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnOKClick:(id)sender {
    int baud[]={9600,19200,38400,57600,115200};
	int m_baud = (int)[radioGroup.value intValue];
	
	if(baud[m_baud]/2<m_samp*3*9)
	{
		string s = "警告：通信速率可能低于传输需要的速率!\n按“是”继续设置，按“否”取消设置";
	}
	
	char m_cmd[1024];
	int m_cmd_len;
	
	m_cmd[0]=0xbf;
	m_cmd[1]=0x13;
	m_cmd[2]=0x97;
	m_cmd[3]=0x74;
	CM_COMBAUDFRM frm;
	
	memset(&frm,0,sizeof(frm));
	frm.head.cmd=0x6033;
	frm.head.sens_id=0;
	frm.head.length=8;
	//	frm.bstart=m_bstart;
	frm.baud=baud[m_baud];
	
	short *p=(short *)&frm;
	frm.chk_sum=0;
	
	for(int i=0;i<(frm.head.length+6)/2-1;i++)
		frm.chk_sum-=p[i];
	
	memcpy(&m_cmd[4],(char *)&frm,frm.head.length+10);
	m_cmd_len=frm.head.length+10;
	
	if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网路连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置串口波特率成功" 
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
