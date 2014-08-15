//
//  LoginManageVeiwController.m
//
//  Created by guan mofeng on 11-12-26.
//  Copyright 2011 北京. All rights reserved.
//

#import "LoginManageVeiwController.h"
#include "GlobalData.h"

@implementation LoginManage

- (IBAction)btnSegment:(id)sender {
    if([btnSegment selectedSegmentIndex] == 0){
		printf("zengjia\n");
	}
	
}


- (void)viewDidLoad
{
		
	tableList.dataSource = self;
	tableList.delegate = self;
	
    [tableList reloadData];
	
}


#pragma mark -
#pragma mark UITableViewDelegate and UITableViewDatasource methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return siteDoc.m_sitelst.size();
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
	label.text = @"台站列表";
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
    
	char * uu = siteDoc.m_sitelst.at(indexPath.row)->m_site.site;
	// get the view controller's info dictionary based on the indexPath's row
	 //NSDictionary *dataDictionary = [menuList objectAtIndex:indexPath.row];
    //cell.textLabel.text = [dataDictionary valueForKey:kTitleKey];
	cell.textLabel.text = [[NSString alloc] initWithUTF8String:uu]; 
	//txtAddress.text = 
	
	return cell;
	
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {        
	return 40.0f;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {        
	
	
	//selectRowData = [self.menuList objectAtIndex:indexPath.row];
		
}


- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	return TRUE;
}




@end
