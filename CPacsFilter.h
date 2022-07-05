#pragma once
#include "CPacs.h"

class CPacsFilter
{
private:
	shared_ptr<CPacs>& m_pacs;
public:
	CPacsFilter(shared_ptr<CPacs>& pacs) : m_pacs(pacs) {};

	void ImageFilterGaussian(float sigma);

	void ImageFilterLaplacian();
	void ImageFilterUnsharpMask();
	void ImageFilterHighboost(float alpha);

	void ImageFilterDiffusion(float lambda, float k, int iter);
};

