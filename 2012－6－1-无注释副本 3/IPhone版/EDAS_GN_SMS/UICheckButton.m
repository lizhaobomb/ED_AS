//
//  UICheckButton.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-12.
//  Copyright 2012 北京. All rights reserved.
//

#import "UICheckButton.h"


@implementation UICheckButton
@synthesize label,icon,value,delegate;


- (id)initWithFrame:(CGRect)frame {
    
	if ( self =[ super initWithFrame : frame ]) {
		//icon =[[ UIImageView alloc ] initWithFrame :
		//	   CGRectMake ( 10 , 0 , frame . size . height , frame . size . height )];
		icon =[[ UIImageView alloc ] initWithFrame :
			   CGRectMake ( 10 , 3 , 24 , 24 )];
		[ self setStyle : CheckButtonStyleDefault ]; // 默认风格为方框（多选）样式
		//self.backgroundColor=[UIColor grayColor];
		[ self addSubview : icon ];
		label =[[ UILabel alloc ] initWithFrame : CGRectMake ( icon . frame . size . width + 24 , 0 ,
															  frame . size . width - icon . frame . size . width - 24 ,
															  frame . size . height )];
		label . backgroundColor =[ UIColor clearColor ];
		label . font =[ UIFont fontWithName : @"Arial" size : 20 ];
		label . textColor =[ UIColor
							colorWithRed : 0xf9 / 255.0
							green : 0xd8 / 255.0
							blue : 0x67 / 255.0
							alpha : 1 ];
		label . textAlignment = UITextAlignmentLeft ;
		[ self addSubview : label ];
		[ self addTarget : self action : @selector ( clicked ) forControlEvents : UIControlEventTouchUpInside ];
	}
	return self ;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code.
}
*/

-( CheckButtonStyle )style{
	return style ;
}
-( void )setStyle:( CheckButtonStyle )st{
	style =st;
	switch ( style ) {
		case CheckButtonStyleDefault :
		case CheckButtonStyleBox :
			checkname = @"checked.png" ;
			uncheckname = @"unchecked.png" ;
			break ;
		case CheckButtonStyleRadio :
			//checkname = @"radio.png" ;
			checkname = @"accept.png" ;
			uncheckname = @"unradio.png" ;
			break ;
		default :
			break ;
	}
	[ self setChecked : checked ];
}
-( BOOL )isChecked{
	return checked ;
}
-( void )setChecked:( BOOL )b{
	if (b!= checked ){
		checked =b;
	}
	if ( checked ) {
		[ icon setImage :[ UIImage imageNamed : checkname ]];
	} else {
		[ icon setImage :[ UIImage imageNamed : uncheckname ]];
	}
}
-( void )clicked{
	[ self setChecked :! checked ];
	
	if ( delegate != nil ) {
		//SEL sel= NSSelectorFromString ( @"checkButtonClicked" );
		SEL sel = @selector(checkButtonClicked:);
		
		if ([delegate respondsToSelector :sel]){
			[ delegate performSelector :sel withObject:self];
		} 
	}
}
-( void )dealloc{
	value = nil ; delegate = nil ;
	[ label release ];
	[ icon release ];
	[ super dealloc ];
}


@end

/*
使用CheckButton类很简单，构造、设置标签文本等属性，然后addSubview：

CheckButton * cb=[[ CheckButton a lloc ] initWithFrame : CGRectMake ( 20 , 60 , 260 , 32 )];
cb. label . text = @"checkbutton1" ;
cb. value =[[ NSNumber alloc ] initWithInt : 18 ];
cb. style = CheckButtonStyleDefault ;
[ self . view addSubview :cb];
*/



