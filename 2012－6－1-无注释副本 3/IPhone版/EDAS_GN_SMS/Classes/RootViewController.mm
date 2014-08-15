//
//  RootViewController.m
//
//  Created by guan mofeng on 11-12-9.
//  Copyright 2011 北京. All rights reserved.
//

#import "RootViewController.h"
#import "Constants.h"
#include "GlobalData.h"

extern char sens_id[];

static RootViewController *uRootViewController;

@implementation RootViewController


@synthesize setMenuViewController, queryMenuViewController, runMenuViewController, modalViewController;//, drawwaveViewController;
@synthesize queryinfoViewController;
@synthesize u_drawview;
@synthesize setDsp1ViewContoller;

@synthesize u_cal_info;

- (void)dealloc
{
	if (self.modalViewController != nil)
		[modalViewController release];
	if (self.runMenuViewController != nil)
		[runMenuViewController release];
//	if (self.drawwaveViewController  !=nil )
//		[self.drawwaveViewController  release];
	if (self.queryinfoViewController!=nil);
		[self.queryinfoViewController release];
	if (self.u_drawview !=nil)
		[self.u_drawview release];
	if(self.u_cal_info!=nil)
		[self.u_cal_info release];
	
	if(self.setDsp1ViewContoller != nil)
	{
		[self.setDsp1ViewContoller release];
	}
	[super dealloc];
}

-(void) showDsp1ViewController :(NSString *) labetext {
	
	if(self.setDsp1ViewContoller == nil){
		self.setDsp1ViewContoller = [[[SetDsp1ViewController alloc] initWithNibName:
								 NSStringFromClass([SetDsp1ViewController class]) bundle:nil] autorelease];
	}
	[self.setDsp1ViewContoller initWithShow : labetext];
	[self.navigationController pushViewController:setDsp1ViewContoller animated:YES];
}

+ (DrawwaveView*) U_drawview {
	return uU_drawview;
}

