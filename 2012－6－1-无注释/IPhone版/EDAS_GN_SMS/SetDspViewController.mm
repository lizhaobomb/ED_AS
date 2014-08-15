//
//  SetDspViewController.m
//
//  Created by guan mofeng on 12-3-27.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetDspViewController.h"
#import "RootViewController.h"
//#import "DrawwaveView.h"
//#include "GlobalData.h"

#define NUMBERSINT @"0123456789\n"
#define NUMBERSFLOAT @"-0123456789.\n"

@implementation SetDspViewController


- (void)viewDidLoad
{
	//if(siteDoc.m_das == NULL){ return;}
	txtMax.delegate = self;
	txtMin.delegate = self;
	txtScanTime.delegate = self;
	
	char temp[1024];
	
	dv =  [RootViewController U_drawview];
	//txtMax.text = dv.m_dspctl
	
	cmboChannel = [[ UIComboBox alloc ] initWithFrame : CGRectMake ( 173 , 20 , 180 , 28 )];
	cmboChannel.dropMaxHeigth = 7 * 28;
	cmboChannel.delegate = self;
	
	[self.view addSubview:cmboChannel]; 
	[cmboChannel release];
	
	checkSameAll =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 140 , 170 , 170 , 30 )];
	checkSameAll.label.text = @"全部一样" ;
	checkSameAll.value =[[ NSNumber alloc ] initWithInt:0 ];
	checkSameAll.style = CheckButtonStyleBox ;
	[ self.view addSubview :checkSameAll];
	[checkSameAll release];
	
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
	if(m_phead == NULL) {return;}
	
	CHANNEL *tmp = m_phead;	
	while(m_phead!=NULL)
	{
		tmp = m_phead;
		m_phead=m_phead->lpnext;
		delete tmp;
		
	}
	
	
	
	
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	
	m_phead=NULL;
	
	CHANNEL *tail,*pt;
	CHANNEL *tmp=siteDoc.m_pchannels;	
	while(tmp!=NULL)
	{
		tail=new CHANNEL;
		tail->chn_id=tmp->chn_id;
		strcpy(tail->stn,tmp->stn);
		strcpy(tail->chn_label,tmp->chn_label);
		tail->dsp.max=tmp->dsp.max;
		tail->dsp.min=tmp->dsp.min;
		tail->dsp.unit=tmp->dsp.unit;
		tail->factor=tmp->factor;
		tail->lpnext=NULL;
		if(m_phead==NULL)
			m_phead=tail;
		else pt->lpnext=tail;
		pt=tail;
		tmp=tmp->lpnext;
	}
	//m_das=siteDoc.m_das;
	if(m_phead == NULL) {return;}
	char temp[1024];
	sprintf(temp, "%d", dv.m_dspctl.scantime);
	txtScanTime.text = [[NSString alloc] initWithUTF8String:temp];
	
	
	tmp=m_phead;
	
	sprintf(temp, "%f", tmp->dsp.max);
	txtMax.text = [[NSString alloc] initWithUTF8String: temp];
	sprintf(temp, "%f", tmp->dsp.min);
	txtMin.text = [[NSString alloc] initWithUTF8String: temp];
	
	NSMutableArray *cmboChannelData = [NSMutableArray array];
		while(tmp!=NULL)
	{
		
		[cmboChannelData addObject:[[NSString alloc] initWithUTF8String:tmp->chn_label]];
		//m_combo_channel.AddString(tmp->chn_label);
		tmp=tmp->lpnext;
	}
	
	cmboChannel.items = cmboChannelData;
	[cmboChannel setSelectIndex:0];
	m_curchan = 0;
	
}



- (IBAction)btnOK:(id)sender 
{
	if(m_phead == NULL){return;}
	CHANNEL *tmp=m_phead;
	int i = 0;
	m_curchan = [cmboChannel getSelectIndex];
	
	for(i=0;i<m_curchan;i++)
		tmp=tmp->lpnext;
	
	
	int m_max = [txtMax.text intValue];
	int m_min = [txtMin.text intValue];
	
	if(m_max <= m_min)
	{
		
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"最小值必须小于最大值" 
													 delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
		[av show];	
		
		return;
		
	}
	
	if([checkSameAll isChecked]){
		tmp=m_phead;
		while(tmp!=NULL)
		{
			tmp->dsp.max=m_max;
			tmp->dsp.min=m_min;
			//tmp->dsp.unit=m_unit;
			//if(m_unit==0)
			//	tmp->factor=1.f;
			//else tmp->factor=m_sensitivity;
			tmp=tmp->lpnext;
		}
	}else{
		tmp->dsp.max=m_max;
		tmp->dsp.min=m_min;
		//tmp->dsp.unit=m_unit;
		//if(m_unit==0)
		//	tmp->factor=1.f;
		//else tmp->factor=m_sensitivity;
	}
	int m_scantime = [txtScanTime.text intValue];
	if(m_scantime<=0)
	{
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"扫描时间不能小于零" 
													 delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
		[av show];	
		return;
	}
	//////////////////////////////////////////////////////
	
	i=0;
	tmp=siteDoc.m_pchannels;
	//tail=dlg.m_phead;
	CHANNEL *tail = m_phead;	
	while(tmp!=NULL)
	{
		tmp->dsp.max=tail->dsp.max;
		tmp->dsp.min=tail->dsp.min;
		if(tmp->dsp.unit!=4)
		{
			tmp->dsp.unit=tail->dsp.unit;
			tmp->factor=tail->factor;
		}
		//UpdateTipText(tmp,i);
		
		tmp=tmp->lpnext;
		i++;
		
		tail=tail->lpnext;
	}
	if(m_scantime != dv.m_dspctl.scantime){
		dv.m_dspctl.scantime=m_scantime;
	}
	
	[dv ReDraw];
	
	
	UIAlertView *av1 = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置显示参数成功" 
												 delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
	[av1 show];	
	return;
	
	
}

