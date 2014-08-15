//
//  SineTableCell.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-31.
//  Copyright 2012 北京. All rights reserved.
//

#import "SineTableCell.h"
#include "GlobalData.h"
#define NUMBERSINT @"0123456789\n"
#define NUMBERSFLOAT @"-0123456789.\n"


extern SINES m_sine[];

@implementation SineTableCell

/*
UITextField *txtCycle;
UITextField *txtPrd;
UITextField *txtAten;
*/

@synthesize txtCycle;
@synthesize txtPrd;
@synthesize txtAten;
@synthesize rowIndex;
@synthesize labID;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code.
		 
		labID = [[UILabel alloc] initWithFrame : CGRectMake ( 0 , 2 , 40 , 30 )];
		labID.text = @"";
		[self addSubview:labID];
		
		txtCycle = [[UITextField alloc] initWithFrame:CGRectMake(40, 2, 90, 30)];
		txtCycle.borderStyle =  UITextBorderStyleRoundedRect;
		txtCycle.delegate =self;
		[self addSubview:txtCycle];
		
		txtPrd = [[UITextField alloc] initWithFrame:CGRectMake(135, 2, 80, 30)];
		txtPrd.borderStyle =  UITextBorderStyleRoundedRect;
		txtPrd.delegate =self;
		[self addSubview:txtPrd];
		
		txtAten = [[UITextField alloc] initWithFrame:CGRectMake(220, 2, 80, 30)];
		txtAten.borderStyle =  UITextBorderStyleRoundedRect;
		txtAten.delegate =self;
		[self addSubview:txtAten];
    }
    return self;
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    
    [super setSelected:selected animated:animated];
    
    // Configure the view for the selected state.
}




- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{	
	if(textField == txtCycle || textField == txtAten) {
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
		
		
	}else if(textField == txtPrd) {
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

- (void)textFieldDidEndEditing:(UITextField *)textField
{
	if(textField == txtCycle) {
		m_sine[rowIndex].cycle = [txtCycle.text intValue];
		//printf("textFieldDidEndEditing %d cycle= %d\n",rowIndex, m_sine[rowIndex].cycle );
	}
	else if(textField == txtAten)
	{
		m_sine[rowIndex].aten = [txtAten.text intValue];
		//printf("textFieldDidEndEditing %d aten= %d\n", rowIndex, m_sine[rowIndex].aten );
		
	}else if(textField == txtPrd)
	{
		float tt = [txtPrd.text floatValue] * 10;
		m_sine[rowIndex].prd = (short) tt; 
		//printf("textFieldDidEndEditing %d prd= %d\n", rowIndex, m_sine[rowIndex].prd );
	}
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	
	
	[textField resignFirstResponder];
	return YES;
}


- (void)dealloc {
    [super dealloc];
	[txtCycle release];
	[txtPrd release];
	[txtAten release];
}


@end