+ (RootViewController*) U_RootViewController {
	//RootViewController *uRootViewController;
	return uRootViewController;
}
- (void)viewDidLoad
{
	[NSTimer scheduledTimerWithTimeInterval:0.2f target:self selector:@selector(ontimer_display:) userInfo:nil repeats:YES];	
	//	[NSTimer scheduledTimerWithTimeInterval:0.3f target:self selector:@selector(ontimer_savedata:) userInfo:nil repeats:YES];	
 	[NSTimer scheduledTimerWithTimeInterval:1.0f target:self selector:@selector(ontimer_checkcon:) userInfo:nil repeats:YES];
}
-(void)loadView{
	// Make the title of this page the same as the title of this app
	self.title = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleName"];
	
	/*
	
    // Create a final modal view controller
	UIButton* modalViewButton = [UIButton buttonWithType:UIButtonTypeInfoLight];
	[modalViewButton addTarget:self action:@selector(modalViewAction:) forControlEvents:UIControlEventTouchUpInside];
	UIBarButtonItem *modalBarButtonItem = [[UIBarButtonItem alloc] initWithCustomView:modalViewButton];
	self.navigationItem.rightBarButtonItem = modalBarButtonItem;
	//self.navigationItem
	[modalBarButtonItem release];
	*/
	
	UISegmentedControl *segmentedControl = [[UISegmentedControl alloc] initWithItems:
											[NSArray arrayWithObjects:
											 @"运行",@"查询",@"设置",@"关于",
											 nil]];
    [segmentedControl addTarget:self action:@selector(segmentAction:) forControlEvents:UIControlEventValueChanged];
	segmentedControl.frame = CGRectMake(0, 0, 180, kCustomButtonHeight);
	segmentedControl.segmentedControlStyle = UISegmentedControlStyleBar;
	segmentedControl.momentary = YES;
	
	UIBarButtonItem *segmentBarItem = [[UIBarButtonItem alloc] initWithCustomView:segmentedControl];
    [segmentedControl release];
    
	self.navigationItem.rightBarButtonItem = segmentBarItem;
    [segmentBarItem release];
	
	
	DrawwaveView * drw=[[DrawwaveView alloc] init];
	self.view = drw;
	self.u_drawview = (DrawwaveView *)self.view;
	self.u_drawview.m_origin_y=kCustomButtonHeight;
	self.view.backgroundColor = [UIColor whiteColor];
	
	uU_drawview = self.view;
	uRootViewController = self;
	[drw release];
	
	UIToolbar * toolbar = [[UIToolbar alloc] initWithFrame: CGRectMake(0,0,480,kCustomButtonHeight)];
	//NSMutableArray * baritems = [NSMutableArray array];
	
	
	// (id)initWithImage:(UIImage *)image style:(UIBarButtonItemStyle)style target:(id)target action:(SEL)action
	//UIBarButtonItem * item_up=[[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"zoom_out.png"] style:UIBarButtonItemStyleBordered target:self action:@selector(OnViewUp:)] ;
	//UIBarButtonItem * item_down=[[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"zoom_int.png"] style:UIBarButtonItemStyleBordered target:self action:@selector(OnViewDown:)] ;
	//UIBarButtonItem * item_center=[[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"zoom.png"] style:UIBarButtonItemStyleBordered target:self action:@selector(OnViewCenter:)] ;
	
	
	UIBarButtonItem * item_up=[[UIBarButtonItem alloc] initWithTitle:@"放大" style:UIBarButtonItemStyleBordered target:self action:@selector(OnViewUp:)] ;
	UIBarButtonItem * item_down=[[UIBarButtonItem alloc] initWithTitle:@"缩小" style:UIBarButtonItemStyleBordered target:self action:@selector(OnViewDown:)] ;
	UIBarButtonItem * item_center=[[UIBarButtonItem alloc] initWithTitle:@"居中" style:UIBarButtonItemStyleBordered target:self action:@selector(OnViewCenter:)] ;
    UIButton* btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = CGRectMake(0, kCustomButtonHeight, 44, 28);
    [btn setImage:[UIImage imageNamed:@"offset_btn"] forState:0];
    [btn addTarget:self action:@selector(OnViewOffset:) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem* item_offset = [[UIBarButtonItem alloc] initWithCustomView:btn];
    //UIBarButtonItem * item_offset=[[UIBarButtonItem alloc] initWithTitle:@"偏移" style:UIBarButtonItemStyleBordered target:self action:@selector(OnViewOffset:)] ;
	UIBarButtonItem * item_scantime=[[UIBarButtonItem alloc] initWithTitle:@"显示时间" style:UIBarButtonItemStyleBordered target:self action:@selector(OnScantime:)] ;

	//item_up.width = 40;
	//item_down.width = 40;
	//item_center.width = 40;
	
	UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(180, kCustomButtonHeight, 120.0f, kCustomButtonHeight) ];
	label.font=[UIFont systemFontOfSize:FONTSIZE ];
	label.textColor=[UIColor whiteColor];
 	label.backgroundColor = [UIColor clearColor];
	self.u_drawview.u_time_info=label;
	
	UIBarButtonItem * item_time_info=[[UIBarButtonItem alloc] initWithCustomView: label];
	[label release];
	
	label = [[UILabel alloc] initWithFrame:CGRectMake(280, kCustomButtonHeight, 120.0f, kCustomButtonHeight) ];
	label.font=[UIFont systemFontOfSize:FONTSIZE];
	label.textColor=[UIColor whiteColor];
 	label.backgroundColor = [UIColor clearColor];
	self.u_cal_info=label;
	
	UIBarButtonItem * item_cal_info=[[UIBarButtonItem alloc] initWithCustomView:  label];
	
	toolbar.items = [NSArray arrayWithObjects:item_up, item_down, item_center, item_offset, item_scantime, item_time_info, item_cal_info, nil];
	toolbar.clipsToBounds=YES;
	[label release];
	
	[item_up release];
	[item_down release];
	[item_center release];
    [item_offset release];
	[item_scantime release];
	[item_time_info release];
	
	[self.u_drawview addSubview:toolbar];
	
	[toolbar release];
		 
	/*if(self.drawwaveViewController==nil){
	  	DrawrtwaveViewController  * sview= [[DrawrtwaveViewController alloc] initWithNibName:@"DrawrtwaveViewController" bundle:nil]; 
 		
		self.drawwaveViewController  =sview;
	 	[sview release];
 
 		[self.view insertSubview:drawwaveViewController.view atIndex:0]; 
		 
		 
		
	 }*/
		 	
	
}




