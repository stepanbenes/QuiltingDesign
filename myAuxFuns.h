#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "rgbArray.h"
#include <algorithm>

using namespace std;

int load_setting(string inFile, string * specimenFile, string * tileSetFile, int * nT, int * nO, int * nSample, double ** sampleCoords );
rgbArray * merge_samples(rgbArray * pS1, rgbArray * pS2, int nO);