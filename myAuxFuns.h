#pragma once
#include <string>
#include "rgbArray.h"
#include "wangSet.h"

void load_JSON_setting(std::string inFile, wangSet * tileSet);
int load_setting(std::string inFile, std::string * specimenFile, std::string * tileSetFile, int * nT, int * nO, int * nSample, double ** sampleCoords );
rgbArray * merge_samples(rgbArray * pS1, rgbArray * pS2, int nO, int nM);