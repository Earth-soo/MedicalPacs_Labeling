#include "pch.h"
#include "CPacs.h"
#include "RGBBYTE.h"
#include "ARGBBYTE.h"

CPacs::CPacs() : m_bpBits(nullptr)
{
}

CPacs::CPacs(const CPacs& r) :
	m_nWidth(r.m_nWidth)
	, m_nHeight(r.m_nHeight)
	, m_nPitch(r.m_nPitch)
	, m_nSize(r.m_nSize)
	, m_nBPP(r.m_nBPP)
	, m_nPsize(r.m_nPsize)
	, m_nRealSize(r.m_nRealSize)
	, m_bpBits(new BYTE[r.m_nPsize])
{
	memcpy(m_bpBits, r.m_bpBits, r.m_nPsize);
}

CPacs::CPacs(CPacsPtr pPacs) : 
	m_nWidth(pPacs->m_nWidth)
	, m_nHeight(pPacs->m_nHeight)
	, m_nPitch(pPacs->m_nPitch)
	, m_nSize(pPacs->m_nSize)
	, m_nBPP(pPacs->m_nBPP)
	, m_nPsize(pPacs->m_nPsize)
	, m_nRealSize(pPacs->m_nRealSize)
	, m_bpBits(new BYTE[pPacs->m_nPsize])
{
	memcpy(m_bpBits, pPacs->GetBits(), pPacs->m_nPsize);
}

CPacs::~CPacs()
{
	if (IsValid()) {
		delete[] m_bpBits;
	}
}

void CPacs::Resize(void* pBits, int nNewWidth, int nNewHeight)
{
	m_nWidth = nNewWidth;
	m_nHeight = nNewHeight;
	m_nSize = nNewWidth * nNewHeight;

	delete[] m_bpBits;
	m_bpBits = static_cast<BYTE*> (pBits);
}

void CPacs::SetCopy(void* pBits)
{
	if (IsValid()) {
		delete[] m_bpBits;
	}
	m_bpBits = static_cast<BYTE*> (pBits);
}

void CPacs::MemCopy(void* pBits)
{
	memcpy(m_bpBits, pBits, m_nPsize);
}

void CPacs::SwapBits(CPacsPtr pPacs) 
{
	swap(m_bpBits, pPacs->m_bpBits);
}

BOOL CPacs::Load(LPCTSTR lpszPathName)
{
	if (!m_CImage.IsNull())
		m_CImage.Destroy();

	m_CImage.Load(lpszPathName);

	SetData();

	return TRUE;
}

void CPacs::SetData()
{
	m_nWidth = m_CImage.GetWidth();
	m_nHeight = m_CImage.GetHeight();
	m_nPitch = m_CImage.GetPitch();
	m_nSize = m_nWidth * m_nHeight;
	m_nBPP = m_CImage.GetBPP();
	m_nPsize = m_nSize * m_nBPP / 8;
	m_nRealSize = abs(m_nPitch) * m_nHeight;
	if (IsValid()) {
		delete[] m_bpBits;
	}
	BYTE* bpBits = (BYTE*)m_CImage.GetBits();
	if (m_nPitch < 0) {
		bpBits = bpBits + ((m_nHeight - 1) * m_nPitch);
	}

	m_bpBits = new BYTE[m_nPsize];
	memcpy(m_bpBits, bpBits, m_nPsize);
}

BOOL CPacs::Draw(CDC* pDC, CRect rect)
{
	shared_ptr<BITMAPINFO> pBitmapInfo;

	if (m_nBPP == 8) {
		pBitmapInfo.reset(new BITMAPINFO[sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * GetPaletteNums())]);
	}
	else {
		pBitmapInfo.reset(new BITMAPINFO[sizeof(BITMAPINFO)]);
	}

	if (pBitmapInfo) {
		pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBitmapInfo->bmiHeader.biWidth = m_nWidth;
		pBitmapInfo->bmiHeader.biHeight = m_nHeight;
		pBitmapInfo->bmiHeader.biPlanes = 1;
		pBitmapInfo->bmiHeader.biBitCount = m_nBPP;
		pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		pBitmapInfo->bmiHeader.biSizeImage = 0;
		pBitmapInfo->bmiHeader.biClrImportant = 0;
		pBitmapInfo->bmiHeader.biClrUsed = 0;
		pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
		if (m_nBPP == 8) {
			for (int nIndex = 0; nIndex < 256; nIndex++) {
				pBitmapInfo->bmiColors[nIndex].rgbRed =
				pBitmapInfo->bmiColors[nIndex].rgbGreen =
				pBitmapInfo->bmiColors[nIndex].rgbBlue = nIndex;
				pBitmapInfo->bmiColors[nIndex].rgbReserved = 0;
			}
		}
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, m_nWidth, m_nHeight, m_bpBits, pBitmapInfo.get(), DIB_RGB_COLORS, SRCCOPY);
		return TRUE;
	}
	return FALSE;
}

