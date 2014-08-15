//
//  ViewController.m
//  EDAS_GN_SMS
//
//  Created by 李 召 on 14-6-21.
//
//

#import "SetZeroPointViewController.h"

@interface SetZeroPointViewController ()

@end

@implementation SetZeroPointViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)cancle:(id)sender{
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnOKClicked:(id)sender {
    char m_cmd[1024];
    int m_cmd_len = 0;
    
    m_cmd[0]=0xbf;
	m_cmd[1]=0x13;
	m_cmd[2]=0x97;
	m_cmd[3]=0x74;
    
    CM_DASOFFFRM frm;
    memset(&frm,0,sizeof(frm));

    
    frm.head.cmd=0x6006;
	frm.head.sens_id=0;
    frm.head.reserved=0;
	frm.head.length=16;
    
    frm.chk_sum=0;
    frm.ud = siteDoc.m_pcur_chn->offset;
    frm.ew = siteDoc.m_pcur_chn->offset;
    frm.ns = siteDoc.m_pcur_chn->offset;
	short *p=(short *)&frm;
	
	for(int i=0;i<(frm.head.length+6)/2-1;i++)
		frm.chk_sum-=p[i];
	
	memcpy(&m_cmd[4],(char *)&frm,frm.head.length+6);
	m_cmd_len=frm.head.length+10;
	
	if(!siteDoc.m_thd->Send(m_cmd,m_cmd_len))
	{	//SendErr();
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"错误" message:@"网络连接错误"
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
	}else {
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置调整零点成功"
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
    }
}

@end
