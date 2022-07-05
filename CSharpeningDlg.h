#pragma once

#include "CPacs.h"
// CSharpeningDlg 대화 상자

class CSharpeningDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSharpeningDlg)

public:
	CSharpeningDlg(CPacsPtr pacs, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSharpeningDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHARPENING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CPacsPtr m_pPacs;

public:
	void DrawImage(CDC* pDC);

public:
	CStatic m_pictureImage;
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonUnsharpMask();
	afx_msg void OnBnClickedButtonHighboost();
	afx_msg void OnBnClickedButtonDiffusion();
};
