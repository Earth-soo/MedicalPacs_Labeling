#pragma once
#include "CPacs.h"
class CPacsEdge
{
private:
	shared_ptr<CPacs>& m_pacs;
public:
	CPacsEdge(shared_ptr<CPacs>& pacs) : m_pacs(pacs) {};

	void ImageEdgeRoverts();
	void ImageEdgePrewitt();
	void ImageEdgePrewitt(vector<BYTE>& bpBits, vector<BYTE>& bpDst);
	void ImageEdgeSobel();
	void ImageEdgeCanny();
	void ImageEdgeColor();
};