- (void)viewDidUnload
{

}


-(void)viewDidAppear:(BOOL)animated
{
	[self  settimer];
	//get context and window size
    self.u_drawview.m_bclear=TRUE; 
	[self.view  setNeedsDisplay];
	TRACE("VIEW APPERA\n");
}

-(void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	self.u_drawview.m_bclear=TRUE;
	[self.view setNeedsDisplay];
//	[self.u_drawview CreateStatusbar];
	TRACE("VIEW ROTATE\n");
}


-(void) ontimer_runmenu: (NSTimer * )timer{
	
	UIViewController *ss = [self.runMenuViewController getSelectViewController];
	if(ss == nil) {return;}
	[self.navigationController pushViewController:ss animated:YES];
	[timer invalidate];
}

-(void) ontimer_setmenu: (NSTimer * )timer{
	
	UIViewController *ss = [self.setMenuViewController getSelectViewController];
	if(ss == nil) {return;}
	[self.navigationController pushViewController:ss animated:YES];
	[timer invalidate];
}

-(void) ontimer_querymenu: (NSTimer * )timer{

	UIViewController *queryUI = [self.queryMenuViewController getSelectViewController];
	if(queryUI == nil) {return;}
	[self.navigationController pushViewController:queryUI animated:YES];
	[timer invalidate];
}


- (IBAction)segmentAction:(id)sender
{
	// The segmented control was clicked, handle it here 
	UISegmentedControl *segmentedControl = (UISegmentedControl *)sender;
	//NSLog(@"Segment clicked: %d", segmentedControl.selectedSegmentIndex);
	if(segmentedControl.selectedSegmentIndex == 0) // run
	{
		if (self.runMenuViewController == nil)
			self.runMenuViewController = [[[RunMenuViewController alloc] initWithNibName:
										 NSStringFromClass([RunMenuViewController class]) bundle:nil] autorelease];
		[NSTimer scheduledTimerWithTimeInterval:0.2f target:self selector:@selector(ontimer_runmenu:) userInfo:nil repeats:YES];
		[self.navigationController presentModalViewController:self.runMenuViewController animated:YES];
//		[self.navigationController pushViewController:self.runMenuViewController animated:NO];
	
	}else if(segmentedControl.selectedSegmentIndex == 1) //query
	{

		if (self.queryMenuViewController == nil)
			self.queryMenuViewController = [[[QueryMenuViewController alloc] initWithNibName:
										   NSStringFromClass([QueryMenuViewController class]) bundle:nil] autorelease];
		//[NSTimer scheduledTimerWithTimeInterval:0.2f target:self selector:@selector(ontimer_querymenu:) userInfo:nil repeats:YES];
        //[self.navigationController presentModalViewController:self.queryMenuViewController animated:YES];
        [self.navigationController pushViewController:self.queryMenuViewController animated:NO];
	
	
	
	
	}else if(segmentedControl.selectedSegmentIndex == 2) //set
	{
	
		if (self.setMenuViewController == nil)
			self.setMenuViewController = [[[SetMenuViewController alloc] initWithNibName:
										   NSStringFromClass([SetMenuViewController class]) bundle:nil] autorelease];
		
//		[NSTimer scheduledTimerWithTimeInterval:0.2f target:self selector:@selector(ontimer_setmenu:) userInfo:nil repeats:YES];
//		[self.navigationController presentModalViewController:self.setMenuViewController animated:YES];
        [self.navigationController pushViewController:self.setMenuViewController animated:NO];
			
	}else if(segmentedControl.selectedSegmentIndex == 3) // about
	{
		if (self.modalViewController == nil)
			self.modalViewController = [[[ModalViewController alloc] initWithNibName:
										 NSStringFromClass([ModalViewController class]) bundle:nil] autorelease];
		
		
		NSLog(@"1Entrer Model, wait to leaver \n");
		[self.navigationController presentModalViewController:self.modalViewController animated:YES];
		
		NSLog(@"1leaver Model \n");
	}
	
	
	
	
}



