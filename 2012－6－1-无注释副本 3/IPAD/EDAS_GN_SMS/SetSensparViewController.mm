//
//  SetSensparViewController.m
//
//  Created by guan mofeng on 12-3-14.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetSensparViewController.h"
#include "GlobalData.h"
#define NUMBERSINT @"0123456789\n"
#define NUMBERSFLOAT @"-0123456789.\n"

extern char sens_id[];
@implementation SetSensparViewController

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

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	[self initUI];
	[cmboSens setSelectIndex:0];
}

- (void)viewDidLoad
{
	
	//-------------------------------------------- 
	
	int xW = 300;
	int yH = 10;
	
	UILabel *labSens = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 20 +yH, 180 , 30 )];
	labSens.text = @"选择地震计：";
	[scrollView addSubview:labSens];[labSens release];
	
	cmboSens= [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 150+xW , 20+yH , 200 , 28 )];
	cmboSens.dropMaxHeigth  = 32*4;
	cmboSens.delegate = self;
	NSMutableArray *comboSensData = [NSMutableArray array];
	int i = 0;
	char temp[100];
	for(i=0;i<siteDoc.m_das.stnpar.sens_num;i++)
	{
		sprintf(temp,"地震计 %c",  sens_id[i]);
		[comboSensData addObject:[[NSString alloc] initWithUTF8String:temp]];
	}
	
	if(siteDoc.m_das.stnpar.sens_num != 0) {
		[cmboSens setItems:comboSensData];
		//[comboSensData release];
	}
	[scrollView addSubview :cmboSens];
	[cmboSens release];
	
	
	
	
	//-------------------------------------------- 
	UILabel *labSens1 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 50+yH , 180 , 30 )];
	labSens1.text = @"地震计型号：";
	[scrollView addSubview:labSens1];[labSens1 release];
		
	cmboSensName = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 150+xW , 50+yH , 200 , 28 )];
	//cmboSensName.dropMaxHeigth  = 32*4;
	NSArray *cmboSensNameData = [[NSArray alloc] initWithObjects:@"FSS-3",@"FBS-3",@"CTS-1",
								 @"JCZ-1",@"BBVS",@"FSS-3",@"FSS-3BDH", @"JCV-100",@"JCV-104", @"SLJ-100", @"BBAS",nil];
	//cmboSensName.itemViewCornerRadius = 5.0f;
	[cmboSensName setItems:cmboSensNameData];
	cmboSensName.delegate = self;
	[cmboSensNameData release];
	[scrollView addSubview:cmboSensName]; 
	[cmboSensName release];
	
	//-------------------------------------------- 
	UILabel *labSens1_1 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 80+yH , 180 , 30 )];
	labSens1_1.text = @"地震计名：";
	[scrollView addSubview:labSens1_1];[labSens1_1 release];
	
	txtSensName = [[UITextField alloc] initWithFrame:CGRectMake(150+xW, 80+yH, 200, 30)];
	txtSensName.borderStyle =  UITextBorderStyleRoundedRect;
	txtSensName.delegate =self;
	[scrollView addSubview:txtSensName];	
	 
	//-------------------------------------------- 
	 UILabel *labSens2 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 110+yH , 180 , 30 )];
	labSens2.text = @"带宽：";
	[scrollView addSubview:labSens2];[labSens2 release];
	
	txtBand = [[UITextField alloc] initWithFrame:CGRectMake(150+xW, 110+yH, 200, 30)];
	txtBand.borderStyle =  UITextBorderStyleRoundedRect;
	txtBand.delegate =self;
	[scrollView addSubview:txtBand];
	
	 //-------------------------------------------- 
	 UILabel *labSens3 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 140+yH , 180 , 30 )];
	labSens3.text = @"标准类型：";
	[scrollView addSubview:labSens3];[labSens3 release];
	
	cmboType = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 150+xW , 142+yH , 200 , 28 )];
	cmboType.delegate = self;
	NSMutableArray *cmboTypeData = [NSMutableArray array];
	if(theApp.m_sptype!=NULL)
	{
		int i = 0;
		for(i=0;i<theApp.m_total_type;i++) 
			
			[cmboTypeData addObject:[[NSString alloc] initWithUTF8String:theApp.m_sptype[i].name]];
	}
	[cmboType setItems:cmboTypeData];
	//[cmboTypeData release];
	[scrollView addSubview:cmboType]; 
	[cmboType release];
	
	
	
	  //-------------------------------------------- 
	 UILabel *labSens4 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 170 +yH, 180 , 30 )];
	labSens4.text = @"记录类型：";
	[scrollView addSubview:labSens4];[labSens4 release];
	/*
	 s.LoadString(IDS_RECTYPE1STR); //"位移"
	 m_combo_rectype.AddString(s);
	 s.LoadString(IDS_RECTYPE2STR); //"速度"
	 m_combo_rectype.AddString(s);
	 s.LoadString(IDS_RECTYPE3STR); // "加速度"
	 m_combo_rectype.AddString(s);
	 s.LoadString(IDS_RECTYPE4STR); //"前兆"
	 m_combo_rectype.AddString(s);
	 
	 */
	
	cmboRecType = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 150+xW , 172+yH , 200 , 28 )];
	//cmboSensName.dropMaxHeigth  = 32*4;
	NSArray *cmboRecTypeData = [[NSArray alloc] initWithObjects:@"位移",@"速度",@"加速度",@"前兆",nil];
	[cmboRecType setItems:cmboRecTypeData];
	[cmboRecTypeData release];
	cmboRecType.delegate = self;
	[scrollView addSubview:cmboRecType]; 
	[cmboRecType release];
	  
	//-------------------------------------------- 
	UILabel *labSens5 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 200 +yH, 180 , 30 )];
	labSens5.text = @"埋深（米）：";
	[scrollView addSubview:labSens5];[labSens5 release];
	txtDepth = [[UITextField alloc] initWithFrame:CGRectMake(150+xW, 200+yH, 200, 30)];
	txtDepth.borderStyle =  UITextBorderStyleRoundedRect;
	txtDepth.delegate =self;
	[scrollView addSubview:txtDepth];	
	
	
	//-------------------------------------------- 
	UILabel *labSens6 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 230 +yH, 180 , 30 )];
	labSens6.text = @"基岩：";
	[scrollView addSubview:labSens6];[labSens6 release];
	txtRock = [[UITextField alloc] initWithFrame:CGRectMake(150+xW, 230+yH, 200, 30)];
	txtRock.borderStyle =  UITextBorderStyleRoundedRect;
	txtRock.delegate =self;
	[scrollView addSubview:txtRock];	
	
	
	//-------------------------------------------- 
	UILabel *labSens7 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 260 +yH, 180 , 30 )];
	labSens7.text = @"序列号：";
	[scrollView addSubview:labSens7];[labSens7 release];

	txtSerial = [[UITextField alloc] initWithFrame:CGRectMake(150+xW, 260+yH, 200, 30)];
	txtSerial.borderStyle =  UITextBorderStyleRoundedRect;
	txtSerial.delegate =self;
	[scrollView addSubview:txtSerial];	
	
	
	//-------------------------------------------- 
	
	UILabel *labSens8 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 290 +yH, 180 , 30 )];
	labSens8.text = @"供应商：";
	[scrollView addSubview:labSens8];[labSens8 release];
	
	txtProvider = [[UITextField alloc] initWithFrame:CGRectMake(150+xW, 290+yH, 200, 30)];
	txtProvider.borderStyle =  UITextBorderStyleRoundedRect;
	txtProvider.delegate =self;
	[scrollView addSubview:txtProvider];	
	
	
	//-------------------------------------------- 
	
	
	UILabel *labSens9 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 320+yH , 180 , 30 )];
	labSens9.text = @"分向信息";
	[scrollView addSubview:labSens9];[labSens9 release];
	
	UILabel *labSens10 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 350+yH , 180 , 30 )];
	labSens10.text = @"分向数：";
	[scrollView addSubview:labSens10];[labSens10 release];
	
	
	txtComp = [[UITextField alloc] initWithFrame:CGRectMake(150+xW, 350+yH, 200, 30)];
	txtComp.borderStyle =  UITextBorderStyleRoundedRect;
	txtComp.delegate =self;
	[scrollView addSubview:txtComp];	
	
	
	//-------------------------------------------- 
	UILabel *labSens11 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 380+yH , 180 , 30 )];
	labSens11.text = @"起始通道：";
	[scrollView addSubview:labSens11];[labSens11 release];
	
	/*
	 //初始化A/D通道
	 UINT id[]={IDS_HAD1,IDS_HAD2,IDS_HAD3,IDS_HAD4,IDS_HAD5,IDS_HAD6,IDS_LAD1,IDS_LAD2,IDS_LAD3,IDS_LAD4,IDS_LAD5,IDS_LAD6};
	 for(i=0;i<12;i++)
	 {
	 s.LoadString(id[i]);
	 m_combo_ch1ad.AddString(s);
	 }
	 */
	
	cmboChn1AD = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 150+xW , 380+yH , 200 , 28 )];
	NSArray *cmboChn1ADData = [[NSArray alloc] initWithObjects:@"高速通道1",@"高速通道2",@"高速通道3",
								 @"高速通道4",@"高速通道5",@"高速通道6",@"低速通道1", @"低速通道2",@"低速通道3", @"低速通道4", @"低速通道5", @"低速通道6", nil];
	//cmboSensName.itemViewCornerRadius = 5.0f;
	cmboChn1AD.delegate = self;
	[cmboChn1AD setItems:cmboChn1ADData];
	[cmboChn1ADData release];
	[scrollView addSubview:cmboChn1AD]; 
	[cmboChn1AD release];
	
	//-------------------------------------------- 
	UILabel *labSens12 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 410+yH , 180 , 30 )];
	labSens12.text = @"分向 1 测项分量代码：";
	[scrollView addSubview:labSens12];[labSens12 release];
	
	cmboChn1cxid = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 205+xW , 410+yH , 170 , 28 )];
	[scrollView addSubview:cmboChn1cxid];[cmboChn1cxid release];
	
	//-------------------------------------------- 
	UILabel *labSens13 = [[UILabel alloc] initWithFrame : CGRectMake ( 20 +xW, 440+yH , 190 , 30 )];
	labSens13.text = @"分向 2 测项分量代码：";
	[scrollView addSubview:labSens13];[labSens13 release];
	
	cmboChn2cxid = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 205 +xW, 440+yH , 170 , 28 )];
	[scrollView addSubview:cmboChn2cxid];[cmboChn2cxid release];

	//-------------------------------------------- 
	UILabel *labSens14 = [[UILabel alloc] initWithFrame : CGRectMake ( 20+xW , 470+yH , 180 , 30 )];
	labSens14.text = @"分向 3 测项分量代码：";
	[scrollView addSubview:labSens14];[labSens14 release];
	cmboChn3cxid = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 205+xW , 470+yH , 170 , 28 )];
	[scrollView addSubview:cmboChn3cxid];[cmboChn3cxid release];
	/*
	UIButton *btnOK = [[UIButton alloc] initWithFrame: CGRectMake(40, 470, 40, 40)];
	btnOK.titleLabel.text = @"确定";
	[scrollView addSubview:btnOK];
	[btnOK release];
	
	UIButton *btnCancel = [[UIButton alloc] initWithFrame: CGRectMake(100, 470, 40, 40)];
	btnCancel.titleLabel.text = @"确定";
	[scrollView addSubview:btnCancel];
	[btnCancel release];
	*/

}

