#include "myAuxFuns.h"

int load_setting(string inFile, string * specimenFile, string * tileSetFile, int * nT, int * nO, int * nSample, double ** sampleCoords)
{
	ifstream inF;
	double * auxPtr = nullptr;

	inF.open(inFile);
	inF >> *tileSetFile;
	inF >> *specimenFile;
	inF >> *nT;
	inF >> *nO;
	inF >> *nSample;

	auxPtr = new double[(*nSample)*2];
	for (int i = 0; i < *nSample; i++) {
		inF >> auxPtr[2 * i] >> auxPtr[2 * i + 1];
	}
	*sampleCoords = auxPtr;

	inF.close();

	return 0;
}

rgbArray * merge_samples(rgbArray * pS1, rgbArray * pS2, int nO)
{
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

	// TODO Apply wedge mask

	// Compute cumulative error
	for (int i = 0; i < nRows; i++) {		
		for (int j = 0; j < nO; j++) {
			cumulErr[i*nO + j] = arrayErr[i*nO + j];
			if (i != 0) {
				cumulErr[i*nO + j] += min( cumulErr[(i-1)*nO+max(0,j-1)], min( cumulErr[(i-1)*nO+j], cumulErr[(i-1)*nO+min(nRows-1,j-1)] ) );
			};
		}
	}

	// TODO Find optimal "quilting seam" (from bottom up)
	int minInd = 0;
	double minVal = cumulErr[(nRows - 1)*nO];
	int * optPath = new int[nRows];

	for (int j = 1; j < nO; j++) {
		if (cumulErr[(nRows - 1)*nO + j] < minVal) {
			minVal = cumulErr[(nRows - 1)*nO + j];
			minInd = j;
		}
	}
	optPath[nRows - 1] = minInd;

	for (int i = nRows - 2; i > -1; i--) {
		
	}

	// TODO Assemble merged image
	rgbArray * mergedSample;
	int mergedSampleSize[2] = { nRows, 2*nRows-nO };

	mergedSample->initialize_array();

	return mergedSample;
}
