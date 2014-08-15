//
//  QueryinfoViewController.h
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-2-23.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface QueryinfoViewController : UIViewController {
	UITextView * u_message_view;
	NSTimer * m_timer;
	
}
@property(nonatomic,retain) NSTimer *m_timer;
@property (nonatomic , retain) IBOutlet UITextView * u_message_view;
-(IBAction) SaveMessage: (id)sender;//save message
-(IBAction) ClearMessage: (id)sender;//clear message
-(IBAction) Refresh: (id)sender;//refresh messsage
-(IBAction) Close: (id)sender;//close view
-(void) ontimer_msg : (NSTimer *)timer;
@end
