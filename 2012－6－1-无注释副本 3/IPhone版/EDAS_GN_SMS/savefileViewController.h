//
//  savefileViewController.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-2-14.
//  Copyright 2012 北京. All rights reserved.
//

//#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface savefileViewController : UIViewController {
	UILabel * label;
	NSString * message;
}
@property (nonatomic, retain) IBOutlet UILabel *  label;
@property (nonatomic, retain) NSString *message;
@end
