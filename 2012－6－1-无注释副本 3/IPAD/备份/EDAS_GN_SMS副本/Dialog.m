//
//  Dialog.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-1-12.
//  Copyright 2012 北京. All rights reserved.
//

#import "Dialog.h"


@implementation Dialog


+ (void)showMsg :(NSString *) title Content : (NSString *) content {
    UIAlertView *av = [[[UIAlertView alloc] initWithTitle:title message:content 
												 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
	[av show];

}


@end
