//
//  SetSysSmpViewController.h
//
//  Created by guan mofeng on 12-3-13.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UIComboBox.h"
#import "UICheckButton.h"
#import "RadioGroup.h"
#import "UIComboBox.h"

@interface SetSysSmpViewController : UIViewController<UIComboBoxDelegate>{
	UICheckButton *radioLine;
    UICheckButton *radioMin;
	RadioGroup *radioGroup;
	UIComboBox *comboSamp;
	UIComboBox *comboSens;
}
- (IBAction)btnCancelClick:(id)sender;
- (IBAction)btnOkClick:(id)sender;
@end
