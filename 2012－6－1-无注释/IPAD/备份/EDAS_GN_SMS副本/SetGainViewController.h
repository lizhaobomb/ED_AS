//
//  SetGainViewController.h
//
//  Created by guan mofeng on 12-3-12.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UICheckButton.h"
#import "RadioGroup.h"

#import "UIComboBox.h"

@interface SetGainViewController : UIViewController <UIComboBoxDelegate>{
    IBOutlet UIButton *btnCancel;
    IBOutlet UIButton *btnOK;
    UICheckButton *radioV10;
    UICheckButton *radioV20;
    UICheckButton *radioV2_5;
    UICheckButton *radioV5;
	
	RadioGroup *radioGroup;
	UIComboBox *comboSens;
	
}
- (IBAction)onClickCancel:(id)sender;
- (IBAction)onClickOk:(id)sender;
@end
