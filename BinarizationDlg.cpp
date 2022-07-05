// BinarizationDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MedicalPacs.h"
#include "BinarizationDlg.h"
#include "afxdialogex.h"


// BinarizationDlg 대화 상자

IMPLEMENT_DYNAMIC(BinarizationDlg, CDialogEx)

BinarizationDlg::BinarizationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BINARIZATION, pParent)
{

}

BinarizationDlg::~BinarizationDlg()
{
}

void BinarizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BinarizationDlg, CDialogEx)
END_MESSAGE_MAP()


// BinarizationDlg 메시지 처리기
