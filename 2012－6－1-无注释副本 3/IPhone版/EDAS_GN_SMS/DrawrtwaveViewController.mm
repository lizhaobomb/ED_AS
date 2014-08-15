//
//  DrawrtwaveViewController.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-2-27.
//  Copyright 2012 北京. All rights reserved.
//

#import "DrawrtwaveViewController.h"
#include "GlobalData.h"

extern char sens_id[];
//char unit[][10]={"count","mV","um/s","gal","Mhz",""};


@implementation DrawrtwaveViewController

@synthesize  u_drawview;
//@synthesize u_time_info;
 
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	
	[NSTimer scheduledTimerWithTimeInterval:0.2f target:self selector:@selector(ontimer_display:) userInfo:nil repeats:YES];	
//	[NSTimer scheduledTimerWithTimeInterval:0.3f target:self selector:@selector(ontimer_savedata:) userInfo:nil repeats:YES];	
 	[NSTimer scheduledTimerWithTimeInterval:0.3f target:self selector:@selector(ontimer_checkcon:) userInfo:nil repeats:YES];
	//[NSDictionary dictionaryWithObjecsAndKeys:timer,@"draw",nil],repeats:YES];
	//	[NSTimer scheduledTimerWithTimeInterval:0.5f invocation: ontimer repeats: yes];
	 if(self.u_drawview==nil)
	 {
		 CGRect r=[[UIScreen mainScreen] applicationFrame];
		 
		 DrawwaveView * drawview;
		 drawview=[[DrawwaveView alloc] init];
		 
		 self.view = drawview;
	   	 self.u_drawview = (DrawwaveView *)self.view;

	
		 [self.u_drawview InitSize : r];
		 
		 [drawview release];
		 
		 [self.view setNeedsDisplay];
	 }
    [super viewDidLoad];
}
 
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return YES;// for supported orientations.
    //return (interfaceOrientation == UIInterfaceOrientationPortrait);
} 
-(void)drawRect:(CGRect)rect{
	CGContextRef c=UIGraphicsGetCurrentContext();
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
	//self.u_time_info=nil;
	//	self.u_samp_info=nil;
	//	self.u_conn_info=nil;
	//	self.u_drawview=nil;
	[super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
 
-(IBAction) changeTimescan: (id)sender{
}
-(IBAction) OnViewUp: (id)sender{
	float offset;
	CHANNEL *pchn=siteDoc.m_pchannels;
	int i=0;
	while(pchn!=NULL)//magnify wave
	{
		offset=(float)(pchn->dsp.max+pchn->dsp.min)/2;
		pchn->dsp.max=offset+(pchn->dsp.max-offset)/2;
		pchn->dsp.min=offset-(offset-pchn->dsp.min)/2;
		if(pchn->dsp.unit==0){
			pchn->dsp.max=ceil(pchn->dsp.max);
			pchn->dsp.min=ceil(pchn->dsp.min);
		}
		if(pchn->dsp.max==pchn->dsp.min)
		{
			pchn->dsp.max+=8;
			pchn->dsp.min-=8;
		}
		//UpdateTipText(pchn,i);
		i++;
		pchn=pchn->lpnext;
	}
	[u_drawview ReDraw];	
	
}

-(IBAction) OnViewDown: (id)sender{
	float offset;
	CHANNEL *pchn=siteDoc.m_pchannels;
	int i=0;
	float max,min;
	double maxv=2147483648.f;
	double minv=-2147483647.f;
	
	while(pchn!=NULL)//magnify wave
	{
		offset=(float)(pchn->dsp.max+pchn->dsp.min)/2;
		
		max=pchn->dsp.max;
		min=pchn->dsp.min;
		
		pchn->dsp.max=offset+(pchn->dsp.max-offset)*2;
		pchn->dsp.min=offset-(offset-pchn->dsp.min)*2;
		
		if(pchn->dsp.unit==0){
			pchn->dsp.max=ceil(pchn->dsp.max);
			pchn->dsp.min=ceil(pchn->dsp.min);
			if(pchn->dsp.max>maxv || pchn->dsp.min<minv){
				pchn->dsp.max=max;
				pchn->dsp.min=min;
			}
		}
		
		if(pchn->dsp.max==pchn->dsp.min)
		{
			pchn->dsp.max+=8;
			pchn->dsp.min-=8;
		}
		//	UpdateTipText(pchn,i);
		i++;
		pchn=pchn->lpnext;
	}
	[u_drawview ReDraw];	
	
}
-(IBAction) OnViewCenter: (id)sender{
	
	CHANNEL *pchn=siteDoc.m_pchannels;
	float factor;
	int i=0;
	while(pchn!=NULL)//magnify wave
	{
		
		if(pchn->dsp.unit==1){
			if(pchn->chn_id<6)
				factor=83886080 /(1000*pchn->factor);//count/mV  83886080=2^23
			else if(pchn->chn_id>=6 && pchn->chn_id<12)
				factor=1.f/2.384/(1000*1000);
			else factor=pchn->factor;
		}else if(pchn->dsp.unit==0)
			factor=1.f;
		else  if(pchn->dsp.unit>4 && (pchn->chn_id>=6 && pchn->chn_id<12) )//◊‘∂®“Âµƒµ•Œª,œﬁ÷∆‘⁄∏®÷˙Õ®µ¿
			factor=1.f/(2.384/(1000*1000))*pchn->factor;//2.384uV/count
		else factor=pchn->factor; 
		
		
		float	range=(float)(fabs(pchn->dsp.max-pchn->dsp.min)/2);
		pchn->dsp.max=(float)pchn->offset/factor+range;
		pchn->dsp.min=(float)pchn->offset/factor-range;
		if(pchn->dsp.unit==0){
			pchn->dsp.max=ceil(pchn->dsp.max);
			pchn->dsp.min=ceil(pchn->dsp.min);
		}
		if(pchn->dsp.max==pchn->dsp.min)
		{
			pchn->dsp.max+=8;
			pchn->dsp.min-=8;
		}
		//UpdateTipText(pchn,i);
		i++;
		pchn=pchn->lpnext;
	}
	[u_drawview ReDraw];
}

-(void) OnCloseCon{
	[u_drawview OnRunView : 0];
}
-(void) SetContext{
	CGContextRef context = UIGraphicsGetCurrentContext();
	[self.u_drawview SetContextRef : context];
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
		[u_drawview OnRunView:1];
		UIAlertView * alert=[[[UIAlertView alloc] initWithTitle: @"Message" message: @"已经建立连接" delegate:nil cancelButtonTitle: @"Close" otherButtonTitles: nil ] autorelease];
		
		[alert show];	
		[timer invalidate];
	}
	else if(ret==-1){
		UIAlertView * alert=[[[UIAlertView alloc] initWithTitle: @"Message" message: @"网络连接超时" delegate:nil cancelButtonTitle: @"Close" otherButtonTitles: nil ] autorelease];
		
		[alert show];	
		[timer invalidate];		 		 
	}
	//show model warning dialog
	//else 
}

-(void ) ontimer_display: (NSTimer * )timer{
	//siteview.displaydata
	[self DisplayData];
	
	/*	if(u_count==0)
	 u_count=1;
	 else u_count=0;
	 
	 NSString * tmp=[[NSString alloc] initWithFormat:@"%d", u_count];
	 self.u_time_info.text =tmp;
	 [tmp release];*/
}

-(void ) ontimer_savedata: (NSTimer * )timer{
	siteDoc.SaveData();
}

-(void ) ontimer_checkcon: (NSTimer * )timer{
	
	if( siteDoc.CheckConStat() ==0)
	{
		//	char temp[200];
		
		//strcpy(temp,siteDoc.m_selsite->m_site.net_con.host);
	 	NSString *msg =[[NSString alloc] initWithFormat:@"%@ 网络中断，重新连接...",siteDoc.m_selsite->m_site.net_con.host];
		UIAlertView * alert=[[[UIAlertView alloc] initWithTitle: @"Message" message: msg delegate:nil cancelButtonTitle: @"Close" otherButtonTitles: nil ] autorelease];
		
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
	BOOL bshow;
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
		//if (self.u_samp_info!=nil) {
		//	[self.u_samp_info release];
		//}
		//self.u_samp_info.text = [[NSString alloc]  initWithUTF8String:s.c_str()];
		
		
	}
	/*
	 /*CDC *pDC=GetDC();
	 
	 InitDC(pDC);
	 */
	 if(u_drawview.m_real_time_status){
		 //œ‘ æ≤®–Œ£¨Õ≥º∆œ‘ æ∑∂Œß≤¢À¢–¬
		 rg=u_drawview.m_rectangle;
		 
		 pchn=siteDoc.m_pchannels;
		 plocal=siteDoc.m_selsite->m_plocal;
		 cn=0;	
		 while(pchn!=NULL)
		 {
			 rg.origin.y=l*u_drawview.m_rectangle.size.height/u_drawview.m_dspctl.channelnum;
			 rg.size.height=(l+1)*u_drawview.m_rectangle.size.height/u_drawview.m_dspctl.channelnum+1-rg.origin.y;
			 if(cn==0){
				 step=0;
				 pdat=pchn->revblk.p_dsp;
				 while(pdat->flg>pdat->pb->flg){
					 step++;
					 pdat=pdat->pn;
				 }
			 }	
		 
			 int posi=pchn->data_cnt;
			 for(int k=0;k<step;k++){
				[u_drawview clear_eara: &rg: posi: 1];
				 posi++;
				 if(posi>=u_drawview.m_dspctl.scantime)
					 posi=0;
			 }
	 
			 l++;
	 
			 pchn=pchn->lpnext;
			 cn++;
	 //TRACE( "cn = %d %d\n",cn,plocal->chn_sum);
			 if(cn==plocal->chn_sum)
			 {	
				 cn=0;
				 plocal=plocal->lpnext;
			 }
		 }
	 } 
	
	pchn=siteDoc.m_pchannels;
	plocal=siteDoc.m_selsite->m_plocal;
	l=0;
	//œ‘ æ≤®–Œ£¨≤Œ ˝°¢±Í∂®–≈œ¢
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
			}
			if(pchn->cali_flag!=pchn->revblk.p_dsp->cali_flag || pchn->revblk.p_dsp->cali_flag!=0 ){
				califlag=1;
				pchn->cali_flag=pchn->revblk.p_dsp->cali_flag;
			}
			
			pchn->revblk.p_dsp=pchn->revblk.p_dsp->pn;
		}
		//œ‘ æ≤Œ ˝
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
		//	if(u_time_info!=nil)
		//		[u_time_info release];
	//	u_time_info.text = [[NSString alloc] initWithUTF8String:calistr];
		
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
	
}

-(void) ShowCaliflag: (int) cali_flag : (int) local_id : (char *) calistr
{
	char tmp[40];
	if(cali_flag==4)
		strcpy(tmp,"脉冲标定");
	else if(cali_flag==2)
		strcpy(tmp,"正弦标定");
	else if(cali_flag==6)
		strcpy(tmp,"伪随机码标定");
	else return;
	
	sprintf(calistr,"地震计：%d %s", sens_id[local_id],tmp);
	
}

-(void) OnMessage  
{
	string info;
	
	info="";
	
	siteDoc.ProcessMessage((char *)siteDoc.m_pcur_chn->cmdblk.p_dsp->pb->ds,info);
	if(info.length()==0)
		return;
	//	TRACE("%s",info.c_str());
	
	siteDoc.m_old_msg = siteDoc.m_old_msg + info;
	siteDoc.m_msg = info;
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

- (void)dealloc {
//	[u_time_info release];
	
	[super dealloc];
}



@end
