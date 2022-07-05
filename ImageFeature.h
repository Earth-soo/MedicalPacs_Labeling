#pragma once
#include "RGBBYTE.h"
#include "CPacs.h"
void ImageEdgeRoverts(shared_ptr<CPacs> pPacs);
void ImageEdgePrewitt(shared_ptr<CPacs> pPacs);
void ImageEdgeSobel(shared_ptr<CPacs> pPacs);
void ImageEdgeCanny(shared_ptr<CPacs> pPacs);

