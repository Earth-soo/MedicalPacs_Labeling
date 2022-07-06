#include "pch.h"
#include "CPacsSegment.h"
#include "RGBBYTE.h"
#include "CPacsEnhance.h"
#include "CPacsColor.h"

//CPacsSegment::CPacsSegment() { };

//CPacsSegment::CPacsSegment(CPacsPtr& pacs) 
//	: m_pacs(pacs)
//	, cx(0)
//	, cy(0)
//	, minx(0)
//	, miny(0)
//	, maxx(0)
//	, maxy(0) 
//{};

//����ȭ
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

//Ʈ��->�׷���
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

//�Ӱ谪 �ڵ�����
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

		// �ݺ��� ���� �Ӱ谪 ����

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
	//int nPicth = abs(m_pacs->GetPitch());

	BYTE* pbSrcBits = m_pacs->GetBits();
	int* pnTmpBits = new int[nsize]{0}; 
	
	const int nMaxLabel = 10000;
	int* pnEq_tbl = new int[nMaxLabel * 2]{ 0 }; //����̺� ����(�ʱ�ȭ �� ����)

	//-------------------------------------------------------------------------
	// ù ��° ��ĵ - �ʱ� ���̺� ���� �� � ���̺� ����
	//-------------------------------------------------------------------------

	int nLabel = 0, nMaxl, n_Minl, n_MinEq, n_MaxEq, i, j;

	for (j = 1; j < nHeight; j++)
		for (i = 1; i < nWidth; i++)
		{
			if (pbSrcBits[j * nWidth + i] == 255)
			{
				// �ٷ� �� �ȼ��� ���� �ȼ� ��ο� ���̺��� �����ϴ� ���
				if ((pnTmpBits[(j - 1) * nWidth + i] != 0) && (pnTmpBits[j * nWidth + (i - 1)] != 0))
				{
					if (pnTmpBits[(j - 1) * nWidth + i] == pnTmpBits[j * nWidth + (i - 1)])
					{
						// �� ���̺��� ���� ���� ���
						pnTmpBits[j * nWidth + i] = pnTmpBits[(j - 1) * nWidth + i];
					}
					else
					{
						// �� ���̺��� ���� �ٸ� ���, ���� ���̺��� �ο�
						nMaxl = max(pnTmpBits[(j - 1) * nWidth + i], pnTmpBits[j * nWidth + (i - 1)]);
						n_Minl = min(pnTmpBits[(j - 1) * nWidth + i], pnTmpBits[j * nWidth + (i - 1)]);

						pnTmpBits[j * nWidth + i] = n_Minl;

						// � ���̺� ����(��, ���� ����)
						n_MinEq = min(pnEq_tbl[nMaxl * 2 + 1], pnEq_tbl[n_Minl * 2 + 1]);
						n_MaxEq = max(pnEq_tbl[nMaxl * 2 + 1], pnEq_tbl[n_Minl * 2 + 1]);

						pnEq_tbl[(pnEq_tbl[n_MaxEq * 2 + 1]) * 2 + 1] = n_MinEq;
					}
				}
				else if (pnTmpBits[(j - 1) * nWidth + i] != 0)
				{
					// �ٷ� �� �ȼ����� ���̺��� ������ ���
					pnTmpBits[j * nWidth + i] = pnTmpBits[(j - 1) * nWidth + i];
				}
				else if (pnTmpBits[j * nWidth + (i - 1)] != 0)
				{
					// �ٷ� ���� �ȼ����� ���̺��� ������ ���
					pnTmpBits[j * nWidth + i] = pnTmpBits[j * nWidth + (i - 1)];
				}
				else
				{
					// �̿��� ���̺��� �������� ������ ���ο� ���̺��� �ο�
					nLabel++;
					pnTmpBits[j * nWidth + i] = nLabel;
					pnEq_tbl[nLabel * 2 + 0] = nLabel;
					pnEq_tbl[nLabel * 2 + 1] = nLabel;
				}
			}
		}

	//-------------------------------------------------------------------------
	// � ���̺� ���� (2�� �����ؾ� �ϴ� �͵� ����)
	//-------------------------------------------------------------------------

	int nTempEql;
	for (i = 1; i <= nLabel; i++)
	{
		nTempEql = pnEq_tbl[i * 2 + 1];
		if (nTempEql != pnEq_tbl[i * 2 + 0])
			pnEq_tbl[i * 2 + 1] = pnEq_tbl[nTempEql * 2 + 1];
	}

	// � ���̺��� ���̺��� 1���� ���ʴ�� ������Ű��

	int* pnHash = new int[nLabel + 1]{ 0 };

	for (i = 1; i <= nLabel; i++)
		pnHash[pnEq_tbl[i * 2 + 1]] = pnEq_tbl[i * 2 + 1];

	int label_cnt = 1;
	for (i = 1; i <= nLabel; i++)
		if (pnHash[i] != 0)
			pnHash[i] = label_cnt++;

	for (i = 1; i <= nLabel; i++)
		pnEq_tbl[i * 2 + 1] = pnHash[pnEq_tbl[i * 2 + 1]];

	delete[] pnHash;

	//-------------------------------------------------------------------------
	// �� ��° ��ĵ - � ���̺��� �̿��Ͽ� ��� �ȼ��� ������ ���̺� �ο�
	//-------------------------------------------------------------------------
	
	int* pnDstBits = new int[nsize]{ 0 };

	int nIdx;

	for (j = 1; j < nHeight; j++) {
		for (i = 1; i < nWidth; i++)
		{
			if (pnTmpBits[j * nWidth + i] != 0)
			{
				nIdx = pnTmpBits[j * nWidth + i];
				pnDstBits[j * nWidth + i] = pnEq_tbl[nIdx * 2 + 1];
			}
		}
	}

	//-------------------------------------------------------------------------
	// IppLabelInfo ���� �ۼ�
	//-------------------------------------------------------------------------

	m_vLabelinfos.resize(label_cnt - 1);

	LabelInfo* plLabel;

	for (j = 1; j < nHeight; j++)
		for (i = 1; i < nWidth; i++)
		{
			if (pnDstBits[j * nWidth + i] != 0)
			{
				plLabel = &m_vLabelinfos.at(pnDstBits[j * nWidth + i] - 1);
				plLabel->pixels.push_back(make_pair(i, j));
				plLabel->cx += i;
				plLabel->cy += j;

				if (i < plLabel->minx) plLabel->minx = i;
				if (i > plLabel->maxx) plLabel->maxx = i;
				if (j < plLabel->miny) plLabel->miny = j;
				if (j > plLabel->maxy) plLabel->maxy = j;
			}
		}

	for (LabelInfo& label : m_vLabelinfos)
	{
		label.cx /= label.pixels.size();
		label.cy /= label.pixels.size();
	}

	for (LabelInfo& info : m_vLabelinfos)
	{
		for (int j = info.miny; j <= info.maxy; j++)
			pbSrcBits[j * nWidth + info.minx] = pbSrcBits[j * nWidth + info.maxx] = 128;

		for (int i = info.minx; i <= info.maxx; i++)
			pbSrcBits[info.miny * nWidth + i] = pbSrcBits[info.maxy * nWidth + i] = 128;
	}
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
//			if (pbSrcBits[j * nWidth + i] == 255 && pbDstBits[j * nWidth + i] == 0) {	//����ȭ�� ���� �����ϸ�
//				nLabel++;
//				//TRACE("%d\n", nLabel);
//				pbDstBits[j * nWidth + i] = nLabel;	//���̺� �� �� �־��ֱ�
//				Connectivity_4Direc(pbSrcBits, pbDstBits, nLabel, nWidth, i, j);
//
//			}
//		}
//	}
//
//	int* rgbArr = new int[nLabel];	//���̺� �� ������ŭ ���� ����
//
//
//	for (int k = 0; k < nLabel; k++) {	//��� ����
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
//				pbDstBits[h * nWidth + w] = limit(rgbArr[labelValue]);	//�׷��̽� �÷�, ���
//				//pbDstBits[h * nWidth + w] = RGB(limit(rgbArr[labelValue]), limit(rgbArr[labelValue]), limit(rgbArr[labelValue]));	//�׷��̽� �÷�, ���
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