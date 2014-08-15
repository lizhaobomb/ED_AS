//
//  SetStrongViewController.m
//
//  Created by guan mofeng on 12-3-30.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetStrongViewController.h"
#include "GlobalData.h"
#define NUMBERSINT @"0123456789\n"
#define NUMBERSFLOAT @"-0123456789.\n"

extern char sens_id[];

@implementation SetStrongViewController 


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
		[cmboSens setItems:comboSensData];
		[self comboBox:cmboSens selectItemAtIndex:0];
		//[comboSensData release];
	}
	
	
	
	
}
- (void) initUI1: (NSInteger)m_sensid
{
	//int m_sensid = [cmboSens getSelectIndex];
	
	
	char temp[1024];
	[ckBTimer setChecked:siteDoc.m_das.sens[m_sensid].strong.btimer];
	
	//sprintf(temp,"%d", siteDoc.m_das.sens[m_sensid].pulse.amp);
	//txtAmp.text = [[NSString alloc]initWithUTF8String : temp];
	//[cmboAmpType setSelectIndex:siteDoc.m_das.sens[m_sensid].pulse.amptype]; 
	
	//m_type=m_das->sens[m_sensid].pulse.amptype;
	//m_dur=m_das->sens[m_sensid].pulse.dur/10.f;
	
	//sprintf(temp,"%f", siteDoc.m_das.sens[m_sensid].pulse.dur/10.f);
	//txtDur.text = [[NSString alloc]initWithUTF8String : temp];
	
	//m_tm_method=m_das->sens[m_sensid].pulse.method;
	[cmbMethod setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.method];
	
	if(siteDoc.m_das.sens[m_sensid].strong.method==0)
	{
		[cmboHour setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start1];
		[cmboMin setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start2];
		labDay.text = @"日：";
		//m_tm_hr=m_das->sens[m_sensid].pulse.tm_start1;
		//m_tm_min=m_das->sens[m_sensid].pulse.tm_start2;
		//m_static_date.LoadString(IDS_TMLABEL1); "日：
		
	}else if(siteDoc.m_das.sens[m_sensid].strong.method==1)
	{
		
		[cmboDay setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start1 - 1];
		[cmboHour setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start2];
		[cmboMin setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start3];
		labDay.text = @"星期：";
		//m_tm_date=m_das->sens[m_sensid].pulse.tm_start1-1;
		//m_tm_hr=m_das->sens[m_sensid].pulse.tm_start2;
		//m_tm_min=m_das->sens[m_sensid].pulse.tm_start3;
		//m_static_date.LoadString(IDS_TMLABEL2); //"星期："
	}else if(siteDoc.m_das.sens[m_sensid].strong.method==2){
		
		[cmboDay setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start1 - 1];
		[cmboHour setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start2];
		[cmboMin setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start3];
		labDay.text = @"日：";
		//m_tm_date=m_das->sens[m_sensid].pulse.tm_start1-1;
		//m_tm_hr=m_das->sens[m_sensid].pulse.tm_start2;
		//m_tm_min=m_das->sens[m_sensid].pulse.tm_start3;
		//m_static_date.LoadString(IDS_TMLABEL1); "日：
	}else if(siteDoc.m_das.sens[m_sensid].strong.method==3){
		
		[cmboMon setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start1 - 1];
		[cmboDay setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start2 - 1];
		[cmboHour setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start3];
		[cmboMin setSelectIndex:siteDoc.m_das.sens[m_sensid].strong.tm_start4];
		labDay.text = @"日：";
		//m_tm_mon=m_das->sens[m_sensid].pulse.tm_start1-1;
		//m_tm_date=m_das->sens[m_sensid].pulse.tm_start2-1;
		//m_tm_hr=m_das->sens[m_sensid].pulse.tm_start3;
		//m_tm_min=m_das->sens[m_sensid].pulse.tm_start4;
		
		//m_static_date.LoadString(IDS_TMLABEL1);"日：
	}/*else{
	  m_interval=m_das->sens[m_sensid].pulse.tm_start1;
	  m_static_date.LoadString(IDS_TMLABEL1);
	  }
	  CTime t;
	  if(m_das->sens[m_sensid].pulse.time>=0)
	  t=m_das->sens[m_sensid].pulse.time;
	  else t=CTime::GetCurrentTime();
	  
	  m_date=t;
	  m_time=t;
	  */
	/*
	if(siteDoc.m_das.sens[m_sensid].pulse.amptype == 0){
		labUnit.text = @"(count)";
	}else
		labUnit.text = @"(uA)";
	*/
	[self EnableCtl:siteDoc.m_das.sens[m_sensid].strong.method];
	//UpdateData(FALSE);
}



