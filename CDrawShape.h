#pragma once


class CDrawShape
{
protected:
	CRect m_rPosition;
	LOGPEN m_logpen;
	LOGBRUSH m_logbrush;

public:
	CDrawShape() {};
	virtual void Draw(CDC* pDC) {};
};

class CDrawRect : public CDrawShape
{
protected:
	enum Shape { rectangle, ellipse, line };
	Shape m_nShape;
public:
	CDrawRect(LOGPEN logpen, LOGBRUSH logbrush);
	virtual void Draw(CDC* pDC) override;
};
	