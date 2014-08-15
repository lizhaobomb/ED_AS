//
//  SetStartCalViewController.m
//
//  Created by guan mofeng on 12-3-13.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetStartCalViewController.h"
#include "GlobalData.h"
extern char sens_id[];

@implementation SetStartCalViewController


- (void) initUI {
	
	int i = 0;
	char temp[100];
	NSMutableArray *comboSensData = [NSMutableArray array];
	for(i=0;i<siteDoc.m_das.stnpar.sens_num;i++)
	{
		sprintf(temp,"地震计 %c",  sens_id[i]);
		[comboSensData addObject:[[NSString alloc] initWithUTF8String:temp]];
	}
	
	if(siteDoc.m_das.stnpar.sens_num != 0) {
		[comboSens setItems:comboSensData];
		//[comboSensData release];
	}
	
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	[self initUI];
}
- (void)viewDidLoad
{
	int xW = 300;
	int yH = 200;
	
	comboSens= [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 140+xW , 8+yH, 200 , 30 )];
	comboSens.dropMaxHeigth  = 32*4;
	NSMutableArray *comboSensData = [NSMutableArray array];
	int i = 0;
	char temp[100];
	for(i=0;i<siteDoc.m_das.stnpar.sens_num;i++)
	{
		sprintf(temp,"地震计 %c",  sens_id[i]);
		[comboSensData addObject:[[NSString alloc] initWithUTF8String:temp]];
	}
	
	if(siteDoc.m_das.stnpar.sens_num != 0) {
		[comboSens setItems:comboSensData];
		//[comboSensData release];
	}
	[ self.view addSubview :comboSens];
	[comboSens release];
	self.title =@"启动标定";
	
}	

- (IBAction)btnStartPseudoClick:(id)sender {
    
	char buf[100];
	short *p=(short *)&buf[4];
	
	if(comboSens.items == nil) {return;}
	short m_sensid = [comboSens getSelectIndex];
	
	memset(buf,0,sizeof(buf));
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	CM_STARTCALFRM * frm;
	frm=(CM_STARTCALFRM *)&buf[4];
	
	frm->head.sens_id=m_sensid;
	frm->head.cmd=0x6026;
	frm->time=-1;
	frm->head.length=8;
	frm->chk_sum=0;
	for(int i=0;i<(frm->head.length+4)/2;i++)
		frm->chk_sum-=p[i];
	//siteDoc.m_thd->Send(<#char *cmd#>, <#int cmd_len#>)
	if(!siteDoc.m_thd->Send(buf,frm->head.length+10))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置启动随机码标定成功" 
		//									delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];
		[self.navigationController popViewControllerAnimated:YES];
	}

	
}

- (IBAction)btnStartPulseClick:(id)sender {
    char buf[100];
	short *p=(short *)&buf[4];
	
	if(comboSens.items == nil) {return;}
	short m_sensid = [comboSens getSelectIndex];
	
	memset(buf,0,sizeof(buf));
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	CM_STARTCALFRM * frm;
	frm=(CM_STARTCALFRM *)&buf[4];
	
	frm->head.sens_id=m_sensid;
	frm->head.cmd=0x6024;
	frm->time=-1;
	frm->head.length=8;
	frm->chk_sum=0;
	for(int i=0;i<(frm->head.length+4)/2;i++)
		frm->chk_sum-=p[i];
	//siteDoc.m_thd->Send(<#char *cmd#>, <#int cmd_len#>)

	if(!siteDoc.m_thd->Send(buf,frm->head.length+10))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置启动脉冲标定成功" 
		//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];
		[self.navigationController popViewControllerAnimated:YES];
	}
	
	
	
	
}

- (IBAction)btnStartSineClick:(id)sender {
    char buf[100];
	short *p=(short *)&buf[4];
	
	if(comboSens.items == nil) {return;}
	short m_sensid = [comboSens getSelectIndex];
	
	memset(buf,0,sizeof(buf));
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	CM_STARTCALFRM * frm;
	frm=(CM_STARTCALFRM *)&buf[4];
	
	frm->head.sens_id=m_sensid;
	frm->head.cmd=0x6025;
	frm->time=-1;
	frm->head.length=8;
	frm->chk_sum=0;
	for(int i=0;i<(frm->head.length+4)/2;i++)
		frm->chk_sum-=p[i];
	
	
	if(!siteDoc.m_thd->Send(buf,frm->head.length+10))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置启动正弦标定成功" 
		//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];
		[self.navigationController popViewControllerAnimated:YES];
	}
	
	
}

- (IBAction)btnStartStongClick:(id)sender {
    char buf[100];
	short *p=(short *)&buf[4];
	
	if(comboSens.items == nil) {return;}
	short m_sensid = [comboSens getSelectIndex];

	
	memset(buf,0,sizeof(buf));
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	CM_STARTCALFRM * frm;
	frm=(CM_STARTCALFRM *)&buf[4];
	
	frm->head.sens_id=m_sensid;
	frm->head.cmd=0x6027;
	frm->time=-1;
	frm->head.length=8;
	frm->chk_sum=0;
	for(int i=0;i<(frm->head.length+4)/2;i++)
		frm->chk_sum-=p[i];
	
	
	if(!siteDoc.m_thd->Send(buf,frm->head.length+10))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置启动强震标定成功" 
		//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];
		[self.navigationController popViewControllerAnimated:YES];
	}
	
	
	
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

- (IBAction)btnCancelClick:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

@end