- (IBAction)modalViewAction:(id)sender
{
    if (self.modalViewController == nil)
        self.modalViewController = [[[ModalViewController alloc] initWithNibName:
									   NSStringFromClass([ModalViewController class]) bundle:nil] autorelease];
	
	[self.navigationController presentModalViewController:self.modalViewController animated:YES];
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
	//return ((toInterfaceOrientation == UIDeviceOrientationPortraitUpsideDown) ||
	//(toInterfaceOrientation == UIDeviceOrientationPortrait));
	
	return ((toInterfaceOrientation == UIDeviceOrientationLandscapeLeft) ||
			(toInterfaceOrientation == UIDeviceOrientationLandscapeRight));
	//return TRUE;
}

//the following is defined for showing display and message on timer
-(void) OnCloseCon{
	[self.u_drawview OnRunView : 0];
}
 

-(void ) settimer{
	if(siteDoc.m_update_id==1)	
	{
		[NSTimer scheduledTimerWithTimeInterval:0.5f target:self selector:@selector(ontimer_processcon:) userInfo:nil repeats:YES];
		siteDoc.m_update_id=0;
	}else if(siteDoc.m_update_id==-2)//run disconnected
	{
		siteDoc.m_update_id=0;
		[self OnCloseCon];
	}
}

-(void ) ontimer_processcon: (NSTimer *)timer{
	int ret = siteDoc.ProcessCon();
	if(ret==1){
		[self.u_drawview OnRunView:1];
	//	UIAlertView * alert=[[[UIAlertView alloc] initWithTitle: @"Message" message: @"已经建立连接" delegate:nil cancelButtonTitle: @"Close" otherButtonTitles: nil ] autorelease];
		
	//	[alert show];	
		[timer invalidate];
	}
	else if(ret==-1){
		UIAlertView * alert=[[[UIAlertView alloc] initWithTitle: @"Message" message: @"网络连接超时" delegate:nil cancelButtonTitle: @"确定" otherButtonTitles: nil ] autorelease];
		
		[alert show];	
		[timer invalidate];		 		 
	}
	//show model warning dialog
	//else 
}

-(void ) ontimer_display: (NSTimer * )timer{
	//siteview.displaydata
	[self DisplayData];
	 
}

-(void ) ontimer_savedata: (NSTimer * )timer{
	siteDoc.SaveData();
}

-(void ) ontimer_checkcon: (NSTimer * )timer{
	
	if( siteDoc.CheckConStat() ==0)
	{
		char temp[1024];
		sprintf(temp, "%s 网络中断，重新连接...", siteDoc.m_selsite->m_site.net_con.host);
		NSString *msg = [[NSString alloc] initWithUTF8String:temp];
		//strcpy(temp,siteDoc.m_selsite->m_site.net_con.host);
	 	//NSString *msg =[[NSString alloc] initWithFormat:@"%@ 网络中断，重新连接...",host1];
		//NSString *msg =[[NSString alloc] initWithFormat:@"%@ 网络中断，重新连接...",siteDoc.m_selsite->m_site.net_con.host];
		
		UIAlertView * alert=[[[UIAlertView alloc] initWithTitle: @"提示" message: msg delegate:nil cancelButtonTitle: @"确定" otherButtonTitles: nil ] autorelease];
		
		[alert show];	
		[timer invalidate];		 	
		
	}
	
	
}

