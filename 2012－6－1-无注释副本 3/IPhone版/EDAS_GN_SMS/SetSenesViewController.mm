//
//  SetSenesViewController.m
//
//  Created by guan mofeng on 12-3-14.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetSenesViewController.h"
#include "GlobalData.h"
extern char sens_id[];

//112 44
@implementation SetSenesViewController

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
	//[self initUI];
}
- (void)viewDidLoad
{
	comboSens= [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 190 , 40 , 135 , 30 )];
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
	
}


- (IBAction)onCmgSenesonClick:(id)sender {
   
	
	if(comboSens.items == nil) {return;}
	short m_sensid = [comboSens getSelectIndex];
	
	char buf[20];
	short *p=(short *)buf;
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	//p[2]=dlg.m_sensid;
	p[2]=m_sensid;
	p[3]=0x6052;
	p[4]=6;
	p[5]=0;
	p[6]=0;
	p[7]=0;
	int i = 2;
	for( i=2;i<7;i++)
		p[7]-=p[i];
	
	if(!siteDoc.m_thd->Send(buf,16))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置启动开摆成功" 
		//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];	
		[self.navigationController popViewControllerAnimated:YES];
	}
	
	//if(!m_thd->Send(buf,16))
	//	SendErr();
}

- (IBAction)onCmgSensoffClick:(id)sender {
    //CString s;
	//s.LoadString(IDS_TIP11);
	
	//CSelsensDlg dlg;
	
	//dlg.m_das=&m_das;
	//dlg.m_tip=s;
	//if(dlg.DoModal()==IDCANCEL)
	//	return;
	
	if(comboSens.items == nil) {return;}
	short m_sensid = [comboSens getSelectIndex];
	
	char buf[20];
	short *p=(short *)buf;
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	//p[2]=dlg.m_sensid;
	p[2]=m_sensid;
	p[3]=0x6052;
	p[4]=6;
	p[5]=0;
	p[6]=1;
	p[7]=0;
	int i =0;
	for(i=2;i<7;i++)
		p[7]-=p[i];
	//if(!m_thd->Send(buf,16))
	//	SendErr();
	
	if(!siteDoc.m_thd->Send(buf,16))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置锁摆成功" 
		//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];	
		[self.navigationController popViewControllerAnimated:YES];
	}
}

- (IBAction)onSensCtloffClcik:(id)sender {
   	
	//CString s;
	//s.LoadString(IDS_TIP15);
	//SensSet(0x6051,s);	
	
	unsigned short cmd=  0x6051;
	//CSelsensDlg dlg;
	
	//dlg.m_das=&m_das;
	//dlg.m_tip=tip;
	//if(dlg.DoModal()==IDCANCEL)
	//	return;
	
	if(comboSens.items == nil) {return;}
	short m_sensid = [comboSens getSelectIndex];
	
	char buf[20];
	short *p=(short *)buf;
	unsigned short* ps=(unsigned short *)buf;
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	//p[2]=dlg.m_sensid;
	p[2]=m_sensid;
	ps[3]=cmd;
	p[4]=4;
	p[5]=0;
	p[6]=0;
	for(int i=2;i<6;i++)
		p[6]-=p[i];
	//if(!m_thd->Send(buf,14))
	//	SendErr();
	if(!siteDoc.m_thd->Send(buf,16))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"停止调零成功" 
		//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];	
		[self.navigationController popViewControllerAnimated:YES];
	}
	
	
}

- (IBAction)onSensCtlonClick:(id)sender {
	//CString s;
	//s.LoadString(IDS_TIP8);
	//SensSet(0x6050,s);
	if(comboSens.items == nil) {return;}
	short m_sensid = [comboSens getSelectIndex];
	
	unsigned short cmd=  0x6050;
	//CSelsensDlg dlg;
	
	//dlg.m_das=&m_das;
	//dlg.m_tip=tip;
	//if(dlg.DoModal()==IDCANCEL)
	//	return;
	
	char buf[20];
	short *p=(short *)buf;
	unsigned short* ps=(unsigned short *)buf;
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	//p[2]=dlg.m_sensid;
	p[2]=m_sensid;
	ps[3]=cmd;
	p[4]=4;
	p[5]=0;
	p[6]=0;
	for(int i=2;i<6;i++)
		p[6]-=p[i];
	//if(!m_thd->Send(buf,14))
	//	SendErr();
	if(!siteDoc.m_thd->Send(buf,16))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"启动调零成功" 
		//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];	
		
		[self.navigationController popViewControllerAnimated:YES];
	}
	
	
	
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortrait) || (toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown));
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}
@end
