//
//  RunMenuViewController.m
//
//  Created by guan mofeng on 11-12-12.
//  Copyright 2011 北京. All rights reserved.
//

#import "RunMenuViewController.h"
#import "Constants.h"
#include "GlobalData.h"

@implementation RunMenuViewController

@synthesize menuList;

- (void)dealloc
{
	if (self.menuList != nil)
		[menuList release];
	if (menuTableView != nil)
		[menuTableView release];
	 
	[super dealloc];
}


- (void)viewDidLoad
{
	self.menuList = [NSMutableArray array];
	/*
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"运行／停止", kTitleKey,
							  @"运行／停止", kDetailKey,
							  @"LoginManage", kViewControllerNameKey,
							  nil
							  ]];
	 */
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"运行／停止", kTitleKey,
							  @"运行／停止", kDetailKey,
							  @"SiteListViewController", kViewControllerNameKey,
							  nil
							  ]];
	/*
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"存盘／停止", kTitleKey,
							  @"存盘／停止", kDetailKey,
							  @"SetStationViewController", kViewControllerNameKey,
							  nil
							  ]];
	 */
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"退出", kTitleKey,
							  @"退出", kDetailKey,
							  @"SetStationViewController", kViewControllerNameKey,
							  nil
							  ]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"取消", kTitleKey,
							  @"取消", kDetailKey,
							  @"", kViewControllerNameKey,
							  nil
							  ]];
	
	menuTableView.dataSource = self;
	menuTableView.delegate = self;
	
    [menuTableView reloadData];
	
	 
	
}




- (void)viewDidUnload
{
	
}





#pragma mark -
#pragma mark UITableViewDelegate and UITableViewDatasource methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return [menuList count];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{        
	return 1;
}	

- (NSString *)tableView:(UITableView *)aTableView titleForHeaderInSection:(NSInteger)section 
{        
	return nil;
} 

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
	UILabel *label = [[[UILabel alloc] initWithFrame:CGRectMake(0.0f,0.0f, 120.0f,20.0f)]autorelease];  
	label.backgroundColor = [UIColor clearColor];
	label.text = @"运行菜单";
	label.textColor = [UIColor grayColor];
	label.font =  [UIFont systemFontOfSize:10];
	label.textAlignment = UITextAlignmentCenter;  
	return label;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *kCellIdentifier = @"cellID";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kCellIdentifier];
	if (!cell)
	{
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:kCellIdentifier] autorelease];
        
		cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        
        cell.textLabel.backgroundColor = [UIColor clearColor];
		cell.textLabel.opaque = NO;
		cell.textLabel.textColor = [UIColor blackColor];
		cell.textLabel.highlightedTextColor = [UIColor whiteColor];
		cell.textLabel.font = [UIFont boldSystemFontOfSize:18];
		
		cell.detailTextLabel.backgroundColor = [UIColor clearColor];
		cell.detailTextLabel.opaque = NO;
		cell.detailTextLabel.textColor = [UIColor grayColor];
		cell.detailTextLabel.highlightedTextColor = [UIColor whiteColor];
		cell.detailTextLabel.font = [UIFont systemFontOfSize:14];
    }
    
	// get the view controller's info dictionary based on the indexPath's row
    NSDictionary *dataDictionary = [menuList objectAtIndex:indexPath.row];
    cell.textLabel.text = [dataDictionary valueForKey:kTitleKey];
    cell.detailTextLabel.text = [dataDictionary valueForKey:kDetailKey];
	
	return cell;
	
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {        
	return 40.0f;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {        
	
	/*
	if(selectRowData != nil)
	{
		[selectRowData release];
		selectRowData = nil;
	}
	 */
	if(indexPath.row==1)//exit
		exit(0);
	else if(indexPath.row==0 && siteDoc.IsConnected())
	{
		siteDoc.CloseCon();
		siteDoc.m_update_id=-2;
		selectRowData = nil;
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"网络连接已断开" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
		[self dismissModalViewControllerAnimated:YES];
		return;
	}
	
	selectRowData = [self.menuList objectAtIndex:indexPath.row];
	UIViewController *targetViewController = [selectRowData objectForKey:kViewControllerKey];
	if (!targetViewController)
	{
        // The view controller has not been created yet, create it and set it to our menuList array
        NSString *viewControllerName = [selectRowData objectForKey:kViewControllerNameKey];
		if([viewControllerName length] == 0){
			[self dismissModalViewControllerAnimated:YES];
			selectRowData = nil;
			return;
		} 
		
        targetViewController = [[NSClassFromString(viewControllerName) alloc] initWithNibName:viewControllerName bundle:nil];
        [selectRowData setValue:targetViewController forKey:kViewControllerKey];
        [targetViewController release];
    }
	[self dismissModalViewControllerAnimated:NO];
	tempSelectRowData = selectRowData;
	
}

- (UIViewController *) getSelectViewController {
	if(tempSelectRowData == nil){
		return nil;
	}
	UIViewController *targetViewController  = [tempSelectRowData objectForKey:kViewControllerKey];
	tempSelectRowData = nil;
	selectRowData = nil;
	return targetViewController;	
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortrait) || (toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown));
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

@end
