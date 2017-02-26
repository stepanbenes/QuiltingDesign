#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "wangTile.h"
#include "wangSet.h"
#include "picojson.h"
#include "myAuxFuns.h"

#define LOC_DEBUG 0

using namespace std;

void load_JSON_setting(std::string inFile, wangSet & tileSet, parameters & inParameters, std::vector<sample> & inSamples)
{
	// Load script setting provided in JSON file

	// Load input string
	std::ifstream JSONfile(inFile);
	std::string JSONstring((std::istreambuf_iterator<char>(JSONfile)), std::istreambuf_iterator<char>());

	// Parse buffer string
	picojson::value JSONvalue;
	std::string JSONerr = picojson::parse(JSONvalue, JSONstring);
	if (!JSONerr.empty()) {
		//throw std::exception("Loaded JSON string couldn't be parsed.");
	}

	// Test parsed JSON values	
	if (!JSONvalue.is<picojson::object>()) {
		//throw std::exception("JSON is not an object.");
	}
	if ( !(JSONvalue.contains("tileSize") && JSONvalue.contains("sampleOverlap") && JSONvalue.contains("samples") && JSONvalue.contains("tiles")) ) {
		//throw std::exception("Loaded JSON doesn't contain all requried fields.");
	}

	// Load all data
	inParameters.nT = (int)JSONvalue.get("tileSize").get<double>();
	inParameters.nO = (int)JSONvalue.get("sampleOverlap").get<double>();
	inParameters.nS = inParameters.nT + inParameters.nO - 1;					// Define sample edge size in terms of ROTATED pixels

	picojson::array tilesArray = JSONvalue.get("tiles").get<picojson::array>();
	int nTiles = (int)tilesArray.size();
	tileSet.set_nTiles(nTiles);
	for (picojson::array::const_iterator it = tilesArray.begin(); it != tilesArray.end(); it++) {
		int auxID = 0;
		int auxCodes[4] = { 0,0,0,0 };
		auxID = (int)it->get("id").get<double>();
		for (int i = 0; i < 4; i++) {
			auxCodes[i] = (int)it->get("codes").get<picojson::array>()[i].get<double>();
		}
		wangTile auxTile(auxID, auxCodes);
		tileSet.add_tile(auxTile);
	}

	picojson::array sampleArray = JSONvalue.get("samples").get<picojson::array>();
	for (picojson::array::const_iterator it = sampleArray.begin(); it != sampleArray.end(); it++) {
		inSamples.push_back( sample((int)it->get("originX").get<double>(), (int)it->get("originY").get<double>(), inParameters.nS) );
	}


	// Print all data (only in LOC_DEBUG mode)
#if LOC_DEBUG == 1

	std::cout << "Identified JSON values:" << std::endl;
	std::cout << "    tileSize:      " << (int)JSONvalue.get("tileSize").get<double>() << std::endl;
	std::cout << "    sampleOverlap: " << (int)JSONvalue.get("sampleOverlap").get<double>() << std::endl;

	//picojson::array tilesArray = JSONvalue.get("tiles").get<picojson::array>();
	std::cout << "    # tiles:       " << tilesArray.size() << std::endl;
	for (picojson::array::const_iterator it = tilesArray.begin(); it != tilesArray.end(); it++) {
		std::cout << "        Tile " << it->get("id").get<double>() << ": [ "
			<< it->get("codes").get<picojson::array>()[0] << ", "
			<< it->get("codes").get<picojson::array>()[1] << ", "
			<< it->get("codes").get<picojson::array>()[2] << ", "
			<< it->get("codes").get<picojson::array>()[3] << " ]"
			<< std::endl;
	}

	//picojson::array sampleArray = JSONvalue.get("samples").get<picojson::array>();
	std::cout << "    # samples:       " << sampleArray.size() << std::endl;
	for (picojson::array::const_iterator it = sampleArray.begin(); it != sampleArray.end(); it++) {
		std::cout << "        Sample " << it - sampleArray.begin() << ": [ "
			<< it->get("originX").get<double>() << ", "
			<< it->get("originY").get<double>() << " ]"
			<< std::endl;
	}

#endif

};

//int load_setting(string inFile, string * specimenFile, string * tileSetFile, int * nT, int * nO, int * nSample, double ** sampleCoords)
//{
//	ifstream inF;
//	double * auxPtr = nullptr;
//
//	inF.open(inFile);
//	inF >> *tileSetFile;
//	inF >> *specimenFile;
//	inF >> *nT;
//	inF >> *nO;
//	inF >> *nSample;
//
//	auxPtr = new double[(*nSample)*2];
//	for (int i = 0; i < *nSample; i++) {
//		inF >> auxPtr[2 * i] >> auxPtr[2 * i + 1];
//	}
//	*sampleCoords = auxPtr;
//
//	inF.close();
//
//	return 0;
//}

