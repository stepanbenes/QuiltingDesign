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

	// Apply wedge mask
	int intHalf = (int)(ceil(nO / 2.0) - 1.0);		// "Lower" integer half of the overlap
	int maskWidth = intHalf;						// Width of wedge mask (adaptivelly changed in the second run)
	double maxErr = 500;							// Max value in arrayErr can be sqrt(3*pow(255,2)) ~ 442
	for (int i = 0; i < intHalf; i++) {		
		for (int j = 0; j < nO; j++) {
			if (j < maskWidth) {
				arrayErr[i*nO + j] = maxErr;
			}
			else if (j >= (nO - maskWidth)) {
				arrayErr[i*nO + j] = maxErr;
			}
		}
	}							
	for (int i = 0; i < intHalf; i++) {
		maskWidth = intHalf - i;
		for (int j = 0; j < nO; j++) {
			if (j < maskWidth) {
				arrayErr[(intHalf+i)*nO + j] = maxErr;
			}
			else if (j >= (nO - maskWidth)) {
				arrayErr[(intHalf + i)*nO + j] = maxErr;
			}
		}
	}

	// Plot and check array err
	for (int i = 0; i < nRows; i++) {
		cout << i << "th row: ";
		for (int j = 0; j < nO; j++) {
			cout << arrayErr[i*nO + j] << ", ";
		}
		cout << endl;
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
	int mergedSampleSize[2] = { nRows, 2*nRows-nO };

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
					//mergedSample->data[mergedSample->convert_3D_to_1D(i, offset + j, k)] = (int)( 0.5*pS1->data[pS1->convert_3D_to_1D(i, offset + j, k)] + 0.5*pS2->data[pS2->convert_3D_to_1D(i, j, k)]);
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
