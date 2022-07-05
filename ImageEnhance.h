#pragma once
#include "CPacs.h"


void ImageHistogram(BYTE* bpBits, float histo[256], const int size);
void ImageHistogram(CPacs* pacs, float histoR[256], float histoG[256], float histoB[256]);

void ImageHistogramStretching(CPacs* pacs);
void ImageHistogramEqualization(BYTE* bpBits, const int size);