#pragma once
#include "CPacs.h"

class CPacsColor
{
private:
	shared_ptr<CPacs>& m_pacs;

public:
	CPacsColor(shared_ptr<CPacs>& pacs) : m_pacs(pacs) {};

	void ImageInverse();
	

};

