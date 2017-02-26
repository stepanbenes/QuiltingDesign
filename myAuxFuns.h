#pragma once
#include <string>
#include "rgbArray.h"
#include "wangSet.h"
#include "sample.h"

struct parameters {
	int nT = 0;					// Define length of tile width
	int nO = 0;					// Defines width of sample overlap
	int nS = 0;					// Define length of sample width
	double tileScale = 2.0 / sqrt(2.0);
};

void load_JSON_setting(std::string inFile, wangSet & tileSet, parameters & inParameters, std::vector<sample> & inSamples);
//int load_setting(std::string inFile, std::string * specimenFile, std::string * tileSetFile, int * nT, int * nO, int * nSample, double ** sampleCoords );
rgbArray * merge_samples(rgbArray * pS1, rgbArray * pS2, int nO, int nM);