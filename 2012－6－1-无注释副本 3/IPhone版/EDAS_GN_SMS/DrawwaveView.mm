//
//  DrawwaveView.m
//  EDAS_GN_SMS
//
//  Created by guan mofeng on 12-2-23.
//  Copyright 2012 北京. All rights reserved.
//
#import <CoreText/CoreText.h>
#import <CoreFoundation/CoreFoundation.h>
#import <CoreFoundation/CFDictionary.h>
#import "RootViewController.h"
#import "DrawwaveView.h"

#include "GlobalData.h"



char unit[][10]={"cnt","mV","um/s","gal","Mhz",""};

@implementation DrawwaveView

@synthesize u_statusbar;
@synthesize u_time_info;
@synthesize u_samp_info;
@synthesize u_save_info;
@synthesize m_rect;
//@synthesize m_xChar,m_yChar;
@synthesize   m_dspctl;
@synthesize m_real_time_status;
@synthesize m_rectangle;
@synthesize m_bclear,m_redraw_status;
@synthesize m_origin_y;
@synthesize m_orient;


- (id)initWithFrame:(CGRect)frame {
    
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code.
		m_dspctl = [[CDSPCTL alloc] init];
	 		
		m_xChar=72;
		m_yChar=1.5*FONTSIZE;
		
	//	[self InitSize : frame];
		 	 
		 		
		m_dspctl.scantime=30;
		m_dspctl.voffset=m_dspctl.hoffset=0;
		m_dspctl.maxrange_Hscroll=m_dspctl.maxrange_Vscroll=1000;
		m_dspctl.iHscrollPos=m_dspctl.iVscrollPos=0;
		
		m_dspctl.channelnum=3;
		
		m_real_time_status=0;
		//m_bk_color=[UIColor  whiteColor].CGColor;
		
		/*
		m_bk_color = [UIColor  colorWithRed : 0xf9 / 255.0
									   green : 0xd8 / 255.0
										blue : 0x67 / 255.0
									   alpha : 1 ]; (206,233,242);
		*/
		m_bk_color = [[UIColor alloc] initWithRed:206/255.0 green: 233/255.0 blue:242/255.0 alpha:1.0].CGColor;
		//m_bk_color=[bk CGColor];
		
		m_grid_color=[UIColor grayColor].CGColor;
		m_line_color=[UIColor redColor].CGColor;
		
		m_blabel = TRUE;
		
		
    }
    return self;
}


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
	TRACE("draw rect\n");
    // Drawing code.
	m_context= UIGraphicsGetCurrentContext();
	
	if(!siteDoc.m_bshow || m_bclear || siteDoc.m_pchannels==NULL){
		if(m_bclear)
		{
			[self InitSize : rect];
			if(rect.size.width < rect.size.height)
				m_orient=1;
			else m_orient=0;//landscape;
			
	 	 	if(siteDoc.m_pchannels!=NULL)
			{
				[self InitDsp];
				[self init_draw];
			}
			m_redraw_status=0;
			[self ResetPos];	
			m_bclear=FALSE;
			
		/*	CGContextSetLineWidth(m_context,2.0);
			CGContextSetStrokeColorWithColor(m_context,m_line_color);
			
			CGContextMoveToPoint(m_context, 72, 38);
			CGContextAddLineToPoint(m_context,85,38);
			CGContextAddLineToPoint(m_context,105,48);
			CGContextStrokePath(m_context);
*/			
		}
		return;
	}
	
	if(m_real_time_status!=0)
	{
	 	if(!CGRectEqualToRect(m_rect, rect ))	
		{
			[self InitSize : rect];
			 
			[self init_draw];
	 		m_rect = rect;
			m_redraw_status=0;
			[self ResetPos];
		}else {
		 	[self init_draw];
			[self DrawWave];
			}
	}else{
		
		if(!CGRectEqualToRect(m_rect,rect))
		{
			[self InitSize : rect];
			[self init_draw];
		}
	}

}
-(void) GetDataRange: (long *) data
{
	CHANNEL * pchan=siteDoc.m_pcur_chn;
	if(pchan->samp==0)
		return;
	
	pchan->offset=pchan->maxvalue=pchan->minvalue=data[0];
	for(int i=1;i<pchan->samp;i++)
	{
		if(pchan->maxvalue<data[i])
			pchan->maxvalue=data[i];
		if(pchan->minvalue>data[i])
			pchan->minvalue=data[i];
		pchan->offset+=data[i];
	}
	
	pchan->offset/=pchan->samp;
}

