//
//  UICheckButton.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-12.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

typedef enum {
    CheckButtonStyleDefault = 0 ,
    CheckButtonStyleBox = 1 ,
    CheckButtonStyleRadio = 2
} CheckButtonStyle;



@interface UICheckButton : UIControl {

	UILabel * label ;
	UIImageView * icon ;
	BOOL checked ;
	id value , delegate ;
	CheckButtonStyle style ;
	NSString * checkname ,* uncheckname ; // 勾选／反选时的图片文件名
	
}

@property ( retain , nonatomic ) id value,delegate;
@property ( retain , nonatomic )UILabel* label;
@property ( retain , nonatomic )UIImageView* icon;
@property ( assign )CheckButtonStyle style;
-( CheckButtonStyle )style;
-( void )setStyle:( CheckButtonStyle )st;
-( BOOL )isChecked;
-( void )setChecked:( BOOL )b;
@end

