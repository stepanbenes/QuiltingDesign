#include "pixel.h"
#include <cmath>
#include <algorithm>

pixel::pixel()
{
	set_val(0, 0, 0);
}

pixel::pixel(int v1, int v2, int v3)
{
	set_val(v1, v2, v3);
}

pixel::~pixel()
{
}

void pixel::set_val(int v1, int v2, int v3)
{
	// Standard setter method
	val[0] = std::min(maxVal, std::max(minVal, v1));
	val[1] = std::min(maxVal, std::max(minVal, v2));
	val[2] = std::min(maxVal, std::max(minVal, v3));
}

void pixel::set_interpolated_val(double iDiff, double jDiff, const pixel & p1, const pixel & p2, const pixel & p3, const pixel & p4)
{
	// Compute pixel values based on linear interpolation of values of four surrounding pixels.
	// The method assumes i-j coordinate system, with iDiff, jDiff \in \[0,1\].
	// Surrounding pixels are listed as p1 = [0,0], p2 = [0,1], p3 = [1,0], p4 = [1,1].
	double auxVal = 0.0;
	int intVal[3] = {0,0,0};
	for (int i = 0; i < 3; i++) {
		auxVal = 0.0;
		auxVal += (1.0 - iDiff) * (1.0 - jDiff) * p1.get_val(i);
		auxVal += (1.0 - iDiff) * jDiff * p2.get_val(i);
		auxVal += iDiff * (1.0 - jDiff) * p3.get_val(i);
		auxVal += iDiff * jDiff * p4.get_val(i);
		intVal[i] = std::min(maxVal, std::max(minVal, (int) auxVal ));
	}
	this->set_val(intVal[0], intVal[1], intVal[2]);
}

int pixel::get_val(int ind) const
{
	// Standard getter method
	if ((ind==0) || (ind==1) || (ind==2)) {
		return val[ind];
	}
	else {
		return -1;
	};
}

pixel pixel::operator+(const pixel& addend)
{
	return pixel(this->get_val(0) + addend.get_val(0), this->get_val(1) + addend.get_val(1), this->get_val(2) + addend.get_val(2));
}

double pixel::operator-(const pixel & subtrahend)
{
	// Overload minus operator to return Euclidean norm of voxel component differences. 
	double auxVal = 0.0;
	for (int i = 0; i < 3; i++) {
		auxVal += pow(this->get_val(i) - subtrahend.get_val(i), 2.0);
	}
	return sqrt(auxVal);
}

std::ostream& operator<<(std::ostream& out, const pixel& f)
{
	// Define insertion operator for auxiliary checks.
	return out << "[" << f.get_val(0) << ", " << f.get_val(1) << ", " << f.get_val(2) << "]";
}
