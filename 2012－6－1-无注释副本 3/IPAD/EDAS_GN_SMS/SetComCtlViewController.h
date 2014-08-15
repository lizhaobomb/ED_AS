//
//  SetComCtlViewController.h
//
//  Created by guan mofeng on 12-3-14.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UICheckButton.h"
#import "RadioGroup.h"

@interface SetComCtlViewController : UIViewController {
	
	/*
	 BOOL	m_back;
	 BOOL	m_binit;
	 BOOL	m_bestat;
	 BOOL	m_bebroad;
	 */
	
	UICheckButton *checkBack;
    UICheckButton *checkBinit;
    UICheckButton *checkBestat;
    UICheckButton *checkBebroad;
}
- (IBAction)btnCancelClick:(id)sender;
- (IBAction)btnOKClick:(id)sender;
@end
