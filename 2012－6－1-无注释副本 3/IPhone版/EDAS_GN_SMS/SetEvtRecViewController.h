//
//  SetEvtRecViewController.h
//  EDAS_GN_SMS
//
//  Created by 李 召 on 14-6-12.
//
//

#import <UIKit/UIKit.h>
#include "GlobalData.h"
#import "UICheckButton.h"
@interface SetEvtRecViewController : UIViewController{
    UICheckButton* checkEvtRec;
}
@property (retain, nonatomic) IBOutlet UITextField *pre_evt;
@property (retain, nonatomic) IBOutlet UITextField *after_evt;

@end
