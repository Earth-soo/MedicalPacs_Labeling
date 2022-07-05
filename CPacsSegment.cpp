#include "pch.h"
#include "CPacsSegment.h"
#include "RGBBYTE.h"
#include "CPacsEnhance.h"
#include "CPacsColor.h"

//이진화
void CPacsSegment::ImageBinarization(int threshold)
{
	int nBpp = m_pacs->GetBpp();
	
	int size = m_pacs->GetSize();

	if (nBpp == 8)
	{
		BYTE* bpBits = m_pacs->GetBits();
		BYTE* bpDst = new BYTE[size];

		for (int i = 0; i < size; i++)
		{
			bpDst[i] = (bpBits[i] <= threshold) ? 0 : 255;
		}
		m_pacs->SetCopy(bpDst);
	}

	else if (nBpp == 24)
	{
		RGB_TO_GRAY();

		RGBBYTE* rgbpBits = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* rgbpDst = new RGBBYTE[size];

		
		for (int i = 0; i < size; i++)
		{
			rgbpDst[i].r = (rgbpBits[i].r <= threshold) ? 0 : 255;
			rgbpDst[i].g = (rgbpBits[i].g <= threshold) ? 0 : 255;
			rgbpDst[i].b = (rgbpBits[i].b <= threshold) ? 0 : 255;
		}
		m_pacs->SetCopy(rgbpDst);
	}
}

//트루->그레이
void CPacsSegment::RGB_TO_GRAY()
{
	int nBpp = m_pacs->GetBpp();
	int size = m_pacs->GetSize();
	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();

	float rsum;

	int i, j;

	if (nBpp == 24)
	{
		RGBBYTE* rgbBits = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* rgbpDst = new RGBBYTE[size];

		for (j = 1; j < h - 1; j++)
		{
			for (i = 1; i < w - 1; i++)
			{
				rsum = 0.0;
				rsum = 0.299 * rgbBits[j * w + i].r + 0.587 * rgbBits[j * w + i].g + 0.114 * rgbBits[j * w + i].b;

				rgbpDst[j * w + i].r = static_cast<BYTE>(limit(rsum));
				rgbpDst[j * w + i].g = static_cast<BYTE>(limit(rsum));
				rgbpDst[j * w + i].b = static_cast<BYTE>(limit(rsum));
				
			
			}
		}
		m_pacs->SetCopy(rgbpDst);
	}

}

//임계값 자동설정
int  CPacsSegment::ImageBinarizationIterative()
{
	int nBpp = m_pacs->GetBpp();
	
	if (nBpp == 8)
	{
		
		vector<float> hist(256);
		//ImageHistogram(hist);

		int i, T, Told;

		float sum = 0.f;
		for (i = 0; i < 256; i++)
			sum += (i * hist[i]);

		T = static_cast<int>(sum + .5f);

		// 반복에 의한 임계값 결정

		float a1, b1, u1, a2, b2, u2;
		do {
			Told = T;

			a1 = b1 = u1 = 0.f;
			for (i = 0; i <= Told; i++)
			{
				a1 += (i * hist[i]);
				b1 += hist[i];
			}

			if (b1 != 0.f)
				u1 = a1 / b1;

			a2 = b2 = u2 = 0.f;
			for (i = Told + 1; i < 256; i++)
			{
				a2 += (i * hist[i]);
				b2 += hist[i];
			}

			if (b2 != 0.f)
				u2 = a2 / b2;

			T = static_cast<int>((u1 + u2) / 2 + 0.5f);
		} while (T != Told);

		return T;
	}

	
//	return 0;
}


