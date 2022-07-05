// CSharpeningDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MedicalPacs.h"
#include "CSharpeningDlg.h"
#include "afxdialogex.h"
#include "CPacsFilter.h"

// CSharpeningDlg 대화 상자

IMPLEMENT_DYNAMIC(CSharpeningDlg, CDialogEx)

CSharpeningDlg::CSharpeningDlg(CPacsPtr pPacs, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHARPENING, pParent), m_pPacs(make_shared<CPacs>(pPacs))
{

}

CSharpeningDlg::~CSharpeningDlg()
{
}

void CSharpeningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_PREVIEW, m_pictureImage);
}


BEGIN_MESSAGE_MAP(CSharpeningDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_UNSHARP_MASK, &CSharpeningDlg::OnBnClickedButtonUnsharpMask)
	ON_BN_CLICKED(IDC_BUTTON_HIGHBOOST, &CSharpeningDlg::OnBnClickedButtonHighboost)
	ON_BN_CLICKED(IDC_BUTTON_DIFFUSION, &CSharpeningDlg::OnBnClickedButtonDiffusion)
END_MESSAGE_MAP()


// CSharpeningDlg 메시지 처리기
void CSharpeningDlg::DrawImage(CDC* pDC)
{
	CRect rect;

	m_pictureImage.GetClientRect(rect);
	pDC = m_pictureImage.GetDC();
	m_pPacs->Draw(pDC, rect);
}

BOOL CSharpeningDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSharpeningDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	DrawImage(&dc);
}


void CSharpeningDlg::OnBnClickedButtonUnsharpMask()
{
	CPacsFilter pacs(m_pPacs);
	pacs.ImageFilterUnsharpMask();
	Invalidate(true);
}


void CSharpeningDlg::OnBnClickedButtonHighboost()
{
	CPacsFilter pacs(m_pPacs);
	float alpha = 1.2f;
	pacs.ImageFilterHighboost(alpha);
	Invalidate(true);
}
#include "MedicalPacsDoc.h"
#include "CDiffusionDlg.h"

void CSharpeningDlg::OnBnClickedButtonDiffusion()
{
	CDiffusionDlg dlg;
	if (dlg.DoModal() == IDOK) {
		CPacsFilter pacs(m_pPacs);
		pacs.ImageFilterDiffusion(dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		//UpdateData(TRUE);
		Invalidate(true);
	}
}