- (void)comboBox: (UIComboBox *)comboBox selectItemAtIndex: (NSInteger)index {
	
	if(m_phead == NULL){return;}
	CHANNEL *tmp=m_phead;
	char temp[1024];

	int i = 0;
	for(i=0;i<m_curchan;i++)
		tmp=tmp->lpnext;
	float m_max = [txtMax.text floatValue];
	float m_min = [txtMin.text floatValue];
	
	if(m_max <= m_min)
	{
		
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"最小值必须小于最大值" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
		
		sprintf(temp, "%f", tmp->dsp.max);
		txtMax.text = [[NSString alloc] initWithUTF8String: temp];
		sprintf(temp, "%f", tmp->dsp.min);
		txtMin.text = [[NSString alloc] initWithUTF8String: temp];
		
		[comboBox setSelectIndex:m_curchan];
		
		return;
		
	}else{
		tmp->dsp.max = m_max;
		tmp->dsp.min = m_min;
	}	
	
	m_curchan = index;
	tmp=m_phead;
	for(i=0;i<m_curchan;i++)
		tmp=tmp->lpnext;
	
	sprintf(temp, "%f", tmp->dsp.max);
	txtMax.text = [[NSString alloc] initWithUTF8String: temp];
	sprintf(temp, "%f", tmp->dsp.min);
	txtMin.text = [[NSString alloc] initWithUTF8String: temp];
	
}

- (void)checkButtonClicked:( id )sender {
	if(m_phead == NULL){return;}
	if([checkSameAll isChecked]){
		CHANNEL *tmp=m_phead;
		
		float m_max = [txtMax.text floatValue];
		float m_min = [txtMin.text floatValue];
		
		if(m_max <= m_min)
		{
			
			UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"最小值必须小于最大值" 
														 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
			[av show];	
			return;
			
		}
		
		
		while(tmp!=NULL)
		{
			tmp->dsp.max=m_max;
			tmp->dsp.min=m_min;
			//tmp->dsp.unit=m_unit;
			//if(m_unit==0)
			//	tmp->factor=1.f;
			//else tmp->factor=m_sensitivity;
			tmp=tmp->lpnext;
		}
	}
}



- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	/*
	if(textField == txtSensSum){
		if([self CheckRangeFrom:textField.text Min:1 Max:12] == NO){
			return NO;
		}
	}else if(txtNo == textField){
		if([self CheckRangeFrom:textField.text Min:0 Max:32768] == NO){
			return NO;
		}
	}
	*/
	
	[textField resignFirstResponder];
	return YES;
}

- (BOOL) CheckRangeFrom:(NSString *)from Min : (float)min Max:(float) max {
	//NSLog(from);
	float  ws = [from floatValue];
	if(ws >= min && ws <= max){return YES;}
	else{
		NSString *msg = [NSString stringWithFormat:@"您输入的是%@ 请输入大于%d% 且小于 %d 的数字", from, min,max];
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:msg
													 delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
		[av show];
		return NO;
	}
} 

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	
	
	if(textField == txtMax || textField == txtMin || textField == txtScanTime) {
		NSCharacterSet *cs = [[NSCharacterSet characterSetWithCharactersInString:NUMBERSFLOAT] invertedSet];
		NSString *filter = [[string componentsSeparatedByCharactersInSet:cs] componentsJoinedByString:@""];
		BOOL basiTest = [string	isEqualToString:filter];
		if(!basiTest){
			UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"请输入数字" 
														 delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
			[av show];
			return NO;
		}else{
			return YES;
		}
	}
	
	/*
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
	*/
	return YES;
}



- (IBAction)btnCancel:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortrait) || (toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown));
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}


@end
