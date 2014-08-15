//
//  SetIPViewController.h
//
//  Created by guan mofeng on 12-3-13.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#include "GlobalData.h"

@interface SetIPViewController : UIViewController<UITextFieldDelegate> {
    IBOutlet UITextField *txtGwCtl;
    IBOutlet UITextField *txtIPCtl;
    IBOutlet UITextField *txtMaskCtl;
}
- (IBAction)btnCancelClick:(id)sender;
- (IBAction)btnOkClick:(id)sender;
@end
