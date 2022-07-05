#pragma once
#include "CPacs.h"
//#include "CPacsColor.h"
//#include "CPacsEdge.h"

class CPacsSegment
{
private:
	CPacsPtr& m_pacs;

public:
	CPacsSegment(CPacsPtr& pacs) : m_pacs(pacs) {};

	void ImageBinarization(int threshold);
	int  ImageBinarizationIterative();

//	void IppContourTracing(int sx, int sy, std::vector<IppPoint>& cp);
	void RGB_TO_GRAY();
	void ImageLabeling();


	/*void Connectivity_4Direc(BYTE* pSrc, BYTE* pDst, int cnt, int p, int w, int h);*/
};

