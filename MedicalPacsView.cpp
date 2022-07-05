
// MedicalPacsView.cpp: CMedicalPacsView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MedicalPacs.h"
#endif

#include "MedicalPacsDoc.h"
#include "MedicalPacsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMedicalPacsView

IMPLEMENT_DYNCREATE(CMedicalPacsView, CView)

BEGIN_MESSAGE_MAP(CMedicalPacsView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMedicalPacsView 생성/소멸

CMedicalPacsView::CMedicalPacsView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMedicalPacsView::~CMedicalPacsView()
{
}

BOOL CMedicalPacsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMedicalPacsView 그리기

void CMedicalPacsView::OnDraw(CDC* pDC)
{
	CMedicalPacsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rect;

	if (pDoc->m_pPacs) {
		pDoc->m_pPacs->Draw(pDC);
	}
	
}


// CMedicalPacsView 인쇄

BOOL CMedicalPacsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMedicalPacsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMedicalPacsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMedicalPacsView 진단

#ifdef _DEBUG
void CMedicalPacsView::AssertValid() const
{
	CView::AssertValid();
}

void CMedicalPacsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMedicalPacsDoc* CMedicalPacsView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMedicalPacsDoc)));
	return (CMedicalPacsDoc*)m_pDocument;
}
#endif //_DEBUG


// CMedicalPacsView 메시지 처리기


void CMedicalPacsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}


void CMedicalPacsView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);
}


void CMedicalPacsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonUp(nFlags, point);
}
