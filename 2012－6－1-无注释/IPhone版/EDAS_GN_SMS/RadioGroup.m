//
//  RadioGroup.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-12.
//  Copyright 2012 北京. All rights reserved.
//

#import "RadioGroup.h"


@implementation RadioGroup
@synthesize text,value;

-( id )init{
	if ( self =[ super init ]){
		children =[[ NSMutableArray alloc ] init ];
	}
	return self ;
}
-( void )add:( UICheckButton *)cb{
	cb.delegate = self ;
	if ([cb isChecked] ) {
		text =cb.label.text ;
		value =cb.value ;
	}
	[ children addObject :cb];
}
-( void )checkButtonClicked:( id )sender {
	UICheckButton * cb=( UICheckButton *)sender;
	//if ([cb isChecked] ) {
		// 实现单选
		for ( UICheckButton * each in children ){
			if ([each isChecked] ) {
				//[each setChecked : NO ];
				[each setChecked:NO];
			}
		}
	//}
	[cb setChecked : YES ];
	// 复制选择的项
	text =cb.label.text ;
	value =cb.value ;
	
	NSLog ( @"text:%@,value:%d" , text ,[( NSNumber *) value intValue ]);
}
-( void )dealloc{
	[ text release ];
	value = nil ;
	[ children release ];
	[ super dealloc ];
}

@end

/*
 单选按钮组在ViewController中的使用：
 
 -( id )initWithNibName:( NSString *)nibNameOrNil bundle:( NSBundle *)nibBundleOrNil{
 if ( self =[ super initWithNibName :nibNameOrNil bundle :nibBundleOrNil]){
 // 单选按钮组
 rg =[[ RadioGroup alloc ] init ];
 // 第 1 个单选按钮
 CheckButton * cb=[[ CheckButton alloc ] initWithFrame : CGRectMake ( 20 , 60 , 260 , 32 )];
 // 把单选按钮加入按钮组
 [ rg add :cb];
 cb. label . text = @"★" ;
 cb. value =[[ NSNumber alloc ] initWithInt : 1 ];
 // 把按钮设置为单选按钮样式
 cb. style = CheckButtonStyleRadio ;
 // 加入视图
 [ self . view addSubview :cb];
 [cb release ]; //add 后，会自动持有，可以释放
 // 第 2 个单选按钮
 cb=[[ CheckButton alloc ] initWithFrame : CGRectMake ( 20 , 100 , 260 , 32 )];
 [ rg add :cb];
 cb. label . text = @"★★" ;
 cb. value =[[ NSNumber alloc ] initWithInt : 2 ];
 cb. style = CheckButtonStyleRadio ;
 [ self . view addSubview :cb];
 [cb release ];
 // 第 3 个单选按钮
 cb=[[ CheckButton alloc ] initWithFrame : CGRectMake ( 20 , 140 , 260 , 32 )];
 // 各种属性必须在 [rg addv] 之前设置，否则 text 和 value 不会被 populate
 cb. checked = YES ;
 cb. label . text = @"★★★" ;
 cb. value =[[ NSNumber alloc ] initWithInt : 3 ];
 cb. style = CheckButtonStyleRadio ;
 [ self . view addSubview :cb];
 [ rg add :cb]; // 属性设置完之后再 add
 [cb release ];
 // 第 4 个单选按钮
 cb=[[ CheckButton alloc ] initWithFrame : CGRectMake ( 20 , 180 , 260 , 32 )];
 [ rg add :cb];
 cb. label . text = @"★★★★" ;
 cb. value =[[ NSNumber alloc ] initWithInt : 4 ];
 cb. style = CheckButtonStyleRadio ;
 [ self . view addSubview :cb];
 [cb release ];
 // 第 5 个单选按钮
 cb=[[ CheckButton alloc ] initWithFrame : CGRectMake ( 20 , 220 , 260 , 32 )];
 [ rg add :cb];
 cb. label . text = @"★★★★★" ;
 cb. value =[[ NSNumber alloc ] initWithInt : 5 ];
 cb. style = CheckButtonStyleRadio ;
 [ self . view addSubview :cb];
 [cb release ];
 }
 return self ;
 }
 
 */




