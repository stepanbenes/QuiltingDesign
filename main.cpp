#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdint> 
#include <fstream>
#include <streambuf>
#include "myAuxFuns.h"
#include "rgbArray.h"
//#include "rgbSpecimen.h"
#include "tileSet.h"
#include "pixel.h"
#include "pixelArray.h"
#include "picojson.h"

#define DEBUG 1		// Define DEBUG macro such that certain parts are neglected or printed (set 0 for release)

// TODO Unresolved issues:
// NOTE No error control has been implemented (e.g., during opening files)
// NOTE Discuss validity of the wedge mask
// NOTE Interpolating RGB pixel (linear element-wise interpolation is not optimal)
// NOTE Decide whether to allow non-integer sample origins
// NOTE Change rgbArray into more general imgArray capable of handling both grayscale and RGB

int main(int argc, char * argv[])
{
	pixel voxA;
	pixel voxB(1, 3, 5);
	pixel voxC,voxD;
	pixelArray specimen("test-iofiles/_specimen.tpg");
	//uint8_t myUint8Var;
	//char myCharVar;
	//int myIntVar;
	string inputJSONfile = "test-iofiles/_specimen.tpg";
	string inputSpecimenFile = "test-iofiles/_specimen.tpg";
	string outputFolder = "test-iofiles/";


	// test json
	std::ifstream JSONfile("test-iofiles/muLib_ImageInput.json");
	std::string JSONstring( (std::istreambuf_iterator<char>(JSONfile)), std::istreambuf_iterator<char>());

	std::string json = JSONstring;
	picojson::value v;
	std::string err = picojson::parse(v, json);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}


	// check if the type of the value is "object"
	if (!v.is<picojson::object>()) {
		std::cerr << "JSON is not an object" << std::endl;
		exit(2);
	}

	// obtain a const reference to the map, and print the contents
	const picojson::value::object& obj = v.get<picojson::object>();
	for (picojson::value::object::const_iterator i = obj.begin();
	i != obj.end();
		++i) {
		std::cout << i->first << ": " << i->second.to_str() << std::endl;
	}

	// Check input parameters
	if (argc != 3) {
		std::cout << "The script requires three inputs: " << std::endl;
		std::cout << "			[1] absolute path to the JSON input file;" << std::endl;
		std::cout << "			[2] absolute path to an input BMP image;" << std::endl;
		std::cout << "			[3] an absolute path to the output folder." << std::endl;
		#if DEBUG == 1
				// With DEBUG macro, use predefined input/output folders/files
				std::cout << "In DEBUG mode, using default parameters: " << std::endl;
		#else
				return 1;
		#endif
	}
	else {
		inputJSONfile = (string)argv[1];
		inputSpecimenFile = (string)argv[2];
		outputFolder = (string)argv[3];

	}

	// Try JSON parsing
	
	std::cout << "Press any key to abort the program.";
	std::cin.get();




	return 0;


	//specimen.load_textified_image("test-iofiles/_specimen.tpg");
	//specimen.save_binary_image("test-iofiles/_specimen2.bim");
	//specimen.load_binary_image("test-iofiles/_specimen2.bim");
	//specimen.save_textified_image("test-iofiles/_specimen2.tpg");
	//specimen.load_textified_image("test-iofiles/_specimen2.tpg");
	//specimen.save_BMP("test-iofiles/_specimen2.bmp");
	//specimen.load_BMP("test-iofiles/_specimen2.bmp");
	//specimen.save_textified_image("test-iofiles/_specimen3.tpg");

	//voxD = voxB;
	//voxA.set_val(2, 4, 6);
	//voxD = voxA;
	//voxC = voxA + voxB;
	//cout << (voxA - voxD) << endl;

	//
	//cout << "Testing overloaded functions" << endl;
	//cout << "VoxA = " << voxA << endl;
	//cout << "VoxB = " << voxB << endl;
	//cout << "VoxC = " << voxC << endl;
	//cout << "VoxD = " << voxD << endl;

	//pixel voxA + voxB;

	//// Define input file name
	//string settingsFile = "test-iofiles/_allSettings.set"; 
	//ostringstream label;
	//string labelBegin = "test-iofiles/sample";
	//string labelEnd = ".tpg";
	//std::cout << "Starting QuiltingDesing script..." << endl;

	//// Allocate variables for setting parameters and load them
	//string tileSetFile  = "";
	//string specimenFile = "";
	//int nT = 0;						// Define length of the resulting tile in terms of ORIGINAL pixels
	//int nO = 0;						// [OPTIMALLY ODD NUMBER] Define samples overlap in terms of ROTATED pixels
	//int nSample = 0;
	//double * sampleCoords = nullptr;
	//load_setting(settingsFile, &specimenFile, &tileSetFile, &nT, &nO, &nSample, &sampleCoords);

	//// Load tile set definition
	//tileSet myTileSet;
	//myTileSet.load_tileSet_data(tileSetFile);

	//// Load specimen
	//rgbArray specimen;
	//specimen.load_textified_image(specimenFile);

	//// Load samples
	//int nS = nT + nO - 1;					// Define sample edge size in terms of ROTATED pixels
	//int sampleSize[2] = { nS, nS };			// Reshape into approporiate form
	//double sampleScale = sqrt(2.0)/2.0;		// Sample has different pixel size (two new pixels per a diagonal of the specimen pixel
	//double sampleOrigin[2] = { 0.0, 0.0 };
	//rgbArray ** allSamples = nullptr;
	//allSamples = new rgbArray * [nSample];
	//for (int i = 0; i < nSample; i++) {
	//	sampleOrigin[0] = sampleCoords[i*2];
	//	sampleOrigin[1] = sampleCoords[i*2+1];
	//	label.str("");
	//	label << labelBegin << i << labelEnd;

	//	allSamples[i] = specimen.extract_sample(sampleOrigin, sampleSize, sampleScale, 1);
	//	allSamples[i]->save_textified_image(label.str());
	//}

	//// Construct tiles
	//int tileSize[2] = {nT, nT};
	//double tileScale = 2.0/sqrt(2.0);
	//double tileOrigin[2] = { (nO - 1.0) / 2.0, ((2.0*nT+nO)-1.0)/2.0 };
	//rgbArray ** allTiles = nullptr;
	//allTiles = new rgbArray *[myTileSet.nTiles];

	//for (int iT = 0; iT < myTileSet.nTiles; iT++) {	
	//	std::cout << "Processing " << iT << "th tile (" << myTileSet.tilesCodes[iT * 4] << myTileSet.tilesCodes[iT * 4 + 1] << myTileSet.tilesCodes[iT * 4 + 2] << myTileSet.tilesCodes[iT * 4 + 3] << ")" << endl;

	//	rgbArray * mergedSample14 = nullptr;
	//	rgbArray * rotated2Sample2 = nullptr;
	//	rgbArray * rotated2Sample3 = nullptr;
	//	rgbArray * mergedRotated2Sample23 = nullptr;
	//	rgbArray * mergedSample32 = nullptr;
	//	rgbArray * rotatedMerged1Sample14 = nullptr;
	//	rgbArray * rotatedMerged1Sample32 = nullptr;
	//	rgbArray * rotatedMerged1Sample1432 = nullptr;
	//	rgbArray * mergedSample1432 = nullptr;
	//	rgbArray * auxTile = nullptr;

	//	// Join samples corresponding to edges 1 and 4
	//	mergedSample14 = merge_samples(allSamples[ myTileSet.tilesCodes[iT*4+0] ], allSamples[ myTileSet.tilesCodes[iT*4+3] ], nO, 1);

	//	// Join samples corresponding to edges 3 and 2 (performed on 180°-rotated arrays in order to apply correct mask and search directions)
	//	rotated2Sample2 = allSamples[ myTileSet.tilesCodes[iT*4+1] ]->rotate_n90(2);
	//	rotated2Sample3 = allSamples[ myTileSet.tilesCodes[iT*4+2] ]->rotate_n90(2);
	//	mergedRotated2Sample23 = merge_samples(rotated2Sample2, rotated2Sample3, nO, 1);
	//	mergedSample32 = mergedRotated2Sample23->rotate_n90(2);

	//	delete rotated2Sample2;
	//	delete rotated2Sample3;
	//	delete mergedRotated2Sample23;

	//	// Join bi-samples (rotated by 90°, wedge mask has to be applied to both ends of the overlap) 
	//	rotatedMerged1Sample14 = mergedSample14->rotate_n90(1);
	//	rotatedMerged1Sample32 = mergedSample32->rotate_n90(1);
	//	rotatedMerged1Sample1432 = merge_samples(rotatedMerged1Sample14, rotatedMerged1Sample32, nO, 2);
	//	mergedSample1432 = rotatedMerged1Sample1432->rotate_n90(3);

	//	//rotatedMerged1Sample14->save_textified_image("test-iofiles/_mergedSample14.tpg");
	//	//rotatedMerged1Sample32->save_textified_image("test-iofiles/_mergedSample32.tpg");
	//	//rotatedMerged1Sample1432->save_textified_image("test-iofiles/_mergedRotated1Sample1432.tpg");
	//	//mergedSample1432->save_textified_image("test-iofiles/_mergedSample1432.tpg");

	//	delete mergedSample14;
	//	delete mergedSample32;
	//	delete rotatedMerged1Sample14;
	//	delete rotatedMerged1Sample32;
	//	delete rotatedMerged1Sample1432;

	//	// Extract final tile
	//	allTiles[iT] = mergedSample1432->extract_sample(tileOrigin, tileSize, tileScale, -1);

	//	label.str("");
	//	label << "test-iofiles/tile" << iT << ".tpg";
	//	allTiles[iT]->save_textified_image(label.str());

	//	delete mergedSample1432;
	//}

	//std::cout << "Hit any key to exit the script ..." << endl;
	//std::cin.get();

	//// Free allocated variables
	//delete sampleCoords;
	//for (int i = 0; i < nSample; i++) {
	//	delete allSamples[i];
	//}
	//delete[] allSamples;
	//for (int i = 0; i < myTileSet.nTiles; i++) {
	//	delete allTiles[i];
	//}
	//delete[] allTiles;

	//return 0;
}