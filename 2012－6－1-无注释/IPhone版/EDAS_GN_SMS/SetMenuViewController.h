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
}

@property (nonatomic, retain) NSMutableArray *menuList;

@end
