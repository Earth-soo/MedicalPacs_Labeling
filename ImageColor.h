#pragma once
#include "CPacs.h"
#include "RGBBYTE.h"

void RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V);
void YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B);

void ImageColorSplitRGB(CPacs* pacs, shared_ptr<BYTE> imgR, shared_ptr<BYTE> imgG, shared_ptr<BYTE> imgB);
void ImageColorSplitYUV(CPacs* pacs, shared_ptr<BYTE> imgY, shared_ptr<BYTE> imgU, shared_ptr<BYTE> imgV);

void ImageColorCombineYUV(shared_ptr<BYTE> imgY, shared_ptr<BYTE> imgU, shared_ptr<BYTE> imgV, CPacs* pacs);



//void ImageEdgeColor(shared_ptr<CPacs> pPacs);