// CBinarizationDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MedicalPacs.h"
#include "CBinarizationDlg.h"
#include "afxdialogex.h"
#include "CPacsSegment.h"

// CBinarizationDlg 대화 상자

IMPLEMENT_DYNAMIC(CBinarizationDlg, CDialogEx)

CBinarizationDlg::CBinarizationDlg(CPacsPtr pPacs, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BINARIZATION, pParent), m_pPacs(make_shared<CPacs>(pPacs)), m_pOrgPacs(pPacs)
	,m_nThreshold(127)
{

}

CBinarizationDlg::~CBinarizationDlg()
{
}

void CBinarizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_PREVIEW1, m_pictureImage);
	DDX_Control(pDX, IDC_THRESHOLD_SLIDER, m_sliderThreshold);
	DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_nThreshold);
	DDX_Control(pDX, IDC_THRESHOLD_EDIT, m_editThreshold);
}


BEGIN_MESSAGE_MAP(CBinarizationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_THRESHOLD_EDIT, &CBinarizationDlg::OnEnChangeThresholdEdit)
	ON_BN_CLICKED(IDC_BUTTON_BINARIZATION, &CBinarizationDlg::OnBnClickedButtonBinarization)
	ON_BN_CLICKED(IDC_BUTTON_RGBTOGRAY, &CBinarizationDlg::OnBnClickedButtonRgbtogray)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CBinarizationDlg::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CBinarizationDlg 메시지 처리기
/**/
void CBinarizationDlg::DrawImage()
{
	if (m_pictureImage.GetSafeHwnd() == nullptr) return;

	CRect rect;

	m_pictureImage.GetClientRect(rect);
	CDC* pDC = m_pictureImage.GetDC();
	m_pPacs->Draw(pDC, rect);

	//m_nThreshold = ImageBinarizationIterative(imgSrc);
}

//void CBinarizationDlg::MakePreviewImage()
//{
////	CPacsSegment pacs(m_pPacs);
////	pacs.ImageBinarization(m_nThreshold);
////	DrawImage();
//}

BOOL CBinarizationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetTicFreq(32);
	//m_sliderThreshold.SetPageSize(32);
	m_sliderThreshold.SetPos(m_nThreshold);

	//int size = m_pPacs->GetRsize();
	//m_bpBitsTmp = new BYTE[size];
	//memcpy(m_bpBitsTmp, m_pPacs->GetBits(), size);

	changed = FALSE;

	return TRUE;  
}


void CBinarizationDlg::OnPaint()
{
	CPaintDC dc(this); 

	DrawImage();

}


void CBinarizationDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_sliderThreshold.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int nPos = m_sliderThreshold.GetPos();
		m_nThreshold = nPos;
		UpdateData(FALSE);

		// 현재 설정된 임계값을 이용하여 미리보기 영상의 이진화를 수행한다.
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CBinarizationDlg::OnEnChangeThresholdEdit()
{
	//UpdateData(TRUE);
	m_nThreshold = GetDlgItemInt(IDC_THRESHOLD_EDIT);
	m_sliderThreshold.SetPos(m_nThreshold);

	//MakePreviewImage();
	//Invalidate(true);
}


void CBinarizationDlg::OnBnClickedButtonBinarization()
{
	changed = TRUE;

	CPacsSegment pacs(m_pPacs);
	pacs.ImageBinarization(m_nThreshold);
	Invalidate(true);
}

#include "CPacsColor.h"

void CBinarizationDlg::OnBnClickedButtonRgbtogray()
{
	CPacsSegment pacs(m_pPacs);
	pacs.RGB_TO_GRAY();

	Invalidate(true);
}


void CBinarizationDlg::OnBnClickedButtonReset()
{
	if (changed) {
		m_sliderThreshold.SetPos(127);
		m_nThreshold = 127;

		UpdateData(FALSE);

		m_pPacs->MemCopy(m_pOrgPacs->GetBits());
		Invalidate(TRUE);
	}
	else 
		AfxMessageBox(_T("이진화 후 다시 시도해주세요."));

	changed = FALSE;
}
