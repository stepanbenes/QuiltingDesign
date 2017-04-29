#pragma once
#include <string>
#include "wangSet.h"
#include "sample.h"
#include "pixelArray.h"

struct parameters {
	int nT = 0;					// Define length of tile width
	int nO = 0;					// Defines width of sample overlap
	int nS = 0;					// Define length of sample width
	double tileScale = 2.0 / sqrt(2.0);
};

void load_JSON_setting(std::string inFile, wangSet & tileSet, parameters & inParameters, std::vector<sample> & inSamples);
void save_JSON_results(std::string & outFile, std::string & date, double duration, double quiltError);
pixelArray convert_lightnessMap_to_pixelArray(std::vector<double> & lightnessMap, int nTx, int nTy);