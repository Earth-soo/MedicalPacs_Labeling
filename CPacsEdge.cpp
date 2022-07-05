#include "pch.h"
#include "CPacsEdge.h"
#include "RGBBYTE.h"

void CPacsEdge::ImageEdgeRoverts()
{
	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();
	int size = m_pacs->GetSize();
	int i, j, h1, h2;
	double hval;
	int nBpp = m_pacs->GetBpp();

	if (nBpp == 8) {
		BYTE* bpBits = m_pacs->GetBits();
		BYTE* bpDst = new BYTE[size];

		for (j = 1; j < h; j++) {
			for (i = 1; i < w - 1; i++) {

				h1 = bpBits[j * w + i] - bpBits[j * w + i - 2];
				h2 = bpBits[j * w + i] - bpBits[j * w + i];

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

				bpDst[j * w + i] = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		m_pacs->SetCopy(bpDst);
	}
	else if (nBpp == 24) {
		RGBBYTE* rgbpBits = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* rgbpDst(new RGBBYTE[size]);

		for (j = 1; j < h; j++) {
			for (i = 1; i < w - 1; i++) {

				h1 = rgbpBits[j * w + i].r - rgbpBits[j * w + i - 2].r;
				h2 = rgbpBits[j * w + i].r - rgbpBits[j * w + i].r;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

				rgbpDst[j * w + i].r = static_cast<BYTE>(limit(hval + 0.5));


				h1 = rgbpBits[j * w + i].g - rgbpBits[j * w + i - 2].g;
				h2 = rgbpBits[j * w + i].g - rgbpBits[j * w + i].g;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

				rgbpDst[j * w + i].g = static_cast<BYTE>(limit(hval + 0.5));


				h1 = rgbpBits[j * w + i].b - rgbpBits[j * w + i - 2].b;
				h2 = rgbpBits[j * w + i].b - rgbpBits[j * w + i].b;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

				rgbpDst[j * w + i].b = static_cast<BYTE>(limit(hval + 0.5));


			}
		}
		m_pacs->SetCopy(rgbpDst);
	}
}

void CPacsEdge::ImageEdgePrewitt()
{
	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();
	int size = m_pacs->GetSize();
	int i, j, h1, h2;
	double hval;
	int nBpp = m_pacs->GetBpp();

	if (nBpp == 8) {

		BYTE* bpBits = m_pacs->GetBits();
		BYTE* bpDst = new BYTE[size];

		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++) {
				h1 = -bpBits[j * w + i - 2] - bpBits[j * w + i - 1] - bpBits[j * w + i]
					+ bpBits[j * w + i] + bpBits[j * w + i + 1] + bpBits[j * w + i + 2];
				h2 = -bpBits[j * w + i - 2] - bpBits[j * w + i - 1] - bpBits[j * w + i]
					+ bpBits[j * w + i] + bpBits[j * w + i + 1] + bpBits[j * w + i + 2];

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				bpDst[j * w + i] = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		m_pacs->SetCopy(bpDst);
	}


	else if (nBpp == 24) {
		RGBBYTE* rgbBits = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* rgbpDst = new RGBBYTE[size];

		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++) {
				h1 = -rgbBits[j * w + i - 2].r - rgbBits[j * w + i - 1].r - rgbBits[j * w + i].r
					+ rgbBits[j * w + i].r + rgbBits[j * w + i + 1].r + rgbBits[j * w + i + 2].r;
				h2 = -rgbBits[j * w + i - 2].r - rgbBits[j * w + i - 1].r - rgbBits[j * w + i].r
					+ rgbBits[j * w + i].r + rgbBits[j * w + i + 1].r + rgbBits[j * w + i + 2].r;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst[j * w + i].r = static_cast<BYTE>(limit(hval + 0.5));

				h1 = -rgbBits[j * w + i - 2].g - rgbBits[j * w + i - 1].g - rgbBits[j * w + i].g
					+ rgbBits[j * w + i].g + rgbBits[j * w + i + 1].g + rgbBits[j * w + i + 2].g;
				h2 = -rgbBits[j * w + i - 2].g - rgbBits[j * w + i - 1].g - rgbBits[j * w + i].g
					+ rgbBits[j * w + i].g + rgbBits[j * w + i + 1].g + rgbBits[j * w + i + 2].g;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst[j * w + i].g = static_cast<BYTE>(limit(hval + 0.5));

				h1 = -rgbBits[j * w + i - 2].b - rgbBits[j * w + i - 1].b - rgbBits[j * w + i].b
					+ rgbBits[j * w + i].b + rgbBits[j * w + i + 1].b + rgbBits[j * w + i + 2].b;
				h2 = -rgbBits[j * w + i - 2].b - rgbBits[j * w + i - 1].b - rgbBits[j * w + i].b
					+ rgbBits[j * w + i].b + rgbBits[j * w + i + 1].b + rgbBits[j * w + i + 2].b;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst[j * w + i].b = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		m_pacs->SetCopy(rgbpDst);
	}
}

void CPacsEdge::ImageEdgePrewitt(vector<BYTE>& bpBits, vector<BYTE>& bpDst)
{
	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();

	int i, j, h1, h2;
	double hval;

	for (j = 1; j < h - 1; j++) {
		for (i = 1; i < w - 1; i++) {
			h1 = -bpBits[j * w + i - 2] - bpBits[j * w + i - 1] - bpBits[j * w + i]
				+ bpBits[j * w + i] + bpBits[j * w + i + 1] + bpBits[j * w + i + 2];
			h2 = -bpBits[j * w + i - 2] - bpBits[j * w + i - 1] - bpBits[j * w + i]
				+ bpBits[j * w + i] + bpBits[j * w + i + 1] + bpBits[j * w + i + 2];

			hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
			bpDst[j * w + i] = static_cast<BYTE>(limit(hval + 0.5));
		}
	}
}


void CPacsEdge::ImageEdgeSobel()
{
	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();
	int size = m_pacs->GetSize();
	int i, j, h1, h2;
	double hval;
	int nBpp = m_pacs->GetBpp();

	if (nBpp == 8) {

		BYTE* bpBits = m_pacs->GetBits();
		BYTE* pDst = new BYTE[size];


		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++) {
				h1 = -bpBits[j * w + i - 2] - 2 * bpBits[j * w + i - 1] - bpBits[j * w + i]
					+ bpBits[j * w + i] + 2 * bpBits[j * w + i + 1] + bpBits[j * w + i + 2];
				h2 = -bpBits[j * w + i - 2] - 2 * bpBits[j * w + i - 1] - bpBits[j * w + i]
					+ bpBits[j * w + i] + 2 * bpBits[j * w + i + 1] + bpBits[j * w + i + 2];
				hval = sqrt(static_cast<double>((h1 * h1) + (h2 * h2)));

				pDst[j * w + i] = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		m_pacs->SetCopy(pDst);
	}
	else if (nBpp == 24) {
		RGBBYTE* rgbBits = (RGBBYTE*)m_pacs->GetBits();
		RGBBYTE* rgbpDst = new RGBBYTE[size];

		for (j = 1; j < h - 1; j++) {
			for (i = 1; i < w - 1; i++) {
				h1 = -rgbBits[j * w + i - 2].r - 2 * rgbBits[j * w + i - 1].r - rgbBits[j * w + i].r
					+ rgbBits[j * w + i].r + 2 * rgbBits[j * w + i + 1].r + rgbBits[j * w + i + 2].r;
				h2 = -rgbBits[j * w + i - 2].r - 2 * rgbBits[j * w + i - 1].r - rgbBits[j * w + i].r
					+ rgbBits[j * w + i].r + 2 * rgbBits[j * w + i + 1].r + rgbBits[j * w + i + 2].r;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst[j * w + i].r = static_cast<BYTE>(limit(hval + 0.5));

				h1 = -rgbBits[j * w + i - 2].g - 2 * rgbBits[j * w + i - 1].g - rgbBits[j * w + i].g
					+ rgbBits[j * w + i].g + 2 * rgbBits[j * w + i + 1].g + rgbBits[j * w + i + 2].g;
				h2 = -rgbBits[j * w + i - 2].g - 2 * rgbBits[j * w + i - 1].g - rgbBits[j * w + i].g
					+ rgbBits[j * w + i].g + 2 * rgbBits[j * w + i + 1].g + rgbBits[j * w + i + 2].g;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst[j * w + i].g = static_cast<BYTE>(limit(hval + 0.5));

				h1 = -rgbBits[j * w + i - 2].b - 2 * rgbBits[j * w + i - 1].b - rgbBits[j * w + i].b
					+ rgbBits[j * w + i].b + 2 * rgbBits[j * w + i + 1].b + rgbBits[j * w + i + 2].b;
				h2 = -rgbBits[j * w + i - 2].b - 2 * rgbBits[j * w + i - 1].b - rgbBits[j * w + i].b
					+ rgbBits[j * w + i].b + 2 * rgbBits[j * w + i + 1].b + rgbBits[j * w + i + 2].b;

				hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));
				rgbpDst[j * w + i].b = static_cast<BYTE>(limit(hval + 0.5));
			}
		}
		m_pacs->SetCopy(rgbpDst);
	}
}

void CPacsEdge::ImageEdgeCanny()
{

}

void CPacsEdge::ImageEdgeColor()
{
	int w = m_pacs->GetWidth();
	int h = m_pacs->GetHeight();
	const int size = m_pacs->GetSize();
	RGBBYTE* pDst = new RGBBYTE[size];

	vector<BYTE> imgY(size);
	vector<BYTE> imgU(size);
	vector<BYTE> imgV(size);

	m_pacs->ColorSplitYUV(imgY, imgU, imgV);
	vector<BYTE> edgeY(size);
	vector<BYTE> edgeU(size);
	vector<BYTE> edgeV(size);
	
	ImageEdgePrewitt(imgY, edgeY);
	ImageEdgePrewitt(imgU, edgeU);
	ImageEdgePrewitt(imgV, edgeV);

	double dist;
	for (int i = 0; i < size; i++)
	{
		dist = (edgeY[i] * edgeY[i]) +
			(0.5 * edgeU[i]) * (0.5 * edgeU[i]) +
			(0.5 * edgeV[i]) * (0.5 * edgeV[i]);
		pDst[i].r = pDst[i].g = pDst[i].b = static_cast<BYTE>(limit(sqrt(dist)));
	}
	m_pacs->SetCopy(pDst);
}
