#pragma once
typedef unsigned char BYTE;

class ARGBBYTE
{
public:
	BYTE a;
	BYTE b;
	BYTE g;
	BYTE r;

public:
	ARGBBYTE() : a(0), b(0), g(0), r(0) {}
	ARGBBYTE(BYTE gray) : a(gray), b(gray), g(gray), r(gray) {}
	ARGBBYTE(const BYTE& _a, const BYTE& _r, const BYTE& _g, const BYTE& _b) :a(_a), b(_b), g(_g), r(_r) {}
};
