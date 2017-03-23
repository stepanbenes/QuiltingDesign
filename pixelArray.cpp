#include <fstream>
#include <cstdint> 
#include <cmath>
#include <exception>
#include "pixelArray.h"
#include "EasyBMP.h"

pixelArray::pixelArray()
{
}

pixelArray::pixelArray(int inResolution[2])
{
	resolution[0] = inResolution[0];
	resolution[1] = inResolution[1];
	nPixelValues = 3;
	data.reserve(resolution[0] * resolution[1]);
}

pixelArray::pixelArray(int iResolution, int jResolution)
{
	resolution[0] = iResolution;
	resolution[1] = jResolution;
	nPixelValues = 3;
	data.reserve(resolution[0] * resolution[1]);
}

pixelArray::pixelArray(std::string inFile)
{
	load_textified_image(inFile);
}

pixelArray::~pixelArray()
{
}

int pixelArray::convert_IJ_indices(int iCoord, int jCoord) const
{
	return (iCoord*resolution[1] + jCoord);
}

void pixelArray::get_resolution(int & resX, int & resY)
{
	resX = resolution[0];
	resY = resolution[1];
}

int pixelArray::get_value_at(int iCoord, int jCoord, int iV)
{
	//return (int)(data.at(convert_IJ_indices(iCoord, jCoord))).get_val(iV);
	return (int)(data[iCoord*resolution[1] + jCoord]).get_val(iV);
}

void pixelArray::set_values_at(int v1, int v2, int v3, int iCoord, int jCoord)
{
	//data.at(convert_IJ_indices(iCoord, jCoord)).set_val(v1, v2, v3);
	data[iCoord*resolution[1] + jCoord].set_val(v1, v2, v3);
}

pixel pixelArray::get_pixel_at(int iCoord, int jCoord)
{
	//return data.at(convert_IJ_indices(iCoord, jCoord));	
	return data[iCoord*resolution[1] + jCoord];
}