- (void)comboBox: (UIComboBox *)comboBox selectItemAtIndex: (NSInteger)index {
	if(comboBox == cmboSensName){
		[self comboBoxSensNameSel:index];
	}else if(comboBox == cmboSens){
		[self comboBoxSensSel:index];
	}else if(comboBox == cmboType){
		[self comboTypeSel:index];
	}
}
- (void) comboBoxSensSel :(NSInteger)index {
	if(cmboSens.items == nil && index>=40){return;}
	[self InitC:index];
}


- (void) comboTypeSel :(NSInteger)index 
{
	if(theApp.m_sptype==NULL)
	{	NSLog(@"comboTypeSel theApp.m_sptype==NULL");
		return;
	}
	//int sleIndexType = [cmboType getSelectIndex];
	int sleIndexType = index;
	[self InitCXIDBox: sleIndexType];
	printf("comboTypeSel %d", sleIndexType);
	if(sleIndexType>=0 && sleIndexType<theApp.m_total_type)
	{	
		
		if(cmboChn1cxid.items != nil ){
			NSString *selName1 = [[NSString alloc] initWithUTF8String:theApp.m_sptype[sleIndexType].cxname[0]];
			NSUInteger selIndexN1 = [cmboChn1cxid.items indexOfObject : selName1];
			if(selIndexN1 != NSNotFound){
				[cmboChn1cxid setSelectIndex : selIndexN1];
			}
		}
		
		if(cmboChn2cxid.items != nil ){
			NSString *selName2 = [[NSString alloc] initWithUTF8String:theApp.m_sptype[sleIndexType].cxname[1]];
			NSUInteger selIndexN2 = [cmboChn2cxid.items indexOfObject : selName2];
			if(selIndexN2 != NSNotFound){
				[cmboChn2cxid setSelectIndex : selIndexN2];
			}
		}
		
		if(cmboChn3cxid.items != nil ){
			NSString *selName3 = [[NSString alloc] initWithUTF8String:theApp.m_sptype[sleIndexType].cxname[2]];
			NSUInteger selIndexN3 = [cmboChn3cxid.items indexOfObject : selName3];
			if(selIndexN3 != NSNotFound){
				[cmboChn3cxid setSelectIndex : selIndexN3];
			}
		}
	}
	
}