-(void)OnRunView: (int) bview{
	 	
	 if(bview==1){
		
	//	[self init_draw ];
		[self ResetPos];
		m_real_time_status=1;
		
		string s,s1;
		
		s=siteDoc.m_selsite->m_site.site;
		s+=" 实时波形监视";
		//title=s;
	}
	else {
		
		m_real_time_status = 0;
	//    self.title = siteDoc.m_selsite->m_site.site;
	}
}
 
-(void) InitSize : (CGRect )rect{
	CGRect r=m_rect;
	m_rect=CGRectMake(rect.origin.x,rect.origin.y+m_origin_y,rect.size.width,rect.size.height-m_origin_y );
	m_rectangle=CGRectMake(rect.origin.x+m_xChar,m_rect.origin.y,rect.size.width-m_xChar,m_rect.size.height-m_yChar-m_origin_y);
	if(u_statusbar==nil || !CGRectEqualToRect(r,m_rect) )
		[self CreateStatusBar ];
}

-(void) InitDsp{
	m_dspctl.iHscrollPos=m_dspctl.iVscrollPos=0;
	m_dspctl.hoffset=m_dspctl.voffset=0;
	m_dspctl.maxrange_Vscroll=10000;
	
	m_dspctl.totalchannel=0;
	CHANNEL * pchan=siteDoc.m_pchannels;
	while(pchan!=NULL){
		m_dspctl.totalchannel++;
		pchan=pchan->lpnext;
	}
	m_dspctl.channelnum=m_dspctl.totalchannel;
	[self CalChannelRange];
}

