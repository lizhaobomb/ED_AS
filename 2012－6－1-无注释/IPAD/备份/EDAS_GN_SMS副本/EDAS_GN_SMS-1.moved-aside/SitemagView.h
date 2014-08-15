// sitemagView.h : interface of the CSitemagView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITEMAGVIEW_H__25DD53C3_9565_49B2_AA05_76B0AA4B072B__INCLUDED_)
#define AFX_SITEMAGVIEW_H__25DD53C3_9565_49B2_AA05_76B0AA4B072B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "labelbutton.h"
#include "showmsg.h"

class CSitemagView : public CView
{
protected: // create from serialization only
	CSitemagView();
	DECLARE_DYNCREATE(CSitemagView)

// Attributes
public:
	CSitemagDoc* GetDocument();
	//重画波形
	void ReDraw();
	//单道缩小，pchan：通道
	void OnViewSDown(CHANNEL * pchan);
	//单道放大，pchan：通道
	void OnViewSUp(CHANNEL *pchan); 
	//1-显示单道窗
	BOOL m_bctrlwave;
private:
	//选中的通道指针	
	CHANNEL * m_selchannel;
	//tip显示的通道指针
	CHANNEL *m_tipchannel;

	//通道按钮
	CLabelButton *m_stnbtn;
	//1-实时波形显示
	int 	m_real_time_status;
	//显示控制
	DSPCTL m_dspctl;
	//波形区
	CRect m_rectangle;
	//tip尺寸
	CRect m_tiprect;
	//字符大小
	int m_xChar,m_yChar;
	//颜色
	COLORREF m_gridcolor,m_lncolor,m_bkcolor,m_badlncolor;
	//画笔
	CPen *m_pPen,*m_pGridPen,*m_pBadPen;
	//刷子
	CBrush *m_pBrush;
	//屏幕区
	CRect m_rect;
	//重画状态
    BOOL m_redraw_status;

	BOOL m_bCommand;
	//tip
	CToolTipCtrl *m_Tips;
	
	BOOL m_blabel;

	//通道采样,标定说明和时间标志字体
	LOGFONT m_sfont;
	CFont m_font;
private:

	//初始化dc
	void InitDC(CDC *pDC);
	//初始化显示
	void InitDsp();
	//初始化画波形
	void init_draw(CDC *,CRect &);
	//计算各通道显示范围
	void CalChannelRange();

	//画波形
	void DrawWave();
	//复位画波形参数
    void ResetPos();
	
	//显示波形
	void DisplayData();
	//计算1道波形坐标，p:通道，l:屏幕索引
	void dsp_data(long * pin,CHANNEL *p,int l);

	//清除rect区
	void clear_eara(CDC * pDC,CRect * rect,int posi,int step );
	
	//建立台站按钮
	void CreateStnButton();
	//重布局台站按钮
	void RelayoutStnButton();
	//设置各按钮对应通道
	void SetStnbtnChannel();

	void DrawStnlabel(CDC *pDC,CRect &rect);
	//更新时间栏
	void UpdateTimeBar(CString & timestr);
	
	//取选中的通道
	CHANNEL * GetSelChannel(CPoint & );
	//显示单道窗
	void ShowCtrlWave(CPoint &point);

	//建立tip
	BOOL CreateTips();
	//更新tip内容
	void UpdateTipText(CHANNEL *pchan,int index);

	//处理显示波形数据,start:通道在屏幕续号
	void OnData(CHANNEL * pchn,int start,int chn_sum,CDC *pDC);
	//显示标定标记
	void ShowCaliflag(int cali_flag,int local_id,CString & calistr);
	//显示接收到的参数信息
	void OnMessage(CHANNEL * pchn);
	void ShowMsg();
// Operations
public:
	void OnDisplay();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSitemagView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSitemagView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSitemagView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRunView();
	afx_msg void OnUpdateRunView(CCmdUI* pCmdUI);
	afx_msg void OnViewDisplay();
	afx_msg void OnViewCenter();
	afx_msg void OnViewDown();
	afx_msg void OnViewUp();
	afx_msg void OnViewMsg();
	afx_msg void OnViewShowlabel();
	afx_msg void OnUpdateViewShowlabel(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in sitemagView.cpp
inline CSitemagDoc* CSitemagView::GetDocument()
   { return (CSitemagDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SITEMAGVIEW_H__25DD53C3_9565_49B2_AA05_76B0AA4B072B__INCLUDED_)
