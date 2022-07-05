#include "pch.h"
#include "ImageColor.h"
#include "RGBBYTE.h"
#include "ImageFeature.h"

void ImageColorSplitRGB(CPacs* pacs, shared_ptr<BYTE> imgR, shared_ptr<BYTE> imgG, shared_ptr<BYTE> imgB)
{
	const int size = pacs->GetSize();
	RGBBYTE* rgbBits = (RGBBYTE*)pacs->GetBits();

	for (int i = 0; i < size; i++) {
		imgR.get()[i] = rgbBits[i].r;
		imgG.get()[i] = rgbBits[i].g;
		imgB.get()[i] = rgbBits[i].b;
	}
}

void RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V)
{
	Y = static_cast<BYTE>(limit(0.299 * R + 0.587 * G + 0.114 * B + 0.5));
	U = static_cast<BYTE>(limit(-0.169 * R - 0.331 * G + 0.500 * B + 128 + 0.5));
	V = static_cast<BYTE>(limit(0.500 * R - 0.419 * G - 0.081 * B + 128 + 0.5));
}
void YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B)
{
	R = static_cast<BYTE>(limit(Y + 1.4075 * (V - 128) + 0.5));
	G = static_cast<BYTE>(limit(Y - 0.3455 * (U - 128) - 0.7169 * (V - 128) + 0.5));
	B = static_cast<BYTE>(limit(Y + 1.7790 * (U - 128) + 0.5));
}

void ImageColorSplitYUV(CPacs* pacs, shared_ptr<BYTE> imgY, shared_ptr<BYTE> imgU, shared_ptr<BYTE> imgV)
{
	const int size = pacs->GetSize();
	RGBBYTE* rgbBits = (RGBBYTE*)pacs->GetBits();

	for (int i = 0; i < size; i++)
	{
		RGB_TO_YUV(rgbBits[i].r, rgbBits[i].g, rgbBits[i].b, imgY.get()[i], imgU.get()[i], imgV.get()[i]);
	}
}

void ImageColorCombineYUV(shared_ptr<BYTE> imgY, shared_ptr<BYTE> imgU, shared_ptr<BYTE> imgV, CPacs* pacs)
{
	const int size = pacs->GetSize();
	RGBBYTE* rgbBits = (RGBBYTE*)pacs->GetBits();

	for (int i = 0; i < size; i++)
	{
		YUV_TO_RGB(imgY.get()[i], imgU.get()[i], imgV.get()[i], rgbBits[i].r, rgbBits[i].g, rgbBits[i].b);
	}
}