-(void) init_draw{
 //	u_time_info.text=@"test";
    self.backgroundColor = [UIColor whiteColor];
 

	CGContextSetFillColorWithColor(m_context,m_bk_color);
	CGContextAddRect(m_context,m_rectangle);
	CGContextDrawPath(m_context,kCGPathFillStroke);
	 
   
 	NSString * labels;
	
 	labels = [[NSString alloc] initWithFormat: @"0"];
	
	int y=m_rectangle.origin.y+m_rectangle.size.height ;

	//show text 
	[self Showlabel: m_rectangle.origin.x : y: labels];
	[labels release];

	labels = [[NSString alloc] initWithFormat: @"%dsec",m_dspctl.scantime];
	[self Showlabel: m_rect.size.width-[labels length]*FONTSIZE : y : labels];
	[labels release];
	 
	//CGContextTranslateCTM(m_context, 0, m_rect.size.height);
    //CGContextScaleCTM(m_context, 1.0, -1.0);
	
	
	//draw grid len	
	CGFloat phase[]={2.0f,2.0f};
 	CGContextSetLineWidth(m_context,1.0);
	//CGContextSetLineDash(m_context,3.0f,phase,2);
	CGContextSetStrokeColorWithColor(m_context,m_grid_color);
	
	int k,n,j,i,m;
//	k= m_dspctl.scantime*120/m_rectangle.size.width;
	k= m_dspctl.scantime*120/m_rectangle.size.width;
	if(k==0) 
		k=1;
	i = n = m_dspctl.scantime/k;
	if(k>1 && i>2) 
	{
		for( j=i-1,m=m_dspctl.scantime;j<i+2;j++)
		{
			k = m_dspctl.scantime/j;
			if(m>=m_dspctl.scantime-j*k) 
				m = m_dspctl.scantime-j*k,n=j;
		}
		i = n;
		k = m_dspctl.scantime/i;
		if(i>1){
			for(j=1;j<i;j++){
				CGContextMoveToPoint(m_context,(CGFloat)j*m_rectangle.size.width*k/m_dspctl.scantime+m_rectangle.origin.x,
									 (CGFloat)m_rectangle.origin.y);
				CGContextAddLineToPoint(m_context,(CGFloat)j*m_rectangle.size.width*k/m_dspctl.scantime+m_rectangle.origin.x,
									(CGFloat)m_rectangle.origin.y+m_rectangle.size.height);
				CGContextStrokePath(m_context);
				
				TRACE("x=%.3f y=%.3f -> %.3f %.3f\n",(CGFloat)j*m_rectangle.size.width*k/m_dspctl.scantime+m_rectangle.origin.x,
					  (CGFloat)m_rectangle.origin.y, (CGFloat)j*m_rectangle.size.width*k/m_dspctl.scantime+m_rectangle.origin.x,
					  (CGFloat)m_rectangle.origin.y+m_rectangle.size.height);
				
		//		trect.left = j*m_rectangle.size.width*k/m_dspctl.scantime+m_rectangle.origin.x;
		//		trect.top=m_rectangle.size.height+m_yChar;
		//		trect.bottom=trect.origin.y+m_yChar;
				//sprintf(ss,"%d",j*k);
				labels = [[NSString alloc] initWithFormat:@"%d",j*k];
				[self Showlabel : j*m_rectangle.size.width*k/m_dspctl.scantime+m_rectangle.origin.x : y : labels];
				[labels release];
				 //		trect.right = trect.origin.x+(ss.length()*m_xChar;
				
			//	pDC->DrawText(LPCTSTR(ss),ss.GetLength(),trect,DT_LEFT | DT_TOP);		
				
			}
		}
	}
	if(!m_blabel){
	//	oldpen->DeleteObject();
		return;
	}
	//show channel display range
	CHANNEL * pchan=siteDoc.m_pchannels;
	int height=m_rectangle.size.height/m_dspctl.channelnum;
	y=m_rectangle.origin.y;
	char ss[40];
	
	i=0;
	j=0;
	while(pchan!=NULL){
		if(!pchan->bShow){
			pchan=pchan->lpnext;
			i++;
			continue;
		}
		labels = [[NSString alloc] initWithUTF8String: pchan->chn_label];
		[self CreateStnButton : j : y+height/2: labels];
		[labels release];
		
		if(pchan->dsp.unit!=0)
			sprintf(ss,"%.3G %s",pchan->dsp.max,unit[pchan->dsp.unit]);
		else sprintf(ss,"%d %s",(long)pchan->dsp.max,unit[pchan->dsp.unit]);
		
		labels = [[NSString alloc ] initWithCString: ss ];
		
		[self Showlabel : 1: y: labels];
		[labels release];
		
		y+=height;
		/*if(pchan->dsp.unit!=0)
			sprintf(ss,"%.3G %s",pchan->dsp.min,unit[pchan->dsp.unit]);
		else sprintf(ss,"%d %s",(long)pchan->dsp.min,unit[pchan->dsp.unit]);
		pDC->textout(3,y-m_yChar,ss);
		*/
		if(pchan->dsp.unit!=0)
			sprintf(ss,"%.3G %s",pchan->dsp.min,unit[pchan->dsp.unit]);
		else sprintf(ss,"%d %s",(long)pchan->dsp.min,unit[pchan->dsp.unit]);
		
		labels = [[NSString alloc ] initWithCString: ss ];

		[self Showlabel : 3 : y-FONTSIZE : labels];
		[labels release];
		
		//UpdateTipText(pchan,i);
		i++;
		j++;
		pchan=pchan->lpnext;
		
	}
	[self layoutSubviews];
 
}
-(void) Showlabel : (int)x: (int)y : (NSString *) labels{
	
 
	 
	// create a font, quasi systemFontWithSize:24.0
	CTFontRef sysUIFont = CTFontCreateUIFontForLanguage(kCTFontSystemFontType,
														FONTSIZE , NULL);
	 
	// blue
	CGColorRef color = [UIColor blueColor].CGColor;
	
	// single underline
//	NSNumber *underline = [NSNumber numberWithInt:kCTUnderlineStyleSingle];
//	NSNumber align = [NSNumber numberWithInt: kCTRightTextAlignment];
	// pack it into attributes dictionary
	NSDictionary *attributesDict = [NSDictionary dictionaryWithObjectsAndKeys:
									(id)sysUIFont, (id)kCTFontAttributeName,
									color, (id)kCTForegroundColorAttributeName,nil];
								   //  align,(id)kCTParagraphStyleSpecifierAlignment,nil];
								//	underline, (id)kCTUnderlineStyleAttributeName, nil];
	
	// make the attributed string
	NSAttributedString *stringToDraw = [[NSAttributedString alloc] initWithString:labels  attributes:attributesDict];
	
	
	// flip the coordinate system
    // CGContextSetTextMatrix(m_context, CGAffineTransformIdentity);
	CGContextTranslateCTM(m_context, 0, m_rect.size.height);
    CGContextScaleCTM(m_context, 1.0, -1.0);
    
	// draw
  	CTLineRef line = CTLineCreateWithAttributedString( (CFAttributedStringRef)stringToDraw);
	CGContextSetTextPosition(m_context, (CGFloat)x,(CGFloat)m_rect.size.height-y-FONTSIZE);//y输出按照新的坐标系 
  	CTLineDraw(line, m_context);
	
//	// clean up
	CFRelease(line);
	CFRelease(sysUIFont);
	[stringToDraw release]; 
	//按照当前的 坐标系再次翻转 
	CGContextTranslateCTM(m_context, 0, m_rect.size.height);
    CGContextScaleCTM(m_context, 1.0, -1.0);
	 	 
	
	
}

