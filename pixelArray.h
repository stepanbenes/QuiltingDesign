#pragma once
#include <vector>
#include <string>
#include "pixel.h"

class pixelArray
{
protected:
	int resolution[2] = { 0, 0 };		// Number of pixels in i- and j-direction
	int nPixelValues = 0;				// Distinguish between RGB and grayscale input/output (OBSOLETE)
	std::vector<pixel> data;
public:
	pixelArray();
	pixelArray(int inResolution[2]); 
	pixelArray(int iResolution, int jResolution);
	pixelArray(std::string inFile);
	~pixelArray();
	int convert_IJ_indices(int iCoord, int jCoord) const;
	void get_resolution(int & resX, int & resY);
	int get_value_at(int iCoord, int jCoord, int iV);
	void set_values_at(int v1, int v2, int v3, int iCoord, int jCoord);
	pixel get_pixel_at(int iCoord, int jCoord);
	pixel * getptr_pixel_at(int iCoord, int jCoord);
	void add_pixel(pixel & inPixel);
	void add_pixel(pixel * inPixelPtr);
	void add_pixel(int v1, int v2, int V3);
	void set_pixel_at(pixel & inPixel, int iCoord, int jCoord);
	void set_pixel_at(pixel * inPixel, int iCoord, int jCoord);
	void load_textified_image(std::string inFile);
	void save_textified_image(std::string outFile);
	void load_binary_image(std::string inFile);
	void save_binary_image(std::string outFile);
	void load_BMP(std::string inFile);
	void save_BMP(std::string outFile);
	double compute_averaged_lightness() const;
	double compute_averaged_lightness_of_patch(int minI, int minJ, int maxI, int maxJ) const;
	std::vector<double> compute_lightness_map(int nTx, int nTy) const;
	pixelArray extract_patch(double o[2], int s[2], double a, int d);
	pixelArray rotate_n90(int n);
	std::vector<pixel> get_data_vector();
	std::vector<pixel> * getptr_data_vector();
};

