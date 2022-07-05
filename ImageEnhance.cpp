#include "pch.h"
#include "ImageEnhance.h"
#include "RGBBYTE.h"

void ImageHistogram(BYTE* bpBits, float histo[256], const int size)
{
	int cntGvalue[256];
	memset(cntGvalue, 0, sizeof(int) * 256);
	for (int i = 0; i < size; i++) {
		cntGvalue[bpBits[i]]++;
	}

	for (int i = 0; i < 256; i++) {
		histo[i] = static_cast<float>(cntGvalue[i]) / size;
	}
}

void ImageHistogram(CPacs* pPacs, float histoR[256], float histoG[256], float histoB[256])
{
	int nSize = pPacs->GetSize();
	RGBBYTE* rgbpBits = (RGBBYTE*)pPacs->GetBits();

	int cntRvalue[256];
	int cntGvalue[256];
	int cntBvalue[256];

	memset(cntRvalue, 0, sizeof(int) * 256);
	memset(cntGvalue, 0, sizeof(int) * 256);
	memset(cntBvalue, 0, sizeof(int) * 256);

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

void ImageHistogramStretching(CPacs* pPacs)
{
	int nSize = pPacs->GetSize();
	int nBpp = pPacs->GetBpp();

	if (nBpp == 8) {
		BYTE* bpBits = pPacs->GetBits();

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
	else if (nBpp == 24) {
		RGBBYTE* rgbpBits = (RGBBYTE*)pPacs->GetBits();

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
}

void ImageHistogramEqualization(BYTE* bpBits, const int size)
{
	float hist[256];
	ImageHistogram(bpBits, hist, size);

	float cdf[256] = { 0.0, };
	cdf[0] = hist[0];
	for (int i = 1; i < 256; i++)
		cdf[i] = cdf[i - 1] + hist[i];

	// 히스토그램 균등화
	for (int i = 0; i < size; i++)
	{
		bpBits[i] = static_cast<BYTE>(limit(cdf[bpBits[i]] * 255));
	}
}