-(void)CalChannelRange{
	m_dspctl.channel_range=m_rectangle.size.height/m_dspctl.channelnum;
	if(m_dspctl.channel_range==0)
		m_dspctl.channel_range=1;
	m_dspctl.maxrange_Vscroll=(m_dspctl.totalchannel-m_dspctl.channelnum)*m_dspctl.channel_range;//-rect.bottom+rect.left+1;
}


-(void) dsp_data: (long *)pin : (int) l
{
    int i;
    int dspoints;
	float offst;
	CHANNEL * p=siteDoc.m_pcur_chn;
	float rms;
	float factor;
    
	double scale=(double)m_rectangle.size.height/(m_dspctl.channelnum*(p->dsp.max-p->dsp.min));
	int width=m_rectangle.size.width;
    int ibase=(2*l+1)*m_rectangle.size.height/(m_dspctl.channelnum*2)+m_rectangle.origin.y;
	int	maxvalue=min(m_rectangle.size.height,m_rectangle.size.height*(l+1)/m_dspctl.channelnum+m_rectangle.origin.y);
	int minvalue=max(m_rectangle.origin.y,m_rectangle.origin.y+m_rectangle.size.height*l/m_dspctl.channelnum);
    
	if(!siteDoc.m_offset)
		rms=0.f;//不做偏移
	else rms=p->offset;//均值
    
	if(p->dsp.unit==1)
		factor=33554.432;//mv
	else if(p->dsp.unit==0)
		factor=1.f;
	else factor=p->factor;
    
	dspoints = m_dspctl.scantime * p->samp;
	offst=(p->dsp.max+p->dsp.min)/2;
    if( dspoints < 2 * width )
	{
		for(i=0;i<p->samp;i++)
		{
			p->point[p->position].x = m_rectangle.origin.x + width * p->position / dspoints;
			// pin[i]-rms: 扣除均值
			p->point[p->position].y=ibase-(int)(( ((float)pin[i]-rms) /factor-offst)*scale);
			
			if(p->point[p->position].y>maxvalue)
				p->point[p->position].y=maxvalue;
			else if(p->point[p->position].y<minvalue)
				p->point[p->position].y=minvalue;
			
			p->position++;
		}
		
	}else {
		if(p->position == 0)
		{
			p->position=1;
			p->point[0].x=m_rectangle.origin.x;
			p->point[1].x=m_rectangle.origin.x;
			
			
			p->xa = p->xi = pin[0] ;
			p->xaj = p->xij = 0;
		}
		if(p->cnt>=(p->position-2)*dspoints/2/width)
		{
			p->xa = p->xi = pin[0] ;
			p->xaj=p->xij=p->cnt;
		}
		for(i=0;i<p->samp;i++)
		{
			if(p->xa<=pin[i])
			{
				p->xa=pin[i];
				p->xaj=p->cnt;
			}
			if(p->xi>=pin[i])
			{
				p->xi=pin[i];
				p->xij=p->cnt;
			}
			p->cnt++;
			if(p->cnt>=p->position*dspoints/2/width)
			{
				//计算坐标前扣除均值: p->xi-rms  p->xa-rms
				if(p->xaj>p->xij){
					p->point[p->position].y=ibase-(int)(( ((float)p->xa-rms)/factor-offst)*scale);
					p->point[p->position-1].y=ibase-(int)(((float)p->xi/factor-offst)*scale);
				}else{
					p->point[p->position].y=ibase-(int)(( ((float)p->xi-rms) /factor-offst)*scale);
					p->point[p->position-1].y=ibase-(int)(( ((float)p->xa-rms) /factor-offst)*scale);
				}
				if(p->point[p->position].y>maxvalue)
					p->point[p->position].y=maxvalue;
				else if(p->point[p->position].y<minvalue)
					p->point[p->position].y=minvalue;
				
				if(p->point[p->position-1].y>maxvalue)
					p->point[p->position-1].y=maxvalue;
				else if(p->point[p->position-1].y<minvalue)
					p->point[p->position-1].y=minvalue;
				
				p->point[p->position-1].x=p->point[p->position].x=
				m_rectangle.origin.x + p->position / 2;
				p->position += 2;
				if(i+1<p->samp)
				{
					p->xa=p->xi=pin[i];
					p->xaj=p->xij=p->cnt;
				}
			}
			
			if(p->xaj>p->xij){
				p->point[p->position].y=ibase-(int)(( ((float)p->xa-rms) /factor-offst)*scale);
				p->point[p->position-1].y=ibase-(int)(( ((float)p->xi-rms) /factor-offst)*scale);
			}else{
				p->point[p->position].y=ibase-(int)(( ((float)p->xi-rms) /factor-offst)*scale);
				p->point[p->position-1].y=ibase-(int)(( ((float)p->xa-rms) /factor-offst)*scale);
			}
			if(p->point[p->position].y>maxvalue)
				p->point[p->position].y=maxvalue;
			else if(p->point[p->position].y<minvalue)
				p->point[p->position].y=minvalue;
			
			if(p->point[p->position-1].y>maxvalue)
				p->point[p->position-1].y=maxvalue;
			else if(p->point[p->position-1].y<minvalue)
				p->point[p->position-1].y=minvalue;
			
			p->point[p->position-1].x=p->point[p->position].x=
			m_rectangle.origin.x + p->position / 2;
		}
	}
}

