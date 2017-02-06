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