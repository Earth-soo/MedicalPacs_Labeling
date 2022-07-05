#include "pch.h"
#include "CPacsFilter.h"
#include "RGBBYTE.h"

const double PI = 3.14159265358979323846;
const float  PI_F = 3.14159265358979323846f;

void CPacsFilter::ImageFilterGaussian(float sigma)
{
	register int i, j, k, x;

	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();
	int size = m_pacs->GetSize();
	int nBpp = m_pacs->GetBpp();

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
		BYTE* pSrc = m_pacs->GetBits();
		BYTE* pDst = new BYTE[size];
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

				pDst[j * w + i] = sum2 / sum1;
			}

		m_pacs->SetCopy(pDst);
	}
	else if (nBpp == 24) {
		RGBBYTE* pSrc = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* pDst = new RGBBYTE[size];
		shared_ptr<RGBBYTE> pBuf(new RGBBYTE[size]);

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
				pDst[j * w + i].r = sumR2 / sumR1;
				pDst[j * w + i].g = sumG2 / sumG1;
				pDst[j * w + i].b = sumB2 / sumB1;
			}
		}
		m_pacs->SetCopy(pDst);

	}

}

void CPacsFilter::ImageFilterLaplacian()
{
	int nBpp = m_pacs->GetBpp();

	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();
	int size = m_pacs->GetSize();

	int i, j, sum;

	if (nBpp == 8) 
	{
		BYTE* pSrc = m_pacs->GetBits();
		BYTE* pDst = new BYTE[size];
	
		
		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++)
			{
				sum = pSrc[j * w + i -1] + pSrc[j * w + i - 1] + pSrc[j * w + i + 1] + pSrc[j * w + i + 1]
					- 4 * pSrc[j * w + i];

				pDst[j * w + i] = static_cast<BYTE>(limit(sum + 128));
			}
		}
		m_pacs->SetCopy(pDst);

	}

	else if (nBpp == 24)
	{
		RGBBYTE* pSrc = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* pDst = new RGBBYTE[size];

		int sumR, sumG, sumB;

		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++)
			{
				sumR = pSrc[j * w + i -1].r + pSrc[j * w + i - 1].r + pSrc[j * w + i + 1].r + pSrc[j * w + i + 1].r
					- 4 * pSrc[j * w + i].r;
				sumG = pSrc[j * w + i - 1].g + pSrc[j * w + i - 1].r + pSrc[j * w + i + 1].g + pSrc[j * w + i + 1].g
					- 4 * pSrc[j * w + i].g;
				sumB = pSrc[j * w + i - 1].b + pSrc[j * w + i - 1].r + pSrc[j * w + i + 1].b + pSrc[j * w + i + 1].b
					- 4 * pSrc[j * w + i].b;

				pDst[j * w + i].r = static_cast<BYTE>(limit(sumR + 128));
				pDst[j * w + i].g = static_cast<BYTE>(limit(sumG + 128));
				pDst[j * w + i].b = static_cast<BYTE>(limit(sumB + 128));
			}
		}
		m_pacs->SetCopy(pDst);
	}

}

void CPacsFilter::ImageFilterUnsharpMask()
{
	int nBpp = m_pacs->GetBpp();

	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();
	int size = m_pacs->GetSize();

	int i, j, sum;

	if (nBpp == 8)
	{
		BYTE* pSrc = m_pacs->GetBits();
		BYTE* pDst = new BYTE[size];


		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++)
			{
				sum = 5 * pSrc[j * w + i] - pSrc[j * w + i - 1] - pSrc[j * w + i - 1] - pSrc[j * w + i + 1] - pSrc[j * w + i + 1];

				pDst[j * w + i] = static_cast<BYTE>(limit(sum));
			}
		}
		m_pacs->SetCopy(pDst);

	}

	else if (nBpp == 24)
	{
		RGBBYTE* pSrc = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* pDst = new RGBBYTE[size];

		int sumR, sumG, sumB;

		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++)
			{
				sumR = 5 * pSrc[j * w + i].r - pSrc[j * w + i - 1].r - pSrc[j * w + i - 1].r - pSrc[j * w + i + 1].r - pSrc[j * w + i + 1].r;
				sumG = 5 * pSrc[j * w + i].g - pSrc[j * w + i - 1].g - pSrc[j * w + i - 1].g - pSrc[j * w + i + 1].g - pSrc[j * w + i + 1].g;
				sumB = 5 * pSrc[j * w + i].b - pSrc[j * w + i - 1].b - pSrc[j * w + i - 1].b - pSrc[j * w + i + 1].b - pSrc[j * w + i + 1].b;

				pDst[j * w + i].r = static_cast<BYTE>(limit(sumR));
				pDst[j * w + i].g = static_cast<BYTE>(limit(sumG));
				pDst[j * w + i].b = static_cast<BYTE>(limit(sumB));
			}
		}
		m_pacs->SetCopy(pDst);
	}
}

