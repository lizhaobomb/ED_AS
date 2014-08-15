// sitemagView.cpp : implementation of the CSitemagView class
//

#include "stdafx.h"
#include "sitemag.h"

#include "sitemagDoc.h"
#include "sitemagView.h"
#include "dspdlg.h"
#include "mainfrm.h"
#include "ctrlwavedlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSitemagApp theApp;
extern char sens_id[];
CCtrlWaveDlg *m_ctrwdlg;
//显示参数
CShowMsg m_msg;
char unit[][10]={"count","mV","um/s","gal","Mhz",""};

void ShowCtrlWave(CPoint &point,CView * pview,CHANNEL *pchan)
{
	POINT p;
	p.x=point.x;
	p.y=point.y;
	ClientToScreen(pview->m_hWnd,&p);
	CSitemagView *dview=(CSitemagView *)pview;
	if(!dview->m_bctrlwave)
	{
		m_ctrwdlg=new CCtrlWaveDlg;
		m_ctrwdlg->m_view=dview;
		m_ctrwdlg->m_pchannel=pchan;
		m_ctrwdlg->Create(IDD_DIALOG_CTRLWAVE,NULL);
		m_ctrwdlg->ShowWindow(SW_SHOW);
		dview->m_bctrlwave=TRUE;
	}else m_ctrwdlg->m_pchannel=pchan;

	CRect rect;
	m_ctrwdlg->GetWindowRect(rect);
	m_ctrwdlg->MoveWindow(p.x,p.y,rect.Width(),rect.Height());
	m_ctrwdlg->ReDraw();
}

void GetDataRange(long * data,CHANNEL *pchan)
{
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
/////////////////////////////////////////////////////////////////////////////
// CSitemagView

IMPLEMENT_DYNCREATE(CSitemagView, CView)

BEGIN_MESSAGE_MAP(CSitemagView, CView)
	//{{AFX_MSG_MAP(CSitemagView)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_RUN_VIEW, OnRunView)
	ON_UPDATE_COMMAND_UI(ID_RUN_VIEW, OnUpdateRunView)
	ON_COMMAND(ID_VIEW_DISPLAY, OnViewDisplay)
	ON_COMMAND(ID_VIEW_CENTER, OnViewCenter)
	ON_COMMAND(ID_VIEW_DOWN, OnViewDown)
	ON_COMMAND(ID_VIEW_UP, OnViewUp)
	ON_COMMAND(ID_VIEW_MSG, OnViewMsg)
	ON_COMMAND(ID_VIEW_SHOWLABEL, OnViewShowlabel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWLABEL, OnUpdateViewShowlabel)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSitemagView construction/destruction

CSitemagView::CSitemagView()
{
	// TODO: add construction code here
	m_gridcolor=RGB(255,255,255);
	m_lncolor=RGB(255,0,0);
	m_badlncolor=RGB(0,0,0);
	m_bkcolor=RGB(206,233,242);

	m_pBrush=new CBrush;
	m_pBrush->CreateSolidBrush(m_bkcolor);
	m_pGridPen=new CPen;
	m_pGridPen->CreatePen(PS_SOLID,1,m_gridcolor);
	m_pPen=new CPen;
	m_pPen->CreatePen(PS_SOLID,1,m_lncolor);
	m_pBadPen=new CPen;
	m_pBadPen->CreatePen(PS_SOLID,1,m_badlncolor);

	m_dspctl.scantime=30;
	m_dspctl.voffset=m_dspctl.hoffset=0;
	m_dspctl.maxrange_Hscroll=m_dspctl.maxrange_Vscroll=1000;
	m_dspctl.iHscrollPos=m_dspctl.iVscrollPos=0;

	m_selchannel=NULL;
	m_real_time_status=0;
	m_dspctl.channelnum=3;

	m_Tips=NULL;
	m_stnbtn=NULL;
	m_bctrlwave=FALSE;
	m_msg.m_bcreated=FALSE;
	m_blabel=FALSE;

	m_sfont.lfHeight=-13;
	m_sfont.lfWidth=0;
	m_sfont.lfEscapement=0;
	m_sfont.lfOrientation=0;
	m_sfont.lfWeight=400;
	m_sfont.lfItalic=0;
	m_sfont.lfUnderline=0;
	m_sfont.lfStrikeOut=0;
	m_sfont.lfCharSet=134;
	m_sfont.lfOutPrecision=3;
	m_sfont.lfClipPrecision=2;
	m_sfont.lfQuality=1;
	m_sfont.lfPitchAndFamily=2;
	lstrcpy(m_sfont.lfFaceName,"宋体");
		 
	m_font.CreateFontIndirect(&m_sfont);
}

CSitemagView::~CSitemagView()
{
	if(m_Tips!=NULL)
	{
		delete m_Tips;
		m_Tips=NULL;
	}
	if(m_pBrush!=NULL){
		delete m_pBrush;
		m_pBrush=NULL;
	}
	if(m_pPen!=NULL){
		delete m_pPen;
		m_pPen=NULL;
	}
	if(m_pGridPen!=NULL){
		delete m_pGridPen;
		m_pGridPen=NULL;
	}
	if(m_pBadPen!=NULL){
		delete m_pBadPen;
		m_pBadPen=NULL;
	}
}

BOOL CSitemagView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSitemagView drawing

void CSitemagView::OnDraw(CDC* pDC)
{
	CSitemagDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CRect rect;

	GetClientRect(& rect);
	m_rectangle.top=rect.top;
	m_rectangle.left=m_tiprect.right;
	m_rectangle.bottom=rect.bottom-2*m_yChar;
	m_rectangle.right=rect.right;
	
	if(!GetDocument()->m_bshow || GetDocument()->m_pchannels==NULL)
		return;

	if(m_real_time_status!=0)
	{
		if(m_rect!=rect )	
		{
			init_draw(pDC,rect);
			m_rect = rect;
			m_redraw_status=0;
			ResetPos();
		}else {
			init_draw(pDC,rect);
			DrawWave();
		}
	}else{
		init_draw(pDC,rect);
		if(m_rect!=rect)
			m_rect = rect;
	}
	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CSitemagView printing

BOOL CSitemagView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSitemagView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSitemagView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSitemagView diagnostics

#ifdef _DEBUG
void CSitemagView::AssertValid() const
{
	CView::AssertValid();
}

void CSitemagView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSitemagDoc* CSitemagView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSitemagDoc)));
	return (CSitemagDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSitemagView message handlers


void CSitemagView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if(GetDocument()!=NULL)
	{
		SetTimer( 2,200,NULL);//display data
		SetTimer( 3,300,NULL);//save data
		SetTimer( 4,300,NULL);//check connection state
		if(GetDocument()->m_bshow)
		{
			CreateStnButton();
			if(GetDocument()->m_pchannels!=NULL)
				OnDisplay();
		}

	}

}


