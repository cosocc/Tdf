
// TdfDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Tdf.h"
#include "TdfDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTdfDlg 对话框



CTdfDlg::CTdfDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TDF_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//制作随机种子
	srand((UINT)time(NULL) * 17777);
}

void CTdfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CTdfDlg::LoadPictures()
{
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	m_dcBack.Create(cx, cy);
	m_mdc.Create(cx, cy);//草稿，缓冲的内存DC
	CMemoryDC mdc;
	mdc.LoadBitmap(IDB_BACK);
	//MoveWindow第五参数：
	//指定 CWnd 是否将重新绘制。 如果 TRUE， CWnd 照常接收到其 OnPaint 消息处理程序中 WM_PAINT 消息。 
	//如果此参数是 FALSE，重新绘制任何类型不会发生。 这适用于客户端区域，应用于非工作区(包括标题和滚动条)和于由于 CWnd的移动找到的父窗口的任何部分。 
	//当此参数是 FALSE时，应用程序必须显式无效或重绘必须重绘 CWnd 和父窗口的所有部件。

	MoveWindow(0, 0, cx, cy, FALSE);
	//将背景图全屏
	m_dcBack.StretchBlt(0, 0, cx, cy, &mdc, 0, 0, mdc.GetWidth(), mdc.GetHeight(), SRCCOPY);//这里调用CDC 函数  
	int i = -1;
	while (++i < FLY_COUNT)
		m_dcFlys[i].LoadBitmap(IDB_FLY1 + i);    //这个是winapi

}

BEGIN_MESSAGE_MAP(CTdfDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CTdfDlg 消息处理程序

BOOL CTdfDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LoadPictures();
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//重载了ontimer（）   需要设置下定时器频率
	SetTimer(1, 16, NULL);//
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTdfDlg::OnPaint()
{
	//if (IsIconic())
	//{
	//	CPaintDC dc(this); // 用于绘制的设备上下文

	//	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	//	// 使图标在工作区矩形中居中
	//	int cxIcon = GetSystemMetrics(SM_CXICON);
	//	int cyIcon = GetSystemMetrics(SM_CYICON);
	//	CRect rect;
	//	GetClientRect(&rect);
	//	int x = (rect.Width() - cxIcon + 1) / 2;
	//	int y = (rect.Height() - cyIcon + 1) / 2;

	//	// 绘制图标
	//	dc.DrawIcon(x, y, m_hIcon);
	//}
	//else
	//{
	//	CDialogEx::OnPaint();
	//}
	
	CPaintDC dc(this);
	dc.BitBlt(0, 0, m_dcBack.GetWidth(), m_dcBack.GetHeight(), &m_dcBack, 0, 0, SRCCOPY);

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTdfDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTdfDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CClientDC dc(this);
	m_mdc.BitBlt(0, 0, m_dcBack.GetWidth(), m_dcBack.GetHeight(), &m_dcBack, 0, 0, SRCCOPY);
	auto pos = m_list.GetHeadPosition();
	while (pos)
	{
		SData& d = m_list.GetNext(pos);
		d.Move();
		auto pDC = m_dcFlys + d.nIndex;
		pDC->BitTrans(d.pos.x, d.pos.y, pDC->GetWidth(), pDC->GetHeight(), &m_mdc, 0, 0, 0xFF00FF);
	}
	dc.BitBlt(0, 0, m_mdc.GetWidth(), m_mdc.GetHeight(), &m_mdc, 0, 0, SRCCOPY);

	CDialogEx::OnTimer(nIDEvent);
}


void CTdfDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SData d;
	d.pos = point;
	d.nIndex = rand() % FLY_COUNT;
	d.dir = { 5,5 };
	if (rand() % 2)
		d.dir.x *= -1;
	if (rand() % 2)
		d.dir.y *= -1;
	m_list.AddTail(d);
	CDialogEx::OnLButtonDown(nFlags, point);
}
