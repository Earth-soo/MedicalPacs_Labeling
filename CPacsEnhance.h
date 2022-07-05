#pragma once
#include "CPacs.h"
class CPacsEnhance
{
private:
	shared_ptr<CPacs>& m_pPacs;
public:
	CPacsEnhance(shared_ptr<CPacs>& pacs) : m_pPacs(pacs) {};

	void ImageHistogram(vector<float>& histo);
	void ImageHistogram(vector<float>& histoR, vector<float>& histoG, vector<float>& histoB);
	void ImageHistogramStretching();
	void ImageHistogramStretchingGray();
	void ImageHistogramStretchingRGB();
	void ImageHistogramEqualization();
	void ImageHistogramEqualizationGray();
	void ImageHistogramEqualizationGray(vector<BYTE>& bpBits);
	void ImageHistogramEqualizationRGB();
};