void CPacsSegment::ImageLabeling() 
{
	const int nWidth = m_pacs->GetWidth();
	const int nHeight = m_pacs->GetHeight();
	const int nsize = m_pacs->GetRsize();

	BYTE* pbSrcBits = m_pacs->GetBits();
	BYTE* pbTmpBits = new BYTE[nsize]{0};
	
	const int maxLabel = 10000;
	int* eq_tbl = new int[maxLabel * 2]{ 0 }; //등가테이블 생성(초기화 된 상태)

	//-------------------------------------------------------------------------
	// 첫 번째 스캔 - 초기 레이블 지정 및 등가 테이블 생성
	//-------------------------------------------------------------------------

	int nLabel = 0, maxl, minl, min_eq, max_eq, i, j;

	for (j = 1; j < nHeight; j++)
		for (i = 1; i < nWidth; i++)
		{
			if (pbSrcBits[j * nWidth + i] == 255)
			{
				// 바로 위 픽셀과 왼쪽 픽셀 모두에 레이블이 존재하는 경우
				if ((pbTmpBits[(j - 1) * nWidth + i] != 0) && (pbTmpBits[j * nWidth + (i - 1)] != 0))
				{
					if (pbTmpBits[(j - 1) * nWidth + i] == pbTmpBits[j * nWidth + (i - 1)])
					{
						// 두 레이블이 서로 같은 경우
						pbTmpBits[j * nWidth + i] = pbTmpBits[(j - 1) * nWidth + i];
					}
					else
					{
						// 두 레이블이 서로 다른 경우, 작은 레이블을 부여
						maxl = max(pbTmpBits[(j - 1) * nWidth + i], pbTmpBits[j * nWidth + (i - 1)]);
						minl = min(pbTmpBits[(j - 1) * nWidth + i], pbTmpBits[j * nWidth + (i - 1)]);

						pbTmpBits[j * nWidth + i] = minl;

						// 등가 테이블 조정(위, 왼쪽 조정)
						min_eq = min(eq_tbl[maxl * 2 + 1], eq_tbl[minl * 2 + 1]);
						max_eq = max(eq_tbl[maxl * 2 + 1], eq_tbl[minl * 2 + 1]);

						eq_tbl[(eq_tbl[max_eq * 2 + 1]) * 2 + 1] = min_eq;
					}
				}
				else if (pbTmpBits[(j - 1) * nWidth + i] != 0)
				{
					// 바로 위 픽셀에만 레이블이 존재할 경우
					pbTmpBits[j * nWidth + i] = pbTmpBits[(j - 1) * nWidth + i];
				}
				else if (pbTmpBits[j * nWidth + (i - 1)] != 0)
				{
					// 바로 왼쪽 픽셀에만 레이블이 존재할 경우
					pbTmpBits[j * nWidth + i] = pbTmpBits[j * nWidth + (i - 1)];
				}
				else
				{
					// 이웃에 레이블이 존재하지 않으면 새로운 레이블을 부여
					nLabel++;
					pbTmpBits[j * nWidth + i] = nLabel;
					eq_tbl[nLabel * 2 + 0] = nLabel;
					eq_tbl[nLabel * 2 + 1] = nLabel;
				}
			}
		}

	//-------------------------------------------------------------------------
	// 등가 테이블 정리 (2번 조정해야 하는 것들 정리)
	//-------------------------------------------------------------------------

	int temp;
	for (i = 1; i <= nLabel; i++)
	{
		temp = eq_tbl[i * 2 + 1];
		if (temp != eq_tbl[i * 2 + 0])
			eq_tbl[i * 2 + 1] = eq_tbl[temp * 2 + 1];
	}

	// 등가 테이블의 레이블을 1부터 차례대로 증가시키기

	int* hash = new int[nLabel + 1]{ 0 };
	/*memset(hash, 0, sizeof(int) * (nLabel + 1));*/

	for (i = 1; i <= nLabel; i++)
		hash[eq_tbl[i * 2 + 1]] = eq_tbl[i * 2 + 1];

	int label_cnt = 1;
	for (i = 1; i <= nLabel; i++)
		if (hash[i] != 0)
			hash[i] = label_cnt++;

	for (i = 1; i <= nLabel; i++)
		eq_tbl[i * 2 + 1] = hash[eq_tbl[i * 2 + 1]];

	delete[] hash;

	//-------------------------------------------------------------------------
	// 두 번째 스캔 - 등가 테이블을 이용하여 모든 픽셀에 고유의 레이블 부여
	//-------------------------------------------------------------------------
	
	BYTE* pbDstBits = new BYTE[nsize]{ 0 };

	int idx;

	for (j = 1; j < nHeight; j++) {
		for (i = 1; i < nWidth; i++)
		{
			if (pbTmpBits[j * nWidth + i] != 0)
			{
				idx = pbTmpBits[j * nWidth + i];
				pbDstBits[j * nWidth + i] = eq_tbl[i * nWidth + 1];
			}
		}
	}

	int* rgbArr = new int[nLabel];

	for (int k = 0; k < nLabel; k++) {	//명암 설정
		rgbArr[k] = (k + 1) * static_cast <int> (255 / nLabel);
		TRACE("%d\n", rgbArr[k]);
	}

	for (int h = 0; h < nHeight - 1; h++)
	{
		for (int w = 0; w < nWidth - 1; w++)
		{
			if (pbDstBits[h * nWidth + w] == 0)
				pbDstBits[h * nWidth + w] = 0;
		
			else
			{
				int labelValue = pbDstBits[h * nWidth + w] - 1;
				TRACE("\n%d\n", labelValue);
				pbDstBits[h * nWidth + w] = limit(rgbArr[labelValue]);	//그레이스 컬러, 명암
				//pbDstBits[h * nWidth + w] = RGB(limit(rgbArr[labelValue]), limit(rgbArr[labelValue]), limit(rgbArr[labelValue]));	//그레이스 컬러, 명암
				TRACE("%d\n", pbDstBits[h * nWidth + w]);
			}
		}
	}
		
		m_pacs->SetCopy(pbDstBits);
		delete[] rgbArr;
}



