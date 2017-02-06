#pragma once
#include "rgbArray.h"

class rgbSpecimen :
	public rgbArray
{
public:
	rgbSpecimen();
	~rgbSpecimen();
	rgbArray * extract_sample(double o[2], int s[2], double a, int n);
};

