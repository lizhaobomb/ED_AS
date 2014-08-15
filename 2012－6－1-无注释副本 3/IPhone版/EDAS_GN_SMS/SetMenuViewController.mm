//
//  SetMenuViewController.m
//
//  Created by guan mofeng on 11-12-15.
//  Copyright 2011 北京. All rights reserved.
//

#import "SetMenuViewController.h"
#import "Constants.h"
#import "GlobalData.h"
@implementation SetMenuViewController

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
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"台站参数", kTitleKey,
							  @"台站参数", kDetailKey,
							  @"SetStationViewController", kViewControllerNameKey,
							  nil
							  ]];
	
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"加速度计参数", kTitleKey,
							  @"加速度计参数", kDetailKey,
							  @"SetSensparViewController", kViewControllerNameKey,
							  nil
							  ]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"采样率", kTitleKey,
							  @"采样率", kDetailKey,
							  @"SetSysSmpViewController", kViewControllerNameKey,
							  nil
							  ]];
//	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
//							  @"量程", kTitleKey,
//							  @"量程", kDetailKey,
//							  @"SetGainViewController", kViewControllerNameKey,
//							  nil
//							  ]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"网络地址", kTitleKey,
							  @"网络地址", kDetailKey,
							  @"SetIPViewController", kViewControllerNameKey,
							  nil
							  ]];
//	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
//							  @"串口", kTitleKey,
//							  @"串口", kDetailKey,
//							  @"SetComLabViewController", kViewControllerNameKey,
//							  nil
//							  ]];
//	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
//							  @"脉冲标定", kTitleKey,
//							  @"设置脉冲标定", kDetailKey,
//							  @"SetPulseViewController", kViewControllerNameKey,
//							  nil
//							  ]];
//	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
//							  @"正弦标定", kTitleKey,
//							  @"设置 正弦标定", kDetailKey,
//							  @"SetSineViewController", kViewControllerNameKey,
//							  nil
//							  ]];
	
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"强震标定", kTitleKey,
							  @"设置强震标定", kDetailKey,
							  @"SetStrongViewController", kViewControllerNameKey,
							  nil
							  ]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"启动强震标定", kTitleKey,
							  @"启动强震标定", kDetailKey,
							  @"SetStartCalViewController1", kViewControllerNameKey,
							  nil
							  ]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"停止标定", kTitleKey,
							  @"停止标定", kDetailKey,
							  @"SetStopCalViewControllor", kViewControllerNameKey,
							  nil
							  ]];
	
//	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
//							  @"地震计控制", kTitleKey,
//							  @"地震计控制", kDetailKey,
//							  @"SetSenesViewController", kViewControllerNameKey,
//							  nil
//							  ]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"事件记录", kTitleKey,
							  @"事件记录", kDetailKey,
							  @"SetEvtRecViewController", kViewControllerNameKey,
							  nil
							  ]];
    
    [self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"调整零点", kTitleKey,
							  @"调整零点", kDetailKey,
							  @"SetZeroPointViewController", kViewControllerNameKey,
							  nil
							  ]];
    
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"系统时间", kTitleKey,
							  @"设置系统时间", kDetailKey,
							  @"SetSystimeViewController", kViewControllerNameKey,
							  nil
							  ]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"系统时区", kTitleKey,
							  @"设置系统时区", kDetailKey,
							  @"SetTimezoneViewController", kViewControllerNameKey,
							  nil
							  ]];
	
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"出厂设置", kTitleKey,
							  @"出厂设置", kDetailKey,
							  @"SetResetSysViewController", kViewControllerNameKey,
							  nil
							  ]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"显示参数", kTitleKey,
							  @"设置显示参数", kDetailKey,
							  @"SetDspViewController", kViewControllerNameKey,
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
	label.text = @"设置菜单";
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
	
	
	printf("select set query= %d\n", indexPath.row);
	/*
	 if(selectRowData != nil)
	 {
	 [selectRowData release];
	 selectRowData = nil;
	 }
	 */
	selectRowData = [self.menuList objectAtIndex:indexPath.row];
	UIViewController *targetViewController = [selectRowData objectForKey:kViewControllerKey];
	if (!targetViewController)
	{
        // The view controller has not been created yet, create it and set it to our menuList array
        NSString *viewControllerName = [selectRowData objectForKey:kViewControllerNameKey];
		if([viewControllerName length] == 0){
			//[self dismissModalViewControllerAnimated:YES];
            [self.navigationController popViewControllerAnimated:NO];
			selectRowData = nil;
			return;
		}
        
        if ([viewControllerName isEqualToString:@"SetStartCalViewController1"]) {
            [self btnStartStongClick:nil];
            return;
        }
		
        targetViewController = [[NSClassFromString(viewControllerName) alloc] initWithNibName:viewControllerName bundle:nil];
        [selectRowData setValue:targetViewController forKey:kViewControllerKey];
        [targetViewController release];
    }
	//[self dismissModalViewControllerAnimated:NO];
    [self.navigationController pushViewController:targetViewController animated:YES];
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

#pragma mark - IBAction
- (IBAction)btnStartStongClick:(id)sender {
    char buf[100];
	short *p=(short *)&buf[4];
	
	//if(comboSens.items == nil) {return;}
	short m_sensid = 0;
    
	
	memset(buf,0,sizeof(buf));
	
	buf[0]=0xbf;
	buf[1]=0x13;
	buf[2]=0x97;
	buf[3]=0x74;
	
	CM_STARTCALFRM * frm;
	frm=(CM_STARTCALFRM *)&buf[4];
	
	frm->head.sens_id=m_sensid;
	frm->head.cmd=0x6027;
	frm->time=-1;
	frm->head.length=8;
	frm->chk_sum=0;
	for(int i=0;i<(frm->head.length+4)/2;i++)
		frm->chk_sum-=p[i];
	
	
	if(!siteDoc.m_thd->Send(buf,frm->head.length+10))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误"
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
	}else {
		
		//UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置启动强震标定成功"
		//											 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		//[av show];
		//[self.navigationController popViewControllerAnimated:YES];
	}
	
}


@end