BOOL CPacs::Draw(CDC* pDC)
{
	shared_ptr<BITMAPINFO> pBitmapInfo;
	
	if(m_nBPP == 8){
		pBitmapInfo.reset(new BITMAPINFO[sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * GetPaletteNums())]);
	}
	else {
		pBitmapInfo.reset(new BITMAPINFO[sizeof(BITMAPINFO)]);
	}

	if (pBitmapInfo) {
		pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBitmapInfo->bmiHeader.biWidth = m_nWidth;
		pBitmapInfo->bmiHeader.biHeight = m_nHeight;
		pBitmapInfo->bmiHeader.biPlanes = 1;
		pBitmapInfo->bmiHeader.biBitCount = m_nBPP;
		pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		pBitmapInfo->bmiHeader.biSizeImage = 0;
		pBitmapInfo->bmiHeader.biClrImportant = 0;
		pBitmapInfo->bmiHeader.biClrUsed = 0;
		pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
		if (m_nBPP == 8) {
			for (int nIndex = 0; nIndex < 256; nIndex++) {
				pBitmapInfo->bmiColors[nIndex].rgbRed =
				pBitmapInfo->bmiColors[nIndex].rgbGreen =
				pBitmapInfo->bmiColors[nIndex].rgbBlue = nIndex;
				pBitmapInfo->bmiColors[nIndex].rgbReserved = 0;
			}
		}

		SetDIBitsToDevice(pDC->m_hDC
			, 0, 0, m_nWidth, m_nHeight
			, 0, 0, 0, m_nHeight
			, m_bpBits, pBitmapInfo.get(), DIB_RGB_COLORS
		);
		return TRUE;
	}
	return FALSE;
}

BOOL CPacs::IsValid()
{
	return m_bpBits != nullptr ? TRUE : FALSE;
}
	
int CPacs::GetPaletteNums() const
{
	switch (m_nBPP)
	{
	case 1:     return 2;
	case 4:     return 16;
	case 8:     return 256;
	default:    return 0;
	}
}


void CPacs::ColorSplitRGB(vector<BYTE>& imgR, vector<BYTE>& imgG, vector<BYTE>& imgB)
{
	RGBBYTE* rgbBits = (RGBBYTE*)m_bpBits;

	for (int i = 0; i < m_nSize; i++) {
		imgR[i] = rgbBits[i].r;
		imgG[i] = rgbBits[i].g;
		imgB[i] = rgbBits[i].b;
	}
}

void CPacs::RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V)
{
	Y = static_cast<BYTE>(limit(0.299 * R + 0.587 * G + 0.114 * B + 0.5));
	U = static_cast<BYTE>(limit(-0.169 * R - 0.331 * G + 0.500 * B + 128 + 0.5));
	V = static_cast<BYTE>(limit(0.500 * R - 0.419 * G - 0.081 * B + 128 + 0.5));
}
void CPacs::YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B)
{
	R = static_cast<BYTE>(limit(Y + 1.4075 * (V - 128) + 0.5));
	G = static_cast<BYTE>(limit(Y - 0.3455 * (U - 128) - 0.7169 * (V - 128) + 0.5));
	B = static_cast<BYTE>(limit(Y + 1.7790 * (U - 128) + 0.5));
}

void CPacs::ColorSplitYUV(vector<BYTE>& imgY, vector<BYTE>& imgU, vector<BYTE>& imgV)
{
	RGBBYTE* rgbBits = (RGBBYTE*)m_bpBits;

	for (int i = 0; i < m_nSize; i++)
	{
		RGB_TO_YUV(rgbBits[i].r, rgbBits[i].g, rgbBits[i].b, imgY[i], imgU[i], imgV[i]);
	}
}

void CPacs::ColorCombineYUV(vector<BYTE> imgY, vector<BYTE> imgU, vector<BYTE> imgV)
{
	RGBBYTE* rgbBits = (RGBBYTE*)m_bpBits;

	for (int i = 0; i < m_nSize; i++)
	{
		YUV_TO_RGB(imgY[i], imgU[i], imgV[i], rgbBits[i].r, rgbBits[i].g, rgbBits[i].b);
	}
}