- (void) EnableCtl: (NSInteger)index 
{
	//int m_tm_method = [cmbMethod getSelectIndex];
	int m_tm_method = index;
	if(m_tm_method==0)
	{
		[cmboMon setEnabled:FALSE];
		[cmboDay setEnabled:FALSE];
		[cmboHour setEnabled:TRUE];
		[cmboMin setEnabled:TRUE];
		/*
		 m_combo_mon.EnableWindow(FALSE);
		 m_combo_date.EnableWindow(FALSE);
		 m_combo_hr.EnableWindow(TRUE);
		 m_combo_min.EnableWindow(TRUE);
		 */
		
	}else if(m_tm_method==1 || m_tm_method==2){
		
		[cmboMon setEnabled:FALSE];
		[cmboDay setEnabled:TRUE];
		[cmboHour setEnabled:TRUE];
		[cmboMin setEnabled:TRUE];
		/*
		 m_combo_mon.EnableWindow(FALSE);
		 m_combo_date.EnableWindow(TRUE);
		 m_combo_hr.EnableWindow(TRUE);
		 m_combo_min.EnableWindow(TRUE);
		 */
		//		m_edit_interval.EnableWindow(FALSE);
	}else if(m_tm_method==3)
	{
		[cmboMon setEnabled:TRUE];
		[cmboDay setEnabled:TRUE];
		[cmboHour setEnabled:TRUE];
		[cmboMin setEnabled:TRUE];
		/*
		 m_combo_mon.EnableWindow(TRUE);
		 m_combo_date.EnableWindow(TRUE);
		 m_combo_hr.EnableWindow(TRUE);
		 m_combo_min.EnableWindow(TRUE);
		 */
		//		m_edit_interval.EnableWindow(FALSE);
	}else if(m_tm_method==4){
		[cmboMon setEnabled:FALSE];
		[cmboDay setEnabled:FALSE];
		[cmboHour setEnabled:FALSE];
		[cmboMin setEnabled:FALSE];
		/*
		 m_combo_mon.EnableWindow(FALSE);
		 m_combo_date.EnableWindow(FALSE);
		 m_combo_hr.EnableWindow(FALSE);
		 m_combo_min.EnableWindow(FALSE);
		 */
		//		m_edit_interval.EnableWindow(TRUE);
	}
	
	//m_combo_date.ResetContent();
	if(m_tm_method==1){
		NSArray *cmboDay1 = [[NSArray alloc] initWithObjects: @"星期一",@"星期二",@"星期三",@"星期四",@"星期五",@"星期六",@"星期日",nil];
		[cmboDay setItems:cmboDay1];
	}else {
		NSMutableArray *cmboDay2 = [NSMutableArray array];
		char temp[1024];
		for(int i=1;i<=31;i++)
		{
			sprintf(temp,"  %d  ",  i);
			[cmboDay2 addObject:[[NSString alloc] initWithUTF8String:temp]];
		}
		
		[cmboDay setItems:cmboDay2];
	}
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	[self initUI];
	[cmboSens setSelectIndex:0];
	[self initUI1:0];
	
	
}

