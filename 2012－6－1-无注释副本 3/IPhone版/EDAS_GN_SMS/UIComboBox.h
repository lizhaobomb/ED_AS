//
//  UICombox.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-13.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>

@class UIComboBox;

@protocol UIComboBoxDelegate <NSObject>

@optional

// 选择item
- (void)comboBox: (UIComboBox *)comboBox selectItemAtIndex: (NSInteger)index;

@end

@interface UIComboBox : UIView <UITableViewDelegate,UITableViewDataSource>{
	
	bool   _selected;		// ComboBox选择状态
	CGRect _oldRect;		// ComboBox原始尺寸
	
    UIButton		*_comboButton;
	UIView          *_itemView;
	UITableView     *_itemContentView;
	
}


// Delegate
@property (nonatomic, assign) id <UIComboBoxDelegate> delegate;

// 下拉框的最大高度
@property (nonatomic, assign) NSInteger dropMaxHeigth;

// item标题数组
@property (nonatomic, assign) NSArray *items;
// itemViw圆角角度		  默认 2.0f
@property (nonatomic, assign) CGFloat itemViewCornerRadius;

@property (nonatomic, retain) UIImage *comboItemNormalImage;
@property (nonatomic, retain) UIImage *comboItemDownImage;

// 默认索引
@property (nonatomic, assign) NSInteger defaultIndex; 

// 边框颜色    默认 灰色
@property (nonatomic, assign) UIColor *borderColor;
// 分割线颜色   默认 灰色
@property (nonatomic, assign) UIColor *separatorColor;

- (void)setBackgroundImage: (UIImage *)img forState: (UIControlState)state;


//- (void)setDefaultIndex: (NSInteger)value;
- (void)setSelectIndex: (NSInteger)value;
- (NSInteger) getSelectIndex;
- (void)setEnabled: (BOOL)value;

@end
