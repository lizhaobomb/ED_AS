//
//  SetMenuViewController.h
//
//  Created by guan mofeng on 11-12-15.
//  Copyright 2011 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface SetMenuViewController : UIViewController <UITableViewDelegate,UITableViewDataSource>{
    IBOutlet UITableView *menuTableView;
	NSMutableArray *menuList;
	NSMutableDictionary *selectRowData;
	NSMutableDictionary *tempSelectRowData;
    
    IBOutlet UITextField* recordBefore;
    IBOutlet UITextField* recordAfter;
    IBOutlet UISwitch* recordSwt;
    IBOutlet UIView* eventRecordView;
}

@property (nonatomic, retain) NSMutableArray *menuList;

@end
