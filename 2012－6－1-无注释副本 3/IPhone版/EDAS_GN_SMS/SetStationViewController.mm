//
//  SetStationViewController.m
//
//  Created by guan mofeng on 12-2-10.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetStationViewController.h"
#include "GlobalData.h"

#define NUMBERSINT @"0123456789\n"
#define NUMBERSFLOAT @"-0123456789.\n"

short g_chnnum[] = {3, 6, 9, 12};

@implementation SetStationViewController



- (void)viewDidLoad
{
    [super viewDidLoad];
	//siteDoc.m_sitelst.at(selIndex);
	//siteDoc.GetDASInfo();
	//if(siteDoc.m_das == NULL){ return;}
	txtNo.delegate = self;
	txtAbbr.delegate = self;
	txtLat.delegate = self;
	txtAlt.delegate = self;
	txtLon.delegate = self;
	txtNetid.delegate = self;
	txtStn.delegate = self;
	txtSensSum.delegate = self;
	
//	cmboADsum = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 220 , 170 , 150 , 28 )];
//	NSArray *cmboADsumData = [[NSArray alloc] initWithObjects:@"3",@"6",@"9", @"12", nil];
//	cmboADsum.dropMaxHeigth = 2 * 28;
//	[cmboADsum setItems:cmboADsumData];
//	[cmboADsumData release];
//	[self.view addSubview:cmboADsum]; 
//	[cmboADsum release];
	
}


- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	
	/*
	char buf[10];
	
	m_ad_sum=0;
	int num=4;//3,6,9,12
	for(int i=0;i<num;i++){
		sprintf(buf,"%d",g_chnnum[i]);
		//m_combo_adsum.AddString(buf);
		if(siteDoc.m_das.stnpar.chn_sum==g_chnnum[i])
			m_ad_sum=i;
	}
	
	m_sens_sum=siteDoc.m_das.stnpar.sens_num;
	*/
	
	int num=4;//3,6,9,12
	for(int i=0;i<num;i++){
	
		if(siteDoc.m_das.stnpar.chn_sum==g_chnnum[i])
			[cmboADsum setSelectIndex:i];
	}
	
	
	char temp[1000];
	sprintf(temp,"%d", siteDoc.m_das.stnpar.sens_num);
	txtSensSum.text = [[NSString alloc] initWithUTF8String:temp];
    txtSensSum.text = @"1";
	NSLog(@"sens_num = %d", siteDoc.m_das.stnpar.sens_num);
	
	sprintf(temp,"%d", siteDoc.m_das.stnpar.id);
	txtNo.text = [[NSString alloc] initWithUTF8String:temp]; 
	
	sprintf(temp,"%s", siteDoc.m_das.stnpar.abbr);
	txtAbbr.text = [[NSString alloc] initWithUTF8String:temp]; 
	
	sprintf(temp,"%f", siteDoc.m_das.stnpar.lat);
	txtLat.text = [[NSString alloc] initWithUTF8String:temp];
	
	sprintf(temp,"%f", siteDoc.m_das.stnpar.alt);
	txtAlt.text = [[NSString alloc] initWithUTF8String:temp];
	
	sprintf(temp,"%f", siteDoc.m_das.stnpar.lon);
	txtLon.text = [[NSString alloc] initWithUTF8String:temp];
	
	sprintf(temp,"%s", siteDoc.m_das.stnpar.netid);
	txtNetid.text = [[NSString alloc] initWithUTF8String:temp];
	
	sprintf(temp,"%s", siteDoc.m_das.stnpar.name);
	txtStn.text = [[NSString alloc] initWithUTF8String:temp];
	
}
 

- (void)btnGPSClick:(id)sender 
{
	siteDoc.OnSetGps();
	m_timer = [NSTimer scheduledTimerWithTimeInterval:0.5f target:self selector:@selector(onGPSTimer:) userInfo:nil repeats:YES];
	
}

