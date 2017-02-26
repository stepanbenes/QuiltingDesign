#pragma once
#include "pixelArray.h"

class sample :
	public pixelArray
{
private:
	int origin[2] = { 0,0 };
public:
	sample();
	sample(int originX, int originY, int resolution);
	~sample();
	void acquire_data_from_specimen(pixelArray & specimen);
};

