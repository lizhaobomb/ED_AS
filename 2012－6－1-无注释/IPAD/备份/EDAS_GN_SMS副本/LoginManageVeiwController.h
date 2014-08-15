//
//  LoginManageVeiwController.h
//
//  Created by guan mofeng on 11-12-26.
//  Copyright 2011 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface LoginManage : UIViewController<UITableViewDelegate,UITableViewDataSource> {
	
    IBOutlet UISegmentedControl *btnSegment;
    IBOutlet UITextField *txtAddress;
    IBOutlet UITextField *txtName;
    IBOutlet UITextField *txtPassword;
    IBOutlet UITextField *txtPort;
    IBOutlet UITextField *txtUser;
    IBOutlet UITableView *tableList;
}
- (IBAction)btnSegment:(id)sender;
@end
