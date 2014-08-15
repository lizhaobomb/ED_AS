//
//  UICombox.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-3-13.
//  Copyright 2012 北京. All rights reserved.
//

#import "UIComboBox.h"
#import <QuartzCore/QuartzCore.h>

@interface UIComboBox()

@property (nonatomic, retain) NSIndexPath  *lastIndexPath;

@end

@implementation UIComboBox

#pragma mark -
#pragma mark --- Property ---

@synthesize lastIndexPath         = _lastIndexPath;

@synthesize delegate              = _delegate;
@synthesize items			      = _items;
@synthesize itemViewCornerRadius  = _itemViewCornerRadius;

@synthesize comboItemNormalImage = _comboItemNormalImage;
@synthesize comboItemDownImage   = _comboItemDownImage;

@synthesize defaultIndex         = _defaultIndex;

@synthesize separatorColor;
@synthesize borderColor;
@synthesize dropMaxHeigth;

#pragma mark -
#pragma mark --- Memory Management ---

- (void)dealloc
{
	[_items release];
	[_lastIndexPath release];
	[_comboItemNormalImage release];
	[_comboItemDownImage release];
	
    [super dealloc];
}

#pragma mark -
#pragma mark --- Utils method ---

- (UIFont *)fontOfString: (NSString *)str
{
	UIFont *systemFontBig		  = [UIFont systemFontOfSize: 14.0f];
	UIFont *systemFontSmall		  = [UIFont systemFontOfSize: 12.0f];
	
	CGSize titleSize              = [str sizeWithFont: systemFontBig];
	
	return (titleSize.width <= 56.0f) ? systemFontBig : systemFontSmall;
}

#pragma mark -
#pragma mark --- Target ---

