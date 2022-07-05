// CHistogramDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MedicalPacs.h"
#include "CHistogramDlg.h"
#include "afxdialogex.h"
#include "CPacsEnhance.h"


// CHistogramDlg 대화 상자

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CPacsPtr pPacs, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTOGRAM, pParent), m_pPacs(make_shared<CPacs>(pPacs))
{
	
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_IMAGE, m_pictureImage);
	DDX_Control(pDX, IDC_PICTURE_HISTOGRAM_RED, m_pictureHistRed);
	DDX_Control(pDX, IDC_PICTURE_HISTOGRAM_GREEN, m_pictureHistGreen);
	DDX_Control(pDX, IDC_PICTURE_HISTOGRAM_BLUE, m_pictureHistBlue);
	DDX_Control(pDX, IDC_BTN_STRETCHING, m_btnStretching);
	DDX_Control(pDX, IDC_BTN_EQUALIZATION, m_btnEqualization);
	DDX_Control(pDX, IDC_SLIDER_HISTOGRAM_RED, m_sliderHistRed);
	DDX_Control(pDX, IDC_SLIDER_HISTOGRAM_GREEN, m_sliderHistGreen);
	DDX_Control(pDX, IDC_SLIDER_HISTOGRAM_BLUE, m_sliderHistBlue);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_STRETCHING, &CHistogramDlg::OnBnClickedBtnStretching)
	ON_BN_CLICKED(IDC_BTN_EQUALIZATION, &CHistogramDlg::OnBnClickedBtnEqualization)
END_MESSAGE_MAP()


// CHistogramDlg 메시지 처리기

void CHistogramDlg::SetDialog()
{
	CRect rect;
	CDC* pDC;
	CGdiObject* pOldPen;
	
	switch (m_pPacs->GetBpp()) {
	case 8:
		m_pictureHistRed.ShowWindow(SW_HIDE);
		m_pictureHistBlue.ShowWindow(SW_HIDE);
		m_sliderHistRed.ShowWindow(SW_HIDE);
		m_sliderHistBlue.ShowWindow(SW_HIDE);
		break;
	case 24:
		break;
	}
}
BOOL CHistogramDlg::SetHistogram()
{
	int nBpp = m_pPacs->GetBpp();

	CPacsEnhance pacs(m_pPacs);
	
	
	if (nBpp == 8) {

		vector<float> freqHistGray(256);
		pacs.ImageHistogram(freqHistGray);
		float maxValuehistGray = GetMaxFreqHist(freqHistGray);

		for (int i = 0; i < 256; i++) {
			m_HistogramGray[i] = static_cast<int>(freqHistGray[i] * 100 / maxValuehistGray);
		}
	}
	else if (nBpp == 24) {

		float maxValueHistR;
		float maxValueHistG;
		float maxValueHistB;

		vector<float> freqHistR(256);
		vector<float> freqHistG(256);
		vector<float> freqHistB(256);
		pacs.ImageHistogram(freqHistR, freqHistG, freqHistB);

		maxValueHistR = GetMaxFreqHist(freqHistR);
		maxValueHistG = GetMaxFreqHist(freqHistG);
		maxValueHistB = GetMaxFreqHist(freqHistB);

		for (int i = 0; i < 256; i++) {
			m_HistogramR[i] = static_cast<int>(freqHistR[i] * 100 / maxValueHistR);
			m_HistogramG[i] = static_cast<int>(freqHistG[i] * 100 / maxValueHistG);
			m_HistogramB[i] = static_cast<int>(freqHistB[i] * 100 / maxValueHistB);
		}
	}
	else {
		return FALSE;
	}

	return TRUE;
}
void CHistogramDlg::DrawHistogram(CDC* pDC)
{
	CPen pen;
	CPen* pOldpen;
	CRect rect;
	CGdiObject* pOldPen;

	int nBpp = m_pPacs->GetBpp();

	if (nBpp == 8) {

		m_pictureHistGreen.GetClientRect(rect);
		pDC = m_pictureHistGreen.GetWindowDC();

		pOldPen = pDC->SelectStockObject(DC_PEN);
		pDC->SetDCPenColor(RGB(128, 128, 128));

		for (int i = 0; i < 256; i++) {
			pDC->MoveTo(rect.left + i, rect.Height());
			pDC->LineTo(rect.left + i, rect.Height() - m_HistogramGray[i]);
		}
		pDC->SelectObject(pOldPen);

	}
	else if (nBpp == 24) {
		m_pictureHistRed.GetClientRect(rect);
		pDC = m_pictureHistRed.GetWindowDC();

		pOldPen = pDC->SelectStockObject(DC_PEN);
		pDC->SetDCPenColor(RGB(255, 0, 0));

		for (int i = 0; i < 256; i++) {
			pDC->MoveTo(rect.left + i, rect.Height());
			pDC->LineTo(rect.left + i, rect.Height() - m_HistogramR[i]);
		}
		pDC->SelectObject(pOldPen);

		m_pictureHistGreen.GetClientRect(rect);
		pDC = m_pictureHistGreen.GetWindowDC();
		int saveDC = pDC->SaveDC();

		pOldPen = pDC->SelectStockObject(DC_PEN);
		pDC->SetDCPenColor(RGB(0, 255, 0));
		for (int i = 0; i < 256; i++) {
			pDC->MoveTo(rect.left + i, rect.Height());
			pDC->LineTo(rect.left + i, rect.Height() - m_HistogramG[i]);
		}
		pDC->SelectObject(pOldPen);

		m_pictureHistBlue.GetClientRect(rect);
		pDC = m_pictureHistBlue.GetWindowDC();

		pOldPen = pDC->SelectStockObject(DC_PEN);
		pDC->SetDCPenColor(RGB(0, 0, 255));

		for (int i = 0; i < 256; i++) {
			pDC->MoveTo(rect.left + i, rect.Height());
			pDC->LineTo(rect.left + i, rect.Height() - m_HistogramB[i]);
		}
		pDC->SelectObject(pOldPen);
	}
}

void CHistogramDlg::DrawImage(CDC* pDC)
{
	CRect rect;

	m_pictureImage.GetWindowRect(rect);
	pDC = m_pictureImage.GetDC();
	m_pPacs->Draw(pDC, rect);
}

BOOL CHistogramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDialog();

	return TRUE;
}

void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this);
	SetHistogram();
	DrawHistogram(&dc);
	DrawImage(&dc);
}

void CHistogramDlg::OnBnClickedBtnStretching()
{
	CPacsEnhance pacs(m_pPacs);
	pacs.ImageHistogramStretching();
	Invalidate(true);
}


void CHistogramDlg::OnBnClickedBtnEqualization()
{
	CPacsEnhance pacs(m_pPacs);
	pacs.ImageHistogramStretching();
	int nBpp = m_pPacs->GetBpp();
	pacs.ImageHistogramEqualization();
	

	Invalidate(true);

}
