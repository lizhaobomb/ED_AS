//
//  SetStopCalViewControllor.m
//
//  Created by guan mofeng on 12-3-29.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetStopCalViewControllor.h"
#include "GlobalData.h"

@implementation SetStopCalViewControllor


- (IBAction)btnOKClick:(id)sender {
    char buf[128];
	short * p=(short *)buf;
	unsigned short * ps=(unsigned short *)buf;
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	p[2]=0;
	ps[3]=0x6028;
	p[4]=4;
	p[5]=0;
	p[6]=0;
	for(int i=2;i<6;i++)
		p[6]-=p[i];
	//if(!m_thd->Send(buf,14))
	//	SendErr();
	
	if(!siteDoc.m_thd->Send(buf,14))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网路连接失败" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"信息" message:@"系统停止标定成功" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}
	
	
}



- (IBAction)btnCancelClick:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortrait) || (toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown));
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

@end
