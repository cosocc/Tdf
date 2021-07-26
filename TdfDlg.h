
// TdfDlg.h: 头文件
//

#pragma once
#include"MemoryDC.h"

// CTdfDlg 对话框
class CTdfDlg : public CDialogEx
{
// 构造
public:
	
	CTdfDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TDF_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	struct SData
	{
		int nIndex=0;//帧数
		CPoint pos, dir;
		void Move()
		{
			if (++nIndex >= FLY_COUNT)
				nIndex = 0;
			pos.Offset(dir);
			if (pos.x > 1550 || pos.x < -20)
				dir.x *= -1;
			if (pos.y > 840 || pos.y < -20)
				dir.y *= -1;
		}

	};
	CList<SData> m_list;
	CSize m_size;
	enum { FLY_COUNT = 7 };
	CMemoryDC m_dcBack, m_mdc;
	CMemoryDC m_dcFlys[FLY_COUNT];
	void LoadPictures();


public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

};
