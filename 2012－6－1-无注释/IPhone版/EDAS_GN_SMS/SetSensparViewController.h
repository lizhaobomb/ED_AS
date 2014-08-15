//
//  SetSensparViewController.h
//
//  Created by guan mofeng on 12-3-19.
//  Copyright 2012 北京. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "UIComboBox.h"

@interface SetSensparViewController:UIViewController<UITextFieldDelegate,UIComboBoxDelegate>  {
    IBOutlet UIScrollView *scrollView;
    IBOutlet UIButton *btnCancel;
    IBOutlet UIButton *btnOk;
	
	UIComboBox *cmboSens;
	UIComboBox *cmboSensName;
	UITextField *txtSensName;
	UITextField *txtBand;
	UIComboBox *cmboType;
	UIComboBox *cmboRecType;
	UITextField *txtDepth;
	UITextField *txtRock;
	UITextField *txtSerial;
	UITextField *txtProvider;
	UITextField *txtComp;
	UIComboBox *cmboChn1AD;
	UIComboBox *cmboChn1cxid;
	UIComboBox *cmboChn2cxid;
	UIComboBox *cmboChn3cxid;
	//int		m_sensid;
	int		m_type;
	int		m_rectype;
	uint	m_comp;
	
}
- (IBAction)btnCancelClick:(id)sender;
- (IBAction)btnOKClick:(id)sender;

- (void) comboBoxSensSel :(NSInteger)index;
- (void) comboTypeSel :(NSInteger)index;
- (void) InitC;
- (void) InitCXIDBox :(int) type; 
- (void) comboBoxSensNameSel :(NSInteger)index;
@end
