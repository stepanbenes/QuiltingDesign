#pragma once
#include <iostream>
#include <cstdint>
#include <algorithm>

class pixel
{
private:
	uint8_t val[3] = {0,0,0};
public:
	pixel();
	pixel(uint8_t v1, uint8_t v2, uint8_t v3);
	pixel::pixel(int v1, int v2, int v3);
	~pixel();
	void set_val(uint8_t v1, uint8_t v2, uint8_t v3);
	void set_val(int v1, int v2, int v3);
	void pixel::set_interpolated_val(double iDiff, double jDiff, const pixel & p1, const pixel & p2, const pixel & p3, const pixel & v4);
	uint8_t get_val(int ind) const;
	double get_lightness_val() const;
	pixel operator+(const pixel & addend);
	double operator-(const pixel & subtrahend);
	pixel operator*(double factor);
	friend std::ostream& operator<<(std::ostream & out, const pixel & f);
};
