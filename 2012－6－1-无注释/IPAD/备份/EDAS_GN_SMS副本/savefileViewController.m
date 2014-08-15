//
//  savefileViewController.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-2-14.
//  Copyright 2012 北京. All rights reserved.
//

#import "savefileViewController.h"


@implementation savefileViewController

@synthesize label;
@synthesize message;

- (void)viewWillAppear:(BOOL)animated{
	message = @"存盘／停止";
	label.text = message;
	[super viewWillAppear:<#(BOOL)animated#>];
}

-(void)viewDidUnload{
	self.label=nil;
	self.message=nil;
	[super viewDidUnload];
}

-(void)dealloc{
	[label release];
	[message release];
	[super dealloc];
}

@end
