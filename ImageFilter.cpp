#include "pch.h"
#include "ImageFilter.h"
#include "RGBBYTE.h"

const double PI = 3.14159265358979323846;
const float  PI_F = 3.14159265358979323846f;

void ImageFilterGaussian(shared_ptr<CPacs> pPacs, float sigma)
{
	register int i, j, k, x;

	int w = pPacs->GetWidth();
	int h = pPacs->GetHeight();
	int size = pPacs->GetSize();
	int nBpp = pPacs->GetBpp();

	//-------------------------------------------------------------------------
		// 1차원 가우시안 마스크 & 실수 연산을 위한 버퍼 이미지 생성
		//-------------------------------------------------------------------------
	int dim = static_cast<int>(2 * 4 * sigma + 1.0);
	if (dim < 3) dim = 3;
	if (dim % 2 == 0) dim++;
	int dim2 = dim / 2;

	shared_ptr<float> pMask(new float[size]);

	for (i = 0; i < dim; i++)
	{
		x = i - dim2;
		pMask.get()[i] = exp(-(x * x) / (2 * sigma * sigma)) / (sqrt(2 * PI_F) * sigma);
	}

	if (nBpp == 8) {
		BYTE* pSrc = pPacs->GetBits();
		shared_ptr<BYTE> pDst(new BYTE[size]);
		shared_ptr<float> pBuf(new float[size]);

		//-------------------------------------------------------------------------
		// 세로 방향 마스크 연산
		//-------------------------------------------------------------------------

		float sum1, sum2;
		for (i = 0; i < w; i++)
			for (j = 0; j < h; j++)
			{
				sum1 = sum2 = 0.f;

				for (k = 0; k < dim; k++)
				{
					x = k - dim2 + j;

					if (x >= 0 && x < h)
					{
						sum1 += pMask.get()[k];
						sum2 += (pMask.get()[k] * pSrc[x * w + i]);
					}
				}

				pBuf.get()[j * w + i] = sum2 / sum1;
			}

		//-------------------------------------------------------------------------
		// 가로 방향 마스크 연산
		//-------------------------------------------------------------------------

		for (j = 0; j < h; j++)
			for (i = 0; i < w; i++)
			{
				sum1 = sum2 = 0.f;

				for (k = 0; k < dim; k++)
				{
					x = k - dim2 + i;

					if (x >= 0 && x < w)
					{
						sum1 += pMask.get()[k];
						sum2 += (pMask.get()[k] * pBuf.get()[j * w + x]);
					}
				}

				pDst.get()[j * w + i] = sum2 / sum1;
			}

		pPacs->SetBits(pDst.get());
	}
	else if (nBpp == 24) {
		RGBBYTE* pSrc = (RGBBYTE*)pPacs->GetBits();
		shared_ptr<RGBBYTE> pDst(new RGBBYTE[size]);
		shared_ptr<FRGBBYTE> pBuf(new FRGBBYTE[size]);

		//-------------------------------------------------------------------------
		// 세로 방향 마스크 연산
		//-------------------------------------------------------------------------

		float sumR1, sumR2, sumG1, sumG2, sumB1, sumB2;
		for (i = 0; i < w; i++) {
			for (j = 0; j < h; j++)
			{
				sumR1 = sumR2 = sumG1 = sumG2 = sumB1 = sumB2 = 0.f;

				for (k = 0; k < dim; k++)
				{
					x = k - dim2 + j;

					if (x >= 0 && x < h)
					{
						sumR1 += pMask.get()[k];
						sumR2 += (pMask.get()[k] * pSrc[x * w + i].r);
						sumG1 += pMask.get()[k];
						sumG2 += (pMask.get()[k] * pSrc[x * w + i].g);
						sumB1 += pMask.get()[k];
						sumB2 += (pMask.get()[k] * pSrc[x * w + i].b);
					}
				}

				pBuf.get()[j * w + i].r = sumR2 / sumR1;
				pBuf.get()[j * w + i].g = sumG2 / sumG1;
				pBuf.get()[j * w + i].b = sumB2 / sumB1;
			}
		}
			

		//-------------------------------------------------------------------------
		// 가로 방향 마스크 연산
		//-------------------------------------------------------------------------

		for (j = 0; j < h; j++) {
			for (i = 0; i < w; i++) {
				sumR1 = sumR2 = sumG1 = sumG2 = sumB1 = sumB2 = 0.f;

				for (k = 0; k < dim; k++) {
					x = k - dim2 + i;

					if (x >= 0 && x < w) {
						sumR1 += pMask.get()[k];
						sumR2 += (pMask.get()[k] * pBuf.get()[j * w + x].r);
						sumG1 += pMask.get()[k];
						sumG2 += (pMask.get()[k] * pBuf.get()[j * w + x].g);
						sumB1 += pMask.get()[k];
						sumB2 += (pMask.get()[k] * pBuf.get()[j * w + x].b);
					}
				}
				pDst.get()[j * w + i].r = sumR2 / sumR1;
				pDst.get()[j * w + i].g = sumG2 / sumG1;
				pDst.get()[j * w + i].b = sumB2 / sumB1;
			}
		}
		pPacs->SetBits(pDst.get());
		
	}
	
}