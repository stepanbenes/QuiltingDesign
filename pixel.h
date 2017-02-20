#pragma once
#include <iostream>

class pixel
{
private:
	int val[3] = {0,0,0};
	int minVal = 0;		// Should be const but it disrupts implicit definition of assignment operator
	int maxVal = 255;	// Should be const but it disrupts implicit definition of assignment operator
public:
	pixel();
	pixel(int v1, int v2, int v3);
	~pixel();
	void set_val(int v1, int v2, int v3);
	void pixel::set_interpolated_val(double iDiff, double jDiff, const pixel & p1, const pixel & p2, const pixel & p3, const pixel & v4);
	int get_val(int ind) const;
	pixel operator+(const pixel & addend);
	double operator-(const pixel & subtrahend);
	friend std::ostream& operator<<(std::ostream & out, const pixel & f);
};
