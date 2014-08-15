//
//  SiteListViewController.h
//
//  Created by guan mofeng on 12-1-12.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface SiteListViewController : UIViewController<UITableViewDelegate,UITableViewDataSource, UITextFieldDelegate> {
	IBOutlet UISegmentedControl *btnSegment;
    IBOutlet UITableView *tableSite;
    IBOutlet UITextField *txtAddress;
    IBOutlet UITextField *txtChNum;
    IBOutlet UITextField *txtName;
    IBOutlet UITextField *txtPassword;
    IBOutlet UITextField *txtPort;
    IBOutlet UITextField *txtUser;
	int selectPos;
	NSTimer * m_timer;
	UIAlertView *baseAlert;
	uint m_ret; //1-connect 2-close
	BOOL m_benable_connect; //1-enable connect button at initialize
	BOOL m_isconnected; //1-正处于连接状态
	float currConnetTime;
}
- (IBAction)btnOnClick:(id)sender;

-(void) addSite;
-(void) editSite;
-(void) deleteSite;
-(void) connectSite;
-(void) quitSite;
-(BOOL) saveSiteinfo;
-(BOOL) isValidator;

@end