void CSitemagView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!GetDocument()->m_bshow)
		return;
	// TODO: Add your message handler code here
	CDC *pDC=GetDC();
	InitDC(pDC);
	ReleaseDC(pDC);

	GetClientRect(&m_tiprect);
	m_tiprect.right=13*m_xChar;
	m_tiprect.bottom-=2*m_yChar;

	CRect rect;
	GetClientRect(&rect);
	m_rectangle.top=rect.top;
	m_rectangle.left=m_tiprect.right;
	m_rectangle.bottom=rect.bottom-2*m_yChar;
	m_rectangle.right=rect.right;

	RelayoutStnButton();
	ReDraw();	
}

void CSitemagView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(GetDocument()->m_bshow)
	{
		if(nIDEvent==2)
			DisplayData();
		else if(nIDEvent==3)
			GetDocument()->SaveData();
		else if(nIDEvent==4)
			GetDocument()->CheckConStat();
		else if(nIDEvent==5)
			GetDocument()->ProcessCon();
	}	
	CView::OnTimer(nIDEvent);
}


void CSitemagView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(lHint==ID_CHANGESITE){

		OnDisplay();
	}else if(lHint==ID_DISCON)
		m_real_time_status=0;
	else if(lHint==ID_CHECKCON)
		SetTimer(5,100,NULL);
	else if(lHint==ID_KILLTIMER)
		KillTimer(5);
	
}
void CSitemagView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	InitDC(pDC);
	
	CView::OnPrepareDC(pDC, pInfo);
}

void CSitemagView::InitDC(CDC *pDC)
{
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	m_xChar=tm.tmAveCharWidth;
	m_yChar=tm.tmHeight+tm.tmExternalLeading;
	pDC->SetBkMode(TRANSPARENT);
}
void CSitemagView::InitDsp()
{
    m_dspctl.iHscrollPos=m_dspctl.iVscrollPos=0;
	m_dspctl.hoffset=m_dspctl.voffset=0;
	m_dspctl.maxrange_Vscroll=10000;
	
	m_dspctl.totalchannel=0;
	CHANNEL * pchan=GetDocument()->m_pchannels;
	while(pchan!=NULL){
		m_dspctl.totalchannel++;
		pchan=pchan->lpnext;
	}
	m_dspctl.channelnum=m_dspctl.totalchannel;
	CalChannelRange();
	GetDocument()->m_bshow=FALSE;
	SetScrollRange(SB_VERT,0,m_dspctl.maxrange_Vscroll,FALSE);
	SetScrollPos(SB_VERT,m_dspctl.iVscrollPos);
	GetDocument()->m_bshow=TRUE;

}

void CSitemagView::ResetPos()
{
	CHANNEL *pt=GetDocument()->m_pchannels;
	while(pt!=NULL)
	{
		pt->position=pt->data_cnt=pt->max_posi=pt->cnt=0;
		pt=pt->lpnext;
	}
}

