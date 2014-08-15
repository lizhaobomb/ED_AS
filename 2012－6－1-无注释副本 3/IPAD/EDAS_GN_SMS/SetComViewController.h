//
//  SetComViewController.h
//
//  Created by guan mofeng on 12-3-14.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UICheckButton.h"
#import "RadioGroup.h"

//#ifndef __GDATA__
//#define __GDATA__
//	#include "GlobalData.h"
//#endif


@interface SetComViewController : UIViewController {
	
	UICheckButton *radio9600;
    UICheckButton *radio19200;
    UICheckButton *radio38400;
    UICheckButton *radio57600;
	UICheckButton *radio115200;
	
	RadioGroup *radioGroup;
	/*
	int	m_baud;
	char m_cmd[128];
	int m_cmd_len;
	*/
	 int m_samp;
}
- (IBAction)btnCancelClick:(id)sender;
- (IBAction)btnOKClick:(id)sender;

@end