void CPacsFilter::ImageFilterHighboost(float alpha) 
{
	int nBpp = m_pacs->GetBpp();

	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();
	int size = m_pacs->GetSize();

	int i, j, sum;

	if (nBpp == 8)
	{
		BYTE* pSrc = m_pacs->GetBits();
		BYTE* pDst = new BYTE[size];


		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++)
			{
				sum = (4 + alpha) * pSrc[j * w + i] - pSrc[j * w + i - 1] - pSrc[j * w + i - 1] - pSrc[j * w + i + 1] - pSrc[j * w + i + 1];

				pDst[j * w + i] = static_cast<BYTE>(limit(sum + 0.5f));
			}
		}
		m_pacs->SetCopy(pDst);

	}

	else if (nBpp == 24)
	{
		RGBBYTE* pSrc = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* pDst = new RGBBYTE[size];

		float sumR;
		float sumG;
		float sumB;

		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++)
			{
				sumR = (4 + alpha) * pSrc[j * w + i].r - pSrc[j * w + i - 1].r - pSrc[j * w + i - 1].r - pSrc[j * w + i + 1].r - pSrc[j * w + i + 1].r;
				sumG = (4 + alpha) * pSrc[j * w + i].g - pSrc[j * w + i - 1].g - pSrc[j * w + i - 1].g - pSrc[j * w + i + 1].g - pSrc[j * w + i + 1].g;
				sumB = (4 + alpha) * pSrc[j * w + i].b - pSrc[j * w + i - 1].b - pSrc[j * w + i - 1].b - pSrc[j * w + i + 1].b - pSrc[j * w + i + 1].b;

				pDst[j * w + i].r = static_cast<BYTE>(limit(sumR + 0.5f));
				pDst[j * w + i].g = static_cast<BYTE>(limit(sumG + 0.5f));
				pDst[j * w + i].b = static_cast<BYTE>(limit(sumB + 0.5f));
			}
		}
		m_pacs->SetCopy(pDst);
	}
}

