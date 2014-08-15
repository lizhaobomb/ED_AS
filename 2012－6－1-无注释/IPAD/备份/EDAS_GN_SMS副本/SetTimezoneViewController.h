//
//  SetTimezoneViewController.h
//
//  Created by guan mofeng on 12-3-15.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface SetTimezoneViewController : UIViewController <UITableViewDelegate,UITableViewDataSource> {
    IBOutlet UIButton *btnCancel;
    IBOutlet UIButton *btnOK;
    IBOutlet UITableView *tbView;
	
	NSMutableArray *menuList;
	NSMutableDictionary *selectRowData;
}
@property (nonatomic, retain) NSMutableArray *menuList;

- (IBAction)btnCancelClick:(id)sender;
- (IBAction)btnOKClick:(id)sender;
@end
