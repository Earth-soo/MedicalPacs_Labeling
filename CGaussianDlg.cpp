// CGaussianDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MedicalPacs.h"
#include "CGaussianDlg.h"
#include "afxdialogex.h"


// CGaussianDlg 대화 상자

IMPLEMENT_DYNAMIC(CGaussianDlg, CDialogEx)

CGaussianDlg::CGaussianDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAUSSIAN, pParent)
	, m_fSigma(1.40f)
	, m_fSliderSigma(0)
{

}

CGaussianDlg::~CGaussianDlg()
{
}

void CGaussianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GAUSSIAN_SIGMA, m_fSigma);
	DDX_Control(pDX, IDC_SLIDER_GAUSSIAN_SIGMA, m_sliderSigma);
	DDX_Slider(pDX, IDC_SLIDER_GAUSSIAN_SIGMA, m_fSliderSigma);
	DDX_Control(pDX, IDC_EDIT_GAUSSIAN_SIGMA, m_editSigma);
	DDV_MinMaxFloat(pDX, m_fSigma, 0.20f, 5.00f);
}


BEGIN_MESSAGE_MAP(CGaussianDlg, CDialogEx)
	//ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER_GAUSSIAN_SIGMA, &CGaussianDlg::OnTRBNThumbPosChangingSliderGaussianSigma)
	ON_EN_CHANGE(IDC_EDIT_GAUSSIAN_SIGMA, &CGaussianDlg::OnEnChangeEditGaussianSigma)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CGaussianDlg 메시지 처리기

/*
void CGaussianDlg::OnTRBNThumbPosChangingSliderGaussianSigma(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	NMTRBTHUMBPOSCHANGING* pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING*>(pNMHDR);
	
	();
	
	m_fSigma = pNMTPC->dwPos;
	UpdateData(FALSE);
	
	*pResult = 0;
}
*/

BOOL CGaussianDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderSigma.SetRange(10, 250);
	m_sliderSigma.SetTicFreq(20);
	m_sliderSigma.SetPageSize(20);
	m_sliderSigma.SetPos(static_cast<int>(m_fSigma * 50));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGaussianDlg::OnEnChangeEditGaussianSigma()
{
	UpdateData(TRUE);
	m_sliderSigma.SetPos(static_cast<int>(m_fSigma * 50));
	UpdateData(false);
}


void CGaussianDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_sliderSigma.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderSigma.GetPos();
		m_fSigma = (pos / 50.f);
		CString strSigma;
		strSigma.Format(_T("%2f"), m_fSigma);
		m_editSigma.SetWindowText(strSigma);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