-(void) DisplayData
{
	//CTime tmcv;
	
	CGRect rg;
	//	CPen *oldpen;
	CHANNEL *pchn;
	SHDAT1 * pdat;
	LOCALREVBLK * plocal;
	int step;
	short *p;
	int cn,j;
	string s;
	char s1[20];
	//	char s[100];
	BOOL bshow,brefresh;
	char calistr[200] ;
	int califlag;
	
	int l=0;
	//	TRACE("display\n");
	
	if(siteDoc.m_selsite==NULL)
		return;
	if(!siteDoc.IsConnected())
		return;
	
	
	//show sample rate
	plocal=siteDoc.m_selsite->m_plocal;
	
	
	s=	"采样率：";
	//strcpy(s,"采样率：");
	bshow=0;
	while(plocal!=NULL)
	{
		if(plocal->samp>0){
			sprintf(s1,"%ld ",plocal->samp);
			//strcat(s,s);
			s =s+s1;
			bshow=1;
		}
		plocal=plocal->lpnext;
	}
	if(bshow==1){
		//change samp_info label
		NSString * str=[[NSString alloc] initWithUTF8String:s.c_str()];
		self.u_drawview.u_samp_info.text = str;
		[str release];
		
		
	}

	brefresh=FALSE;
	pchn=siteDoc.m_pchannels;
	plocal=siteDoc.m_selsite->m_plocal;
	l=0;
	 
	califlag=0;
	strcpy(calistr,"");
	
	
	while(plocal!=NULL && pchn!=NULL)
	{
		
		while(pchn->revblk.p_dsp->flg>pchn->revblk.p_dsp->pb->flg)
		{
			if(u_drawview.m_real_time_status)//œ‘ æ≤®–Œ
			{
				siteDoc.m_pcur_chn=pchn;
				[self.u_drawview OnData: l : plocal->chn_sum];
				brefresh=TRUE;
				 
			}
			if(pchn->cali_flag!=pchn->revblk.p_dsp->cali_flag || pchn->revblk.p_dsp->cali_flag!=0 ){
				califlag=1;
				pchn->cali_flag=pchn->revblk.p_dsp->cali_flag;
			}
			
			pchn->revblk.p_dsp=pchn->revblk.p_dsp->pn;
		}
		//œ‘ æ≤Œ ˝
        //TRACE(">>>>>>>%ld,%ld<<<<<<<<<<<",pchn->cmdblk.p_dsp->flg, pchn->cmdblk.p_dsp->pb->flg);
		while(pchn->cmdblk.p_dsp->flg>pchn->cmdblk.p_dsp->pb->flg)
		{	
			pchn->cmdblk.p_dsp=pchn->cmdblk.p_dsp->pn;
			siteDoc.m_pcur_chn=pchn;
			[self OnMessage];
			
		}
		//œ‘ æ±Í∂®±Í÷æ
		[self ShowCaliflag: pchn->cali_flag : plocal->local_id : calistr];
		
		l+=plocal->chn_sum;
		for(j=0;j<plocal->chn_sum;j++)
			pchn=pchn->lpnext;
		plocal=plocal->lpnext;
	}
	if(califlag)
	{
		u_cal_info.text = [[NSString alloc] initWithUTF8String:calistr];
		
	}
	//show received message
	/*if(siteDoc.m_old_msg.length()>0)
	 {
	 NSString* msg=[[NSString alloc] initWithUTF8String: siteDoc.m_old_msg.c_str()]; 
	 UIAlertView * alert=[[[UIAlertView alloc] initWithTitle: @"Message" message: msg delegate:self cancelButtonTitle: @"Close" otherButtonTitles: @"Save", @"Clear", nil ] autorelease];
	 [msg release];
	 
	 [alert show];
	 }*/
	//pDC->SelectObject(oldpen);
	//	ReleaseDC(pDC);
	if(brefresh)
	{
		u_drawview.m_redraw_status=1;
		
		[self.view  setNeedsDisplay];
	}
	
}

