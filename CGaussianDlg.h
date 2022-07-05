#pragma once


// CGaussianDlg 대화 상자

class CGaussianDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGaussianDlg)

public:
	CGaussianDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGaussianDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAUSSIAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_fSigma;
	CSliderCtrl m_sliderSigma;
	//afx_msg void OnTRBNThumbPosChangingSliderGaussianSigma(NMHDR* pNMHDR, LRESULT* pResult);

	virtual BOOL OnInitDialog();
	int m_fSliderSigma;
	CEdit m_editSigma;
	afx_msg void OnEnChangeEditGaussianSigma();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