void CSitemagView::init_draw(CDC *pDC,CRect &rect)
{
	CFont * oldfont=NULL;

	oldfont=pDC->SelectObject(&m_font);

	//set label
	CBrush *pOldBrush=pDC->SelectObject(m_pBrush);
	CGdiObject *pPenOld = pDC->SelectStockObject(NULL_PEN);

	pDC->Rectangle(m_rectangle);
	pOldBrush->DeleteObject();
		

	CRect trect;
	trect.left=rect.left;
	trect.right=m_rectangle.left;

	CPen *oldpen=pDC->SelectObject(m_pGridPen);
	CString ss;
	ss.Format("%d",0);
	int y=m_rectangle.bottom+m_yChar;
	pDC->TextOut(m_rectangle.left,y,ss);

	ss.Format("%dSec",m_dspctl.scantime);
	pDC->TextOut(m_rectangle.right-(ss.GetLength()+1)*m_xChar,y,ss);

	int k,n,i;
	k= m_dspctl.scantime*120/m_rectangle.Width();
	if(k==0) k=1;
	i = n = m_dspctl.scantime/k;
	if(k>1 && i>2) 
	{
		for(int j=i-1,m=m_dspctl.scantime;j<i+2;j++)
		{
			k = m_dspctl.scantime/j;
			if(m>=m_dspctl.scantime-j*k) 
				m = m_dspctl.scantime-j*k,n=j;
		}
		i = n;
		k = m_dspctl.scantime/i;
		if(i>1)
			for(j=1;j<i;j++){
				pDC->MoveTo(j*m_rectangle.Width()*k/m_dspctl.scantime+m_rectangle.left,m_rectangle.top);
				pDC->LineTo(j*m_rectangle.Width()*k/m_dspctl.scantime+m_rectangle.left,m_rectangle.bottom);
			
				trect.left = j*m_rectangle.Width()*k/m_dspctl.scantime+m_rectangle.left;
				trect.top=m_rectangle.bottom+m_yChar;
				trect.bottom=trect.top+m_yChar;
				ss.Format("%d",j*k);
				trect.right = trect.left+(ss.GetLength()+1)*m_xChar;

				pDC->DrawText(LPCTSTR(ss),ss.GetLength(),trect,DT_LEFT | DT_TOP);		
		
			}
	}
	if(oldfont!=NULL)
		pDC->SelectObject(oldfont);

	if(!m_blabel){
		oldpen->DeleteObject();
		return;
	}
	//show channel display range
	CHANNEL * pchan=GetDocument()->m_pchannels;
	int height=m_rectangle.Height()/m_dspctl.channelnum;
	y=m_rectangle.top;

	i=0;
	while(pchan!=NULL){
		if(!pchan->bShow){
			pchan=pchan->lpnext;
			i++;
			continue;
		}
		if(pchan->dsp.unit!=0)
			ss.Format("%.3G %s",pchan->dsp.max,unit[pchan->dsp.unit]);
		else ss.Format("%d %s",(long)pchan->dsp.max,unit[pchan->dsp.unit]);
		pDC->TextOut(1,y,ss);

		y+=height;
		if(pchan->dsp.unit!=0)
			ss.Format("%.3G %s",pchan->dsp.min,unit[pchan->dsp.unit]);
		else ss.Format("%d %s",(long)pchan->dsp.min,unit[pchan->dsp.unit]);
		pDC->TextOut(3,y-m_yChar,ss);
		
		UpdateTipText(pchan,i);
		i++;
		pchan=pchan->lpnext;
	
	}
	oldpen->DeleteObject();
	
}


void CSitemagView::CalChannelRange()
{
	m_dspctl.channel_range=m_rectangle.Height()/m_dspctl.channelnum;
	if(m_dspctl.channel_range==0)
		m_dspctl.channel_range=1;
	m_dspctl.maxrange_Vscroll=(m_dspctl.totalchannel-m_dspctl.channelnum)*m_dspctl.channel_range;//-rect.bottom+rect.left+1;
}


