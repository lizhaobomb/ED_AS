//
//  EDAS_GN_SMSAppDelegate.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 11-12-9.
//  Copyright 2011 北京. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface EDAS_GN_SMSAppDelegate : NSObject <UIApplicationDelegate> {
    
    UIWindow *window;
    UINavigationController *navigationController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;

@end