-(void) clear_eara: (CGRect *) rect : (int)posi : (int)step
{
	CGRect r;
//	CGdiObject  *pBrushold,*pPenOld;
	int i,j,k,m,n;
	 
									 
	r.origin.y = rect->origin.y;
	r.size.height = rect->size.height;
	r.origin.x= posi*rect->size.width/m_dspctl.scantime + rect->origin.x;
	if(posi) r.origin.x += 2;
	r.size.width = (posi+step)*rect->size.width/m_dspctl.scantime + rect->origin.x + 5-r.origin.x;
	
	if(r.origin.x+r.size.width > rect->origin.x+rect->size.width) 
		r.size.width = rect->origin.x+rect->size.width +1 - r.origin.x;
	//draw rectangel r with blue
	 
	CGContextSetFillColorWithColor(m_context,m_bk_color);
	CGContextAddRect(m_context,r);
	CGContextDrawPath(m_context,kCGPathFillStroke);
	
	//draw grid len	
	CGFloat phase[]={2.0f,2.0f};
 	CGContextSetLineWidth(m_context,1.0);
	//CGContextSetLineDash(m_context,3.0f,phase,2);
	CGContextSetStrokeColorWithColor(m_context,m_grid_color);
	
	k = m_dspctl.scantime*120/(rect->size.width);
	if(k==0) k=1;
	i = n = m_dspctl.scantime/k;
	if(k>1 && i>2) for(j=i-1,m=m_dspctl.scantime;j<i+2;j++){
		k = m_dspctl.scantime/j;
		if(m>=m_dspctl.scantime-j*k) m = m_dspctl.scantime-j*k,n=j;
	}
	i = n;
	if(i==0)
		i=1;
	k = m_dspctl.scantime/i;
	if(i>1)for(j=1;j<i;j++){
		m=j*(rect->size.width)*k/m_dspctl.scantime+rect->origin.x;
		if(m>=r.origin.x && m<=r.origin.x+r.size.width){
			CGContextMoveToPoint(m_context,m,rect->origin.y);
			CGContextAddLineToPoint(m_context,m,rect->origin.y+rect->size.height);
			CGContextStrokePath(m_context);
			
		}
	}
	
}
-(void)  DrawWave
{
 
//	InitDC(pDC);
 	//CTime tmcvt;
	//CString s;
	string s;
	CGRect rgn;
//	CHANNEL * tmp;
	
	 
	CGContextSetLineWidth(m_context,1.0);
	CGContextSetStrokeColorWithColor(m_context,m_line_color);
	
 	int l=0;
	int i=0;
	CHANNEL *pt=siteDoc.m_pchannels;
	while(pt!=NULL)
	{
		if(pt->bShow)
		{
			CGContextMoveToPoint(m_context,pt->point[0].x,pt->point[0].y);
			for(int k=1;k<pt->position-1;k++)
				CGContextAddLineToPoint(m_context,pt->point[k].x,pt->point[k].y);
			CGContextStrokePath(m_context);
			 			
			if(m_redraw_status==1 && pt->position< pt->max_posi -10)
			{
				CGContextMoveToPoint(m_context, (CGFloat)pt->point[pt->position+1].x,(CGFloat)pt->point[pt->position+1].y);
			 
				for(int k=pt->position+1;k<pt->max_posi;k++)
					CGContextAddLineToPoint(m_context,(CGFloat)pt->point[k].x,(CGFloat)pt->point[k].y);
				CGContextStrokePath(m_context);	 
				
				rgn.origin.y=l*m_rectangle.size.height/m_dspctl.channelnum;
				rgn.origin.x=m_rectangle.origin.x;
				
			//	UpdateTipText(pt,i);
			}
			l++;
		}
		i++;
		pt=pt->lpnext;
    }
	 	
}
  
