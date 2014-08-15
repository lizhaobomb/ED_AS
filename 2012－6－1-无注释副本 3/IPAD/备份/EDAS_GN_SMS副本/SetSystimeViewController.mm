//
//  SetSystimeViewController.m
//
//  Created by guan mofeng on 12-3-15.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetSystimeViewController.h"
#include "GlobalData.h"

@implementation SetSystimeViewController

- (void)viewDidLoad
{
	//siteDoc.m_sitelst.at(selIndex);
	self.title = @"设置系统时间";
}


- (void)viewDidAppear:(BOOL)animated
{
	try {
		//long timel = siteDoc.m_pchannels->curtm;
		if(siteDoc.m_pchannels != NULL) {
		//printf("timel=%ld \n", siteDoc.m_pchannels->curtm);
		//NSDate *p = [NSDate dateWithTimeIntervalSinceNow:timel];
		//NSDate *p = [NSDate dateWithTimeIntervalSinceNow:timel];
		dataPicker.date = [NSDate date];
		}
		//[ p release];
		
	}
	catch (exception e) {
		
	}
	
}


- (IBAction)btnOKClick:(id)sender {
    
	//CTime t(m_date.GetYear(),m_date.GetMonth(),m_date.GetDay(),m_time.GetHour(),m_time.GetMinute(),m_time.GetSecond());
	long timel = [dataPicker.date timeIntervalSince1970]; 
	
	printf("SetSystime = %d \n", timel);
	CM_SYSTIMEFRM frm;
	
	char m_cmd[1024]; int m_cmd_len;
	m_cmd[0]=0xbf;
	m_cmd[1]=0x13;
	m_cmd[2]=0x97;
	m_cmd[3]=0x74;
	
	memset(&frm,0,sizeof(frm));
	
	frm.head.cmd=0x6074;
	frm.head.sens_id=0;
	frm.head.length=8;
	
	//frm.sec=t.GetTime();
	 frm.sec=timel;
	short *p=(short *)&frm;
	frm.chk_sum=0;
	
	for(int i=0;i<(frm.head.length+6)/2-1;i++)
		frm.chk_sum-=p[i];
	
	//memcpy(&m_cmd[4],(char *)&m_cmd,frm.head.length+6);
	memcpy(&m_cmd[4],(char *)&frm,frm.head.length+6);
	m_cmd_len=frm.head.length+10;
	
	if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置系统时间成功" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortrait) || (toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown));
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

- (IBAction)btnCancelClick:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}
@end
