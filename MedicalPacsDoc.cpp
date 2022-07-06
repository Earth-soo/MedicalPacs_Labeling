
// MedicalPacsDoc.cpp: CMedicalPacsDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MedicalPacs.h"
#endif
#include "CPacsEdge.h"
#include "CPacsFilter.h"
#include "CPacsSegment.h"

#include "MedicalPacsDoc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CHistogramDlg.h"
#include "MedicalPacsView.h"
#include "CGaussianDlg.h"
#include "CSharpeningDlg.h"
#include "CBinarizationDlg.h"

// CMedicalPacsDoc

IMPLEMENT_DYNCREATE(CMedicalPacsDoc, CDocument)

BEGIN_MESSAGE_MAP(CMedicalPacsDoc, CDocument)
	ON_COMMAND(ID_HISTOGRAM, &CMedicalPacsDoc::OnHistogram)
	ON_COMMAND(ID_GAUSSIAN, &CMedicalPacsDoc::OnGaussian)
	ON_COMMAND(ID_EDGE_SOBEL, &CMedicalPacsDoc::OnEdgeSobel)
	ON_COMMAND(ID_EDGE_COLOR, &CMedicalPacsDoc::OnEdgeColor)
	ON_COMMAND(ID_EDGE_PREWITT, &CMedicalPacsDoc::OnEdgePrewitt)
	ON_COMMAND(ID_ROVERTS, &CMedicalPacsDoc::OnRoverts)
	ON_COMMAND(ID_EDGE_CANNY, &CMedicalPacsDoc::OnEdgeCanny)
	ON_COMMAND(ID_IMAGE_RESET, &CMedicalPacsDoc::OnImageReset)
	ON_COMMAND(ID_SHARPENING, &CMedicalPacsDoc::OnSharpening)
	ON_COMMAND(ID_SEGMENT_BINARIZATION, &CMedicalPacsDoc::OnSegmentBinarization)
	ON_COMMAND(ID_CONTOUR_TRACING, &CMedicalPacsDoc::OnContourTracing)
	
	ON_COMMAND(ID_LABELING, &CMedicalPacsDoc::OnLabeling)
END_MESSAGE_MAP()


// CMedicalPacsDoc 생성/소멸

CMedicalPacsDoc::CMedicalPacsDoc() noexcept : m_pPacs(nullptr)
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	
}

CMedicalPacsDoc::~CMedicalPacsDoc()
{
}

BOOL CMedicalPacsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	
	if (m_pPacs) {
		m_pPacs.reset(new CPacs);
	}

	return TRUE;
}




// CMedicalPacsDoc serialization

void CMedicalPacsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
		
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CMedicalPacsDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CMedicalPacsDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMedicalPacsDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMedicalPacsDoc 진단

#ifdef _DEBUG
void CMedicalPacsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMedicalPacsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMedicalPacsDoc 명령
BOOL CMedicalPacsDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (m_pPacs == nullptr) {
		m_pPacs = make_shared<CPacs>();
	}
	if(! m_pPacs->Load(lpszPathName)) return FALSE;
}

void CMedicalPacsDoc::OnHistogram()
{
	CHistogramDlg dlg(m_pPacs);
	if (dlg.DoModal() == IDOK) {
		m_pPacs->MemCopy(dlg.m_pPacs->GetBits());
		UpdateAllViews(NULL);
	}

}



void CMedicalPacsDoc::OnCloseDocument()
{
	
	CDocument::OnCloseDocument();
}


void CMedicalPacsDoc::OnGaussian()
{
	if (!m_pPacs->IsValid()) {
		AfxMessageBox(_T("처리할 이미지가 없습니다."));
		return;
	}

	CGaussianDlg dlg;

	if (dlg.DoModal() == IDOK) {
		CPacsFilter pacs(m_pPacs);
		pacs.ImageFilterGaussian(dlg.m_fSigma);
		UpdateAllViews(NULL);
	}
	
	
}

void CMedicalPacsDoc::OnRoverts()
{
	CPacsEdge pacs(m_pPacs);
	pacs.ImageEdgeRoverts();
	UpdateAllViews(NULL);
}

void CMedicalPacsDoc::OnEdgePrewitt()
{
	CPacsEdge pacs(m_pPacs);
	pacs.ImageEdgePrewitt();
	UpdateAllViews(NULL);
}

void CMedicalPacsDoc::OnEdgeSobel()
{
	CPacsEdge pacs(m_pPacs);
	pacs.ImageEdgeSobel();
	UpdateAllViews(NULL);
}

void CMedicalPacsDoc::OnEdgeColor()
{
	CPacsEdge pacs(m_pPacs);
	pacs.ImageEdgeColor();
	UpdateAllViews(NULL);
}

void CMedicalPacsDoc::OnEdgeCanny()
{

	UpdateAllViews(NULL);
}


void CMedicalPacsDoc::OnImageReset()
{
	m_pPacs->SetData();
	UpdateAllViews(NULL);
}


void CMedicalPacsDoc::OnSharpening()
{
	CSharpeningDlg dlg(m_pPacs);
	if (dlg.DoModal() == IDOK) {
		m_pPacs->MemCopy(dlg.m_pPacs->GetBits());
		UpdateAllViews(NULL);
	}
}


void CMedicalPacsDoc::OnSegmentBinarization()
{
	CBinarizationDlg dlg(m_pPacs);
	if (dlg.DoModal() == IDOK) {
		//m_pPacs->MemCopy(dlg.m_pPacs->GetBits());
		
		m_pPacs->SwapBits(dlg.m_pPacs);
		UpdateAllViews(NULL);
	}
}


void CMedicalPacsDoc::OnContourTracing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CMedicalPacsDoc::OnLabeling()
{
	CPacsSegment pacs(m_pPacs);
	pacs.ImageLabeling();


	UpdateAllViews(NULL);
}
