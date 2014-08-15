//
//  SetDsp1ViewController.m
//
//  Created by guan mofeng on 12-5-21.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetDsp1ViewController.h"
#import "RootViewController.h"
#include "GlobalData.h"

#define NUMBERSINT @"0123456789\n"
#define NUMBERSFLOAT @"-0123456789.\n"
CHANNEL *tmpCHANNEL;

@implementation SetDsp1ViewController

- (void) initWithShow:(NSString *) showLabText {
	lableText = showLabText;
}
- (void)viewDidLoad
{
	//if(siteDoc.m_das == NULL){ return;}
	txtMax.delegate = self;
	txtMin.delegate = self;
	dv =  [RootViewController U_drawview];
		
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
		
	
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
	tmpCHANNEL = NULL;
	char buffer[30];
	char temp[1024];
	sprintf(buffer,"%s",[lableText UTF8String]);
	
	labChannel.text = lableText;
	
	CHANNEL *tmp=siteDoc.m_pchannels;	
	while(tmp!=NULL)
	{
		if(strcmp(buffer, tmp->chn_label) == 0) {
			tmpCHANNEL = tmp;
			sprintf(temp, "%f", tmp->dsp.max);
			txtMax.text = [[NSString alloc] initWithUTF8String: temp];
			sprintf(temp, "%f", tmp->dsp.min);
			txtMin.text = [[NSString alloc] initWithUTF8String: temp];
			break;
		}
		tmp=tmp->lpnext;
	}
		
}



- (IBAction)btnOK:(id)sender {
    int m_max = [txtMax.text intValue];
	int m_min = [txtMin.text intValue];
	
	if(m_max <= m_min)
	{
		
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"最小值必须小于最大值" 
													 delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
		[av show];	
		
		return;
		
	}else {
		if(tmpCHANNEL != NULL) {
			
			//int m_max = [txtMax.text intValue];
			//int m_min = [txtMin.text intValue];
			tmpCHANNEL->dsp.max = m_max;
			tmpCHANNEL->dsp.min= m_min;
			/*
			char temp[1024];
			sprintf(temp, "%f", tmpCHANNEL->dsp.max);
			txtMax.text = [[NSString alloc] initWithUTF8String: temp];
			sprintf(temp, "%f", tmpCHANNEL->dsp.min);
			txtMin.text = [[NSString alloc] initWithUTF8String: temp];
			 */
		}
		[dv ReDraw];
		[self.navigationController popViewControllerAnimated:YES];
		
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
	
	
	if(textField == txtMax || textField == txtMin) {
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
