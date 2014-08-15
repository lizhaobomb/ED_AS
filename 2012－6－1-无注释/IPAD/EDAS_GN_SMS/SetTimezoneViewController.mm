//
//  SetTimezoneViewController.m
//
//  Created by guan mofeng on 12-3-15.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetTimezoneViewController.h"
#include "GlobalData.h"

@implementation SetTimezoneViewController
@synthesize menuList;

- (void)viewDidLoad
{
	self.title = @"设置系统时区";
	self.menuList = [NSMutableArray array];
	
	/*
	 
	 NZDT GMT+13 努库阿洛法
	 NZT GMT+12 西伯利亚，新西兰，斐济群岛，堪察加半岛，马绍尔群岛
	 AESST GMT+11 库页岛，马加丹，索罗门群岛
	 AEST GMT+10 堪培拉，悉尼，墨尔本,布里斯班
	 AEST GMT+10 塔斯马尼亚岛，新几内亚，符拉迪沃斯托克
	 JST GMT+9 东京、大阪、札幌，首尔，平壤，雅库茨克
	 CST GMT+8 北京，重庆，香港特别行政区，乌鲁木齐
	 CCT GMT+8 吉隆波，新加波，台北
	 WST GMT+7 曼谷、河内、雅加达、克拉斯诺亚尔斯克
	 ALMT GMT+6 仰光、新西伯利亚、阿斯塔纳、达卡
	 MVT GMT+5 叶卡捷琳堡、伊斯兰堡、卡拉奇、塔什干
	 IOT GMT+5 孟买、加德满都、新德里、加尔各达
	 MUT GMT+4 阿布扎比、马斯喀特、巴库、第比利斯、埃里温
	 EAT GMT+3 莫斯科、圣彼得堡
	 BT GMT+3 巴格达、科威特、内罗毕、德黑兰
	 CEST GMT+2 布加勒斯特、开罗、雅典、赫尔辛基
	 BDST GMT+2 耶鲁撒冷、伊斯坦布尔
	 EET GMT+1 中欧、中非西部
	 UTC GMT 格林威治平时、西欧
	 WAT GMT-1 亚速尔群岛，佛得角群岛
	 NDT GMT-2 中大西洋
	 AWT GMT-3 布宜诺斯艾利斯、乔治敦、格陵兰、巴西利亚、纽芬兰
	 AST GMT-4 大西洋时间（加拿大）、加拉加斯、拉巴斯、圣地亚哥
	 EST GMT-5 印第安那（东）、东部时间（美国和加拿大）
	 CDT GMT-5 波哥大、利马
	 MDT GMT-6 中部时间（美国和加拿大）
	 MDT GMT-6 中美洲、萨斯喀彻温、墨西哥城
	 MST GMT-7 亚利桑那、山地时间（美国和加拿大）
	 PST GMT-8 太平洋时间（美国和加拿大）、蒂华纳
	 HDT GMT-9 阿拉斯加
	 HST GMT-10 夏威夷
	 NT GMT-11 中途岛、萨摩亚群岛
	 IDLW GMT-12 埃尼威托克、夸贾林岛
	 */
	/*
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							  @"NZDT GMT+13 努库阿洛法", @"zoneShowKey",
							  13, @"zoneValue",
							  nil];
	 */
	
	
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"NZT GMT+12 西伯利亚，新西兰，斐济群岛，堪察加半岛，马绍尔群岛", @"zoneShowKey",
							   [[ NSNumber alloc ] initWithInt:12 ], @"offsetValue",
							     @"NZT", @"zoneValue",
							   nil]];
	 
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"AESST GMT+11 库页岛，马加丹，索罗门群岛",  @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:11 ], @"offsetValue",
							  @"AESST", @"zoneValue",
							   nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"AEST GMT+10 堪培拉，悉尼，墨尔本,布里斯班", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:10 ],@"offsetValue",
							  @"AESST", @"zoneValue",
							   nil]];  
	 
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"AEST GMT+10 塔斯马尼亚岛，新几内亚，符拉迪沃斯托克", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:10 ], @"offsetValue",
							  @"AESST", @"zoneValue",
							   nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
								@" JST GMT+9 东京、大阪、札幌，首尔，平壤，雅库茨克", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:9 ],@"offsetValue",
							  @"JST", @"zoneValue",
								nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"CST GMT+8 北京，重庆，香港特别行政区，乌鲁木齐", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:8 ], @"offsetValue",
							  @"CST", @"zoneValue",
							   nil]]; 
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"CCT GMT+8 吉隆波，新加波，台北", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:8 ], @"offsetValue",
							  @"CCT", @"zoneValue",
							  nil]];
	
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"WST GMT+7 曼谷、河内、雅加达、克拉斯诺亚尔斯克", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:7 ], @"offsetValue",
							  @"WST", @"zoneValue",
							nil]];
	
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"ALMT GMT+6 仰光、新西伯利亚、阿斯塔纳、达卡", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:6 ], @"offsetValue",
							  @"ALMT", @"zoneValue",
							nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"MVT GMT+5 叶卡捷琳堡、伊斯兰堡、卡拉奇、塔什干", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:5 ],  @"offsetValue",
							  @"MVT", @"zoneValue",
							nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"IOT GMT+5 孟买、加德满都、新德里、加尔各达", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:5 ], @"offsetValue",
							  @"IOT", @"zoneValue",
							nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"MUT GMT+4 阿布扎比、马斯喀特、巴库、第比利斯、埃里温", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:4 ],  @"offsetValue",
							  @"MUT", @"zoneValue",
							nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"EAT GMT+3 莫斯科、圣彼得堡", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:3 ],  @"offsetValue",
							  @"EAT", @"zoneValue",
							nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"BT GMT+3 巴格达、科威特、内罗毕、德黑兰", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:3 ],  @"offsetValue",
							  @"BT", @"zoneValue",
							nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"CEST GMT+2 布加勒斯特、开罗、雅典、赫尔辛基", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:2 ],  @"offsetValue",
							  @"CEST", @"zoneValue",
							nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"BDST GMT+2 耶鲁撒冷、伊斯坦布尔", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:2 ],  @"offsetValue",
							  @"BDST", @"zoneValue",
							nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							@"EET GMT+1 中欧、中非西部", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:1 ],  @"offsetValue",
							  @"EET", @"zoneValue",
							nil]];
	
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							 @"UTC GMT 格林威治平时、西欧", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:0 ], @"offsetValue",
							  @"UTC", @"zoneValue",
							 nil]];
    [self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"WAT GMT-1 亚速尔群岛，佛得角群岛", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-1 ], @"offsetValue",
							  @"WAT", @"zoneValue",
							   nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"NDT GMT-2 中大西洋", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-2 ],  @"offsetValue",
							  @"NDT", @"zoneValue",
							   nil]]; 
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
								@"AWT GMT-3 布宜诺斯艾利斯、乔治敦、格陵兰、巴西利亚、纽芬兰", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-3 ], @"offsetValue",
							  @"AWT", @"zoneValue",
								nil]]; 
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"AST GMT-4 大西洋时间（加拿大）、加拉加斯、拉巴斯、圣地亚哥", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-4 ], @"offsetValue",
							  @"AST", @"zoneValue",
							   nil]];  
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"EST GMT-5 印第安那（东）、东部时间（美国和加拿大）", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-4 ], @"offsetValue",
							  @"EST", @"zoneValue",
							   nil]]; 
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"CDT GMT-5 波哥大、利马", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-5 ],@"offsetValue",
							  @"CDT", @"zoneValue",
							   nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"MDT GMT-6 中部时间（美国和加拿大）", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-6 ], @"offsetValue",
							  @"MDT", @"zoneValue",
							   nil]];
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"MST GMT-7 亚利桑那、山地时间（美国和加拿大", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-7 ], @"offsetValue",
							  @"MST", @"zoneValue",
							   nil]];  
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"PST GMT-8 太平洋时间（美国和加拿大）、蒂华纳", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-8 ], @"offsetValue",
							  @"PST", @"zoneValue",
							   nil]];  
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"HDT GMT-9 阿拉斯加", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-9 ],  @"offsetValue",
							  @"HDT", @"zoneValue",
							   nil]];  
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"HST GMT-10 夏威夷", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-10 ], @"offsetValue",
							  @"HST", @"zoneValue",
							   nil]]; 
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"NT GMT-11 中途岛、萨摩亚群岛", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-11 ], @"offsetValue",
							  @"NT", @"zoneValue",
							   nil]]; 
	[self.menuList addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:
							   @"IDLW GMT-12 埃尼威托克、夸贾林岛", @"zoneShowKey",
							  [[ NSNumber alloc ] initWithInt:-12 ], @"offsetValue",
							  @"IDLW", @"zoneValue",
							   nil]]; 
	
	
	
	tbView.dataSource = self;
	tbView.delegate = self;
	
    [tbView reloadData];
	
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
		label.text = @"时区";
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
			 cell.textLabel.font = [UIFont boldSystemFontOfSize:14];
		 }
		 
		 // get the view controller's info dictionary based on the indexPath's row
		 NSDictionary *dataDictionary = [menuList objectAtIndex:indexPath.row];
		 cell.textLabel.text = [dataDictionary valueForKey:@"zoneShowKey"];
		 
		 return cell;
		 
	 }
	 
	 - (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {        
		 return 40.0f;
	 }
	 
	 - (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {        
		 
		 selectRowData = [self.menuList objectAtIndex:indexPath.row];
	 }
	 
	

- (IBAction)btnOKClick:(id)sender {
    if(selectRowData == nil) {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"Erro" message:@"未选择时区"
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];	
		
	}else {
		
		char m_cmd[1024];
		int m_cmd_len;
		CM_TIMEZONEFRM frm;
		
		
		//[selectRowData valueForKey:@"zoneValue"];
		
		short m_zoff = (short)[[selectRowData valueForKey:@"offsetValue"] intValue];
		m_zoff =m_zoff * -1;
		
		m_cmd[0]=0xbf;
		m_cmd[1]=0x13;
		m_cmd[2]=0x97;
		m_cmd[3]=0x74;
		
		memset(&frm,0,sizeof(frm));
		
		frm.head.cmd=0x6075;
		frm.head.sens_id=0;
		frm.head.length=16;
		
		sprintf(frm.zone,"%s",[[selectRowData valueForKey:@"zoneValue"] UTF8String]);
		//wsprintf(frm.zone,"%s",m_zlist[m_zone]);
		frm.offset=m_zoff;
		
		short *p=(short *)&frm;
		frm.chk_sum=0;
		
		for(int i=0;i<(frm.head.length+6)/2-1;i++)
			frm.chk_sum-=p[i];
		
		memcpy(&m_cmd[4],(char *)&frm,frm.head.length+6);
		m_cmd_len=frm.head.length+10;
		
		if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
		{	//SendErr();
			UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误" 
														 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
			[av show];	
		}else {
			UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置系统时区成功" 
														 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
			[av show];	
		}
		
	}
	
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortrait) || (toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown));
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

- (IBAction)btnCancelClick:(id)sender {
	[self.navigationController popViewControllerAnimated:YES];
}

@end
