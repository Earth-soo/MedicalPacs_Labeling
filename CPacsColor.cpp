#include "pch.h"
#include "CPacsColor.h"
#include "RGBBYTE.h"

void CPacsColor::ImageInverse()
{
	int nBpp = m_pacs->GetBpp();
	int size = m_pacs->GetSize();

	RGBBYTE* rgbpBits = (RGBBYTE*)m_pacs->GetBits();
	
	/*
	for (int i = 0; i < size; i++) {
		rgbpBits[i].r = 255 - rgbpBits[i].r;
		rgbpBits[i].g = 255 - rgbpBits[i].g;
		rgbpBits[i].b = 255 - rgbpBits[i].b;
	}
	*/

	vector<BYTE> imgR(size);
	vector<BYTE> imgG(size);
	vector<BYTE> imgB(size);

	//m_pacs->RGB_TO_HSI(imgR, imgG, imgB);
}