- (void)viewDidLoad
{
	self.title = @"设置强震标定";
	//-------------------------------------------- 
//	UILabel *labSens = [[UILabel alloc] initWithFrame : CGRectMake ( 50 , 30 , 180 , 30 )];
//	labSens.text = @"选择地震计：";
//	[scrollView addSubview:labSens];[labSens release];
//	
//	cmboSens= [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 165 , 30 , 200 , 28 )];
//	cmboSens.dropMaxHeigth  = 32*4;
//	cmboSens.delegate = self;
//	NSMutableArray *comboSensData = [NSMutableArray array];
	int i = 0;
	char temp[100];
//	for(i=0;i<siteDoc.m_das.stnpar.sens_num;i++)
//	{
//		sprintf(temp,"地震计 %c",  sens_id[i]);
//		[comboSensData addObject:[[NSString alloc] initWithUTF8String:temp]];
//	}
//	
//	if(siteDoc.m_das.stnpar.sens_num != 0) {
//		[cmboSens setItems:comboSensData];
//		//[comboSensData release];
//	}
//	[scrollView addSubview :cmboSens];
//	[cmboSens release];
	
	//-------------------------------------------- 
	ckBTimer = [[ UICheckButton alloc ] initWithFrame : CGRectMake ( 50 , 65 , 200 , 30 )];
	ckBTimer.label.text = @"定时标定" ;
	ckBTimer.value =[[ NSNumber alloc ] initWithInt:0 ];
	ckBTimer.style = CheckButtonStyleBox ;
	[scrollView addSubview :ckBTimer];
	[ckBTimer release];
	
	
	//-------------------------------------------- 
	UILabel *labMothed = [[UILabel alloc] initWithFrame : CGRectMake ( 50 , 100 , 180 , 30 )];
	labMothed.text = @"定时方式：";
	[scrollView addSubview:labMothed];[labMothed release];
	
	cmbMethod= [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 165 , 100 , 200 , 28 )];
	cmbMethod.dropMaxHeigth  = 32*4;
	cmbMethod.delegate = self;
	
	NSArray *cmbMethodData = [[NSArray alloc] initWithObjects:@"每日",@"每星期",@"每月", @"每年",nil];
	
	[cmbMethod setItems:cmbMethodData];
	[scrollView addSubview :cmbMethod];
	[cmbMethod release];
	[cmbMethodData release];
	
	
	//-------------------------------------------- 
	UILabel *labMon = [[UILabel alloc] initWithFrame : CGRectMake ( 50 , 135 , 180 , 30 )];
	labMon.text = @"月：";
	[scrollView addSubview:labMon];[labMon release];
	
	
	cmboMon= [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 165 , 135 , 200 , 28 )];
	cmboMon.dropMaxHeigth  = 32*4;
	cmboMon.delegate = self;
	NSMutableArray *comboMonData = [NSMutableArray array];
	
	for(i=1;i<=12;i++)
	{
		sprintf(temp,"  %d  ",  i);
		[comboMonData addObject:[[NSString alloc] initWithUTF8String:temp]];
	}
	[cmboMon setItems:comboMonData];
	[scrollView addSubview :cmboMon];
	[cmboMon release];
	
	//-------------------------------------------- 
	labDay = [[UILabel alloc] initWithFrame : CGRectMake ( 50 , 170 , 180 , 30 )];
	labDay.text = @"日：";
	[scrollView addSubview:labDay];[labDay release];
	
	
	cmboDay= [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 165 , 170 , 200 , 28 )];
	cmboDay.dropMaxHeigth  = 32*4;
	cmboDay.delegate = self;
	NSMutableArray *comboDayData = [NSMutableArray array];
	
	for(i=1;i<=31;i++)
	{
		sprintf(temp,"  %d  ",  i);
		[comboDayData addObject:[[NSString alloc] initWithUTF8String:temp]];
	}
	
	[cmboDay setItems:comboDayData];
	[scrollView addSubview :cmboDay];
	[cmboDay release];
	
	
	//----------------------------------------------------------------------------  
	UILabel *labHour = [[UILabel alloc] initWithFrame : CGRectMake ( 50 , 205 , 180 , 30 )];
	labHour.text = @"时：";
	[scrollView addSubview:labHour];[labHour release];
	
	
	cmboHour = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 165 , 205 , 200 , 28 )];
	cmboHour.dropMaxHeigth  = 32*4;
	cmboHour.delegate = self;
	NSMutableArray *comboHourData = [NSMutableArray array];
	
	for(i=0;i<=23;i++)
	{
		sprintf(temp,"  %d  ",  i);
		[comboHourData addObject:[[NSString alloc] initWithUTF8String:temp]];
	}
	
	[cmboHour setItems:comboHourData];
	[scrollView addSubview :cmboHour];
	[cmboHour release];
	
	//----------------------------------------------------------------------------  
	UILabel *labMin = [[UILabel alloc] initWithFrame : CGRectMake ( 50 , 240 , 180 , 30 )];
	labMin.text = @"分：";
	[scrollView addSubview:labMin];[labMin release];
	
	
	cmboMin = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 165 , 240 , 200 , 28 )];
	cmboMin.dropMaxHeigth  = 32*4;
	cmboMin.delegate = self;
	NSMutableArray *comboMinData = [NSMutableArray array];
	
	for(i=0;i<=59;i++)
	{
		sprintf(temp,"  %d  ",  i);
		[comboMinData addObject:[[NSString alloc] initWithUTF8String:temp]];
	}
	
	[cmboMin setItems:comboMinData];
	[scrollView addSubview :cmboMin];
	[cmboMin release];
	
	
	
	
	
}	


