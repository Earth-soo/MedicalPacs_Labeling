#include "pch.h"
#include "CDrawShape.h"
CDrawRect::CDrawRect(LOGPEN logpen, LOGBRUSH logbrush)
{
	m_logpen = logpen;
	m_logbrush = logbrush;
	
}
void CDrawRect::Draw(CDC* pDC)
{
	Graphics graphics(*pDC);
	Pen pen(m_logpen.lopnColor, m_logpen.lopnWidth.x * m_logpen.lopnWidth.y);
	pen.SetDashStyle(static_cast<DashStyle>(m_logpen.lopnStyle));

	CRect rect = m_rPosition;
	switch (m_nShape) 
	{
	case rectangle:
		pDC->Rectangle(rect);
		break;
	case ellipse:
		pDC->Ellipse(rect);
		break;
	case line:
		if (rect.top > rect.bottom)
		{
			rect.top -= m_logpen.lopnWidth.y / 2;
			rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
		}
		else
		{
			rect.top += (m_logpen.lopnWidth.y + 1) / 2;
			rect.bottom -= m_logpen.lopnWidth.y / 2;
		}

		if (rect.left > rect.right)
		{
			rect.left -= m_logpen.lopnWidth.x / 2;
			rect.right += (m_logpen.lopnWidth.x + 1) / 2;
		}
		else
		{
			rect.left += (m_logpen.lopnWidth.x + 1) / 2;
			rect.right -= m_logpen.lopnWidth.x / 2;
		}
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(rect.BottomRight());
		break;
	}
	
}