-(void) ResetPos{
	
	CHANNEL *pt=siteDoc.m_pchannels;
	while(pt!=NULL)
	{
		pt->position=pt->data_cnt=pt->max_posi=pt->cnt=0;
		pt=pt->lpnext;
	}
}

-(void) OnData: (int) start : (int )chn_sum
{
	//CTime tmcv;
	//CString s;
	string s;
	CHANNEL * tmp;
	long time;
	int l=start;
	
	 
	////CGContextSetLineWidth(m_context,2.0);
//	CGContextSetStrokeColorWithColor(m_context,[UIColor redColor].CGColor);
	
	tmp=siteDoc.m_pcur_chn;//save the first channel
	time=tmp->revblk.p_dsp->time;
	for(int j=0;j<chn_sum;j++)
	{
		int i=siteDoc.m_pcur_chn->position;
		siteDoc.m_pcur_chn->samp=tmp->samp;
		 
		[self dsp_data: &tmp->revblk.p_dsp->ds[1+siteDoc.m_pcur_chn->samp*j] : l];
	 	if(i>2)
			i-=2;
	//	CGFloat px,py;
	//	px=siteDoc.m_pcur_chn->point[i].x;
	//	py=siteDoc.m_pcur_chn->point[i].y;
		
		/*CGContextMoveToPoint(m_context,px,py);
		TRACE("x=%.3f y=%.3f\n",px,py);
		for(int k=i;k<siteDoc.m_pcur_chn->position-1;k++){
			px=siteDoc.m_pcur_chn->point[k].x;
			py=siteDoc.m_pcur_chn->point[k].y;
			CGContextAddLineToPoint(m_context,px,py);
			
			TRACE("x=%.3f y=%.3f\n",px,py);
		}
		
		CGContextStrokePath(m_context);
		 
		*/
		siteDoc.m_pcur_chn->data_cnt++;
		if(siteDoc.m_pcur_chn->data_cnt>=m_dspctl.scantime)
		{
			siteDoc.m_pcur_chn->data_cnt = 0;
			siteDoc.m_pcur_chn->max_posi = siteDoc.m_pcur_chn->position-1;
			siteDoc.m_pcur_chn->position = 0;
			siteDoc.m_pcur_chn->cnt=0;
			m_redraw_status = 1;
		}
		//Õ≥º∆±æ√Î ˝æ›µƒ◊Ó¥Û£¨◊Ó–°÷µ
		[self GetDataRange: &tmp->revblk.p_dsp->ds[1+tmp->samp*j]];
		siteDoc.m_pcur_chn->curtm=time;
		
		
		if(l==0){
			struct tm *ctm=localtime(&(siteDoc.m_pcur_chn->revblk.p_dsp->time));
			NSString * labels = [[NSString alloc] initWithFormat:@"%d-%d-%d %d:%d:%d",ctm->tm_year+1900,ctm->tm_mon+1,ctm->tm_mday,
								 ctm->tm_hour,ctm->tm_min,ctm->tm_sec];
		    u_time_info.text = labels;
			[labels release];
		}
		/*UpdateTipText(tmp,l);*/
		siteDoc.m_pcur_chn=siteDoc.m_pcur_chn->lpnext;
		l++;
	}
	siteDoc.m_pcur_chn=tmp;
}
///draw lines of siteDoc.m_pcur_chn, l: index channel at the screen
-(void) CreateStnButton  :(int)l : (int)y : (NSString *) title{ 
	CGFloat height[]={30.0f,40.0f};
	float ht = m_dspctl.channel_range / 2;
	if(height[m_orient] < ht) ht = height[m_orient];
	
	if(m_stn_btn[l]==nil)
	{
		CGRect rect = CGRectMake(m_rect.origin.x,y-ht/2,72,ht);
		m_stn_btn[l] = [UIButton buttonWithType: UIButtonTypeRoundedRect];
		[m_stn_btn[l] setFrame:rect]; 
		//[m_stn_btn[l] addTarget:self action:nil forControlEvents:UIControlEventTouchUpInside];
		
		[m_stn_btn[l] addTarget:self action:@selector(m_stn_btn_click:)forControlEvents:UIControlEventTouchUpInside];
	
		m_stn_btn[l].titleLabel.font  = [UIFont systemFontOfSize: 12];
		m_stn_btn[l].titleLabel.lineBreakMode   = UILineBreakModeTailTruncation;
		m_stn_btn[l].titleLabel.shadowOffset    = CGSizeMake (1.0, 0.0);
		

		[m_stn_btn[l] setTitle:title forState:UIControlStateNormal];
		[self addSubview: m_stn_btn[l]];
	}else {
		CGRect rect = CGRectMake(m_rect.origin.x,y-ht/2,72,ht);
		[m_stn_btn[l] setFrame:rect]; 
		[m_stn_btn[l] setTitle:title forState:UIControlStateNormal];
	//	[self layoutSubviews];
		
	}

	
	 
}

