#include "pixelArray.h"

pixelArray::pixelArray()
{
}

pixelArray::pixelArray(int inResolution[2])
{
	resolution[0] = inResolution[0];
	resolution[1] = inResolution[1];
	nPixelValues = 3;
	data.reserve( resolution[0]*resolution[1] );
}

pixelArray::pixelArray(std::string inFile)
{
	load_textified_image(inFile);
}

pixelArray::~pixelArray()
{
}

int pixelArray::convert_IJ_indices(int iCoord, int jCoord)
{
	return (iCoord*resolution[1]+jCoord);
}

int pixelArray::get_value_at(int iCoord, int jCoord, int iV)
{
	return (data.at(convert_IJ_indices(iCoord, jCoord))).get_val(iV);
}

void pixelArray::set_values_at(int v1, int v2, int v3, int iCoord, int jCoord)
{
	(data.at(convert_IJ_indices(iCoord, jCoord))).set_val(v1, v2, v3);
}

pixel pixelArray::get_pixel_at(int iCoord, int jCoord)
{
	return data.at(convert_IJ_indices(iCoord, jCoord));
}

void pixelArray::set_pixel_at(pixel & inPixel, int iCoord, int jCoord)
{
	data.at(convert_IJ_indices(iCoord, jCoord)) = inPixel;
}

void pixelArray::load_textified_image(std::string inFile)
{
	int val1 = 0;
	int val2 = 0;
	int val3 = 0;
	std::ifstream inF;
	inF.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Read textified jpeg file
	try {
		inF.open(inFile);
	}
	catch (std::ios_base::failure& inErr) {
		std::cerr << "The requested input file probably doesn't exist. System error: " << inErr.what() << std::endl;
	}
	inF >> resolution[0];
	inF >> resolution[1];
	data.reserve(resolution[0] * resolution[1]);
	if (!data.empty()) {
		std::cout << "Warning: Data vector is not empty. Loading new image will overwrite data." << std::endl;
		data.clear();
	}
	inF >> nPixelValues;
	for (int i = 0; i < resolution[0] * resolution[1]; i++) {
		if (nPixelValues == 1) {
			inF >> val1;
			val2 = val1;
			val3 = val1;
		}
		else if (nPixelValues == 3) {
			inF >> val1;
			inF >> val2;
			inF >> val3;
		}
		data.push_back(pixel(val1, val2, val3));
	}
	inF.close();
}

void pixelArray::save_textified_image(std::string outFile)
{
	std::ofstream outF;
	outF.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::vector<pixel>::const_iterator iD;

	// Write output file 
	try {
		outF.open(outFile);
	}
	catch (std::ios_base::failure& inErr) {
		std::cerr << "The requested output file cannot be created. System error: " << inErr.what() << std::endl;
	}
	outF << resolution[0] << std::endl;
	outF << resolution[1] << std::endl;
	outF << nPixelValues << std::endl;
	outF << std::endl;
	for (iD = data.begin(); iD != data.end(); ++iD) {
		for (int i = 0; i < nPixelValues; i++) {
			outF << iD->get_val(i) << std::endl;
		}
	}
	outF.close();

}

void pixelArray::load_binary_image(std::string inFile)
{
	int auxInt = 0;
	uint8_t val1 = 0;
	uint8_t val2 = 0;
	uint8_t val3 = 0;
	std::ifstream inF;
	inF.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Read textified jpeg file
	try {
		inF.open(inFile, std::ios::binary);
	}
	catch (std::ios_base::failure& inErr) {
		std::cerr << "The requested input file probably doesn't exist. System error: " << inErr.what() << std::endl;
	}
	inF.read((char *)&auxInt, sizeof(int));
	resolution[0] = auxInt;
	inF.read((char *)&auxInt, sizeof(int));
	resolution[1] = auxInt;
	if (!data.empty()) {
		std::cout << "Warning: Data vector is not empty. Loading new image will overwrite data." << std::endl;
		data.clear();
	}
	inF.read((char *)&auxInt, sizeof(int));
	nPixelValues = auxInt;
	for (int i = 0; i < resolution[0] * resolution[1]; i++) {
		if (nPixelValues == 1) {
			inF.read((char *)&val1, sizeof(uint8_t));
			val2 = val1;
			val3 = val1;
		}
		else if (nPixelValues == 3) {
			inF.read((char *)&val1, sizeof(uint8_t));
			inF.read((char *)&val2, sizeof(uint8_t));
			inF.read((char *)&val3, sizeof(uint8_t));
		}
		data.push_back(pixel((int)val1, (int)val2, (int)val3));
	}
	inF.close();
}

void pixelArray::save_binary_image(std::string outFile)
{
	std::ofstream outF;
	outF.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::vector<pixel>::const_iterator iD;
	uint8_t auxInt = 0;

	// Write output file 
	try {
		outF.open(outFile, std::ios::binary | std::ios::out );
	}
	catch (std::ios_base::failure& inErr) {
		std::cerr << "The requested output file cannot be created. System error: " << inErr.what() << std::endl;
	}
	outF.write((char *)&resolution, 2 * sizeof(int));
	outF.write((char *)&nPixelValues, sizeof(int));
	for (iD = data.begin(); iD != data.end(); ++iD) {
		for (int i = 0; i < nPixelValues; i++) {
			auxInt = (uint8_t)iD->get_val(i);
			outF.write((char *)&auxInt, sizeof(uint8_t));
		}
	}
	outF.close();
}
