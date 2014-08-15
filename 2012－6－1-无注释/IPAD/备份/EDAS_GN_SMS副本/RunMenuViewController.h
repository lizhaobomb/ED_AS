//
//  RunMenuViewController.h
//
//  Created by guan mofeng on 11-12-12.
//  Copyright 2011 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface RunMenuViewController : UIViewController <UITableViewDelegate,UITableViewDataSource>{
   
	IBOutlet UITableView *menuTableView;
	NSMutableArray *menuList;
	NSMutableDictionary *selectRowData;
	NSMutableDictionary *tempSelectRowData;
}

@property (nonatomic, retain) NSMutableArray *menuList;


@end
