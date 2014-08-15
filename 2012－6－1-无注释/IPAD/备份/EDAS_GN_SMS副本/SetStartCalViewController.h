//
//  SetStartCalViewController.h
//
//  Created by guan mofeng on 12-3-13.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UIComboBox.h"


@interface SetStartCalViewController : UIViewController {
    IBOutlet UIButton *btnCancel;
    IBOutlet UIButton *btnStartPseudo;
    IBOutlet UIButton *btnStartPulse;
    IBOutlet UIButton *btnStartSine;
    IBOutlet UIButton *btnStartStrong;
	
	UIComboBox *comboSens;
}
- (IBAction)btnCancelClick:(id)sender;
- (IBAction)btnStartPseudoClick:(id)sender;
- (IBAction)btnStartPulseClick:(id)sender;
- (IBAction)btnStartSineClick:(id)sender;
- (IBAction)btnStartStongClick:(id)sender;
@end