void CSitemagView::dsp_data(long * pin,CHANNEL *p,int l)
{
    int i;
    int dspoints;
	float offst;

	double scale=(double)m_rectangle.Height()/(m_dspctl.channelnum*(p->dsp.max-p->dsp.min));
	int width=m_rectangle.Width();
    int ibase=(2*l+1)*m_rectangle.Height()/(m_dspctl.channelnum*2)+m_rectangle.top;
	int	maxvalue=min(m_rectangle.bottom,m_rectangle.Height()*(l+1)/m_dspctl.channelnum+m_rectangle.top);
	int minvalue=max(m_rectangle.top,m_rectangle.top+m_rectangle.Height()*l/m_dspctl.channelnum);
	
	float factor;

	if(p->dsp.unit==1){
		if(p->chn_id<6)
			factor=8388608/(1000*p->factor);//count/v  8388608=2^23 
		else if(p->chn_id>=6 && p->chn_id<12)
			factor=1.f/(2.384/(1000*1000));//2.384uV/count
		else factor=p->factor;
	}else if(p->dsp.unit==0)
		factor=1.f;
	else  if(p->dsp.unit>4 && (p->chn_id>=6 && p->chn_id<12) )//自定义的单位,限制在辅助通道
		factor=1.f/(2.384/(1000*1000))*p->factor;//2.384uV/count
	else factor=p->factor; 

	dspoints = m_dspctl.scantime * p->samp;
	offst=(p->dsp.max+p->dsp.min)/2;
    if( dspoints < 2 * width ) 
	{
		for(i=0;i<p->samp;i++)
		{
			p->point[p->position].x = m_rectangle.left + width * p->position / dspoints;
			p->point[p->position].y=ibase-(int)(((float)pin[i]/factor-offst)*scale);
		
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
			p->point[0].x=m_rectangle.left;
			p->point[1].x=m_rectangle.left;
			
			
			p->xa = p->xi = pin[0];
			p->xaj = p->xij = 0;
		}
		if(p->cnt>=(p->position-2)*dspoints/2/width)
		{
			p->xa = p->xi = pin[0];
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
				if(p->xaj>p->xij){
					p->point[p->position].y=ibase-(int)(((float)p->xa/factor-offst)*scale);
					p->point[p->position-1].y=ibase-(int)(((float)p->xi/factor-offst)*scale);
				}else{
					p->point[p->position].y=ibase-(int)(((float)p->xi/factor-offst)*scale);
					p->point[p->position-1].y=ibase-(int)(((float)p->xa/factor-offst)*scale);
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
					m_rectangle.left + p->position / 2;
				p->position += 2;
				if(i+1<p->samp)
				{
					p->xa=p->xi=pin[i];
					p->xaj=p->xij=p->cnt;
				}
			}

			if(p->xaj>p->xij){
				p->point[p->position].y=ibase-(int)(((float)p->xa/factor-offst)*scale);
				p->point[p->position-1].y=ibase-(int)(((float)p->xi/factor-offst)*scale);
			}else{
				p->point[p->position].y=ibase-(int)(((float)p->xi/factor-offst)*scale);
				p->point[p->position-1].y=ibase-(int)(((float)p->xa/factor-offst)*scale);
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
				m_rectangle.left + p->position / 2;
		}
	}

}
void CSitemagView::clear_eara(CDC * pdc,CRect * rect,int posi,int step)
{
	CRect r;
	CGdiObject  *pBrushold,*pPenOld;
	int i,j,k,m,n;
	
	r.top = rect->top;
	r.bottom = rect->bottom;
	r.left = posi*rect->Width()/m_dspctl.scantime + rect->left;
	if(posi) r.left += 2;
	r.right = (posi+step)*rect->Width()/m_dspctl.scantime + rect->left + 5;

	if(r.right > rect->right) r.right = rect->right+1;

	pBrushold = pdc->SelectObject(m_pBrush);
	pPenOld = pdc->SelectStockObject(NULL_PEN);
	pdc->Rectangle(r);
	pPenOld=pdc->SelectObject(m_pGridPen);

	k = m_dspctl.scantime*120/(rect->Width());
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
		m=j*(rect->Width())*k/m_dspctl.scantime+rect->left;
		if(m>=r.left && m<=r.right){
			pdc->MoveTo(m,rect->top);
			pdc->LineTo(m,rect->bottom);
			}
		}
	
	pdc->SelectObject(pPenOld);
	pdc->SelectObject(pBrushold);
}
void CSitemagView::DrawWave()
{
	CDC *pDC=GetDC();
	InitDC(pDC);
	CPen *pPenOld=pDC->SelectObject(m_pPen);
	CTime tmcvt;
	CString s;
	CRect rgn;
	int l=0;
	int i=0;
	CHANNEL *pt=GetDocument()->m_pchannels;
	while(pt!=NULL)
	{
		if(pt->bShow)
		{
			pDC->Polyline(pt->point,pt->position-1);

			if(m_redraw_status==1 && pt->position< pt->max_posi -10)
			{
				pDC->MoveTo(pt->point[pt->position+1]);
				for(int k=pt->position+1;k<pt->max_posi;k++)
					pDC->LineTo(pt->point[k]);

				rgn.top=l*m_rectangle.Height()/m_dspctl.channelnum;
				rgn.left=m_rectangle.left;

				UpdateTipText(pt,i);
			}
			l++;
		}
		i++;
		pt=pt->lpnext;
    }
	pDC->SelectObject(pPenOld);
    ReleaseDC(pDC);

}

void CSitemagView::ReDraw()
{
	CDC *pDC=GetDC();
	InitDC(pDC);

	CRect rect;
	GetClientRect(rect);
	GetDocument()->m_bshow=FALSE;
	Invalidate(TRUE);
	GetDocument()->m_bshow=TRUE;
	init_draw(pDC,rect);
	m_redraw_status=0;
	ResetPos();	
	ReleaseDC(pDC);
}

