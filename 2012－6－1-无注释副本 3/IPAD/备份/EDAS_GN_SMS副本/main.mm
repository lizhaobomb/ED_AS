//
//  main.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 11-12-9.
//  Copyright 2011 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "GlobalData.h"
#include "SineCal.h"

int main(int argc, char *argv[]) {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentPath = [paths objectAtIndex:0];
	
	//char *p = 	//theApp.InitInstance();

	
	char temp[1000];
	sprintf(temp,"%s", [documentPath UTF8String]);
	printf("path=%s \n",  temp);
	theApp.InitInstance(temp);
	siteDoc.OnNewDocument();
	initSaveFeqToFile(temp);
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
