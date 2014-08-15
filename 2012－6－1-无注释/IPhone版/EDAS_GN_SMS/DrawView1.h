//
//  DrawView.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-1-12.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface DrawView : UIView {
	
	UIButton * showButton;
	UIView * drawView;
	CGContextRef context;
	
	
}

-(void)initSizeX:(int)x Y:(int)y Width: (int)width Heigth:(int)heigth;  

-(void)initDraw;

@end