rgbArray * merge_samples(rgbArray * pS1, rgbArray * pS2, int nO, int nM)
{
	// MERGE_SAMPLES performs image quilting algorithm on two samples,
	// which are assumed to be aligned side-by-side, pS1 first. Input 
	// parameter nM defines whether the wedge mask should be applied 
	// only to the upper end (nM=1) or both (nM=2);

	int v1 = 0;
	int v2 = 0;
	double locErr = 0.0;
	int offset = pS1->size[1] - nO;		// Define indices offset
	int nRows = pS1->size[0];
	double * arrayErr = new double[nRows*nO];
	double * cumulErr = new double[nRows*nO];

	// Compute local error
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nO; j++) {
			locErr = 0.0;
			for (int k = 0; k < 3; k++) {
				v1 = pS1->data[pS1->convert_3D_to_1D(i, offset+j, k)];
				v2 = pS2->data[pS2->convert_3D_to_1D(i, j, k)];
				locErr += pow(v1-v2, 2);
			}
			arrayErr[i*nO+j] = sqrt(locErr);
		}
	}

	// Apply wedge mask
	int intHalf = (int)(ceil(nO / 2.0) - 1.0);		// "Lower" integer half of the overlap
	int maskWidth = intHalf;						// Width of wedge mask (adaptivelly changed in the second run)
	double maxErr = 500;							// Max value in arrayErr can be sqrt(3*pow(255,2)) ~ 442
	for (int i = 0; i < intHalf; i++) {		
		for (int j = 0; j < nO; j++) {
			if (j < maskWidth) {
				arrayErr[i*nO + j] = maxErr;
				if (nM == 2) {
					arrayErr[(nRows-1-i)*nO + j] = maxErr;
				}
			}
			else if (j >= (nO - maskWidth)) {
				arrayErr[i*nO + j] = maxErr;
				if (nM == 2) {
					arrayErr[(nRows - 1 - i)*nO + j] = maxErr;
				}
			}
		}
	}							
	for (int i = 0; i < intHalf; i++) {
		maskWidth = intHalf - i;
		for (int j = 0; j < nO; j++) {
			if (j < maskWidth) {
				arrayErr[(intHalf+i)*nO + j] = maxErr;
				if (nM == 2) {
					arrayErr[(nRows - 1 - intHalf - i)*nO + j] = maxErr;
				}
			}
			else if (j >= (nO - maskWidth)) {
				arrayErr[(intHalf + i)*nO + j] = maxErr;
				if (nM == 2) {
					arrayErr[(nRows - 1 - intHalf - i)*nO + j] = maxErr;
				}
			}
		}
	}

	// Compute cumulative error
	for (int i = 0; i < nRows; i++) {		
		for (int j = 0; j < nO; j++) {
			cumulErr[i*nO + j] = arrayErr[i*nO + j];
			if (i != 0) {
				cumulErr[i*nO + j] += min( cumulErr[(i-1)*nO+max(0,j-1)], min( cumulErr[(i-1)*nO+j], cumulErr[(i-1)*nO+min(nO-1,j+1)] ) );
			};
		}
	}

	// Find optimal "quilting seam" (from bottom up)
	int minInd = 0;
	double minVal = cumulErr[(nRows - 1)*nO];
	int * optPath = new int[nRows];

	for (int j = 1; j < nO; j++) {				// Scan the whole bottom line for the minimum value
		if (cumulErr[(nRows - 1)*nO + j] < minVal) {
			minVal = cumulErr[(nRows - 1)*nO + j];
			minInd = j;
		}
	}
	optPath[nRows - 1] = minInd;
		
	for (int i = nRows - 2; i > -1; i--) {		// Scan one-pixel neighbourhood of the previously identified minimum
		for (int j = max(0, optPath[i+1]-1); j < min(nO, optPath[i+1]+2); j++) {
			if (cumulErr[i*nO + j] <= minVal) {
				minVal = cumulErr[i*nO + j];	// No need to reinitialize minVal since it comes from cumulative error; therefore, the minimum value in the upper three pixels is maximally the same value (hence the <=) 
				minInd = j;
			}
		}
		optPath[i] = minInd;
	}

	// Assemble merged image
	rgbArray * mergedSample;
	mergedSample = new rgbArray;
	int mergedSampleSize[2] = { nRows, (pS1->size[1]+ pS2->size[1])-nO };

	mergedSample->initialize_array(mergedSampleSize);
	for (int i = 0; i < nRows; i++) {
		for (int k = 0; k < 3; k++) {
			// Fill the first part from the first input
			for (int j = 0; j < (pS1->size[1] - nO); j++) {
				mergedSample->data[mergedSample->convert_3D_to_1D(i, j, k)] = pS1->data[pS1->convert_3D_to_1D(i,j,k)];
			}

			// Fill the overlap
			for (int j = 0; j < nO; j++) {
				if (j < optPath[i]) {
					mergedSample->data[mergedSample->convert_3D_to_1D(i, offset + j, k)] = pS1->data[pS1->convert_3D_to_1D(i, offset+j, k)];
				}
				else if (j > optPath[i]) {
					mergedSample->data[mergedSample->convert_3D_to_1D(i, offset + j, k)] = pS2->data[pS2->convert_3D_to_1D(i, j, k)];
				}
				else {
					// TODO Decide what to do with the seam pixels (averaged for now)
					mergedSample->data[mergedSample->convert_3D_to_1D(i, offset + j, k)] = (int)( 0.5*pS1->data[pS1->convert_3D_to_1D(i, offset + j, k)] + 0.5*pS2->data[pS2->convert_3D_to_1D(i, j, k)]);
				}
			}

			// Fill the remaining part from the second input
			for (int j = 0; j < (pS2->size[1] - nO); j++) {
				mergedSample->data[mergedSample->convert_3D_to_1D(i, (pS1->size[1])+j, k)] = pS2->data[pS1->convert_3D_to_1D(i,nO+j, k)];
			}
		}
	}

	return mergedSample;
}
