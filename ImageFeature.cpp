#include "pch.h"
#include "ImageFeature.h"

void ImageEdgeRoverts(shared_ptr<CPacs> pPacs)
{
	int w = pPacs->GetWidth();
	int h = pPacs->GetHeight();
	int size = pPacs->GetSize();
	int i, j, h1, h2;
	double hval;
	int nBpp = pPacs->GetBpp();

	if (nBpp == 8) {
		BYTE* bpBits = pPacs->GetBits();
		shared_ptr<BYTE> bpDst(new BYTE[size]);

		for (j = 1; j < h; j++) {
			for (i = 1; i < w - 1; i++) {

				h1 = bpBits[j * w + i] - bpBits[j * w + i - 2];
				h2 = bpBits[j * w + i] - bpBits[j * w + i];

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

				bpDst.get()[j*w+i] = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		pPacs->SetBits(bpDst.get());
	}
	else if (nBpp == 24) {
		RGBBYTE* rgbpBits = (RGBBYTE*)pPacs->GetBits();
		shared_ptr<RGBBYTE> rgbpDst(new RGBBYTE[size]);

		for (j = 1; j < h; j++) {
			for (i = 1; i < w - 1; i++) {

				h1 = rgbpBits[j * w + i].r - rgbpBits[j * w + i - 2].r;
				h2 = rgbpBits[j * w + i].r - rgbpBits[j * w + i].r;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

				rgbpDst.get()[j * w + i].r = static_cast<BYTE>(limit(hval + 0.5));


				h1 = rgbpBits[j * w + i].g - rgbpBits[j * w + i - 2].g;
				h2 = rgbpBits[j * w + i].g - rgbpBits[j * w + i].g;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

				rgbpDst.get()[j * w + i].g = static_cast<BYTE>(limit(hval + 0.5));


				h1 = rgbpBits[j * w + i].b - rgbpBits[j * w + i - 2].b;
				h2 = rgbpBits[j * w + i].b - rgbpBits[j * w + i].b;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

				rgbpDst.get()[j * w + i].b = static_cast<BYTE>(limit(hval + 0.5));

				
			}
		}
		pPacs->SetBits(rgbpDst.get());
	}
}

void ImageEdgePrewitt(shared_ptr<CPacs> pPacs)
{
	int w = pPacs->GetWidth();
	int h = pPacs->GetHeight();
	int size = pPacs->GetSize();
	int i, j, h1, h2;
	double hval;
	int nBpp = pPacs->GetBpp();

	if (nBpp == 8) {

		BYTE* bpBits = pPacs->GetBits();
		shared_ptr<BYTE> bpDst(new BYTE[size]);
		
		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++) {
				h1 = -bpBits[j * w + i - 2] - bpBits[j * w + i - 1] - bpBits[j * w + i]
					+ bpBits[j * w + i] + bpBits[j * w + i + 1] + bpBits[j * w + i + 2];
				h2 = -bpBits[j * w + i - 2] - bpBits[j * w + i - 1] - bpBits[j * w + i]
					+ bpBits[j * w + i] + bpBits[j * w + i + 1] + bpBits[j * w + i + 2];

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				bpDst.get()[j * w + i] = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		pPacs->SetBits(bpDst.get());
	}
	else if (nBpp == 24) {
		RGBBYTE* rgbBits = (RGBBYTE*)pPacs->GetBits();
		shared_ptr<RGBBYTE> rgbpDst(new RGBBYTE[size]);

		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++) {
				h1 = -rgbBits[j * w + i - 2].r - rgbBits[j * w + i - 1].r - rgbBits[j * w + i].r
					+ rgbBits[j * w + i].r + rgbBits[j * w + i + 1].r + rgbBits[j * w + i + 2].r;
				h2 = -rgbBits[j * w + i - 2].r - rgbBits[j * w + i - 1].r - rgbBits[j * w + i].r
					+ rgbBits[j * w + i].r + rgbBits[j * w + i + 1].r + rgbBits[j * w + i + 2].r;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst.get()[j * w + i].r = static_cast<BYTE>(limit(hval + 0.5));

				h1 = -rgbBits[j * w + i - 2].g - rgbBits[j * w + i - 1].g - rgbBits[j * w + i].g
					+ rgbBits[j * w + i].g + rgbBits[j * w + i + 1].g + rgbBits[j * w + i + 2].g;
				h2 = -rgbBits[j * w + i - 2].g - rgbBits[j * w + i - 1].g - rgbBits[j * w + i].g
					+ rgbBits[j * w + i].g + rgbBits[j * w + i + 1].g + rgbBits[j * w + i + 2].g;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst.get()[j * w + i].g = static_cast<BYTE>(limit(hval + 0.5));

				h1 = -rgbBits[j * w + i - 2].b - rgbBits[j * w + i - 1].b - rgbBits[j * w + i].b
					+ rgbBits[j * w + i].b + rgbBits[j * w + i + 1].b + rgbBits[j * w + i + 2].b;
				h2 = -rgbBits[j * w + i - 2].b - rgbBits[j * w + i - 1].b - rgbBits[j * w + i].b
					+ rgbBits[j * w + i].b + rgbBits[j * w + i + 1].b + rgbBits[j * w + i + 2].b;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst.get()[j * w + i].b = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		pPacs->SetBits(rgbpDst.get());
	}
}


void ImageEdgeSobel(shared_ptr<CPacs> pPacs)
{
	int w = pPacs->GetWidth();
	int h = pPacs->GetHeight();
	int size = pPacs->GetSize();
	int i, j, h1, h2;
	double hval;
	int nBpp = pPacs->GetBpp();

	if (nBpp == 8) {

		//IMAGEDATA<BYTE> pByte(static_pointer_cast<BYTE>(pPacs->GetBits()) , size);
		//IMAGEDATA<RGBBYTE> pRGB(static_pointer_cast<RGBBYTE>(pPacs->GetBits()) , size);
		BYTE* bpBits = pPacs->GetBits();
		shared_ptr<BYTE> pDst(new BYTE[size]);


		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++) {
				h1 = -bpBits[j * w + i - 2] - 2 * bpBits[j * w + i - 1] - bpBits[j * w + i]
					+ bpBits[j * w + i] + 2 * bpBits[j * w + i + 1] + bpBits[j * w + i + 2];
				h2 = -bpBits[j * w + i - 2] - 2 * bpBits[j * w + i - 1] - bpBits[j * w + i]
					+ bpBits[j * w + i] + 2 * bpBits[j * w + i + 1] + bpBits[j * w + i + 2];
				hval = sqrt(static_cast<double>((h1 * h1) + (h2 * h2)));

				pDst.get()[j * w + i] = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		pPacs->SetBits(pDst.get());
	}
	else if (nBpp == 24) {
		RGBBYTE* rgbBits = (RGBBYTE*)pPacs->GetBits();
		shared_ptr<RGBBYTE> rgbpDst(new RGBBYTE[size]);

		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++) {
				h1 = -rgbBits[j * w + i - 2].r - 2 * rgbBits[j * w + i - 1].r - rgbBits[j * w + i].r
					+ rgbBits[j * w + i].r + 2 * rgbBits[j * w + i + 1].r + rgbBits[j * w + i + 2].r;
				h2 = -rgbBits[j * w + i - 2].r - 2 * rgbBits[j * w + i - 1].r - rgbBits[j * w + i].r
					+ rgbBits[j * w + i].r + 2 * rgbBits[j * w + i + 1].r + rgbBits[j * w + i + 2].r;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst.get()[j * w + i].r = static_cast<BYTE>(limit(hval + 0.5));

				h1 = -rgbBits[j * w + i - 2].g - 2 * rgbBits[j * w + i - 1].g - rgbBits[j * w + i].g
					+ rgbBits[j * w + i].g + 2 * rgbBits[j * w + i + 1].g + rgbBits[j * w + i + 2].g;
				h2 = -rgbBits[j * w + i - 2].g - 2 * rgbBits[j * w + i - 1].g - rgbBits[j * w + i].g
					+ rgbBits[j * w + i].g + 2 * rgbBits[j * w + i + 1].g + rgbBits[j * w + i + 2].g;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst.get()[j * w + i].g = static_cast<BYTE>(limit(hval + 0.5));

				h1 = -rgbBits[j * w + i - 2].b - 2 * rgbBits[j * w + i - 1].b - rgbBits[j * w + i].b
					+ rgbBits[j * w + i].b + 2 * rgbBits[j * w + i + 1].b + rgbBits[j * w + i + 2].b;
				h2 = -rgbBits[j * w + i - 2].b - 2 * rgbBits[j * w + i - 1].b - rgbBits[j * w + i].b
					+ rgbBits[j * w + i].b + 2 * rgbBits[j * w + i + 1].b + rgbBits[j * w + i + 2].b;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst.get()[j * w + i].b = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		pPacs->SetBits(rgbpDst.get());
	}
}

void ImageEdgeCanny(shared_ptr<CPacs> pPacs)
{

}