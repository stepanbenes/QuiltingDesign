#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "pixel.h"

class pixelArray
{
private:
	int resolution[2] = { 0, 0 };		// Number of pixels in i- and j-direction
	int nPixelValues = 0;				// Distinguish between RGB and grayscale input/output
	std::vector<pixel> data;
public:
	pixelArray();
	pixelArray(int inResolution[2]);
	pixelArray(std::string inFile);
	~pixelArray();
	int convert_IJ_indices(int iCoord, int jCoord);
	int get_value_at(int iCoord, int jCoord, int iV);
	void set_values_at(int v1, int v2, int v3, int iCoord, int jCoord);
	pixel get_pixel_at(int iCoord, int jCoord);
	void set_pixel_at(pixel & inPixel, int iCoord, int jCoord);
	void load_textified_image(std::string inFile);
	void save_textified_image(std::string outFile);
};