- (void) onGPSTimer : (NSTimer *)timer
{
	
	if(siteDoc.m_bshow_gps==1){
		//m_lat=m_pDoc->m_lat;
		//m_lon=m_pDoc->m_lon;
		//m_alt=m_pDoc->m_alt;
		char temp[1024];
		
		sprintf(temp,"%f", siteDoc.m_lat);
		txtLat.text = [[NSString alloc] initWithUTF8String:temp];
		
		sprintf(temp,"%f", siteDoc.m_alt);
		txtAlt.text = [[NSString alloc] initWithUTF8String:temp];
		
		sprintf(temp,"%f", siteDoc.m_lon);
		txtLon.text = [[NSString alloc] initWithUTF8String:temp];
		[m_timer invalidate]; 
	}
		
}
- (void) showValidErro:(NSString *)msg {

	UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:msg
												 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
	[av show];	

}
-(void)btnOKClick:(id)sender{
	
	if(txtStn.text.length == 0 || txtAbbr.text.length == 0 || txtNetid.text.length == 0) {return;}
	int selComAdSum = 3;
	char temp[1024];
	int sensSum = 0;
	int m_btip = 0;
	
	sscanf([txtSensSum.text UTF8String], "%d", &sensSum);
	
	if(sensSum < 1 || sensSum > 12){
		[self showValidErro:@"地震计总数只能为大于等于 1, 且小于 12 的整数"];
		return;
	}
	

	//short g_chnnum[] = {3, 6, 9, 12}
	if(g_chnnum[selComAdSum]<sensSum)
		return;
	
	//地震计总数或者A/D通道总数变化时重新启动数采
	//	if(m_das->stnpar.sens_num!=m_sens_sum || m_das->stnpar.chn_sum!=g_chnnum[m_ad_sum])
	if( siteDoc.m_das.stnpar.chn_sum!=g_chnnum[selComAdSum])
		m_btip=1;
	else m_btip=0;
	
	char m_cmd[1024];
	m_cmd[0]=0xbf;
	m_cmd[1]=0x13;
	m_cmd[2]=0x97;
	m_cmd[3]=0x74;
	
	CM_STNPARFRM frm;
	
	memset(&frm,0,sizeof(frm));
	frm.head.cmd=0x6000;
	frm.head.sens_id=0;
	frm.head.length=132;
	int sno = 0;
	sscanf([txtNo.text UTF8String],"%d", &sno);
	frm.id=sno;
	sprintf(frm.name,"%s",[txtStn.text UTF8String]);
	sprintf(frm.abbr,"%s",[txtAbbr.text UTF8String]);
	
	float ftemp= 0.0;
	sscanf([txtLat.text UTF8String],"%f", &ftemp);
	frm.lat=ftemp*3600000;
	sscanf([txtLon.text UTF8String],"%f", &ftemp);
	frm.lon=ftemp*3600000;
	sscanf([txtAlt.text UTF8String],"%f", &ftemp);
	frm.alt=ftemp*100;
	
	sprintf(frm.netid,"%s",[txtNetid.text UTF8String]);
	
	frm.sens_sum=sensSum;
	frm.chn_sum=g_chnnum[selComAdSum];
	
	short *p=(short *)&frm;
	frm.chk_sum=0;
	
	for(int i=0;i<(frm.head.length+6)/2-1;i++)
		frm.chk_sum-=p[i];
	
	memcpy(&m_cmd[4],(char *)&frm,frm.head.length+10);
	int m_cmd_len=frm.head.length+10;
	
	if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网路连接错误" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"台站信息设置成功" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
	}
	siteDoc.m_btip = m_btip;
	
	
}
-(void)btnCancelClick:(id)sender{
	[self.navigationController popViewControllerAnimated:YES];

}


- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	if(textField == txtSensSum){
		if([self CheckRangeFrom:textField.text Min:1 Max:12] == NO){
			return NO;
		}
	}else if(txtNo == textField){
		if([self CheckRangeFrom:textField.text Min:0 Max:32768] == NO){
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

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	if(textField == txtNetid){
		if(range.location >= 4 ){return NO;}
		else{return YES;}
	} else if(textField == txtStn){
		if(range.location >= 32 ){return NO;}
		else{return YES;}
	} else if(textField == txtAbbr){
		if(range.location >= 5 ){return NO;}
		else{return YES;}
	} else if(textField == txtNo || textField == txtSensSum) {
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
		
		
	}else if(textField == txtAlt || textField == txtLat || txtLon == textField) {
		NSCharacterSet *cs = [[NSCharacterSet characterSetWithCharactersInString:NUMBERSFLOAT] invertedSet];
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

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

@end
