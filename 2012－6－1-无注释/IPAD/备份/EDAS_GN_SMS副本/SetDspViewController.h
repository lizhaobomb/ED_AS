//
//  SetDspViewController.h
//
//  Created by guan mofeng on 12-3-27.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "DrawwaveView.h"
#import "UIComboBox.h"
#include "GlobalData.h"
#import "UICheckButton.h"

@interface SetDspViewController : UIViewController <UITextFieldDelegate, UIComboBoxDelegate>{
    IBOutlet UITextField *txtMax;
    IBOutlet UITextField *txtMin;
    IBOutlet UITextField *txtScanTime;
	UIComboBox *cmboChannel; 
	UICheckButton *checkSameAll;
	
	CHANNEL *m_phead;	
	int m_curchan;
	DASCMDSTRUC * m_das;
	DrawwaveView * dv;
}
- (IBAction)btnCancel:(id)sender;
- (IBAction)btnOK:(id)sender;
@end
