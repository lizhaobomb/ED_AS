//
//  SetIPViewController.m
//
//  Created by guan mofeng on 12-3-13.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetIPViewController.h"

@implementation SetIPViewController

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	
	//dlg.m_gw=m_das.das.ip.gw;
	//dlg.m_ip=m_das.das.ip.ip;
	//dlg.m_mask=m_das.das.ip.mask;
	
	//char temp[1000];
	//sprintf(temp,"%d", siteDoc.m_das.das.ip.gw);
	txtIPCtl.text = [[NSString alloc] initWithUTF8String:siteDoc.m_das.das.ip.ip];
	txtGwCtl.text = [[NSString alloc] initWithUTF8String:siteDoc.m_das.das.ip.gw]; 
	txtMaskCtl.text = [[NSString alloc] initWithUTF8String:siteDoc.m_das.das.ip.mask]; 
	//short m_gain = siteDoc.m_das.sens[0].gain_id;
}

- (void)viewDidLoad
{
	txtIPCtl.delegate = self;
	txtGwCtl.delegate =self;
	txtMaskCtl.delegate=self;
	
	self.title = @"设置网络地址";
	
}

-(BOOL) isValidator {
	//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"Erro" message:@"Password is null" 
	//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
	//[av show];	
	return TRUE;
}
- (IBAction)btnCancelClick:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnOkClick:(id)sender {
    //未做效验
	if(![self isValidator]){return;}		
		char m_cmd[1024];
		int m_cmd_len = 0;
		CM_IPGWFRM frm;
		
		memset(&frm,0,sizeof(CM_IPGWFRM));
		m_cmd[0]=0xbf;
		m_cmd[1]=0x13;
		m_cmd[2]=0x97;
		m_cmd[3]=0x74;
		
		sprintf(frm.ip,"%s",[txtIPCtl.text UTF8String]);
		sprintf(frm.gate,"%s",[txtGwCtl.text UTF8String]);
		sprintf(frm.netmask,"%s",[txtMaskCtl.text UTF8String]);
    
		frm.head.cmd=0x6073;
		frm.head.sens_id=0;
		frm.head.length=136;
		frm.head.reserved=0;
		frm.chk_sum=0;
		
		short * p1=(short *)&frm;
		for(int i=0;i<(frm.head.length+6)/2-1;i++)
			frm.chk_sum-=p1[i];
		
		memcpy(&m_cmd[4],(char *)&frm,frm.head.length+6);
		m_cmd_len=frm.head.length+10;
	
	
	
	if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网路连接失败" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"信息" message:@"网络地址设置成功" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}
	
	//siteDoc.OnCfgIp(m_cmd, m_cmd_len);
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	[textField resignFirstResponder];
	return YES;
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

@end
