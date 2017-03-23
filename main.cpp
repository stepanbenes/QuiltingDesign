#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdint> 
#include <sstream>
#include <fstream>
#include <streambuf>
#include <limits>
#include <exception>
#include "myAuxFuns.h"
#include "pixel.h"
#include "pixelArray.h"
#include "wangTile.h"
#include "wangSet.h"
#include "wangTiling.h"
#include "sample.h"

/*
Main file contains two separate executable scripts: (i) QuiltingDesign and (ii) TilingAssembly (original scripts are stored in main_QD.save and main_TA.save)
Choose among the scripts using defined macro
*/

//#define QA		// Uncomment for QuiltingDesign
#define TA		// Uncomment for TilingAssembly



// ****** QUILTING DESIGN MAIN SCRIPT *****
#ifdef QA

int main(int argc, char * argv[]) throw(...)
{
	std::string inputJSONfile = "test-iofiles/muLib_ImageInput.json";
	std::string inputSpecimenFile = "test-iofiles/_specimen.bmp";
	std::string outputFolder = "test-iofiles/";
	std::string outputTilingImage = "test-iofiles/_tiling.bmp";
	const std::string sampleStencil = "sample";
	const std::string sampleSuffix = ".bmp";
	const std::string tileStencil = "tile";
	const std::string tileSuffix = ".bmp";
	wangSet tileSet;
	pixelArray specimen;
	parameters myParams;					// Structure encapsulating settings (defined in myAuxFuns.h)
	std::vector<sample> allSamples;			// Vector containing loaded samples


	// Check input parameters
	if (argc != 4) {
		std::cout << "The script requires three inputs: " << std::endl;
		std::cout << "			[1] absolute path to the JSON input file;" << std::endl;
		std::cout << "			[2] absolute path to an input BMP image;" << std::endl;
		std::cout << "			[3] an absolute path to the output folder." << std::endl;
#ifdef _DEBUG
		// With DEBUG macro, use predefined input/output folders/files
		std::cout << "In DEBUG mode, script uses default I/O files..." << std::endl;
#else
		exit(1);
#endif
	}
	else {
		inputJSONfile = (std::string)argv[1];
		inputSpecimenFile = (std::string)argv[2];
		outputFolder = (std::string)argv[3];
	}


	// Load setting from JSON file
	try {
		load_JSON_setting(inputJSONfile, tileSet, myParams, allSamples);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
#ifdef _DEBUG
		std::cin.get();
#endif
		exit(1);
	}
	std::cout << "Input JSON file loaded" << std::endl;


	// Load specimen data
	try {
		specimen.load_BMP(inputSpecimenFile);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
#ifdef _DEBUG
		std::cin.get();
#endif
		exit(1);
	}
	std::cout << "Specimen BMP file loaded" << std::endl;


	// Extract samples
	for (std::vector<sample>::iterator i = allSamples.begin(); i != allSamples.end(); ++i) {
		try {
			i->acquire_data_from_specimen(specimen);
		}
		catch (const std::exception & e) {
			std::cerr << e.what() << std::endl;
#ifdef _DEBUG
			std::cin.get();
#endif
			exit(1);
		}
#ifdef _DEBUG
		std::string fileName = "";
		fileName += outputFolder;
		fileName += sampleStencil;
		fileName += std::to_string(i - allSamples.begin());
		fileName += sampleSuffix;
		std::cout << fileName << std::endl;
		i->save_BMP(fileName);
#endif
	}
	std::cout << "Samples extracted" << std::endl;


	// Construct tiles
	tileSet.construct_tiles(allSamples, myParams.nO, myParams.nT);
	tileSet.save_tiles_BMP(outputFolder, tileStencil, tileSuffix);
	std::cout << "Tiles created" << std::endl;


	// Test tiling
	wangTiling tiling = tileSet.give_stochastic_tiling(4, 3);
	std::cout << "Stochastic tiling generated" << std::endl;
	tileSet.construct_tiling_image(&tiling);
	std::cout << "Tiling image generated" << std::endl;
	tiling.save_tiling_BMP(outputTilingImage);


#ifdef _DEBUG
	std::cout << "Press any key to abort the program...";
	std::cin.get();
#endif

	return 0;
}

