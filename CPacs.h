#pragma once
#include <tuple>

#define RGB2GRAY(r, g, b) (0.299*(r) + 0.587*(g) + 0.114*(b))

//template<typename T>
//struct IMAGEDATA {
//	shared_ptr<T> bpBits;
//	shared_ptr<T> bpDst;
//
//	IMAGEDATA(T* p, size_t size) : bpBits(p), bpDst(new T[size]) {}
//};

class CPacs;
using CPacsPtr = shared_ptr<CPacs>;

class CPacs
{
private:
	int		m_nWidth;	// 이미지 가로 크기
	int		m_nHeight;	// 이미지 세로 크기
	int		m_nSize;	// 이미지 크기
	//float	m_nSize;	// 이미지 크기
	int		m_nPitch;	// 픽셀 데이터 실제 가로 크기
	int		m_nBPP;		// 이미지 비트 수 8, 24, 32
	int		m_nPsize;
	int		m_nRealSize;
	BYTE*	m_bpBits;	// 이미지 픽셀 데이터

public:

	CImage	m_CImage;	// Read & Write

	CPacs();
	~CPacs();
	CPacs(const CPacs& r);
	CPacs(CPacsPtr pPacs);

	BOOL Load(LPCTSTR lpszPathName);
	void SetData();
	void SetCopy(void* pBits);
	void MemCopy(void* pBits);	
	void SwapBits(CPacsPtr pPacs);	//수정
	BOOL Draw(CDC* pDC);
	BOOL Draw(CDC* pDC, CRect rect);


	int   GetWidth()	{ return m_nWidth; };
	int   GetHeight()	{ return m_nHeight; };
	int   GetSize()		{ return m_nSize; };
	int   GetBpp()		{ return m_nBPP; };
	int	  GetPitch()	{ return m_nPitch; };
	int	  GetRsize()	{ return m_nPsize; };
	BYTE* GetBits()		{ return m_bpBits; };
	int   GetPaletteNums() const;


	BOOL IsValid();
	void Resize(void* pBits, int nNewWidth, int nNewHeight);
	void RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V);
	void YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B);
	void ColorSplitRGB(vector<BYTE>& imgR, vector<BYTE>& imgG, vector<BYTE>& imgB);
	void ColorSplitYUV(vector<BYTE>& imgY, vector<BYTE>& imgU, vector<BYTE>& imgV);
	void ColorCombineYUV(vector<BYTE> imgY, vector<BYTE> imgU, vector<BYTE> imgV);


};

template<typename T>
inline T limit(const T& value)
{
	return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
}

template<typename T>
inline T limit(const T& value, const T& lower, const T& upper)
{
	return ((value > upper) ? upper : ((value < lower) ? lower : value));
}

inline float GetMaxFreqHist(vector<float> hist)
{
	float maxValue = hist[0];

	for (int i = 1; i < 256; i++) {
		if (maxValue < hist[i])
			maxValue = hist[i];
	}

	return maxValue;
}