- (void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag
{
	[self.layer removeAllAnimations];
}

- (void)showMoreItemForState: (BOOL)state
{	
	[UIView beginAnimations: nil context: nil];
	[UIView setAnimationDuration: 0.1f];
	[UIView setAnimationDelegate: self];
	
	if (state)
	{
		_itemView.frame = CGRectMake(_itemView.frame.origin.x, _itemView.frame.origin.y,
									 self.frame.size.width, (self.frame.size.height * _items.count) + 2);
		
		self.frame      = CGRectMake(self.frame.origin.x  , self.frame.origin.y,
									 self.frame.size.width, self.frame.size.height + (self.frame.size.height * _items.count));
	}
	else
	{
		self.frame      = _oldRect;
		_itemView.frame = CGRectMake(_itemView.frame.origin.x, _itemView.frame.origin.y, 
									 self.frame.size.width, 0.0f);
	}
	
	[UIView commitAnimations];
	
	//[self.parent bringSubviewToFront:self];
	[self.superview bringSubviewToFront:self];
}

#pragma mark -
#pragma mark --- Target ---

- (void)comboBoxClick: (UIButton *)sender
{
	/***
	 *
	 *	commboBox单击事件
	 *  
	 */
	
	_selected = !_selected;
	sender.selected = _selected;
	
	[self showMoreItemForState: _selected];
}

#pragma mark -
#pragma mark --- UITableView delegate ---

- (void)hiddedItemView
{
	_selected = NO;
	
	[self showMoreItemForState: _selected];
}

- (void)tableView: (UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
{
	/***
	 *
	 *	选择cell
	 *  
	 */
	
	if (indexPath.row != _lastIndexPath.row)
	{
		if ([_delegate respondsToSelector: @selector(comboBox:selectItemAtIndex:)])
		{
			[_delegate comboBox: self selectItemAtIndex: indexPath.row];
		}
		
		UITableViewCell *cell		= [tableView cellForRowAtIndexPath: indexPath];
		cell.accessoryType			= UITableViewCellAccessoryCheckmark;
		
		UITableViewCell *oldCell	= [tableView cellForRowAtIndexPath: _lastIndexPath]; 
		oldCell.accessoryType		= UITableViewCellAccessoryNone;
		
		self.lastIndexPath          = indexPath;	
		
		if (_comboItemDownImage != nil)
		{
			cell.backgroundView.backgroundColor    = [UIColor colorWithPatternImage: _comboItemDownImage];
			oldCell.backgroundView.backgroundColor = [UIColor colorWithPatternImage: _comboItemNormalImage];
		}
		else
		{
			[tableView deselectRowAtIndexPath: indexPath animated: YES];
		}
		
		[_comboButton setTitle: [_items objectAtIndex: _lastIndexPath.row] forState: UIControlStateNormal];
	}
	
	[self hiddedItemView];
}

#pragma mark -
#pragma mark --- UITableView dataSource ---

- (NSInteger)numberOfSectionsInTableView: (UITableView *)tableView
{
	/***
	 *
	 *	Section number
	 *  
	 */
	
	return 1;
}

- (NSInteger)tableView: (UITableView *)tableView numberOfRowsInSection: (NSInteger)section
{
	/***
	 *
	 *	Cell number
	 *  
	 */
	
	return _items.count;
}

- (UITableViewCell *)tableView: (UITableView *)tableView cellForRowAtIndexPath: (NSIndexPath *)indexPath
{
	/***
	 *
	 *	创建 cell
	 *  
	 */
	
	static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: CellIdentifier];
    if (cell == nil)
	{
        cell = [[[UITableViewCell alloc] initWithStyle: UITableViewCellStyleDefault 
									   reuseIdentifier: CellIdentifier] autorelease];
    }
	
	
	UIView  *bgView = [[UIView alloc] initWithFrame: CGRectMake( 0.0f, 0.0f, _oldRect.size.width, _oldRect.size.height)];
	cell.backgroundView  = bgView;
	[bgView release];
	
	if (_comboItemNormalImage != nil && _comboItemDownImage != nil)
	{		
		cell.selectionStyle = UITableViewCellSelectionStyleNone;
		
		if (indexPath.row == _lastIndexPath.row)
		{
			bgView.backgroundColor  = [UIColor colorWithPatternImage: _comboItemDownImage];
		}
		else
		{
			bgView.backgroundColor  = [UIColor colorWithPatternImage: _comboItemNormalImage];
		}
	}
	else
	{
		bgView.backgroundColor  = [UIColor whiteColor];
	}
	
	NSString *titleText			= [_items objectAtIndex: indexPath.row];
	cell.textLabel.font			= [self fontOfString: titleText];
	cell.textLabel.text			= titleText;
	cell.textLabel.textColor	= [UIColor blackColor];
	
	cell.accessoryType = (indexPath.row == _lastIndexPath.row) ? UITableViewCellAccessoryCheckmark
	: UITableViewCellAccessoryNone;
	
	return cell;
}

#pragma mark -
#pragma mark --- method ---

- (void)setBackgroundImage: (UIImage *)img 
				  forState: (UIControlState)state
{
	/***
	 *
	 *	CommboBox 单击状态背景
	 *  
	 */
	
	_comboButton.backgroundColor = [UIColor clearColor];
	
	[_comboButton setBackgroundImage: img forState: state];
}

- (void)setItemViewCornerRadius: (CGFloat)radius
{
	/***
	 *
	 *	ItemViw圆角角度
	 *  
	 */
	
	_itemViewCornerRadius        = radius;
	_itemView.layer.cornerRadius = radius;
}

- (void)setSeparatorColor: (UIColor *)value
{
	/***
	 *
	 *	设置分割线颜色
	 *  
	 */
	
	_itemContentView.separatorColor = value;
}

- (void)setBorderColor: (UIColor *)value
{
	/***
	 *
	 *	设置边框颜色
	 *  
	 */
	
	_itemContentView.layer.borderColor = value.CGColor;
}

- (UIColor *)separatorColor
{
	/***
	 *
	 *	读取分割线颜色
	 *  
	 */
	
	return _itemContentView.separatorColor;
}

- (UIColor *)borderColor
{
	/***
	 *
	 *	读取边框颜色
	 *  
	 */
	
	return [UIColor colorWithCGColor: _itemContentView.layer.borderColor];
}

- (void)setDefaultIndex: (NSInteger)value
{
	/***
	 *
	 *	设置默认索引
	 *  
	 */
	
	_defaultIndex      = value;
	self.lastIndexPath = [NSIndexPath indexPathForRow: value inSection: 0];
	//[_comboButton setTitle: [_items objectAtIndex: _lastIndexPath.row] forState: UIControlStateNormal];
	//(void)tableView: (UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
	//[self tableView:_itemContentView didSelectRowAtIndexPath:self.lastIndexPath];
}

- (void)setSelectIndex: (NSInteger)value
{
	if(_items == nil || value < 0 || _items.count <= value) {return;}
	
	NSIndexPath *sle = [NSIndexPath indexPathForRow: value inSection: 0];
	if(sle !=nil)
	[self tableView:_itemContentView didSelectRowAtIndexPath:sle];
	
}

- (NSInteger) getSelectIndex 
{
	if(self.lastIndexPath != nil) {
	return self.lastIndexPath.row;
	}else {
		return 0;
	}

}
- (void)setEnabled: (BOOL)value {
	_comboButton.enabled = value;
}
#pragma mark -
#pragma mark --- Property method ---

- (void)setItems: (NSArray *)value
{
	if (_items != nil)
	{
		[_items release];
	}
	_items = [value retain];
	
	NSString *titleText           = [_items objectAtIndex: _lastIndexPath.row];
	_comboButton.titleLabel.font = [self fontOfString: titleText];
	
	// 设置Commbo标题
	[_comboButton setTitle: titleText forState: UIControlStateNormal];
	if(dropMaxHeigth == 0) {
		_itemContentView.frame = CGRectMake( 0.0f, 1.0f, self.frame.size.width, self.frame.size.height * _items.count);
	}else {
		int height1 = self.frame.size.height * _items.count;
		if(height1 > dropMaxHeigth){height1 = dropMaxHeigth;}
		_itemContentView.frame = CGRectMake( 0.0f, 1.0f, self.frame.size.width, height1);
	}

	
	[_itemContentView reloadData];
}

#pragma mark -
#pragma mark --- Initialization Management ---

- (id)initWithFrame: (CGRect)frame
{
    self = [super initWithFrame: frame];
    if (self)
    {
		_selected            = NO;
		_oldRect             = frame;
		_defaultIndex        = 0;
		self.backgroundColor = [UIColor clearColor];
		
		self.lastIndexPath   = [NSIndexPath indexPathForRow: 0 inSection: 0];
		
		// ItemView
		_itemView = [[UIView alloc] initWithFrame: CGRectMake( 0.0f, frame.size.height - 2.0f, frame.size.width, 0.0f)];
		[self addSubview: _itemView];
		[_itemView release];
		
		_itemView.backgroundColor     = [UIColor clearColor];
		_itemView.layer.masksToBounds = YES;
		_itemView.layer.cornerRadius  = 2.0f;
		
		// ItemContentView
		_itemContentView = [[UITableView alloc] initWithFrame: CGRectZero style: UITableViewStylePlain];
		[_itemView addSubview: _itemContentView];
		[_itemContentView release];
		
		_itemContentView.delegate   = self;
		_itemContentView.dataSource = self;
		_itemContentView.rowHeight  = self.frame.size.height;
		_itemContentView.backgroundColor = [UIColor clearColor];
		//_itemContentView.backgroundColor = [UIColor redColor];
		_itemContentView.bounces		 = YES;
		_itemContentView.separatorColor  = [UIColor darkGrayColor];
		_itemContentView.showsHorizontalScrollIndicator = YES;
		_itemContentView.showsVerticalScrollIndicator   = YES;
		_itemContentView.layer.borderWidth = 1.0f;
		_itemContentView.layer.borderColor = [UIColor darkGrayColor].CGColor;
		dropMaxHeigth  = 0;
		// Commbo按钮
		//_comboButton = [UIButton buttonWithType: UIButtonTypeCustom];
		_comboButton = [UIButton buttonWithType: UIButtonTypeRoundedRect];
		
		[self addSubview: _comboButton];
		
		_comboButton.frame = CGRectMake( 0.0f, 0.0f, frame.size.width, frame.size.height);
		_comboButton.titleEdgeInsets = UIEdgeInsetsMake( 0, -25, 0, 0);
		
		// 设定默认背景颜色
		//_comboButton.backgroundColor = [UIColor redColor];
		_comboButton.backgroundColor = [UIColor clearColor];
		
		// 添加Target
		[_comboButton addTarget: self action: @selector(comboBoxClick:) forControlEvents: UIControlEventTouchDown];
    }
    return self;
}

@end
