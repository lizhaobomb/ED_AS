//
//  SetStationViewController.h
//
//  Created by guan mofeng on 12-2-10.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UIComboBox.h"

@interface SetStationViewController : UIViewController<UITextFieldDelegate>  {
    IBOutlet UIButton *btnGPS;
    IBOutlet UITextField *txtAbbr;
    IBOutlet UITextField *txtAdSum;
    IBOutlet UITextField *txtAlt;
    IBOutlet UITextField *txtLat;
    IBOutlet UITextField *txtLon;
    IBOutlet UITextField *txtNetid;
    IBOutlet UITextField *txtNo;
    IBOutlet UITextField *txtSensSum;
    IBOutlet UITextField *txtStn;
	UIComboBox *cmboADsum;
	NSTimer * m_timer;
	
	
	//uint	m_sens_sum;
	//int		m_ad_sum;

	
}
-(IBAction)btnGPSClick:(id)sender;
-(IBAction)btnOKClick: (id)sender;
-(IBAction)btnCancelClick: (id)sender;
@end