-(void) ShowCaliflag: (int) cali_flag : (int) local_id : (char *) calistr
{
	char tmp[40];
	if(cali_flag==4)
		strcpy(tmp,"强震标定");
	else if(cali_flag==2)
		strcpy(tmp,"正弦标定");
	else if(cali_flag==6)
		strcpy(tmp,"伪随机码标定");
	else return;
	
	sprintf(calistr,"%s", tmp);
	
}

-(void) OnMessage1
{
	string info;
	
	info="";
	
	siteDoc.ProcessMessage((char *)siteDoc.m_pcur_chn->cmdblk.p_dsp->pb->ds,info);
	if(info.length()==0)
		return;
	//	TRACE("%s",info.c_str());
	
	siteDoc.m_old_msg = siteDoc.m_old_msg + info;
	siteDoc.m_msg =siteDoc.m_msg+ info;
	//NSLog();
	// 	NSString* msg=[[NSString alloc] initWithUTF8String: siteDoc.m_old_msg.c_str()]; 
	//	UIAlertView * alert=[[[UIAlertView alloc] initWithTitle: @"Message" message: msg delegate:self cancelButtonTitle: @"Close" otherButtonTitles: @"Save", @"Clear", nil ] autorelease];
	
 	//[msg release];
	
 	//[alert show];
	
	if(siteDoc.m_btip==2)
	{
		siteDoc.m_btip=0;
		
	}
	if(siteDoc.m_btip==1)
	{
		siteDoc.m_btip=0;
		UIAlertView * alert1=[[[UIAlertView alloc] initWithTitle: @"EDAS-GN" message: @"重新启动数据采集器后参数才能生效，选择是重新启动" delegate:self cancelButtonTitle:@"否" otherButtonTitles:@"是",nil] autorelease];
		[alert1 show];
		//check if click yes
		//GetDocument()->ResetSys();
		
	}
	
}  


-(void) OnShowTipMsg:(NSString*) msg {
	UIAlertView *av7024 = [[[UIAlertView alloc] initWithTitle:@"提示" message:msg
													 delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] autorelease];
	[av7024 show];
}

-(void) OnShowIsRestSyspMsg {
	UIAlertView * alert1=[[[UIAlertView alloc] initWithTitle: @"EDAS-GN" message: @"已恢复出厂设置，重启数据采集器后新的参数才能生效。\n 选择 ‘是’ 立即重启数据采集器" delegate:self cancelButtonTitle:@"否" otherButtonTitles:@"是",nil] autorelease];
	[alert1 show];
}