void CPacsFilter::ImageFilterDiffusion(float lambda, float k, int iter) 
{
	int nBpp = m_pacs->GetBpp();

	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();
	int size = m_pacs->GetSize();


	register int i, x, y;
	float k2 = k * k;

	
	shared_ptr<float> imgCpy(new float[size]);
	//imgCpy.Convert(imgSrc);

	//imgDst = imgCpy;

	if (nBpp == 8)
	{
		float gradn, grads, grade, gradw;
		float gcn, gcs, gce, gcw;

		BYTE* pCpy = m_pacs->GetBits();
		BYTE* pDst = new BYTE[size];

		shared_ptr<float> imgCpy(new float[size]);
		shared_ptr<float> imgDst(new float[size]);

		for (i = 0; i < iter; i++)
		{
			for (y = 1; y < h - 1; y++)
				for (x = 1; x < w - 1; x++)
				{
					gradn = pCpy[y * w + x - 1] - pCpy[y * w + x];
					grads = pCpy[y * w + x + 1] - pCpy[y * w + x];
					grade = pCpy[y * w + x - 1] - pCpy[y * w + x];
					gradw = pCpy[y * w + x + 1] - pCpy[y * w + x];

					gcn = gradn / (1.0f + gradn * gradn / k2);
					gcs = grads / (1.0f + grads * grads / k2);
					gce = grade / (1.0f + grade * grade / k2);
					gcw = gradw / (1.0f + gradw * gradw / k2);

					pDst[y * w + x] = pCpy[y * w + x] + lambda * (gcn + gcs + gce + gcw);
				}

			// 버퍼 복사 (여기서 오류~!~!~!!~~!~!!~~!~!~!)=======================
			if (i < iter - 1)
				memcpy(imgCpy.get(), imgDst.get(), sizeof(float) * w * h);
		}
		m_pacs->SetCopy(imgDst.get());
	}

	if (nBpp == 24)
	{
		RGBBYTE* pCpy = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* pDst = new RGBBYTE[size];

		shared_ptr<float> imgCpy(new float[size]);
		shared_ptr<float> imgDst(new float[size]);

		float gradnR, gradsR, gradeR, gradwR, gradnG, gradsG, gradeG, gradwG, gradnB, gradsB, gradeB, gradwB;
		float gcnR, gcsR, gceR, gcwR, gcnG, gcsG, gceG, gcwG, gcnB, gcsB, gceB, gcwB;

		for (i = 0; i < iter; i++)
		{
			for (y = 1; y < h - 1; y++)
				for (x = 1; x < w - 1; x++)
				{
					gradnR = pCpy[y * w + x - 1].r - pCpy[y * w + x].r;
					gradsR = pCpy[y * w + x + 1].r - pCpy[y * w + x].r;
					gradeR = pCpy[y * w + x - 1].r - pCpy[y * w + x].r;
					gradwR = pCpy[y * w + x + 1].r - pCpy[y * w + x].r;

					gradnG = pCpy[y * w + x - 1].g - pCpy[y * w + x].g;
					gradsG = pCpy[y * w + x + 1].g - pCpy[y * w + x].g;
					gradeG = pCpy[y * w + x - 1].g - pCpy[y * w + x].g;
					gradwG = pCpy[y * w + x + 1].g - pCpy[y * w + x].g;

					gradnB = pCpy[y * w + x - 1].b - pCpy[y * w + x].b;
					gradsB = pCpy[y * w + x + 1].b - pCpy[y * w + x].b;
					gradeB = pCpy[y * w + x - 1].b - pCpy[y * w + x].b;
					gradwB = pCpy[y * w + x + 1].b - pCpy[y * w + x].b;

					gcnR = gradnR / (1.0f + gradnR * gradnR / k2);
					gcsR = gradsR / (1.0f + gradsR * gradsR / k2);
					gceR = gradeR / (1.0f + gradeR * gradeR / k2);
					gcwR = gradwR / (1.0f + gradwR * gradwR / k2);

					gcnG = gradnG / (1.0f + gradnG * gradnG / k2);
					gcsG = gradsG / (1.0f + gradsG * gradsG / k2);
					gceG = gradeG / (1.0f + gradeG * gradeG / k2);
					gcwG = gradwG / (1.0f + gradwG * gradwG / k2);

					gcnB = gradnB / (1.0f + gradnB * gradnB / k2);
					gcsB = gradsB / (1.0f + gradsB * gradsB / k2);
					gceB = gradeB / (1.0f + gradeB * gradeB / k2);
					gcwB = gradwB / (1.0f + gradwB * gradwB / k2);

					pDst[y * w + x].r = pCpy[y * w + x].r + lambda * (gcnR + gcsR + gceR + gcwR);
					pDst[y * w + x].g = pCpy[y * w + x].g + lambda * (gcnG + gcsG + gceG + gcwG);
					pDst[y * w + x].b = pCpy[y * w + x].b + lambda * (gcnB + gcsB + gceB + gcwB);
				}

			// 버퍼 복사 (여기서 오류~!~!~!!~~!~!!~~!~!~!)=======================
			if (i < iter - 1)
				memcpy(imgCpy.get(), imgDst.get(), sizeof(float) * w * h);
		}
		m_pacs->SetCopy(imgDst.get());
	}
}
