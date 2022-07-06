#pragma once
#include <vector>

using namespace std;

class LabelInfo
{
public:
	vector<pair<int, int>> pixels;
	int cx, cy;
	int minx, miny, maxx, maxy;

public:
	LabelInfo() : cx(0), cy(0), minx(9999), miny(9999), maxx(0), maxy(0) {};
};