void CSitemagView::DisplayData()
{

	CTime tmcv;
	CString s;
	CRect rg;
	CPen *oldpen;
	CHANNEL *pchn;
	SHDAT1 * pdat;
	LOCALREVBLK * plocal;
	int step;
	short *p;
	int cn,j;
	CString s1;
	BOOL bshow;
	CString calistr;
	int califlag;
	
	int l=0;
//	TRACE("display\n");

	if(GetDocument()->m_selsite==NULL)
		return;
	if(!GetDocument()->IsConnected())
		return;
//show sample rate
	plocal=GetDocument()->m_selsite->m_plocal;
	s.LoadString(IDS_SMPRATE);
	s+=": ";
	bshow=0;
	while(plocal!=NULL)
	{
		if(plocal->samp>0){
			s1.Format("%d ",plocal->samp);
			s+=s1;
			bshow=1;
		}
		plocal=plocal->lpnext;
	}
	if(bshow){
		CStatusBar *pBar=(CStatusBar *)theApp.m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
		if(pBar==NULL)
			return;
		pBar->SetPaneText(2,s,TRUE);
	}
	CDC *pDC=GetDC();

	InitDC(pDC);

	if(m_real_time_status){
		//显示波形，统计显示范围并刷新
		rg=m_rectangle;
		
		pchn=GetDocument()->m_pchannels;
		plocal=GetDocument()->m_selsite->m_plocal;
		cn=0;	
		while(pchn!=NULL)
		{
			rg.top=l*m_rectangle.Height()/m_dspctl.channelnum;
			rg.bottom=(l+1)*m_rectangle.Height()/m_dspctl.channelnum+1;
			if(cn==0){
				step=0;
				pdat=pchn->revblk.p_dsp;
				while(pdat->flg>pdat->pb->flg){
				/*while(pdat!=NULL && pdat->pn!=NULL){
					pdat=pdat->pn;
					p=(short *)pdat->ds;
					if(p[1]==0x55ce || p[1]==0x55AA || p[1]==0x55BB || p[1]==0x55CC || p[1]==0x55DD)
						step++;*/
					step++;
					pdat=pdat->pn;
				}
			}
					
			int posi=pchn->data_cnt;
			for(int k=0;k<step;k++){
				clear_eara(pDC,&rg,posi,1);
				posi++;
				if(posi>=m_dspctl.scantime)
					posi=0;
			}
		
			l++;
				
			pchn=pchn->lpnext;
			cn++;
			TRACE( "cn = %d %d\n",cn,plocal->chn_sum);
			if(cn==plocal->chn_sum)
			{
				cn=0;
				plocal=plocal->lpnext;
			}
		}
	}
	oldpen=pDC->SelectObject(m_pPen);

	pchn=GetDocument()->m_pchannels;
	plocal=GetDocument()->m_selsite->m_plocal;
	l=0;
	//显示波形，参数、标定信息
	califlag=0;
	calistr="";
	while(plocal!=NULL && pchn!=NULL)
	{
	
		while(pchn->revblk.p_dsp->flg>pchn->revblk.p_dsp->pb->flg)
		{
			if(m_real_time_status)//显示波形
				OnData(pchn,l,plocal->chn_sum,pDC);
			
			if(pchn->cali_flag!=pchn->revblk.p_dsp->cali_flag || pchn->revblk.p_dsp->cali_flag!=0 ){
				califlag=1;
				pchn->cali_flag=pchn->revblk.p_dsp->cali_flag;
			}

			pchn->revblk.p_dsp=pchn->revblk.p_dsp->pn;
		}
		//显示参数
		while(pchn->cmdblk.p_dsp->flg>pchn->cmdblk.p_dsp->pb->flg)
		{	
			pchn->cmdblk.p_dsp=pchn->cmdblk.p_dsp->pn;
			OnMessage(pchn);
		}
		//显示标定标志
		ShowCaliflag(pchn->cali_flag,plocal->local_id,calistr);

		l+=plocal->chn_sum;
		for(j=0;j<plocal->chn_sum;j++)
			pchn=pchn->lpnext;
		plocal=plocal->lpnext;
	}
	if(califlag)
		((CMainFrame *)theApp.m_pMainWnd)->m_wndToolBar.m_edit_cal.SetWindowText(calistr);
//	TRACE("%s\n",calistr);

	pDC->SelectObject(oldpen);
	ReleaseDC(pDC);

}
void CSitemagView::ShowCaliflag(int cali_flag,int local_id,CString & calistr)
{
	CString s,s1,s2;
	if(cali_flag==4)
		s1.LoadString(IDS_PULSECAL);
	else if(cali_flag==2)
		s1.LoadString(IDS_SINECAL);
	else if(cali_flag==6)
		s1.LoadString(IDS_PSEUDOCAL);
	else s1="";
	
	if(s1.IsEmpty())
		return;
	
	s2.LoadString(IDS_NUMSENS);
	s.Format(s2,sens_id[local_id]);
	s+=s1;
	
	calistr+=" "+s;
}
//显示实时波形
void CSitemagView::OnData(CHANNEL * pchn,int start,int chn_sum,CDC *pDC)
{
	CTime tmcv;
	CString s;
	CHANNEL * tmp;
	long time;
	int l=start;

	tmp=pchn;
	time=pchn->revblk.p_dsp->time;
	for(int j=0;j<chn_sum;j++)
	{
		int i=tmp->position;
		tmp->samp=pchn->samp;
		//计算波形屏幕坐标：对于在同一x坐标显示的数据，只统计和保存y坐标的最大，最小值
		dsp_data(&pchn->revblk.p_dsp->ds[1+tmp->samp*j],tmp,l);
		if(i>2)
			i-=2;
		pDC->MoveTo(tmp->point[i]);
		for(int k=i;k<tmp->position-1;k++){
			pDC->LineTo(tmp->point[k]);
		}
		tmp->data_cnt++;
		if(tmp->data_cnt>=m_dspctl.scantime)
		{
			tmp->data_cnt = 0;
			tmp->max_posi = tmp->position-1;
			tmp->position = 0;
			tmp->cnt=0;
			m_redraw_status = 1;
		}
		//统计本秒数据的最大，最小值
		GetDataRange(&pchn->revblk.p_dsp->ds[1+tmp->samp*j],tmp);
				
		tmp->curtm=time;
		/*pDC->TextOut(tmp->timex,rg.top,s);*/

		if(l==0){
			tmcv=tmp->revblk.p_dsp->time;
	 		s.Format("%s",tmcv.Format("%Y-%m-%d,%H:%M:%S"));
	//		s.Format("%d",tmp->revblk.p_dsp->time);
			UpdateTimeBar(s);
		}
		UpdateTipText(tmp,l);
		tmp=tmp->lpnext;
		l++;
	}
}
//显示收到的信息
void CSitemagView::OnMessage(CHANNEL * pchn)
{
	CString info;

	info="";
	//处理信息，保存显示的数据到info
	GetDocument()->ProcessMessage((char *)pchn->cmdblk.p_dsp->pb->ds,info);
	if(info.IsEmpty())
		return;
	//追加到信息框的显示区
	m_msg.m_rt+=info;
	m_msg.m_old_rt+=info;

	if(GetDocument()->m_btip==2)
	{
		GetDocument()->m_btip=0;
//		Sleep(10000);
	//	GetDocument()->ReConnect();
	}
	//信息框显示info
	ShowMsg();
	//重启提示
	if(GetDocument()->m_btip==1)
	{
		GetDocument()->m_btip=0;
		info.LoadString(IDS_TIP9);
		if(AfxMessageBox(info,MB_YESNO)==IDYES)
			GetDocument()->ResetSys();
	}
}

