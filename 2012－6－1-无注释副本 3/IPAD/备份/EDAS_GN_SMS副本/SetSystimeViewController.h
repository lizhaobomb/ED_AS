//
//  SetSystimeViewController.h
//
//  Created by guan mofeng on 12-3-15.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface SetSystimeViewController : UIViewController {
    IBOutlet UIButton *btnCancel;
    IBOutlet UIButton *btnOK;
    IBOutlet UIDatePicker *dataPicker;
}
- (IBAction)btnCancelClick:(id)sender;
- (IBAction)btnOKClick:(id)sender;
@end