-(void) OnMessage
{
	string info;
	
	info="";
	
	//siteDoc.ProcessMessage((char *)siteDoc.m_pcur_chn->cmdblk.p_dsp->pb->ds,info);
	//////////////////////////////////////////////////////////////////////////////////////
	char *pdata = (char *)siteDoc.m_pcur_chn->cmdblk.p_dsp->pb->ds;
	unsigned short *p=(unsigned short *)pdata;
	TRACE("cmd=%x\n",p[1]);
	switch(p[1])
	{
		case 0x7000:
			siteDoc.OnStnpar(pdata,info);
			break;
		case 0x7001:
			siteDoc.OnSenspar(pdata,info);
			break;
		case 0x7002:
			siteDoc.OnInstpar(pdata,info);
			break;
		case 0x7003:
		case 0x9003:
			siteDoc.OnSmprate(pdata,info);
			break;
			//	case 0x2003:
			//		OnHp(pdata,info);
			//		break;
		case 0x7005:
		case 0x9005:
			siteDoc.OnGain(pdata,info);
			break;
		case 0x7006:
			siteDoc.OnDasoff(pdata,info);
			break;
		case 0x7030:
		case 0x7031:
		case 0x7035:
			siteDoc.OnNetsrv(p[1],pdata,info);
			break;
		case 0x7032:
			siteDoc.OnDataSrv(pdata,info);
			break;
		case 0x7033:
			siteDoc.OnIPUCom(pdata,info);
			break;
		case 0x7034:
			siteDoc.OnComctl(pdata,info);
			break;
		case 0x7020:
			siteDoc.OnCalPulse(pdata,info);
			break;
		case 0x7021:
			siteDoc.OnCalSine(pdata,info);
			break;
		case 0x7022:
			siteDoc.OnCalPseudo(pdata,info);
			break;
		case 0x7023:
			siteDoc.OnCalStrong(pdata,info);
			break;
		case 0x7024:
			[self OnShowTipMsg:@"启动脉冲标定成功"];
			info="";
			break;
		case 0x7025:
			
			[self OnShowTipMsg:@"启动正弦标定成功"];
			info="";
			break;
			
		case 0x7026:
			
			[self OnShowTipMsg:@"启动伪随机标定成功"];
			info="";
			break;
			
		case 0x7027:
		
			[self OnShowTipMsg:@"启动强震标定成功"];
			info="";
			break;
		case 0x7028://new
			siteDoc.OnStopcal(pdata,info);
			break;
		case 0x7029://new
			siteDoc.OnCalDelay(pdata,info);
			break;
		case 0x7050:
		case 0x9050:
			siteDoc.OnCfgSensoff(pdata,info);
			[self OnShowTipMsg:[[NSString alloc] initWithUTF8String:info.c_str()]];
			info="";
			break;
		case 0x7051:
		case 0x9051:
			siteDoc.OnStopSensoff(pdata,info);
			[self OnShowTipMsg:[[NSString alloc] initWithUTF8String:info.c_str()]];
			info="";
			break;
		case 0x7052:
			siteDoc.OnCmgLock(pdata,info);
			[self OnShowTipMsg:[[NSString alloc] initWithUTF8String:info.c_str()]];
			info="";			
			break;
		case 0x9053://new
			siteDoc.OnSensAdjStat(pdata,info);
			[self OnShowTipMsg:[[NSString alloc] initWithUTF8String:info.c_str()]];
			info="";			
			break;
		case 0x7054://new
			siteDoc.OnSensSig(pdata,info);
			break;
		case 0x7040:
			siteDoc.OnTrigauto(pdata,info);
			break;
		case 0x7041:
			siteDoc.OnDetevt(pdata,info);
			break;
		case 0x7060:
			siteDoc.OnHeartbeat(pdata,info);
			break;
		case 0x7061:
			siteDoc.OnVOL(pdata,info);
			break;
		case 0x7062:
			siteDoc.OnTEMP(pdata,info);
			break;
		case 0x7063:
			siteDoc.OnClkerr(pdata,info);
			break;
		case 0x7064:
			siteDoc.OnBatcharge(pdata,info);
			break;
		case 0x7065:
			siteDoc.OnGpsStat(pdata,info);
			break;
		case 0x7066:
			siteDoc.OnGetHeartbeat(pdata,info);
			break;
		case 0x7067:
			siteDoc.OnHdStat(pdata,info);
			break;
		case 0x7068:
			siteDoc.OnGPS(pdata,info);
			break;
		case 0x7069:
			siteDoc.OnGetSensoff(pdata,info);
			break;
		case 0x7070:
			siteDoc.OnWrctl(pdata,info);
			break;
		case 0x7071:
			siteDoc.OnClearSD(pdata,info);
			break;
		case 0x7072:
			siteDoc.OnBackupRule(pdata,info);
			break;
		case 0x7073:
			siteDoc.OnIp(pdata,info);
			break;
		case 0x7074:
			siteDoc.OnSysTm(pdata,info);
			break;
		case 0x7075:
			siteDoc.OnTimezone(pdata,info);
			break;
		case 0x7076:
			siteDoc.OnGpsInt(pdata,info);
			break;
		case 0x7077:
			[self OnShowIsRestSyspMsg];
			info = "";
			//siteDoc.OnResettoDefault(info);
			break;
		case 0x7078:
			siteDoc.OnResetSys(info);
			break;
		case 0x7079:
			siteDoc.OnStopSys(info);
			break;
		case 0x707e:
			siteDoc.OnEvtrec(pdata,info);
			break;
        case 0x707f:
            siteDoc.OnEvtrecFile(pdata, info);
            break;
		case 0x90f0:
			siteDoc.OnEvtTrig(pdata,info);
			break;
		case 0x90f1:
			siteDoc.OnEvtDetrig(pdata,info);
			break;
		case 0x9000:
			siteDoc.OnBroadcast(pdata,info);
			break;
		case 0x90f2:
			siteDoc.OnLd(pdata,info);
			break;
		case 0x90f3:
			siteDoc.OnZaiping(pdata,info);
			break;
			
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	if(info.length()==0)
		return;
	//	TRACE("%s",info.c_str());
	
	siteDoc.m_old_msg = siteDoc.m_old_msg + info;
	siteDoc.m_msg =siteDoc.m_msg+ info;
	//NSLog();
	// 	NSString* msg=[[NSString alloc] initWithUTF8String: siteDoc.m_old_msg.c_str()]; 
	//	UIAlertView * alert=[[[UIAlertView alloc] initWithTitle: @"Message" message: msg delegate:self cancelButtonTitle: @"Close" otherButtonTitles: @"Save", @"Clear", nil ] autorelease];
	
 	//[msg release];
	
 	//[alert show];
	
	if(siteDoc.m_btip==2)
	{
		siteDoc.m_btip=0;
		
	}
	if(siteDoc.m_btip==1)
	{
		siteDoc.m_btip=0;
		UIAlertView * alert1=[[[UIAlertView alloc] initWithTitle: @"EDAS-GN" message: @"重新启动数据采集器后参数才能生效，选择是重新启动" delegate:self cancelButtonTitle:@"否" otherButtonTitles:@"是",nil] autorelease];
		[alert1 show];
		//check if click yes
		//GetDocument()->ResetSys();
		
	}
	
}  





-(void) alertView: (UIAlertView *)alertView
clickedButtonAtIndex: (int) index
{
	if (alertView.title == @"EDAS-GN") {
		
		if(index==1)
			siteDoc.ResetSys();
	}/*else{
	  if(index==1)
	  siteDoc.SaveMessage();
	  else if(index==2)
	  siteDoc.m_old_msg="";
	  }*/
}
 
-(void) OnViewUp:(id)sender{
	[self.u_drawview OnViewUp];
}

-(void) OnViewDown:(id)sender{
	[self.u_drawview OnViewDown];
}

-(void) OnViewCenter:(id)sender{
    if (siteDoc.m_offset) {
        return;
    }
	[self.u_drawview OnViewCenter];
}
-(void) OnScantime: (id)sender{
 
	//show alertmessage to select scantime
	[self.u_drawview OnScantime];
}

-(void)OnViewOffset:(id)sender{
    if(!siteDoc.IsConnected())
		return;
    if (siteDoc.m_offset) {
        siteDoc.m_offset = NO;
        [sender setImage:[UIImage imageNamed:@"offset_btn"] forState:0];
    } else {
        siteDoc.m_offset = YES;
        [sender setImage:[UIImage imageNamed:@"offset_btns"] forState:0];
    }
    
}


@end
