#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "myAuxFuns.h"
#include "rgbArray.h"
#include "rgbSpecimen.h"
#include "tileSet.h"

using namespace std;

// Unresolved issues:
// TODO Interpolating RGB pixel (linear element-wise interpolation is not optimal)
// TODO Decide whether to allow non-integer sample origins
// TODO Change rgbArray into more general imgArray capable of handling both grayscale and RGB

int main()
{
	// Define input file name
	string settingsFile = "_allSettings.set"; 
	ostringstream label;
	string labelBegin = "_sample";
	string labelEnd = ".tpg";
	cout << "Starting QuiltingDesing script..." << endl;

	// Allocate variables for setting parameters and load them
	string tileSetFile  = "";
	string specimenFile = "";
	int nT = 0;						// Define length of the resulting tile in terms of ORIGINAL pixels
	int nO = 0;						// [OPTIMALLY ODD NUMBER] Define samples overlap in terms of ROTATED pixels
	int nSample = 0;
	double * sampleCoords = nullptr;
	load_setting(settingsFile, &specimenFile, &tileSetFile, &nT, &nO, &nSample, &sampleCoords);

	// Load tile set definition
	tileSet myTileSet;
	myTileSet.load_tileSet_data(tileSetFile);

	// Load specimen
	rgbSpecimen specimen;
	specimen.load_textified_image(specimenFile);

	// Test samples
	rgbArray * testSample = nullptr;
	string testSampleFile = "_sample.tpg";

	// Load samples
	int nS = nT + nO;						// Define sample edge size in terms of ROTATED pixels
	int sampleSize[2] = { nS, nS };			// Reshape into approporiate form
	double sampleScale = sqrt(2.0)/2.0;		// Sample has different voxel size (two new voxels per a diagonal of the specimen voxel
	double sampleOrigin[2] = { 0.0, 0.0 };
	rgbArray ** allSamples = nullptr;
	allSamples = new rgbArray * [nSample];
	for (int i = 0; i < nSample; i++) {
		sampleOrigin[0] = sampleCoords[i*2];
		sampleOrigin[1] = sampleCoords[i*2+1];
		label.str("");
		label << labelBegin << i << labelEnd;

		allSamples[i] = specimen.extract_sample(sampleOrigin, sampleSize, sampleScale, 1);
		allSamples[i]->save_textified_image(label.str());
	}

	// Merge samples
	for (int i = 0; i < myTileSet.nTiles; i++) {
		cout << myTileSet.tilesCodes[i*4] << myTileSet.tilesCodes[i*4+1] <<	myTileSet.tilesCodes[i*4+2] << myTileSet.tilesCodes[i*4+3] << endl;
	}
	merge_samples(allSamples[0], allSamples[1], nO);

	cout << "Hit any key to exit the script ...";
	cin.get();

	// Free allocated variables
	delete testSample;
	for (int i = 0; i < nSample; i++) {
		delete allSamples[i];
	}
	delete[] allSamples;

	return 0;
}