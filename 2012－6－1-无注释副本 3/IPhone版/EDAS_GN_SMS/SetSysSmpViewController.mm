//
//  SetSysSmpViewController.m
//
//  Created by guan mofeng on 12-3-13.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetSysSmpViewController.h"
#include "GlobalData.h"

//extern char sens_id[]={'A','B','C','D','E','F','G','H','I','J','K','L'};
extern char sens_id[];
@implementation SetSysSmpViewController

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
- (void) initUI1 :(NSInteger) index {
	
	//int m_sensid = [comboSens getSelectIndex];
	int m_sensid = index;
	printf("sens_id=%d \n", m_sensid);
	int smp[]={50,100,200,500};
	int i = 0;
	for(i=0;i<4;i++)
	{
		if(smp[i]==siteDoc.m_das.sens[m_sensid].samp)
			break;
	}
	if(i<4){
		//comboSamp.defaultIndex = i;
		[comboSamp setSelectIndex:i];
	}
	
	if(siteDoc.m_das.sens[m_sensid].phase==1){
		[radioGroup checkButtonClicked:radioLine];
		
	}else{
		[radioGroup checkButtonClicked:radioMin];
	}	
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	//[self initUI];
	//[comboSens setSelectIndex:0];
	[self initUI1:0];
}

- (void)comboBox: (UIComboBox *)comboBox selectItemAtIndex: (NSInteger)index {
	//if(comboBox == comboSens){
		[self initUI1:index];
	//}
}
- (void)viewDidLoad
{
	self.title = @"设置系统采样率";
	
	radioGroup =[[ RadioGroup alloc ] init ];
	
	radioLine =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 65 , 150 , 160 , 32 )];
	radioLine.label.text = @"线性相位" ;
	radioLine.value =[[ NSNumber alloc ] initWithInt:1 ];	
	radioLine.style = CheckButtonStyleRadio ;
	[radioGroup add :radioLine];
	[ self.view addSubview :radioLine];
	[radioLine release];
	
	radioMin =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 245 , 150 , 160 , 32 )];
	radioMin.label.text = @"最小相位" ;
	radioMin.value =[[ NSNumber alloc ] initWithInt:2 ];	
	radioMin.style = CheckButtonStyleRadio ;
	[radioGroup add :radioMin];
	[ self.view addSubview :radioMin];
	[radioMin release];
	
	comboSamp = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 150 , 67 , 160 , 32)];
	comboSamp.dropMaxHeigth  = 32*5;

	NSArray *coData = [[NSArray alloc] initWithObjects:@"50 HZ",@"100 HZ", @"200 HZ",@"500 HZ", nil];
	//comboSamp.itemViewCornerRadius = 0.2f;
	[comboSamp setItems:coData];
    comboSamp.delegate = self;
	[coData release];
	[ self.view addSubview :comboSamp];
	[comboSamp release];
	
//	comboSens= [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 150 , 35 , 160 , 30 )];
//	comboSens.dropMaxHeigth  = 32*4;
//	NSMutableArray *comboSensData = [NSMutableArray array];
//	int i = 0;
//	char temp[100];
//	for(i=0;i<siteDoc.m_das.stnpar.sens_num;i++)
//	{
//		sprintf(temp,"地震计 %c",  sens_id[i]);
//		[comboSensData addObject:[[NSString alloc] initWithUTF8String:temp]];
//	}
//	comboSens.delegate = self;
//	if(siteDoc.m_das.stnpar.sens_num != 0) {
//	[comboSens setItems:comboSensData];
//	[comboSens setSelectIndex:0];
//	}
//	[ self.view addSubview :comboSens];
//	[comboSens release];

	
	
}

- (IBAction)btnCancelClick:(id)sender {
     [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnOkClick:(id)sender {
	
	char m_phase = (char)[radioGroup.value intValue];
	
	int sleIndex = [comboSamp getSelectIndex];
	
	//if(comboSens.items == nil) {return;}
	short m_sensid = 0;
    //int samp[]={50,100,200,500};
	int samp = 0;
	int samp_id = 0;
	
	if(sleIndex == 0) {samp = 50; samp_id = 4;}
//	else if(sleIndex == 1) {samp = 10; samp_id = 2;}
//	else if(sleIndex == 2) {samp = 20; samp_id = 3;}
//	else if(sleIndex == 1) {samp = 50; samp_id = 4;}
	else if(sleIndex == 1) {samp = 100; samp_id = 5;}
	else if(sleIndex == 2) {samp = 200; samp_id = 7;}
	else if(sleIndex == 3) {samp = 500; samp_id = 8;}
	int i = 0;
	for(i=0;i<16;i++)
	{
		if(siteDoc.m_das.sens[m_sensid].smpstat[i].samp==samp && siteDoc.m_das.sens[m_sensid].smpstat[i].phase==m_phase)
			break;
	}
	/*
	if(siteDoc.m_das.sens[m_sensid].smpstat[i].stat==0)
	{
		string s1,fmt;
		fmt = "采样率%d %s已经被禁用，请先启动该采样率";
		
		if(m_phase==1)
			s1 ="线性相位";
		else s1= "最小相位";
		
		char temp[1000];
		sprintf(temp, fmt.c_str(), samp, s1.c_str());
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"Erro" message:[[NSString alloc] initWithUTF8String:temp] 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
		return;
	}
	*/
	char m_cmd[1024];
	
	m_cmd[0]=0xbf;
	m_cmd[1]=0x13;
	m_cmd[2]=0x97;
	m_cmd[3]=0x74;
	
	CM_SMPRATEFRM frm;
	int m_cmd_len;
	memset(&frm,0,sizeof(frm));
	frm.head.cmd=0x6003;
	frm.head.sens_id=m_sensid;
	frm.head.length=8;
	
	frm.phase_id=m_phase;
 	frm.samp_id=samp_id;
	
	frm.be_ad=1;
	frm.be_syssamp=1;
	
	short *p=(short *)&frm;
	frm.chk_sum=0;
	
	for( i=0;i<(frm.head.length+6)/2-1;i++)
		frm.chk_sum-=p[i];
	
	memcpy(&m_cmd[4],(char *)&frm,frm.head.length+6);
	m_cmd_len=frm.head.length+10;
	
	
	if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网路连接失败" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"信息" message:@"系统采样率设置成功" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}
	
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}
@end
