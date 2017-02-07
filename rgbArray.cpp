#include "rgbArray.h"

rgbArray::rgbArray(){
}

rgbArray::~rgbArray(){
	delete[] data;
}

void rgbArray::initialize_array(int inSize[2]) {
	size[0] = inSize[0];
	size[1] = inSize[1];
	data = new int[size[0] * size[1] * 3];
	for (int i = 0; i < size[0] * size[1] * 3; i++) {
		data[i] = 0;
	}
}

void rgbArray::load_textified_image(string inFile) {

	ifstream inF;
	int thirdSize = {};

	// Read input file
	inF.open(inFile);
	inF >> size[0];
	inF >> size[1];
	inF >> thirdSize;
	if (thirdSize != 3) {
		cout << "Textified input does not provide 3 values for each pixel" << endl;
		cin.get();
		exit(0);
	}	
	initialize_array(size);
	for (int i = 0; i < size[0] * size[1] * 3; i++) {
		inF >> data[i];
	}	
	inF.close();

	// File statistics
	cout << "Input file " << inFile << " with resolution " << size[0] << " x " << size[1] << " pixels was loaded." << endl;

}

void rgbArray::save_textified_image(string outFile) {
	
	ofstream outF;

	// Write output file
	outF.open(outFile);
	outF << size[0] << endl;
	outF << size[1] << endl;
	outF << 3 << endl;
	outF << endl;
	for (int i = 0; i < size[0] * size[1] * 3; i++) {
		outF << data[i] << endl;
	}
	outF.close();

	// File statistics
	cout << "Output file " << outFile << " with resolution " << size[0] << " x " << size[1] << " pixels was saved." << endl;

}

int rgbArray::convert_3D_to_1D(int i, int j, int k) {
	return i*size[1]*3 + j*3 + k;
}

rgbArray * rgbArray::rotate_n90(int n)
{
	// Returns rgbArray rotated by n-times 90° anticlockwise
	int newSize[2] = { size[0], size[1] };
	int transVec[2] = { 0, 0 };
	int rotMtrx[4] = { 1, 0, 0, 1 };
	rgbArray * rotArray = nullptr;

	// Set transformation parameters
	switch (n % 4){
	case 1:
		newSize[0] = size[1]; newSize[1] = size[0];
		transVec[0] = size[1]-1; transVec[1] = 0;
		rotMtrx[0] = 0; rotMtrx[1] = -1; rotMtrx[2] = 1; rotMtrx[3] = 0;
		break;
	case 2:
		transVec[0] = size[0]-1; transVec[1] = size[1]-1;
		rotMtrx[0] = -1; rotMtrx[1] = 0; rotMtrx[2] = 0; rotMtrx[3] = -1;
		break;
	case 3:
		newSize[0] = size[1]; newSize[1] = size[0];
		transVec[0] = 0; transVec[1] = size[0]-1;
		rotMtrx[0] = 0; rotMtrx[1] = 1; rotMtrx[2] = -1; rotMtrx[3] = 0;
		break;
	default:
		break;
	}

	// Fill the new array
	int newI = 0;
	int newJ = 0;
	rotArray = new rgbArray;
	rotArray->initialize_array(newSize);
	for (int i = 0; i < size[0]; i++) {
		for (int j = 0; j < size[1]; j++) {
			newI = transVec[0] + rotMtrx[0]*i + rotMtrx[1]*j;
			newJ = transVec[1] + rotMtrx[2]*i + rotMtrx[3]*j;
			for (int k = 0; k < 3; k++) {
				rotArray->data[rotArray->convert_3D_to_1D(newI, newJ, k)] = data[convert_3D_to_1D(i,j,k)];
			}
		}
	}

	return rotArray;
}

rgbArray * rgbArray::extract_sample(double o[2], int s[2], double a, int d) {
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
			// NOTE that the transformation is from new to old coordinate system (!), hence the inverse rotation matrix
			iOd = o[0] + a * (sqrt(2.0) / 2.0 * iN - d * sqrt(2.0) / 2.0 * jN);
			jOd = o[1] + a * (d * sqrt(2.0) / 2.0 * iN + sqrt(2.0) / 2.0 * jN);

			iOn = (int)floor(iOd);
			jOn = (int)floor(jOd);

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