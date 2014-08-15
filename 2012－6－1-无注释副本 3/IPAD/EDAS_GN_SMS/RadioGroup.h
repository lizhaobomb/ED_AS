//
//  RadioGroup.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-12.
//  Copyright 2012 北京. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "UICheckButton.h"

@interface RadioGroup : NSObject {
	NSMutableArray * children ;
	NSString * text ;
	id value ;
}

@property ( readonly )NSString* text;
@property ( readonly ) id value;
-( void )add:( UICheckButton *)cb;
-( void )checkButtonClicked:( id )sender;
@end

