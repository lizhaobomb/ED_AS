//
//  DrawView.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-1-12.
//  Copyright 2012 北京. All rights reserved.
//

#import "DrawView.h"
	

@implementation DrawView

-(void)initDraw {
	context = UIGraphicsGetCurrentContext();
}


- (id)initWithFrame:(CGRect)frame {
    
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code.
    }
    return self;	
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

-(void)initSizeX:(int)x Y:(int)y Width: (int)width Heigth:(int)heigth {
	
	showButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	showButton.frame = CGRectMake(10,10,20, 30);
	showButton.titleLabel.text = @"testV";
	[self addSubview:showButton];
	
	//CGRect p = [self frame];
}
@end
