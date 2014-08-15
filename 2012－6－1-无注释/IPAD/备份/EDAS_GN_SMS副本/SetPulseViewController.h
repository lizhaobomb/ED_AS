//
//  SetPulseViewController.h
//
//  Created by guan mofeng on 12-3-29.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UIComboBox.h"
#import "UICheckButton.h"

@interface SetPulseViewController : UIViewController <UITextFieldDelegate,UIComboBoxDelegate> {
    IBOutlet UIScrollView *scrollView;
	UIComboBox *cmboSens; 
	UICheckButton *ckBTimer;
	UIComboBox *cmbMethod;
	UIComboBox *cmboMon;
	UIComboBox *cmboDay;
	UIComboBox *cmboHour;
	UIComboBox *cmboMin;
	UIComboBox *cmboAmpType;
	UILabel *labDay;
	UILabel *labUnit;
	
	UITextField *txtAmp;
	UITextField *txtDur;
	
}
- (IBAction)btnCancelClick:(id)sender;
- (IBAction)btnOKClick:(id)sender;
@end
