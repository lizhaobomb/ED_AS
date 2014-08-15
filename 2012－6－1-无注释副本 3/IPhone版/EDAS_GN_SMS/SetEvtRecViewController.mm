//
//  SetEvtRecViewController.m
//  EDAS_GN_SMS
//
//  Created by 李 召 on 14-6-12.
//
//

#import "SetEvtRecViewController.h"

@interface SetEvtRecViewController ()

@end

@implementation SetEvtRecViewController

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
    checkEvtRec =[[ UICheckButton alloc ] initWithFrame : CGRectMake ( 30 , 60 , 170 , 30 )];
	checkEvtRec.label.text = @"记录" ;
	checkEvtRec.value =[[ NSNumber alloc ] initWithInt:0 ];
	checkEvtRec.style = CheckButtonStyleBox ;
	[ self.view addSubview :checkEvtRec];
	[checkEvtRec release];
}

- (void)viewDidAppear:(BOOL)animated{
    [super viewDidAppear:animated];
    [self initUI];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_pre_evt release];
    [_after_evt release];
    [super dealloc];
}
- (void)viewDidUnload {
    [self setPre_evt:nil];
    [self setAfter_evt:nil];
    [super viewDidUnload];
}

-(void)initUI{
    NSLog(@"%d,%d",siteDoc.m_pre_evt, siteDoc.m_aft_evt);
    [checkEvtRec setChecked:siteDoc.be_wr];
    self.pre_evt.text = [NSString stringWithFormat:@"%d",siteDoc.m_pre_evt];
    self.after_evt.text = [NSString stringWithFormat:@"%d",siteDoc.m_aft_evt];
}

#pragma mark - IBAction
- (IBAction)btnOKClicked:(id)sender {
    char m_cmd[1024];
    int m_cmd_len = 0;
    CM_EVTRECFRM frm;
    
    memset(&frm,0,sizeof(CM_EVTRECFRM));
	int pre_evt = [self.pre_evt.text intValue];
    int aft_evt = [self.after_evt.text intValue];
	
	m_cmd[0]=0xbf;
	m_cmd[1]=0x13;
	m_cmd[2]=0x97;
	m_cmd[3]=0x74;
	
	frm.pre_evt = pre_evt;
    frm.aft_evt = aft_evt;
    frm.be_wr = checkEvtRec.isChecked;
    siteDoc.m_pre_evt = frm.pre_evt;
    siteDoc.m_aft_evt = frm.aft_evt;
    siteDoc.be_wr = frm.be_wr;
    frm.head.cmd=0x607e;
	frm.head.sens_id=0;
    frm.head.reserved=0;
	frm.head.length=10;
    frm.chk_sum=0;
    
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
		UIAlertView *av = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"设置事件记录成功"
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
		[av show];
     }
}

-(IBAction)cancel:(id)sender{
    [self.navigationController popViewControllerAnimated:YES];
}

-(IBAction)hideKeyboard:(id)sender{
    [self.after_evt resignFirstResponder];
    [self.pre_evt resignFirstResponder];
}

@end