//////////////////////////////////////////////////////
void CSitemagView::OnRunView() 
{
	// TODO: Add your command handler code here
	CDC *pdc= GetDC();
	InitDC(pdc);
	CRect rect;
	
	GetClientRect(&rect);
	m_rectangle.top=rect.top;
	m_rectangle.left=m_tiprect.right;
	m_rectangle.bottom=rect.bottom-2*m_yChar;
	m_rectangle.right=rect.right;

	if(m_real_time_status==0){
	
		init_draw(pdc,rect);
		ResetPos();
		m_real_time_status=1;
		CString s,s1;

		s=GetDocument()->m_selsite->m_site.site;
		s+=" ";
		s1.LoadString(IDS_REAL);
		s+=s1;
		GetDocument()-> SetTitle(s);
	}
	else {
	
		m_real_time_status = 0;
		GetDocument() -> SetTitle(GetDocument()->m_selsite->m_site.site);
	}
	ReleaseDC(pdc);
}

void CSitemagView::OnUpdateRunView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	CString s;
	pCmdUI->Enable(GetDocument()->IsConnected());
	if(m_real_time_status==1){
		pCmdUI->SetRadio(1);
		s.LoadString(IDS_STOPMON);
		pCmdUI->SetText(s);
	}else {
		pCmdUI->SetRadio(0);
		s.LoadString(IDS_STARTMON);
		pCmdUI->SetText(s);
	}
//	pCmdUI->Enable(GetDocument()->m_selsite!=NULL);
}

//////////////setup display/////////////////////////////////
void CSitemagView::OnViewDisplay() 
{
	// TODO: Add your command handler code here
	CDspDlg dlg;
	dlg.m_phead=NULL;

	CHANNEL *tail,*pt;
	CHANNEL *tmp=GetDocument()->m_pchannels;	
	while(tmp!=NULL)
	{
		tail=new CHANNEL;
		tail->chn_id=tmp->chn_id;
		lstrcpy(tail->stn,tmp->stn);
		lstrcpy(tail->chn_label,tmp->chn_label);
		tail->dsp.max=tmp->dsp.max;
		tail->dsp.min=tmp->dsp.min;
		tail->dsp.unit=tmp->dsp.unit;
		tail->factor=tmp->factor;
		tail->lpnext=NULL;
		if(dlg.m_phead==NULL)
			dlg.m_phead=tail;
		else pt->lpnext=tail;
		pt=tail;
		tmp=tmp->lpnext;
	}
	dlg.m_das=&GetDocument()->m_das;
	dlg.m_scantime=m_dspctl.scantime;
	if(dlg.DoModal()==IDCANCEL)
		return;
	int i=0;
	tmp=GetDocument()->m_pchannels;
	tail=dlg.m_phead;
	while(tmp!=NULL)
	{
		tmp->dsp.max=tail->dsp.max;
		tmp->dsp.min=tail->dsp.min;
		if(tmp->dsp.unit!=4)
		{
			tmp->dsp.unit=tail->dsp.unit;
			tmp->factor=tail->factor;
		}
		UpdateTipText(tmp,i);
		
		tmp=tmp->lpnext;
		i++;

		tail=tail->lpnext;
	}
	if(dlg.m_scantime!=m_dspctl.scantime)
		m_dspctl.scantime=dlg.m_scantime;
	ReDraw();


}

//////////////////////////////////波形调整//////////////////////

void CSitemagView::OnViewCenter() 
{
	// TODO: Add your command handler code here
	CHANNEL *pchn=GetDocument()->m_pchannels;
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
		else  if(pchn->dsp.unit>4 && (pchn->chn_id>=6 && pchn->chn_id<12) )//自定义的单位,限制在辅助通道
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
		UpdateTipText(pchn,i);
		i++;
		pchn=pchn->lpnext;
	}
	ReDraw();		
}
void CSitemagView::OnViewUp() 
{
	// TODO: Add your command handler code here
	float offset;
	CHANNEL *pchn=GetDocument()->m_pchannels;
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
		UpdateTipText(pchn,i);
		i++;
		pchn=pchn->lpnext;
	}
	ReDraw();	
}