void pixelArray::add_pixel(pixel & inPixel)
{
	data.push_back(inPixel);
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
			outF << (int)iD->get_val(i) << std::endl;
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
		outF.open(outFile, std::ios::binary | std::ios::out);
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

void pixelArray::load_BMP(std::string inFile)
{
	if (!data.empty()) {
		std::cout << "Warning: Data vector is not empty. Loading new image will overwrite data." << std::endl;
		data.clear();
	}

	BMP bmp;
	bool success = bmp.ReadFromFile(inFile.c_str());
	if (!success) {
		throw std::exception("BMP file was not loaded.");
	}

	resolution[0] = bmp.TellHeight();
	resolution[1] = bmp.TellWidth();
	data.reserve(resolution[0]*resolution[1]);
	nPixelValues = 3;
	for (int i = 0; i < resolution[0]; i++)
	{
		for (int j = 0; j < resolution[1]; j++)
		{
			// Note swapped indices in EasyBMP and our script
			RGBApixel rgbaPixel = bmp.GetPixel(j, i);
			data.push_back(pixel((int)rgbaPixel.Red, (int)rgbaPixel.Green, (int)rgbaPixel.Blue));
		}
	}
}

void pixelArray::save_BMP(std::string outFile)
{
	BMP output;
	output.SetSize(resolution[1], resolution[0]);
	int index;
	std::vector<pixel>::const_iterator iD;
	if (nPixelValues == 1)
	{
		output.SetBitDepth(8);

		for (iD = data.begin(), index = 0; iD != data.end(); ++iD, ++index)
		{
			RGBApixel rgbaPixel;
			ebmpBYTE value = (ebmpBYTE)iD->get_val(0);
			rgbaPixel.Red = value;
			rgbaPixel.Green = value;
			rgbaPixel.Blue = value;
			rgbaPixel.Alpha = 0;
			output.SetPixel(index % resolution[1], index / resolution[1], rgbaPixel);
		}
	}
	else if (nPixelValues == 3)
	{
		output.SetBitDepth(24);

		for (iD = data.begin(), index = 0; iD != data.end(); ++iD, ++index)
		{
			RGBApixel rgbaPixel;
			rgbaPixel.Red = (ebmpBYTE)iD->get_val(0);
			rgbaPixel.Green = (ebmpBYTE)iD->get_val(1);
			rgbaPixel.Blue = (ebmpBYTE)iD->get_val(2);
			rgbaPixel.Alpha = 0;
			output.SetPixel(index % resolution[1], index / resolution[1], rgbaPixel);
		}
	}

	output.WriteToFile(outFile.c_str());
}

double pixelArray::compute_averaged_lightness() const
{
	double sumLightness = 0.0;
	for (std::vector<pixel>::const_iterator iD = data.begin(); iD != data.end(); ++iD) {
		sumLightness += iD->get_lightness_val();
	}
	return sumLightness / data.size();
}


double pixelArray::compute_averaged_lightness_of_patch(int minI, int maxI, int minJ, int maxJ) const
{
	if (minI<0 || maxI>=resolution[0] || minI>maxI || minJ>maxJ || minJ<0 || maxJ>=resolution[1]) {
		throw std::exception("Patch for lightness computation is out of range.");
	}

	double sumLightness = 0.0;
	for (int i = minI; i <= maxI; i++) {
		for (int j = minJ; j <= maxJ; j++) {
			sumLightness += data.at(convert_IJ_indices(i, j)).get_lightness_val();
		}
	}

	return sumLightness/( (maxI-minI+1)*(maxJ-minJ+1) );
}

std::vector<double> pixelArray::compute_lightness_map(int nTx, int nTy) const
{
	// Parition the pixel array into a tiling of nTx * nTy tiles and compute average lightness for each domain
	// Note different coordinate system for tiling and pixel array and possibility for non-integral decomposition (slightly large patch is assumed)
	std::vector<double> tilingLightness(nTx*nTy);
	int minI = 0;
	int maxI = 0;
	int minJ = 0;
	int maxJ = 0;
	double resX = resolution[1] / nTx;
	double resY = resolution[0] / nTy;


	for (int iY = 0; iY < nTy; iY++) {
		for (int iX = 0; iX < nTx; iX++) {
			minI = (int)std::floor((nTy - iY - 1)*resY);
			maxI = (int)std::ceil((nTy - iY)*resY-1);
			minJ = (int)std::floor(iX*resX);
			maxJ = (int)std::ceil((iX + 1)*resX-1);
			tilingLightness[iY*nTx + iX] = compute_averaged_lightness_of_patch(minI, maxI, minJ, maxJ);
		}
	}

	return tilingLightness;
}


pixelArray pixelArray::extract_patch(double o[2], int s[2], double a, int d)
{
	// Extract a patch of pixel Array with local coordinates i'-j' defined with
	//		{i;j} = {o[0];o[1]} + a * R * {i';j'},
	// where R is a rotation matrix for the angle n * pi/2, n being an integer.
	// Transformation is in i-j coordinate system with (0,0) in the top left corner,
	// i possitive top down and j from left to right. Integer d governs direction of 
	// rotation (d = 1 counter clockwise; d = -1 clockwise). 


	// Check for outlying sample corners
	if (d == 1) {
		double minI = o[0] + a * (sqrt(2.0) / 2.0 * 0 - d * sqrt(2.0) / 2.0 * s[1]);
		double maxI = o[0] + a * (sqrt(2.0) / 2.0 * s[0] - d * sqrt(2.0) / 2.0 * 0);
		double minJ = o[1] + a * (d * sqrt(2.0) / 2.0 * 0 + sqrt(2.0) / 2.0 * 0);
		double maxJ = o[1] + a * (d * sqrt(2.0) / 2.0 * s[0] + sqrt(2.0) / 2.0 * s[1]);

		if ( minI<0 || maxI>resolution[0] || minJ<0 || maxJ>resolution[1]) {
			throw std::exception("Sample origin leads to sample defined out of region.");
		}
	}
	else if(d == -1) {
		double minI = o[0] + a * (sqrt(2.0) / 2.0 * 0 - d * sqrt(2.0) / 2.0 * 0);
		double maxI = o[0] + a * (sqrt(2.0) / 2.0 * s[0] - d * sqrt(2.0) / 2.0 * s[1]);
		double minJ = o[1] + a * (d * sqrt(2.0) / 2.0 * s[0] + sqrt(2.0) / 2.0 * 0);
		double maxJ = o[1] + a * (d * sqrt(2.0) / 2.0 * 0 + sqrt(2.0) / 2.0 * s[1]);

		if (minI<0 || maxI>resolution[0] || minJ<0 || maxJ>resolution[1]) {
			throw std::exception("Sample origin leads to sample defined out of region.");
		}
	}
	else {
		std::cerr << "The fourth input parameter d must be either -1 or 1." << std::endl;
		throw std::exception("The fourth input parameter d must be either -1 or 1.");
	}
	
	pixelArray outArray(s);

	double iOd = 0.0;		// Double version of original coordinates
	double jOd = 0.0;		// Double version of oridinal coordinates
	int iOn = 0;			// Int version of original coordinates
	int jOn = 0;			// Int version of original coordinates
	double iDiff = 0.0;		// Difference between double and int
	double jDiff = 0.0;		// Difference between double and int
	pixel auxPixel;			// Auxiliary pixel for interpolation

	for (int iN = 0; iN < s[0]; iN++) {
		for (int jN = 0; jN < s[1]; jN++) {
			// NOTE that the transformation is from new to old coordinate system (!), hence the inverse rotation matrix
			iOd = o[0] + a * (sqrt(2.0) / 2.0 * iN - d * sqrt(2.0) / 2.0 * jN);
			jOd = o[1] + a * (d * sqrt(2.0) / 2.0 * iN + sqrt(2.0) / 2.0 * jN);

			iOn = (int)floor(iOd);
			jOn = (int)floor(jOd);

			iDiff = iOd - iOn;
			jDiff = jOd - jOn;

			auxPixel.set_interpolated_val(iDiff, jDiff, get_pixel_at(iOn, jOn), get_pixel_at(iOn, jOn + 1), get_pixel_at(iOn + 1, jOn), get_pixel_at(iOn + 1, jOn + 1) );
			outArray.add_pixel(auxPixel);
		}
	}


	return outArray;
}

pixelArray pixelArray::rotate_n90(int n)
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
	pixelArray rotArray(newSize);
	int newI = 0;
	int newJ = 0;
	for (int i = 0; i < resolution[0]; i++) {
		for (int j = 0; j < resolution[1]; j++) {
			newI = transVec[0] + rotMtrx[0] * i + rotMtrx[1] * j;
			newJ = transVec[1] + rotMtrx[2] * i + rotMtrx[3] * j;
			rotArray.set_pixel_at(this->get_pixel_at(i, j), newI, newJ);
		}
	}

	return rotArray;
}

std::vector<pixel> pixelArray::get_data_vector()
{
	return data;
}
