//
//  SetComLabViewController.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-14.
//  Copyright 2012 北京. All rights reserved.
//

#import "SetComLabViewController.h"


@implementation SetComLabViewController

- (void)viewDidLoad
{

	//CGFloat height;
	//CGSize size;

	//CGRect *p = [[UIScreen mainScreen] bounds];
	
	CGFloat wdi = self.tabBar.frame.size.width;
	CGFloat x = self.tabBar.frame.origin.x;
	CGFloat y = self.tabBar.frame.origin.y + 17;
	//UIView *view1= [[UIView alloc] initWithFrame : CGRectMake ( 0 ,0 , wdi , 32)];
	 //view1.backgroundColor = [UIColor redColor];
	//[self.tabBar insertSubview:view1 atIndex:0];
	
	self.tabBar.frame  = CGRectMake ( x ,y , wdi , 32);
	//view1.parent.height = 32;
	
	//self.tabBar = [[UITabBar alloc] initWithFrame : CGRectMake ( 0 ,320 , 320 , 32)];
	
	//---------------------------------
	SetComViewController *comView = [[SetComViewController alloc] init];
	SetComCtlViewController *comCtlView = [[SetComCtlViewController alloc] init];
	
	UITabBarItem *barItem = [[UITabBarItem alloc] initWithTitle:@"发送方式" image:nil tag:0];
	comCtlView.tabBarItem = barItem;
	[barItem release];
	
	
	NSArray *array = [NSArray arrayWithObjects:comView, comCtlView, nil];
	self.viewControllers = array;
	[comView release];
	[comCtlView release];
	[array release];
	self.title = @"设置串口信息";
	
	
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortrait) || (toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown));
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
}

@end
