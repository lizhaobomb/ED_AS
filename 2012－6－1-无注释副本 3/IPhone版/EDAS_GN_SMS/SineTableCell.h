//
//  SineTableCell.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-31.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SineTableCell : UITableViewCell <UITextFieldDelegate>{

	UILabel *labID;
	UITextField *txtCycle;
	UITextField *txtPrd;
	UITextField *txtAten;
	
	int rowIndex;
}

@property (assign) int rowIndex;
@property (assign) UILabel *labID;
@property (assign) UITextField *txtCycle;
@property (assign) UITextField *txtPrd;
@property (assign) UITextField *txtAten;


@end