void CSitemagView::OnViewDown() 
{
	// TODO: Add your command handler code here
	float offset;
	CHANNEL *pchn=GetDocument()->m_pchannels;
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
		UpdateTipText(pchn,i);
		i++;
		pchn=pchn->lpnext;
	}
	ReDraw();	
}
void CSitemagView::OnViewSUp(CHANNEL *pchan) 
{
	// TODO: Add your command handler code here
	float offset;
	CHANNEL *pchn=GetDocument()->m_pchannels;
	int i=0;
	while(pchn!=NULL)//magnify wave
	{
		if(pchn==pchan){
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

			ReDraw();

			UpdateTipText(pchn,i);
			break;
		}
		i++;
		pchn=pchn->lpnext;
	}
}

void CSitemagView::OnViewSDown(CHANNEL *pchan) 
{
	// TODO: Add your command handler code here
	float offset,max,min;
	CHANNEL *pchn=GetDocument()->m_pchannels;
	int i=0;
	while(pchn!=NULL)//magnify wave
	{
		if(pchn==pchan){
			offset=(float)(pchn->dsp.max+pchn->dsp.min)/2;

			max=pchn->dsp.max;
			min=pchn->dsp.min;

			pchn->dsp.max=offset+(pchn->dsp.max-offset)*2;
			pchn->dsp.min=offset-(offset-pchn->dsp.min)*2;
			
			if(pchn->dsp.unit==0){
				pchn->dsp.max=ceil(pchn->dsp.max);
				pchn->dsp.min=ceil(pchn->dsp.min);
				if(pchn->dsp.max>2147483648.f || pchn->dsp.min<-2147483647.f){
					pchn->dsp.max=max;
					pchn->dsp.min=min;
				}
			}
			if(pchn->dsp.max==pchn->dsp.min)
			{
				pchn->dsp.max+=8;
				pchn->dsp.min-=8;
			}

			ReDraw();

			UpdateTipText(pchan,i);
			break;
		}
		i++;
		pchn=pchn->lpnext;
	}
}


/////////////////////////////////////////////////////////////////////////
void CSitemagView::OnDisplay()
{
	InitDsp();
	SetStnbtnChannel();
	CreateTips();

	RelayoutStnButton();
	Invalidate(TRUE);
}


//////////////////////////////////////////////////////////////////////
void CSitemagView::UpdateTimeBar(CString & timestr)
{
	((CMainFrame *)theApp.m_pMainWnd)->m_wndToolBar.m_edit_tm1.SetWindowText(timestr);
}


//////////////////tip/////////////////////////////////

void CSitemagView::UpdateTipText(CHANNEL *pchan,int index)
{
	if(pchan==NULL)
		return;
	CTime tmcv(pchan->curtm);
	// unit: 0-count 1-V 2-um/s 3-gal
	if(pchan->dsp.unit!=0){
		float factor;
	
		if(pchan->dsp.unit==1){
			if(pchan->chn_id<6)
				factor=8388608/(pchan->factor*1000);//count/mV  8388608=2^23 
			else if(pchan->chn_id>=6 && pchan->chn_id<12)
				factor=1.f/2.384/(1000*1000);
			else factor=pchan->factor;
		}else  if(pchan->dsp.unit>4 && (pchan->chn_id>=6 && pchan->chn_id<12) )//自定义的单位,限制在辅助通道
			factor=1.f/(2.384/(1000*1000))*pchan->factor;//2.384uV/count
		else factor=pchan->factor; 
	
	
		theApp.m_tiptext.Format("%s\nRange(%s): [%.3G] [%.3G]\nCurrent Data(%s): %G %G %G\nTime=%s",pchan->chn_label,
			unit[pchan->dsp.unit],pchan->dsp.max,pchan->dsp.min,
			unit[pchan->dsp.unit],pchan->maxvalue/factor,pchan->minvalue/factor,
			pchan->offset/factor,
			tmcv.Format("%Y-%m-%d,%H:%M:%S"));
	}else theApp.m_tiptext.Format("%s\nRange(%s): [%d] [%d]\nCurrent Data(%s): %d %d %d\nTime=%s",pchan->chn_label,
			unit[pchan->dsp.unit],(long)pchan->dsp.max,(long)pchan->dsp.min,
			unit[pchan->dsp.unit],pchan->maxvalue,pchan->minvalue,pchan->offset,
			tmcv.Format("%Y-%m-%d,%H:%M:%S"));
	
	
	if(m_Tips!=NULL){
		m_Tips->UpdateTipText(theApp.m_tiptext,&m_stnbtn[index]);
		m_Tips->Invalidate(TRUE);
	}
}
BOOL CSitemagView::CreateTips()
{
	GetClientRect(& m_tiprect);
	m_tiprect.right=15*m_xChar;
	m_tiprect.bottom-=2*m_yChar;

	if(m_Tips!=NULL)
		delete m_Tips;
	m_Tips=new CToolTipCtrl;
	// TT 3 TT Step 2 : Create the tool tip control
	m_Tips->Create(this, TTS_ALWAYSTIP);
	                     // TTS_ALWAYSTIP - show tip even when parent is not active
	CString s;
	CHANNEL *pchan=GetDocument()->m_pchannels;
	int i=0;
	while(pchan!=NULL){
		m_stnbtn[i].GetWindowText(s);
		m_Tips->AddTool(&m_stnbtn[i],s);
		UpdateTipText(pchan,i);
		pchan=pchan->lpnext;
		i++;
	}
	m_Tips->Activate(TRUE);
	return TRUE;
}

