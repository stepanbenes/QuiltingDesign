#include <memory>
#include <algorithm>
#include "sample.h"

sample::sample()
{
}

sample::sample(int originI, int originJ, int edgeLength)
{
	origin[0] = originI;
	origin[1] = originJ;
	resolution[0] = edgeLength;
	resolution[1] = edgeLength;
	nPixelValues = 3;
	data.reserve(resolution[0] * resolution[1]);
}

sample::sample(int inRes[2])
{
	// Constructor for samples arising from merge_samples method
	origin[0] = 0;
	origin[1] = 0;
	resolution[0] = inRes[0];
	resolution[1] = inRes[1];
	nPixelValues = 3;
	data.clear();
	data.resize(resolution[0] * resolution[1]);
}


sample::~sample()
{
}

void sample::acquire_data_from_specimen(pixelArray & specimen)
{
	double doubleOrigin[2];
	doubleOrigin[0] = (double)origin[0];
	doubleOrigin[1] = (double)origin[1];
	//data = (specimen.extract_patch(doubleOrigin, resolution, sqrt(2.0) / 2.0, 1)).get_data_vector();
	data = *((specimen.extract_patch(doubleOrigin, resolution, sqrt(2.0) / 2.0, 1)).getptr_data_vector());
}

sample sample::merge_sample(sample * that, int nO, int nM)
{
	// MERGE_SAMPLES method performs image quilting algorithm on two samples,
	// which are assumed to be aligned side-by-side, this object first. Input 
	// parameter nM defines whether the wedge mask should be applied 
	// only to the upper end (nM=1) or both (nM=2);

	// Check resolution of both samples
	if (this->resolution[0] != that->resolution[0]) {
		throw std::exception("Sampes to merge have different i-resolution.");
	}
	 
	int v1 = 0;
	int v2 = 0;
	double locErr = 0.0;
	int offset = this->resolution[1] - nO;					// Define auxiliary indices offset
	int nRows = this->resolution[0];						// Store number of i-pixels
	std::unique_ptr<double[]> arrayErr(new double[nRows*nO]);
	std::unique_ptr<double[]> cumulErr(new double[nRows*nO]);

	// Compute local error
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nO; j++) {
			locErr = this->get_pixel_at(i, offset + j) - that->get_pixel_at(i, j);
			arrayErr[i*nO + j] = sqrt(locErr);
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
					arrayErr[(nRows - 1 - i)*nO + j] = maxErr;
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
				arrayErr[(intHalf + i)*nO + j] = maxErr;
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
				cumulErr[i*nO + j] += std::min(cumulErr[(i - 1)*nO + std::max(0, j - 1)], std::min(cumulErr[(i - 1)*nO + j], cumulErr[(i - 1)*nO + std::min(nO - 1, j + 1)]));
			};
		}
	}


	// Find optimal "quilting seam" (from bottom up)
	int minInd = 0;
	double minVal = cumulErr[(nRows - 1)*nO];
	std::unique_ptr<int[]> optPath(new int[nRows]);

	for (int j = 1; j < nO; j++) {				// Scan the whole bottom line for the minimum value
		if (cumulErr[(nRows - 1)*nO + j] < minVal) {
			minVal = cumulErr[(nRows - 1)*nO + j];
			minInd = j;
		}
	}
	optPath[nRows - 1] = minInd;

	for (int i = nRows - 2; i > -1; i--) {		// Scan one-pixel neighbourhood of the previously identified minimum
		for (int j = std::max(0, optPath[i + 1] - 1); j < std::min(nO, optPath[i + 1] + 2); j++) {
			if (cumulErr[i*nO + j] <= minVal) {
				minVal = cumulErr[i*nO + j];	// No need to reinitialize minVal since it comes from cumulative error; therefore, the minimum value in the upper three pixels is maximally the same value (hence the <=) 
				minInd = j;
			}
		}
		optPath[i] = minInd;
	}


	// Assemble merged image
	int mergedSampleSize[2] = { nRows, (this->resolution[1] + that->resolution[1]) - nO };
	sample newSample(mergedSampleSize);


	for (int i = 0; i < nRows; i++) {

		// Fill the first part from the first input
		for (int j = 0; j < (this->resolution[1] - nO); j++) {
			newSample.set_pixel_at(this->getptr_pixel_at(i, j), i, j);
		}

		// Fill the overlap
		for (int j = 0; j < nO; j++) {
			if (j < optPath[i]) {
				newSample.set_pixel_at(this->getptr_pixel_at(i, offset+j), i, offset + j);
			}
			else if (j > optPath[i]) {
				newSample.set_pixel_at(that->getptr_pixel_at(i, j), i, offset + j);
			}
			else { // TODO Decide what to do with the seam pixels (averaged for now)
				//newSample.set_pixel_at( pixel(255,0	,0), i, offset+  j);
				newSample.set_pixel_at(this->get_pixel_at(i, offset + j)*0.5 + that->get_pixel_at(i, j)*0.5, i, offset + j);
			}
		}

		// Fill the remaining part from the second input
		for (int j = 0; j < (that->resolution[1] - nO); j++) {
			newSample.set_pixel_at(that->getptr_pixel_at(i, nO + j), i, this->resolution[1] + j);
		}

	}

	return newSample;
}

sample sample::rotate_n90(int n)
{
	// Returns rgbArray rotated by n-times 90° anticlockwise
	int newSize[2] = { resolution[0], resolution[1] };
	int transVec[2] = { 0, 0 };
	int rotMtrx[4] = { 1, 0, 0, 1 };

	// Set transformation parameters
	switch (n % 4) {
	case 1:
		newSize[0] = resolution[1]; newSize[1] = resolution[0];
		transVec[0] = resolution[1] - 1; transVec[1] = 0;
		rotMtrx[0] = 0; rotMtrx[1] = -1; rotMtrx[2] = 1; rotMtrx[3] = 0;
		break;
	case 2:
		transVec[0] = resolution[0] - 1; transVec[1] = resolution[1] - 1;
		rotMtrx[0] = -1; rotMtrx[1] = 0; rotMtrx[2] = 0; rotMtrx[3] = -1;
		break;
	case 3:
		newSize[0] = resolution[1]; newSize[1] = resolution[0];
		transVec[0] = 0; transVec[1] = resolution[0] - 1;
		rotMtrx[0] = 0; rotMtrx[1] = 1; rotMtrx[2] = -1; rotMtrx[3] = 0;
		break;
	default:
		break;
	}

	// Fill the new array
	sample rotArray(newSize);
	int newI = 0;
	int newJ = 0;
	for (int i = 0; i < resolution[0]; i++) {
		for (int j = 0; j < resolution[1]; j++) {
			newI = transVec[0] + rotMtrx[0] * i + rotMtrx[1] * j;
			newJ = transVec[1] + rotMtrx[2] * i + rotMtrx[3] * j;
			rotArray.set_pixel_at(this->getptr_pixel_at(i, j), newI, newJ);
		}
	}

	return rotArray;
}
