#pragma once
#include "CPacs.h"


// CHistogramDlg 대화 상자

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CPacsPtr pacs, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CHistogramDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	CPacsPtr m_pPacs;

	int m_HistogramGray[256];
	int m_HistogramR[256];
	int m_HistogramG[256];
	int m_HistogramB[256];

public:
	void SetDialog();
	BOOL SetHistogram();
	void DrawHistogram(CDC* pDC);
	void DrawImage(CDC* pDC);

public:
	virtual BOOL OnInitDialog();
	CStatic m_pictureImage;
	CStatic m_pictureHistRed;
	CStatic m_pictureHistGreen;
	CStatic m_pictureHistBlue;
	CButton m_btnStretching;
	CButton m_btnEqualization;
	CSliderCtrl m_sliderHistRed;
	CSliderCtrl m_sliderHistGreen;
	CSliderCtrl m_sliderHistBlue;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnStretching();
	afx_msg void OnBnClickedBtnEqualization();
};