BOOL CSitemagView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_Tips!=NULL){
		m_Tips->Activate(TRUE);
		m_Tips->RelayEvent(pMsg);
	}
	return CView::PreTranslateMessage(pMsg);
}



/////////////////label station///////////////////////////////////
void CSitemagView::SetStnbtnChannel()
{
	CHANNEL *pchan=GetDocument()->m_pchannels;
	int i=0;

	while(pchan!=NULL && i<40)
	{
		m_stnbtn[i].m_pchannel=pchan;
		
		pchan=pchan->lpnext;
		i++;
	}
	for(;i<40;i++)
		m_stnbtn[i].ShowWindow(SW_HIDE);
}
void CSitemagView::RelayoutStnButton()
{
	CHANNEL *pchan=GetDocument()->m_pchannels;
	CString s;
	int i=0;
	CRect r;
	r=m_tiprect;
	int l=0;
	while(pchan!=NULL){
		if(!pchan->bShow){
			pchan=pchan->lpnext;
			m_stnbtn[i].ShowWindow(SW_HIDE);
			i++;
			continue;
		}

		s=pchan->chn_label;
		int height=min(40,m_rectangle.Height()/m_dspctl.channelnum);
		r.top=(l+.5)*m_rectangle.Height()/m_dspctl.channelnum-height/2;
		r.bottom=r.top+height;
		m_stnbtn[i].m_org.x=r.left;
		m_stnbtn[i].m_org.y=r.top;
		m_stnbtn[i].SetWindowText(s);
		m_stnbtn[i].MoveWindow(r);
		m_stnbtn[i].ShowWindow(SW_SHOW);
		pchan=pchan->lpnext;
		i++;
		l++;
	}
}
void CSitemagView::CreateStnButton()
{
	m_stnbtn=new CLabelButton[40];
	CString s("");
	int i;
	CRect r;
	r=m_tiprect;
	
	for(i=0;i<40;i++){
		r.top=0;
		r.bottom=20;
		m_stnbtn[i].Create(s,WS_CHILD | BS_PUSHBUTTON |BS_FLAT,r,this,WM_USER+i);
		m_stnbtn[i].m_parent=this;
		m_stnbtn[i].m_nID=WM_USER+i;
	}
}

void CSitemagView::DrawStnlabel(CDC *pDC,CRect &rect)
{
	CHANNEL *pt=GetDocument()->m_pchannels;
	int l=0;	
	while(pt!=NULL)
	{
		if(pt->bShow)
		{
			if(pt==m_selchannel){
				CString s;
				s.Format("%s-%s",pt->stn,pt->chn_label);
				m_stnbtn[l].SetLabel(s,RGB(255,0,0));
				break;
			}
			l++;
		}
		pt=pt->lpnext;
	}
}


//////////////////////////////////////
CHANNEL * CSitemagView::GetSelChannel(CPoint & point)
{
	int id=(point.y-m_rectangle.top)/m_dspctl.channel_range;
	int l=0;
	CHANNEL *pchan=GetDocument()->m_pchannels;
	while(pchan!=NULL)
	{
		if(pchan->bShow)
		{
			if(l==id)
				return pchan;
			l++;
		}
		pchan=pchan->lpnext;
	}
	return pchan;

}


void CSitemagView::OnViewMsg() 
{
	// TODO: Add your command handler code here
	m_msg.m_rt=m_msg.m_old_rt;
	ShowMsg();

}

void CSitemagView::ShowMsg()
{
	int i;
	CEdit *mdt;
	
	if(!m_msg.m_bcreated ){
		m_msg.m_msg.Create(IDD_DIALOG_DISPLAY ,NULL);
		m_msg.m_bcreated = TRUE;
		m_msg.m_bshow = TRUE;
		m_msg.m_f_st=FALSE;
		m_msg.m_msg.ShowWindow(SW_SHOW);
	}
	else {
		m_msg.m_msg.ShowWindow(SW_SHOW);
		m_msg.m_bshow = TRUE;
	}
	mdt = (CEdit *)(m_msg.m_msg.GetDlgItem(IDC_EDIT_MSG));
	i = mdt->GetLineCount();
	
	m_msg.m_msg.m_edit_msg = m_msg.m_rt;
	m_msg.m_msg.UpdateData(FALSE);
	mdt->LineScroll(i,0);
}
void CSitemagView::OnViewShowlabel() 
{
	// TODO: Add your command handler code here
	m_blabel=!m_blabel;
	CRect rect;

	GetClientRect(rect);
	rect.right=m_rectangle.left;

	InvalidateRect(rect);
}

void CSitemagView::OnUpdateViewShowlabel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_blabel);
}
