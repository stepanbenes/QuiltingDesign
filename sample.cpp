#include "sample.h"

sample::sample()
{
}

sample::sample(int originX, int originY, int edgeLength)
{
	origin[0] = originX;
	origin[1] = originY;
	resolution[0] = edgeLength;
	resolution[1] = edgeLength;
	nPixelValues = 3;
	data.reserve(resolution[0] * resolution[1]);
}

sample::~sample()
{
}

void sample::acquire_data_from_specimen(pixelArray & specimen)
{
	double doubleOrigin[2];
	doubleOrigin[0] = (double)origin[0];
	doubleOrigin[1] = (double)origin[1];
	data = (specimen.extract_patch(doubleOrigin, resolution, sqrt(2.0) / 2.0, 1)).get_data_vector();
}
