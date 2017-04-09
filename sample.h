#pragma once
#include "pixelArray.h"

class sample :
	public pixelArray
{
private:
	int origin[2] = { 0,0 };
public:
	sample();
	sample(int originI, int originJ, int resolution);
	sample(int inRes[2]);
	~sample();
	void acquire_data_from_specimen(pixelArray & specimen);
	sample merge_sample(sample * that, int nO, int nM);
	sample rotate_n90(int n);
};

