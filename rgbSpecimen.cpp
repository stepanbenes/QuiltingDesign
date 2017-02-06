#include "rgbSpecimen.h"

rgbSpecimen::rgbSpecimen(){
}

rgbSpecimen::~rgbSpecimen(){
}

rgbArray * rgbSpecimen::extract_sample(double o[2], int s[2], double a, int d) {
	// Extract a sample with local coordinates i'-j' defined with
	//		{i;j} = {o[0];o[1]} + a * R * {i';j'},
	// where R is a rotation matrix for the angle n * pi/2, n being an integer.
	// Transformation is in i-j coordinate system with (0,0) in the top left corner,
	// i possitive top down and j from left to right. Integer d governs direction of 
	// rotation (d = 1 counter clockwise; d = -1 clockwise).

	// Set pointer and initialize sample
	rgbArray * pSample = nullptr;
	pSample = new rgbArray;
	pSample->initialize_array(s);

	// TODO Check for outlying sample corners

	// Define new sample
	double iOd = 0.0;		// Double version of original coordinates
	double jOd = 0.0;		// Double version of oridinal coordinates
	int iOn = 0;			// Int version of original coordinates
	int jOn = 0;			// Int version of original coordinates
	double iDiff = 0.0;		// Difference between double and int
	double jDiff = 0.0;		// Difference between double and int
	double auxVal = 0.0;
	for (int iN = 0; iN < s[0]; iN++) {
		for (int jN = 0; jN < s[1]; jN++) {
			iOd = o[0] + a * (sqrt(2.0) / 2.0 * iN - d * sqrt(2.0) / 2.0 * jN);
			jOd = o[1] + a * (d * sqrt(2.0) / 2.0 * iN + sqrt(2.0) / 2.0 * jN);

			iOn = floor(iOd);
			jOn = floor(jOd);

			iDiff = iOd - iOn;
			jDiff = jOd - jOn;

			// Interpolate values
			for (int kN = 0; kN < 3; kN++) {
				auxVal = 0.0;
				auxVal += (1.0 - iDiff) * (1.0 - jDiff) * data[convert_3D_to_1D(iOn, jOn, kN)];
				auxVal += (1.0 - iDiff) * jDiff * data[convert_3D_to_1D(iOn, jOn + 1, kN)];
				auxVal += iDiff * (1.0 - jDiff) * data[convert_3D_to_1D(iOn + 1, jOn, kN)];
				auxVal += iDiff * jDiff * data[convert_3D_to_1D(iOn + 1, jOn + 1, kN)];
				pSample->data[pSample->convert_3D_to_1D(iN, jN, kN)] = (int)auxVal;
			}
		}
	}

	return pSample;
}