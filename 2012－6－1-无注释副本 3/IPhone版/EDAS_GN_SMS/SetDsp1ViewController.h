//
//  SetDsp1ViewController.h
//
//  Created by guan mofeng on 12-5-21.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DrawwaveView.h"
//#include "GlobalData.h"
#import <Foundation/Foundation.h>

//#include "sitemagstruc.h"

@interface SetDsp1ViewController : UIViewController <UITextFieldDelegate>{
    IBOutlet UILabel *labChannel;
    IBOutlet UITextField *txtMax;
    IBOutlet UITextField *txtMin;
	DrawwaveView *dv;
	NSString *lableText;

}
- (IBAction)btnCancel:(id)sender;
- (IBAction)btnOK:(id)sender;
- (void) initWithShow:(NSString *) showLabText;

@end