- (void) InitC:(NSInteger) index
{
	char temp[1024];
	//int selCmboSens = [cmboSens getSelectIndex];
	int selCmboSens = index;
	int m_sensid = index;
	//txtBand.text= [[NSString alloc] initWithUTF8String:siteDoc.m_das.sens[selCmboSens].sens_par.bandstr]; 	
	[cmboType setSelectIndex:theApp.GetCXID(siteDoc.m_das.sens[selCmboSens].sens_par.type)];
	
	/*
	if(siteDoc.m_das.sens[m_sensid].sens_par.rectype[0]=='A')
		[cmboRecType setSelectIndex:2];
	else if(siteDoc.m_das.sens[m_sensid].sens_par.rectype[0]=='V')
		[cmboRecType setSelectIndex:1];
	else  if(siteDoc.m_das.sens[m_sensid].sens_par.rectype[0]=='Q')
		[cmboRecType setSelectIndex:3];
	else [cmboRecType setSelectIndex:0];
	
	sprintf(temp, "%d", siteDoc.m_das.sens[selCmboSens].sens_par.comp); 
	txtComp.text = [[NSString alloc] initWithUTF8String:temp];
	*/
	[self InitCXIDBox:theApp.GetCXID(siteDoc.m_das.sens[selCmboSens].sens_par.type)];
	//m_ch1ad=siteDoc.m_das.sens[m_sensid].sens_par.das_id[0]-1;
	[cmboChn1AD setSelectIndex:siteDoc.m_das.sens[selCmboSens].sens_par.das_id[0]-1];

	/////////////////////////////////////////////////////////////////////////////////////////////
	
	int ttype = [cmboType getSelectIndex];
	string ch1cxidtemp;
	if(theApp.GetCXName(ttype, siteDoc.m_das.sens[selCmboSens].sens_par.cxid[0], ch1cxidtemp) == 0)
	{}else{
		printf("0=%s , %s \n", siteDoc.m_das.sens[selCmboSens].sens_par.cxid[0], ch1cxidtemp.c_str());
		NSString *selNameCh1cxid = [[NSString alloc] initWithUTF8String:ch1cxidtemp.c_str()];
		NSUInteger selNameCh1cxidIndexN = [cmboChn1cxid.items indexOfObject : selNameCh1cxid];
		if(selNameCh1cxidIndexN != NSNotFound){
			[cmboChn1cxid setSelectIndex : selNameCh1cxidIndexN];
		}		
	}
	ch1cxidtemp = "";
	if(theApp.GetCXName(ttype, siteDoc.m_das.sens[selCmboSens].sens_par.cxid[1], ch1cxidtemp) == 0)
	{}else{ 
		printf("1=%s , %s \n", siteDoc.m_das.sens[selCmboSens].sens_par.cxid[1], ch1cxidtemp.c_str());
		NSString *selNameCh2cxid = [[NSString alloc] initWithUTF8String:ch1cxidtemp.c_str()];
		NSUInteger selNameCh2cxidIndexN = [cmboChn1cxid.items indexOfObject : selNameCh2cxid];
		if(selNameCh2cxidIndexN != NSNotFound){
			[cmboChn2cxid setSelectIndex : selNameCh2cxidIndexN];
		}	
	}
	ch1cxidtemp = "";
 	if(theApp.GetCXName(ttype, siteDoc.m_das.sens[selCmboSens].sens_par.cxid[2], ch1cxidtemp) == 0)
	{}else{ 
		printf("2=%s , %s \n", siteDoc.m_das.sens[selCmboSens].sens_par.cxid[2], ch1cxidtemp.c_str());
		NSString *selNameCh3cxid = [[NSString alloc] initWithUTF8String:ch1cxidtemp.c_str()];
		NSUInteger selNameCh3cxidIndexN = [cmboChn1cxid.items indexOfObject : selNameCh3cxid];
		if(selNameCh3cxidIndexN != NSNotFound){
			[cmboChn3cxid setSelectIndex : selNameCh3cxidIndexN];
		}
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	txtProvider.text = [[NSString alloc] initWithUTF8String:siteDoc.m_das.sens[selCmboSens].sens_par.provider]; 	
	 
	//m_sensname=m_das->sens[m_sensid].sens_par.name;
	sprintf(temp,"%d",siteDoc.m_das.sens[m_sensid].sens_par.depth);
	txtDepth.text = [[NSString alloc] initWithUTF8String:temp];
	txtSerial.text = [[NSString alloc] initWithUTF8String:siteDoc.m_das.sens[selCmboSens].sens_par.series];
	txtRock.text = 	[[NSString alloc] initWithUTF8String:siteDoc.m_das.sens[selCmboSens].sens_par.rock];
	
	NSString *selName = [[NSString alloc] initWithUTF8String:siteDoc.m_das.sens[selCmboSens].sens_par.name];
	NSUInteger selIndexN = [cmboSensName.items indexOfObject : selName];
	if(selIndexN != NSNotFound){
		[cmboSensName setSelectIndex : selIndexN];
	}
	
	if([selName compare: @"FBS-3" options:NSCaseInsensitiveSearch] == 0) {
		txtBand.text = @"20s~20Hz";
	}else if([selName compare: @"CTS-1" options:NSCaseInsensitiveSearch] == 0) 
	{
		txtBand.text=@"120s~60Hz";
	}else if([selName compare: @"JCZ-1" options:NSCaseInsensitiveSearch]== 0)
	{
		txtBand.text=@"360~60Hz";
	}else if([selName compare: @"BBVS" options:NSCaseInsensitiveSearch]== 0){
		
		txtBand.text=@"60s~60Hz";
	}else if(([selName compare: @"FSS-3" options:NSCaseInsensitiveSearch] ==0) || 
			 ([selName compare: @"FSS-3BDH" options:NSCaseInsensitiveSearch]==0)||
			 ([selName compare: @"JCV-100" options:NSCaseInsensitiveSearch]==0)
			 || ([selName compare: @"JCV-104" options:NSCaseInsensitiveSearch] == 0)){
		txtBand.text=@"1s~20Hz";
		
	}else if([selName compare: @"SLJ-100" options:NSCaseInsensitiveSearch]== 0 )
	{
		txtBand.text=@"DC~80Hz";
		
	}else if([selName compare: @"BBAS" options:NSCaseInsensitiveSearch]== 0){
		txtBand.text=@"DC~80Hz";
	}
	
	
	if(siteDoc.m_das.sens[m_sensid].sens_par.rectype[0]=='A')
		[cmboRecType setSelectIndex:2];
	else if(siteDoc.m_das.sens[m_sensid].sens_par.rectype[0]=='V')
		[cmboRecType setSelectIndex:1];
	else  if(siteDoc.m_das.sens[m_sensid].sens_par.rectype[0]=='Q')
		[cmboRecType setSelectIndex:3];
	else [cmboRecType setSelectIndex:0]; //'D'
	
	txtSensName.text = selName;
	txtBand.text= [[NSString alloc] initWithUTF8String:siteDoc.m_das.sens[selCmboSens].sens_par.bandstr]; 
	sprintf(temp, "%d", siteDoc.m_das.sens[selCmboSens].sens_par.comp); 
	txtComp.text = [[NSString alloc] initWithUTF8String:temp];
	
}


- (void) InitCXIDBox :(int) type
{
	if(theApp.m_sptype==NULL) {
		NSLog(@"theApp.m_sptype==NULL");
		return;
	}
	/*
	CComboBox * pcombo[3];
	pcombo[0]=&m_combo_ch1cxid;
	pcombo[1]=&m_combo_ch2cxid;
	pcombo[2]=&m_combo_ch3cxid;
	*/
	
	int j = 0;
	NSMutableArray *cmboChn1cxidData = [NSMutableArray array];
	NSMutableArray *cmboChn2cxidData = [NSMutableArray array];
	NSMutableArray *cmboChn3cxidData = [NSMutableArray array];
	
	printf("select type=%d \n", type);
	for( j=0;j<theApp.m_sptype[type].comp && j<3;j++)
	{
		//NSLog(<#NSString *format#>)
		[cmboChn1cxidData addObject:[[NSString alloc] initWithUTF8String:theApp.m_sptype[type].cxname[j]]];
		[cmboChn2cxidData addObject:[[NSString alloc] initWithUTF8String:theApp.m_sptype[type].cxname[j]]];
		[cmboChn3cxidData addObject:[[NSString alloc] initWithUTF8String:theApp.m_sptype[type].cxname[j]]];
		//printf("c=%s \n",theApp.m_sptype[type].cxname[j]);
		//NSLog([[NSString alloc] initWithUTF8String:theApp.m_sptype[type].cxname[j]]);
	}
	
	//NSLog(@"InitCXIDBox in for out 1= %d", cmboChn1cxidData.count);
	//NSLog(@"InitCXIDBox in for out 2= %d", cmboChn2cxidData.count);
	//NSLog(@"InitCXIDBox in for out 3= %d", cmboChn3cxidData.count);
	[cmboChn1cxid setItems:cmboChn1cxidData];
	[cmboChn2cxid setItems:cmboChn2cxidData];
	[cmboChn3cxid setItems:cmboChn3cxidData];
	//NSLog(@"InitCXIDBox in for out");
	
	
}


- (void) comboBoxSensNameSel :(NSInteger)index {
	
	NSString *selName = [cmboSensName.items objectAtIndex:index];
	
	txtSensName.text = selName;
	if([selName compare: @"FBS-3" options:NSCaseInsensitiveSearch] == 0) {
		//m_type=theApp.GetCXID(1);
		//m_rectype=1;
		
		[cmboType setSelectIndex:theApp.GetCXID(1)];
		[cmboRecType setSelectIndex:1];
		//[self InitCXIDBox:1];
		txtBand.text = @"20s~20Hz";
	}else if([selName compare: @"CTS-1" options:NSCaseInsensitiveSearch] == 0) 
	{
		//m_type= theApp.GetCXID(2);
		//m_rectype=1;
		[cmboType setSelectIndex:theApp.GetCXID(2)];
		//[cmboRecType setSelectIndex:1];
		[self InitCXIDBox:1];
		
		txtBand.text=@"120s~60Hz";
	}else if([selName compare: @"JCZ-1" options:NSCaseInsensitiveSearch] == 0)
	{
		//m_type= theApp.GetCXID(2);
		//m_rectype=1;
		
		[cmboType setSelectIndex:theApp.GetCXID(2)];
		[cmboRecType setSelectIndex:1];
		//[self InitCXIDBox:1];
		txtBand.text=@"360~60Hz";
	}else if([selName compare: @"BBVS" options:NSCaseInsensitiveSearch] ==0){
		//m_type=theApp.GetCXID(2);
		//m_rectype=1;
		[cmboType setSelectIndex:theApp.GetCXID(2)];
		//[cmboRecType setSelectIndex:1];
		txtBand.text=@"60s~60Hz";
	}else if(([selName compare: @"FSS-3" options:NSCaseInsensitiveSearch]==0) || 
			 ([selName compare: @"FSS-3BDH" options:NSCaseInsensitiveSearch]==0)||
			 ([selName compare: @"JCV-100" options:NSCaseInsensitiveSearch]==0)
			 || ([selName compare: @"JCV-104" options:NSCaseInsensitiveSearch] ==0)){
		//m_type=theApp.GetCXID(0);
		//m_rectype=1;
		[cmboType setSelectIndex:theApp.GetCXID(0)];
		[cmboRecType setSelectIndex:1];
		//[self InitCXIDBox:1];
		txtBand.text=@"1s~20Hz";
		
	}else if([selName compare: @"SLJ-100" options:NSCaseInsensitiveSearch]==0 )
	{
		//m_type=theApp.GetCXID(3);
		//m_rectype=2;
		[cmboType setSelectIndex:theApp.GetCXID(3)];
		[cmboRecType setSelectIndex:2];
		//[self InitCXIDBox:2];
		txtBand.text=@"DC~80Hz";
		
	}else if([selName compare: @"BBAS" options:NSCaseInsensitiveSearch] == 0){
		//m_type=theApp.GetCXID(3);
		//m_rectype=2;
		[cmboType setSelectIndex:theApp.GetCXID(3)];
		[cmboRecType setSelectIndex:2];
		//[self InitCXIDBox:2];
		txtBand.text=@"DC~80Hz";
	}
	int sleIndexType = [cmboType getSelectIndex];
	[self InitCXIDBox:sleIndexType];
	if(sleIndexType>=0 && sleIndexType<theApp.m_total_type)
	{	
				
		if(cmboChn1cxid.items != nil ){
			NSString *selName1 = [[NSString alloc] initWithUTF8String:theApp.m_sptype[sleIndexType].cxname[0]];
			NSUInteger selIndexN1 = [cmboChn1cxid.items indexOfObject : selName1];
			if(selIndexN1 != NSNotFound){
				[cmboChn1cxid setSelectIndex : selIndexN1];
			}
		}
		
		if(cmboChn2cxid.items != nil ){
			NSString *selName2 = [[NSString alloc] initWithUTF8String:theApp.m_sptype[sleIndexType].cxname[1]];
			NSUInteger selIndexN2 = [cmboChn2cxid.items indexOfObject : selName2];
			if(selIndexN2 != NSNotFound){
				[cmboChn2cxid setSelectIndex : selIndexN2];
			}
		}
		
		if(cmboChn3cxid.items != nil ){
			NSString *selName3 = [[NSString alloc] initWithUTF8String:theApp.m_sptype[sleIndexType].cxname[2]];
			NSUInteger selIndexN3 = [cmboChn3cxid.items indexOfObject : selName3];
			if(selIndexN3 != NSNotFound){
				[cmboChn3cxid setSelectIndex : selIndexN3];
			}
		}
	}
	
}



- (IBAction)btnCancelClick:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnOKClick:(id)sender {
	
	if(txtProvider.text.length ==0 || txtSerial.text.length == 0 || txtBand.text.length == 0)
		return;
	
	char temp[1024];
	
	if(cmboSens.items == nil){return;}
	//在分向数和A/D通道发生变化时,要求重新启动数采
	int selCmboSens = [cmboSens getSelectIndex];
	int selCmboChn1AD = [cmboChn1AD getSelectIndex];
	int m_btip=0;
	
	
	int intcomp = 0; 
	sscanf([txtComp.text UTF8String], "%d", &intcomp);
	if(intcomp <1 || intcomp > 3) {return;}
	
	if(siteDoc.m_das.sens[selCmboSens].sens_par.comp==intcomp )
	{
		if(selCmboChn1AD !=siteDoc.m_das.sens[selCmboSens].sens_par.das_id[0]-1)
			m_btip=1;
		
	}else m_btip=1;
	
	siteDoc.m_btip=m_btip;	
	char m_cmd[1024];
	m_cmd[0]=0xbf;
	m_cmd[1]=0x13;
	m_cmd[2]=0x97;
	m_cmd[3]=0x74;
	
	CM_SENSPARFRM frm;
	
	memset(&frm,0,sizeof(frm));
	frm.head.cmd=0x6001;
	frm.head.sens_id=(short)selCmboSens;
	frm.head.length=212;
	
	//NSString *senName = [cmboSensName.items objectAtIndex:[cmboSensName getSelectIndex]];
	NSString *senName = txtSensName.text;
	sprintf(frm.name,"%s",[senName UTF8String]);
	sprintf(frm.provider,"%s",[txtProvider.text UTF8String]);
	sprintf(frm.series,"%s",[txtSerial.text	UTF8String]);
	sprintf(frm.bandstr,"%s",[txtBand.text UTF8String]);
	sprintf(frm.rock,"%s",[txtRock.text UTF8String]);
	frm.comp=intcomp;
	frm.das_id[0]=selCmboChn1AD+1;
	frm.das_id[1]=selCmboChn1AD+2;
	frm.das_id[2]=selCmboChn1AD+3;
	
	int intdepth = 0; 
	sscanf([txtDepth.text UTF8String], "%d", &intdepth);
	
	frm.depth=intdepth;
	sprintf(frm.state,"GC");
	sprintf(frm.rspfile,"sens_%d.cfg",selCmboSens);
	
	
	char rectype[]={'D','V','A','Q'};
	frm.rectype[0]=rectype[[cmboRecType getSelectIndex]];
	
	if(theApp.m_sptype!=NULL)
		sprintf(frm.type,"%s",theApp.m_sptype[[cmboType getSelectIndex]].id);
	else sprintf(frm.type,"");
	
	string s;
	[[cmboChn1cxid.items objectAtIndex:[cmboChn1cxid getSelectIndex]] UTF8String];
	theApp.GetCXSID([cmboType getSelectIndex],[[cmboChn1cxid.items objectAtIndex:[cmboChn1cxid getSelectIndex]] UTF8String], s);
	sprintf(frm.cxid[0],"%s",s.c_str());
	theApp.GetCXSID([cmboType getSelectIndex],[[cmboChn2cxid.items objectAtIndex:[cmboChn2cxid getSelectIndex]] UTF8String], s);
	sprintf(frm.cxid[1],"%s",s.c_str());
	theApp.GetCXSID([cmboType getSelectIndex],[[cmboChn3cxid.items objectAtIndex:[cmboChn3cxid getSelectIndex]] UTF8String], s);
 	sprintf(frm.cxid[2],"%s",s.c_str());
	
	short *p=(short *)&frm;
	frm.chk_sum=0;
	
	for(int i=0;i<(frm.head.length+6)/2-1;i++)
		frm.chk_sum-=p[i];
	
	memcpy(&m_cmd[4],(char *)&frm,frm.head.length+10);
	int m_cmd_len=frm.head.length+10;
	
	if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置地震计参数成功" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}
	
	
	
}



- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	if(textField == txtSerial){
		if(range.location >= 16 ){return NO;}
		else{return YES;}
	} 
	
	else if(textField == txtBand){
		if(range.location >= 16 ){return NO;}
		else{return YES;}
	} 
	else if(textField == txtProvider){
		if(range.location >= 32 ){return NO;}
		else{return YES;}
	} 
	else if(textField == txtRock){
		if(range.location >= 32 ){return NO;}
		else{return YES;}
	}
	
	else if(textField == txtComp) {
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
	return YES;
}



- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	if(textField == txtComp){
		if([self CheckRangeFrom:textField.text Min:1 Max:3] == NO){
			return NO;
		}
	}
	
	
	[textField resignFirstResponder];
	return YES;
}

- (BOOL) CheckRangeFrom:(NSString *)from Min : (int)min Max:(int) max {
	//NSLog(from);
	int ws = [from intValue];
	if(ws >= min && ws <= max){return YES;}
	else{
		NSString *msg = [NSString stringWithFormat:@"您输入的是%@ 请输入大于%d% 且小于 %d 的整数", from, min,max];
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:msg
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
		return NO;
	}
} 

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortrait) || (toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown));
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

@end
