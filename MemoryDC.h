#pragma once
#include "pch.h"
class CMemoryDC :public CDC
{
	CSize m_size;
public:
	int GetWidth() const
	{
		return m_size.cx;
	}
	int GetHeight() const
	{
		return m_size.cy;
	}
	void MakeRgn(CRgn& rgn, COLORREF clTrans)
	{//用于抠图   本项目暂且排除在外
		rgn.DeleteObject();
		rgn.CreateRectRgn(0, 0, 0, 0);
		int i = m_size.cx;
		while (i-- > 0)
		{
			int j = m_size.cy;
			while (j-- > 0)
			{
				auto col = GetPixel(i, j);
				if (clTrans != col)
				{
					CRgn r;
					r.CreateRectRgn(i, j, i + 1, j + 1);
					rgn.CombineRgn(&rgn, &r, RGN_OR);
				}
			}
		}
	}
	void BitTrans(
		int nXDest,		// 目标起点X
		int nYDest,		// 目标起点Y
		int nWidthDest,	// 目标宽度
		int nHeightDest,// 目标高度
		CDC* pDC,		// 目标DC
		int nXSrc,		// 来源起点X
		int nYSrc,		// 来源起点Y
		COLORREF crTrans// 透明色
	)
	{
		CMemoryDC dcImage(nWidthDest, nHeightDest, pDC);//临时DC
		CBitmap bmpMask;
		bmpMask.CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);            // 创建单色掩码位图
		CDC dcMask;//掩码DC 
		dcMask.CreateCompatibleDC(pDC);
		dcMask.SelectObject(bmpMask);
		//将载入位图的内存DC中的位图，拷贝到临时DC中
		dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, this, nXSrc, nYSrc, SRCCOPY);

		//显示原图的动画
		//pDC->BitBlt(0, 0, nWidthDest, nHeightDest, this, nXSrc, nYSrc, SRCCOPY);

		// 设置临时DC的透明色
		dcImage.SetBkColor(crTrans);
		//掩码DC的透明区域为白色其它区域为黑色
		dcMask.BitBlt(0, 0, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCCOPY);
		//显示掩码图  半成品   白底黑图
		//pDC->BitBlt(nWidthDest, 0, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCCOPY);


		//临时DC透明区域为黑色，其它区域保持不变
		dcImage.SetBkColor(RGB(0, 0, 0));      
		dcImage.SetTextColor(RGB(255, 255, 255));
		dcImage.BitBlt(0, 0, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);
		//显示 黑底彩图
		//pDC->BitBlt(2*nWidthDest, 0, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCCOPY);


		// 目标DC透明部分保持屏幕不变，其它部分变成黑色
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->BitBlt(nXDest, nYDest, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);//其实这里是按位与运算&  
		//透明黑图   这里只是想看下  dcmask 通过和背景以及前景色的 运算   得出的结果
		//pDC->BitBlt(3 * (nWidthDest + 10), 0, nWidthDest, nHeightDest, &dcMask, 0, 0, SRCAND);   

		pDC->BitBlt(nXDest, nYDest, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCPAINT);//其实这里是按位与运算|
		//透明彩图  这里只是想看下  dcmask 通过和背景以及前景色的 运算   得出的结果
		//pDC->BitBlt(4 * (nWidthDest + 10), 0, nWidthDest, nHeightDest, &dcImage, 0, 0, SRCPAINT);   
	}
	
	BOOL LoadBitmap(UINT nID, CDC* pDC = NULL)
	{
		CBitmap bmp;
		if (!bmp.LoadBitmap(nID))
			return FALSE;

		if (!GetSafeHdc() && !CreateCompatibleDC(pDC))
			return FALSE;
		this->SelectObject(&bmp);
		BITMAP bm;
		bmp.GetBitmap(&bm);
		m_size.cx = bm.bmWidth;
		m_size.cy = bm.bmHeight;
		return TRUE;
	}

	BOOL Create(int cx, int cy, CDC* pDC = NULL)
	{
		CBitmap bmp;
		if (pDC)
		{
			if (!bmp.CreateCompatibleBitmap(pDC, cx, cy))
				return FALSE;
		}
		else
		{
			CClientDC dc(NULL);
			if (!bmp.CreateCompatibleBitmap(&dc, cx, cy))
				return FALSE;
		}
		if (!GetSafeHdc() && !CreateCompatibleDC(pDC))
			return FALSE;
		SelectObject(&bmp);
		m_size.cx = cx;
		m_size.cy = cy;
		return TRUE;
	}
	CMemoryDC(UINT nID, CDC* pDC = NULL)
	{
		LoadBitmap(nID, pDC);
	}
	CMemoryDC(int cx, int cy, CDC* pDC = NULL)
	{
		Create(cx, cy, pDC);
	}
	CMemoryDC()
	{

	}
	~CMemoryDC()
	{

	}
};