- (void)comboBox: (UIComboBox *)comboBox selectItemAtIndex: (NSInteger)index {
	if(comboBox == cmboSens){
		[self initUI1:index];
	}else if(comboBox == cmbMethod){
		
		if(index==1){
			labDay.text = @"星期:";
		}else{
			labDay.text = @"日:";
		}
		[self EnableCtl:index];
		
	}
	
}




- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{	
	/*
	if(textField == txtAmp) {
		NSCharacterSet *cs = [[NSCharacterSet characterSetWithCharactersInString:NUMBERSINT] invertedSet];
		NSString *filter = [[string componentsSeparatedByCharactersInSet:cs] componentsJoinedByString:@""];
		BOOL basiTest = [string	isEqualToString:filter];
		if(!basiTest){
			UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"请输入数字" 
														 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
			[av show];
			return NO;
		}else{
			return YES;
		}
		
		
	}
	*/
	
	return YES;
}



- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	/*
	 if(textField == txtComp){
	 if([self CheckRangeFrom:textField.text Min:1 Max:3] == NO){
	 return NO;
	 }
	 }
	 */
	
	[textField resignFirstResponder];
	return YES;
}

- (IBAction)btnOKClick:(id)sender 
{
	
	int m_tm_method = [cmbMethod getSelectIndex];
	int m_tm_mon =[cmboMon getSelectIndex];
	int m_tm_date =[cmboDay getSelectIndex];
	int m_tm_hr = [cmboHour getSelectIndex];
	int m_tm_min = [cmboMin	getSelectIndex];
	short m_sensid = 0;
	short m_btimer = [ckBTimer isChecked];
	
	long t=theApp.CalcStarttime(m_tm_method,m_tm_mon,m_tm_date+1,m_tm_hr,m_tm_min);
	char m_cmd[1024];
	int m_cmd_len = 0;
	
	m_cmd[0]=0xbf;
	m_cmd[1]=0x13;
	m_cmd[2]=0x97;
	m_cmd[3]=0x74;
	
	CM_STRONGFRM frm;
	
	memset(&frm,0,sizeof(frm));
	frm.head.cmd=0x6023;
	frm.head.sens_id=m_sensid;
	frm.head.length=20;
	
	frm.btimer=m_btimer;
	
	frm.time=t;//t.GetTime();
	
	frm.method=m_tm_method;
	
	if(m_tm_method==0)
	{
		frm.tm_start1=m_tm_hr;
		frm.tm_start2=m_tm_min;
		frm.tm_start3=frm.tm_start4=0;
	}else if(m_tm_method==1 || m_tm_method==2)
	{
		frm.tm_start1=m_tm_date+1;
		frm.tm_start2=m_tm_hr;
		frm.tm_start3=m_tm_min;
		frm.tm_start4=0;
	}else if(m_tm_method==3){
		frm.tm_start1=m_tm_mon+1;
		frm.tm_start2=m_tm_date+1;
		frm.tm_start3=m_tm_hr;
		frm.tm_start4=m_tm_min;
	}//else frm.tm_start1=m_interval;
	
	
	short *p=(short *)&frm;
	frm.chk_sum=0;
	
	for(int i=0;i<(frm.head.length+6)/2-1;i++)
		frm.chk_sum-=p[i];
	
	memcpy(&m_cmd[4],(char *)&frm,frm.head.length+6);
	m_cmd_len=frm.head.length+10;
	
	if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网路连接失败" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"信息" message:@"设置强震标定成功" 
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
