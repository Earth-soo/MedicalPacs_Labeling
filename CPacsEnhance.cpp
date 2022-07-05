#include "pch.h"
#include "CPacsEnhance.h"
#include "RGBBYTE.h"



void CPacsEnhance::ImageHistogram(vector<float>& histo)
{
	BYTE* bpBits = m_pPacs->GetBits();
	const int nSize = m_pPacs->GetSize();
	vector<int> cntGvalue(256);
	for (int i = 0; i < nSize; i++) {
		cntGvalue[bpBits[i]]++;
	}

	for (int i = 0; i < 256; i++) {
		histo[i] = static_cast<float>(cntGvalue[i]) / nSize;
	}
}

void CPacsEnhance::ImageHistogram(vector<float>& histoR, vector<float>& histoG, vector<float>& histoB)
{
	int nSize = m_pPacs->GetSize();
	RGBBYTE* rgbpBits = (RGBBYTE*)m_pPacs->GetBits();

	vector<int> cntRvalue(256);
	vector<int> cntGvalue(256);
	vector<int> cntBvalue(256);


	for (int i = 0; i < nSize; i++) {
		cntRvalue[rgbpBits[i].r]++;
		cntGvalue[rgbpBits[i].g]++;
		cntBvalue[rgbpBits[i].b]++;
	}

	for (int i = 0; i < 256; i++) {
		histoR[i] = static_cast<float>(cntRvalue[i]) / nSize;
		histoG[i] = static_cast<float>(cntGvalue[i]) / nSize;
		histoB[i] = static_cast<float>(cntBvalue[i]) / nSize;
	}
}

void CPacsEnhance::ImageHistogramStretching()
{
	
	const int nBpp = m_pPacs->GetBpp();

	if (nBpp == 8) {
		ImageHistogramStretchingGray();
	}
	else if (nBpp == 24) {
		ImageHistogramStretchingRGB();
	}
}

void CPacsEnhance::ImageHistogramStretchingGray()
{
	const int nSize = m_pPacs->GetSize();
	BYTE* bpBits = m_pPacs->GetBits();

	BYTE minGray, maxGray;
	minGray = maxGray = bpBits[0];

	for (int i = 1; i < nSize; i++) {
		if (minGray > bpBits[i]) minGray = bpBits[i];
		if (maxGray < bpBits[i]) maxGray = bpBits[i];
	}

	if (minGray == maxGray)
		return;

	for (int i = 0; i < nSize; i++) {
		bpBits[i] = (bpBits[i] - minGray) * 255 / (maxGray - minGray);
	}
}

void CPacsEnhance::ImageHistogramStretchingRGB()
{
	const int nSize = m_pPacs->GetSize();
	RGBBYTE* rgbpBits = (RGBBYTE*)m_pPacs->GetBits();

	BYTE minR, minG, minB, maxR, maxG, maxB;
	minR = maxR = rgbpBits[0].r;
	minG = maxG = rgbpBits[0].g;
	minB = maxB = rgbpBits[0].b;

	for (int i = 1; i < nSize; i++) {
		if (minR > rgbpBits[i].r) minR = rgbpBits[i].r;
		if (minG > rgbpBits[i].g) minG = rgbpBits[i].g;
		if (minB > rgbpBits[i].b) minB = rgbpBits[i].b;

		if (maxR < rgbpBits[i].r) maxR = rgbpBits[i].r;
		if (maxG < rgbpBits[i].g) maxG = rgbpBits[i].g;
		if (maxB < rgbpBits[i].b) maxB = rgbpBits[i].b;
	}

	for (int i = 0; i < nSize; i++) {
		rgbpBits[i].r = (rgbpBits[i].r - minR) * 255 / (maxR - minR);
		rgbpBits[i].g = (rgbpBits[i].g - minG) * 255 / (maxG - minG);
		rgbpBits[i].b = (rgbpBits[i].b - minB) * 255 / (maxB - minB);
	}
}

void CPacsEnhance::ImageHistogramEqualization()
{
	int nBpp = m_pPacs->GetBpp();

	if (nBpp == 8) {
		ImageHistogramEqualizationGray();
	}
	else if (nBpp == 24) {
		ImageHistogramEqualizationRGB();
	}
	
}

void CPacsEnhance::ImageHistogramEqualizationGray() 
{
	const int nSize = m_pPacs->GetSize();
	BYTE* bpBits = m_pPacs->GetBits();
	vector<float> hist(256);
	ImageHistogram(hist);

	vector<float> cdf(256);
	cdf[0] = hist[0];
	for (int i = 1; i < 256; i++)
		cdf[i] = cdf[i - 1] + hist[i];

	// 히스토그램 균등화
	for (int i = 0; i < nSize; i++)
	{
		bpBits[i] = static_cast<BYTE>(limit(cdf[bpBits[i]] * 255));
	}
}

void CPacsEnhance::ImageHistogramEqualizationGray(vector<BYTE>& bpBits)
{
	const int nSize = m_pPacs->GetSize();

	vector<float> hist(256);
	ImageHistogram(hist);

	vector<float> cdf(256);
	cdf[0] = hist[0];
	for (int i = 1; i < 256; i++)
		cdf[i] = cdf[i - 1] + hist[i];

	// 히스토그램 균등화
	for (int i = 0; i < nSize; i++)
	{
		bpBits[i] = static_cast<BYTE>(limit(cdf[bpBits[i]] * 255));
	}
}

void CPacsEnhance::ImageHistogramEqualizationRGB()
{
	const int size = m_pPacs->GetSize();

	vector<BYTE> imgY(size);
	vector<BYTE> imgU(size);
	vector<BYTE> imgV(size);

	m_pPacs->ColorSplitYUV(imgY, imgU, imgV);
	ImageHistogramEqualizationGray(imgY);
	m_pPacs->ColorCombineYUV(imgY, imgU, imgV);
}


