//
//  SetSenesViewController.h
//
//  Created by guan mofeng on 12-3-14.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UIComboBox.h"

@interface SetSenesViewController : UIViewController {
	UIComboBox *comboSens;
}
- (IBAction)onCmgSenesonClick:(id)sender;
- (IBAction)onCmgSensoffClick:(id)sender;
- (IBAction)onSensCtloffClcik:(id)sender;
- (IBAction)onSensCtlonClick:(id)sender;
@end