-(void) m_stn_btn_click :(id) sender {
	UIButton* btn = (UIButton* )sender;
	//NSString *title = sender.title;
	NSLog(@"%@ \n", btn.titleLabel.text);
	[RootViewController.U_RootViewController showDsp1ViewController: btn.titleLabel.text];
}

-(void) OnViewUp{
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
	[self ReDraw];	
	
}

-(void) OnViewDown {
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
	[self ReDraw];	
	
}
-(void) OnViewCenter{
	
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
	[self ReDraw];
}
-(void) OnScantime{
	//show alertmessage to select scantime
	 
	UIActionSheet * menu=[[UIActionSheet alloc] initWithTitle:@"选择显示时间" delegate:self cancelButtonTitle:@"取消" destructiveButtonTitle: nil otherButtonTitles: @"30秒", @"60秒", @"120秒", @"300秒",nil];
	
	[menu showInView:self.u_statusbar];
	 	
}

-(void) actionSheet: (UIActionSheet *)actionSheet
clickedButtonAtIndex: (NSInteger) buttonIndex{
	[actionSheet release];
	 
	switch(buttonIndex)
	{
		case 0:
			m_dspctl.scantime=30;
			break;
		case 1:
			m_dspctl.scantime=60;
			break;
		case 2:
			m_dspctl.scantime=120;
			break;
		case 3:
			m_dspctl.scantime=300;
			break;
		case 4://cancel
			return;
	}
	[self ReDraw];
}

 
-(void) ReDraw{
	m_bclear=TRUE;
	[self setNeedsDisplay];
}
-(void) CreateStatusBar{
	if(u_statusbar!=nil)
	{
		
		CGRect r = CGRectMake(0,m_rect.origin.y+m_rect.size.height-m_origin_y,m_rect.size.width,m_origin_y);

		[u_statusbar setFrame:r];
	}else{
		u_statusbar = [[UIToolbar alloc] initWithFrame: CGRectMake(0,m_rect.origin.y+m_rect.size.height-m_origin_y,m_rect.size.width,m_origin_y)];
		//NSMutableArray * baritems = [NSMutableArray array];
		u_statusbar.clipsToBounds=YES;
		
		UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, m_origin_y,m_rect.size.width/2,m_origin_y) ];
		label.textColor=[UIColor whiteColor];
		label.backgroundColor = [UIColor clearColor];
		self.u_samp_info=label;
		self.u_samp_info.text=@"sample rate";
		
		UIBarButtonItem * item_samp_info=[[UIBarButtonItem alloc] initWithCustomView: label];
		[label release];
		
		label = [[UILabel alloc] initWithFrame:CGRectMake(m_rect.size.width/2,m_origin_y,m_rect.size.width,m_origin_y) ];
		label.font=[UIFont systemFontOfSize:FONTSIZE];
		label.textColor=[UIColor whiteColor];
		label.backgroundColor = [UIColor clearColor];
		self.u_save_info=label;
		
		UIBarButtonItem * item_save_info=[[UIBarButtonItem alloc] initWithCustomView:  label];
		
		u_statusbar.items = [NSArray arrayWithObjects: item_samp_info,item_save_info,nil];
		[label release];
		[self addSubview:u_statusbar]; 
	
	}
}
//-(void) RelayoutStnButton;//relayout channel label;

- (void)dealloc {
	[m_dspctl dealloc];
	[u_time_info release];
	[u_samp_info release];
	[u_save_info release];
	[u_statusbar release];
	
	for(int i=0;i<13;i++)
	{
		if(m_stn_btn[i]!=nil)
			[m_stn_btn release];
	}
    [super dealloc];
}


@end