//void CPacsSegment::ImageLabeling()
//{
//	const int nWidth = m_pacs->GetWidth();
//	const int nHeight = m_pacs->GetHeight();
//	const int nsize = m_pacs->GetRsize();
//
//	BYTE* pbSrcBits = m_pacs->GetBits();
//	BYTE* pbDstBits = new BYTE[nsize]{ 0 };
//
//	int nLabel = 0;
//
//	for (int j = 0; j < nHeight - 1; j++) {
//		for (int i = 0; i < nWidth - 1; i++) {
//			if (pbSrcBits[j * nWidth + i] == 255 && pbDstBits[j * nWidth + i] == 0) {	//이진화된 값이 존재하면
//				nLabel++;
//				//TRACE("%d\n", nLabel);
//				pbDstBits[j * nWidth + i] = nLabel;	//레이블링 된 값 넣어주기
//				Connectivity_4Direc(pbSrcBits, pbDstBits, nLabel, nWidth, i, j);
//
//			}
//		}
//	}
//
//	int* rgbArr = new int[nLabel];	//레이블링 한 개수만큼 색상 설정
//
//
//	for (int k = 0; k < nLabel; k++) {	//명암 설정
//		rgbArr[k] = (k + 1) * static_cast <int> (255 / nLabel);
//		//TRACE("%d\n", rgbArr[k]);
//	}
//
//	for (int h = 0; h < nHeight - 1; h++)
//	{
//		for (int w = 0; w < nWidth - 1; w++)
//		{
//			if (pbDstBits[h * nWidth + w] == 0)
//				pbDstBits[h * nWidth + w] = 0;
//
//			else
//			{
//				int labelValue = pbDstBits[h * nWidth + w] - 1;
//				//TRACE("%d\n\n", labelValue);
//				pbDstBits[h * nWidth + w] = limit(rgbArr[labelValue]);	//그레이스 컬러, 명암
//				//pbDstBits[h * nWidth + w] = RGB(limit(rgbArr[labelValue]), limit(rgbArr[labelValue]), limit(rgbArr[labelValue]));	//그레이스 컬러, 명암
//				TRACE("%d\n", pbDstBits[h * nWidth + w]);
//			}
//		}
//	}
//
//	m_pacs->SetCopy(pbDstBits);
//	delete[] rgbArr;
//}
//
//
//void CPacsSegment::Connectivity_4Direc(BYTE* pSrc, BYTE* pDst, int cnt, int p, int w, int h)
//{
//	if (pSrc[(h - 1) * p + w] == 255 && pDst[(h - 1) * p + w] == 0)
//	{
//		pDst[(h - 1) * p + w] = cnt;
//		Connectivity_4Direc(pSrc, pDst, cnt, p, w, h - 1);
//	}
//
//	if (pSrc[h * p + (w - 1)] == 255 && pDst[(h + 1) * p + (w - 1)] == 0)
//	{
//		pDst[h * p + (w - 1)] = cnt;
//		Connectivity_4Direc(pSrc, pDst, cnt, p, w - 1, h);
//	}
//
//	if (pSrc[h * p + (w + 1)] == 255 && pDst[h * p + (w + 1)] == 0)
//	{
//		pDst[h * p + (w + 1)] = cnt;
//		Connectivity_4Direc(pSrc, pDst, cnt, p, w + 1, h);
//	}
//
//	if (pSrc[(h + 1) * p + w] == 255 && pDst[(h + 1) * p + w] == 0)
//	{
//		pDst[(h + 1) * p + w] = cnt;
//		Connectivity_4Direc(pSrc, pDst, cnt, p, w, h + 1);
//	}
//
//}