#endif




// ****** TILING ASSEMBLY MAIN SCRIPT *****
#ifdef TA

int main(int argc, char * argv[]) throw(...)
{
	std::string inputJSONfile = "test-iofiles/muLib_ImageInput_fromJP.json";
	std::string inputFolder = "test-iofiles/fromJP/modified/";
	std::string outputTilingImage = "test-iofiles/_tiling.bmp";
	std::string inputReferenceImage = "test-iofiles/fromJP/reference.bmp";
	int nTx = 40;
	int nTy = 19;
	const std::string tileStencil = "tile";
	const std::string tileSuffix = ".bmp";
	wangSet tileSet;
	parameters myParams;					// Structure encapsulating settings (defined in myAuxFuns.h)
	std::vector<sample> allSamples;			// Obsolete, only to re-use load_JSON_setting function
	bool useRefImg = true;					// Boolean switch for reference image governed tiling
	pixelArray refImg;
	std::vector<double> lightnessMap;
	wangTiling tiling;


	// Check input parameters
	if (argc != 6 && argc != 7) {
		std::cout << "The script requires five or six inputs: " << std::endl;
		std::cout << "			[1] absolute path to the JSON input file;" << std::endl;
		std::cout << "			[2] absolute path to the input folder;" << std::endl;
		std::cout << "			[3] absolute path to the output tiling BMP file+" << std::endl;
		std::cout << "			[4] number of tiles in x-direction;" << std::endl;
		std::cout << "			[5] number of tiles in y-direction." << std::endl;
		std::cout << "			[6] OPTIONAL absolute path to reference BMP picture." << std::endl;
		std::cout << "With six inputs, script tries to mimic lightness map of a reference picture during stochastic assembly." << std::endl;
#ifdef _DEBUG
		// With DEBUG macro, use predefined input/output folders/files
		std::cout << "In DEBUG mode, script uses default I/O files..." << std::endl;
#else
		exit(1);
#endif
	}
	else {
		inputJSONfile = (std::string)argv[1];
		inputFolder = (std::string)argv[2];
		outputTilingImage = (std::string)argv[3];
		std::istringstream snTx(argv[4]);
		std::istringstream snTy(argv[5]);
		snTx >> nTx;
		snTy >> nTy;
		if (argc == 7) {
			useRefImg = true;
			inputReferenceImage = (std::string)argv[6];
		}
		else {
			useRefImg = false;
		}
	}


	// Load setting from JSON file
	try {
		load_JSON_setting(inputJSONfile, tileSet, myParams, allSamples);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
#ifdef _DEBUG
		std::cin.get();
#endif
		exit(1);
	}
	std::cout << "Input JSON file loaded" << std::endl;


	// Load reference image and define average lightness map
	if (useRefImg) {
		try {
			refImg.load_BMP(inputReferenceImage);
		}
		catch (const std::exception & e) {
			std::cerr << e.what() << std::endl;
#ifdef _DEBUG
			std::cin.get();
#endif
			exit(1);
		}

		// Partition reference image and compute local lightness
		lightnessMap = refImg.compute_lightness_map(nTx, nTy);
	}


	// Load tiles
	try {
		tileSet.load_tiles_BMP(inputFolder, tileStencil, tileSuffix);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
#ifdef _DEBUG
		std::cin.get();
#endif
		exit(1);
	}
	std::cout << "Tile images loaded" << std::endl;
	if (useRefImg) {
		tileSet.compute_averaged_tile_lightness();
	}


	// Test tiling
	if (useRefImg) {
		tiling = tileSet.give_stochastic_tiling(nTx, nTy);
	}
	else {
		tiling = tileSet.give_stochastic_tiling(nTx, nTy, lightnessMap);
	}
	std::cout << "Stochastic tiling [ " << nTx << ", " << nTy << "] generated" << std::endl;
	tileSet.construct_tiling_image(&tiling);
	std::cout << "Tiling image generated" << std::endl;
	tiling.save_tiling_BMP(outputTilingImage);


#ifdef _DEBUG
	std::cout << "Press any key to abort the program...";
	std::cin.get();
#endif

	return 0;
}

#endif