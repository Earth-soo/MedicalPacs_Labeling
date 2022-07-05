#pragma once

#include "CPacs.h"

// CBinarizationDlg 대화 상자

class CBinarizationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBinarizationDlg)

public:
	CBinarizationDlg(CPacsPtr pacs, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBinarizationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINARIZATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CPacsPtr m_pPacs;
	CPacsPtr m_pOrgPacs;
	//BYTE* m_bpBitsTmp;
	BOOL changed;

	void DrawImage();


public:
	CStatic m_pictureImage;
	CSliderCtrl m_sliderThreshold;
	int m_nThreshold;



	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeThresholdEdit();
	CEdit m_editThreshold;
	afx_msg void OnBnClickedButtonBinarization();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonRgbtogray();
	afx_msg void OnBnClickedButtonReset();
};
