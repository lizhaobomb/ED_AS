//
//  SetResetSysViewController.m
//
//  Created by guan mofeng on 12-3-13.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetResetSysViewController.h"

@implementation SetResetSysViewController

- (IBAction)btnCancelClick:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnOkClick:(id)sender {
    char buf[20];
	short *p=(short *)buf;
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	p[2]=0;
	p[3]=0x6077;
	p[4]=4;
	p[5]=0;
	p[6]=0;
	for(int i=2;i<6;i++)
		p[6]-=p[i];
	
	if(!siteDoc.m_thd->Send(buf,14))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"恢复出厂设置成功" 
		//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];	
	}

}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}
@end
