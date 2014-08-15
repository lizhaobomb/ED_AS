//
//  SineTable.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-31.
//  Copyright 2012 北京. All rights reserved.
//

#import "SineTable.h"

SINES m_sine[100];

@implementation SineTable


- (id)initWithFrame:(CGRect)frame {
    
    self = [super initWithFrame:frame];
    if (self) {
		self.delegate = self;
		self.dataSource = self;
    }
    return self;
}



#pragma mark -
#pragma mark UITableViewDelegate and UITableViewDatasource methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	//return [menuList count];
	
	return 100;
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
	/*
	UILabel *label = [[[UILabel alloc] initWithFrame:CGRectMake(0.0f,0.0f, 120.0f,20.0f)]autorelease];  
	label.backgroundColor = [UIColor clearColor];
	label.text = @"运行菜单";
	label.textColor = [UIColor grayColor];
	label.font =  [UIFont systemFontOfSize:10];
	label.textAlignment = UITextAlignmentCenter;  
	return label;
	 */
	
	return nil;
	
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString *kCellIdentifier = @"cellID";
	
	SineTableCell *cell = (SineTableCell *)[tableView dequeueReusableCellWithIdentifier:kCellIdentifier];
	if (!cell)
	{
		cell = [[[SineTableCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:kCellIdentifier] autorelease];
		//cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        
		/*
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
		 */
    }
    
	int index = indexPath.row;
	char temp[1024];
	
	cell.rowIndex = index;
	sprintf(temp, "%d",index + 1);
	cell.labID.text = [[NSString alloc]initWithUTF8String: temp];

	
	sprintf(temp, "%d", m_sine[index].cycle);
	cell.txtCycle.text = [[NSString alloc]initWithUTF8String: temp];
	
	sprintf(temp, "%d", m_sine[index].aten);
	cell.txtAten.text = [[NSString alloc]initWithUTF8String: temp];
	
	sprintf(temp, "%.2f", m_sine[index].prd * 0.1f);
	cell.txtPrd.text = [[NSString alloc]initWithUTF8String: temp];
	
	return cell;
	
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {        
	return 34.0f;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {        
	
	/*
	if(indexPath.row==0 && siteDoc.IsConnected())
	{
		siteDoc.CloseCon();
		siteDoc.m_update_id=-2;
		selectRowData = nil;
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"网络连接已断开" 
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
		[self.parentViewController dismissModalViewControllerAnimated:YES];
		return;
	}
	
	selectRowData = [self.menuList objectAtIndex:indexPath.row];
	UIViewController *targetViewController = [selectRowData objectForKey:kViewControllerKey];
	if (!targetViewController)
	{
        // The view controller has not been created yet, create it and set it to our menuList array
        NSString *viewControllerName = [selectRowData objectForKey:kViewControllerNameKey];
		if([viewControllerName length] == 0){
			[self.parentViewController dismissModalViewControllerAnimated:YES];
			selectRowData = nil;
			CFRunLoopStop(currentLoop);
			
			return;
		} 
		
        targetViewController = [[NSClassFromString(viewControllerName) alloc] initWithNibName:viewControllerName bundle:nil];
        [selectRowData setValue:targetViewController forKey:kViewControllerKey];
        [targetViewController release];
    }
	[self.parentViewController dismissModalViewControllerAnimated:NO];
	CFRunLoopStop(currentLoop);
	*/
}



/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code.
}
*/

- (void)dealloc {
    [super dealloc];
}


@end
