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
	//�ػ�����
	void ReDraw();
	//������С��pchan��ͨ��
	void OnViewSDown(CHANNEL * pchan);
	//�����Ŵ�pchan��ͨ��
	void OnViewSUp(CHANNEL *pchan); 
	//1-��ʾ������
	BOOL m_bctrlwave;
private:
	//ѡ�е�ͨ��ָ��	
	CHANNEL * m_selchannel;
	//tip��ʾ��ͨ��ָ��
	CHANNEL *m_tipchannel;

	//ͨ����ť
	CLabelButton *m_stnbtn;
	//1-ʵʱ������ʾ
	int 	m_real_time_status;
	//��ʾ����
	DSPCTL m_dspctl;
	//������
	CRect m_rectangle;
	//tip�ߴ�
	CRect m_tiprect;
	//�ַ���С
	int m_xChar,m_yChar;
	//��ɫ
	COLORREF m_gridcolor,m_lncolor,m_bkcolor,m_badlncolor;
	//����
	CPen *m_pPen,*m_pGridPen,*m_pBadPen;
	//ˢ��
	CBrush *m_pBrush;
	//��Ļ��
	CRect m_rect;
	//�ػ�״̬
    BOOL m_redraw_status;

	BOOL m_bCommand;
	//tip
	CToolTipCtrl *m_Tips;
	
	BOOL m_blabel;

	//ͨ������,�궨˵����ʱ���־����
	LOGFONT m_sfont;
	CFont m_font;
private:

	//��ʼ��dc
	void InitDC(CDC *pDC);
	//��ʼ����ʾ
	void InitDsp();
	//��ʼ��������
	void init_draw(CDC *,CRect &);
	//�����ͨ����ʾ��Χ
	void CalChannelRange();

	//������
	void DrawWave();
	//��λ�����β���
    void ResetPos();
	
	//��ʾ����
	void DisplayData();
	//����1���������꣬p:ͨ����l:��Ļ����
	void dsp_data(long * pin,CHANNEL *p,int l);

	//���rect��
	void clear_eara(CDC * pDC,CRect * rect,int posi,int step );
	
	//����̨վ��ť
	void CreateStnButton();
	//�ز���̨վ��ť
	void RelayoutStnButton();
	//���ø���ť��Ӧͨ��
	void SetStnbtnChannel();

	void DrawStnlabel(CDC *pDC,CRect &rect);
	//����ʱ����
	void UpdateTimeBar(CString & timestr);
	
	//ȡѡ�е�ͨ��
	CHANNEL * GetSelChannel(CPoint & );
	//��ʾ������
	void ShowCtrlWave(CPoint &point);

	//����tip
	BOOL CreateTips();
	//����tip����
	void UpdateTipText(CHANNEL *pchan,int index);

	//������ʾ��������,start:ͨ������Ļ����
	void OnData(CHANNEL * pchn,int start,int chn_sum,CDC *pDC);
	//��ʾ�궨���
	void ShowCaliflag(int cali_flag,int local_id,CString & calistr);
	//��ʾ���յ��Ĳ�����Ϣ
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
