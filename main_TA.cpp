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
	TILE ASSEMBLY SCRIPT
*/

int main(int argc, char * argv[]) throw(...)
{
	std::string inputJSONfile = "test-iofiles/muLib_ImageInput.json";
	std::string inputFolder = "test-iofiles/";
	std::string outputTilingImage = "test-iofiles/_tiling.bmp";
	int nTx = 4;
	int nTy = 3;
	const std::string tileStencil = "tile";
	const std::string tileSuffix = ".bmp";
	wangSet tileSet;
	parameters myParams;					// Structure encapsulating settings (defined in myAuxFuns.h)
	std::vector<sample> allSamples;			// Obsolete, only to re-use load_JSON_setting function


	// Check input parameters
	if (argc != 6) {
		std::cout << "The script requires five inputs: " << std::endl;
		std::cout << "			[1] absolute path to the JSON input file;" << std::endl;
		std::cout << "			[2] absolute path to the input folder;" << std::endl;
		std::cout << "			[3] absolute path to the output tiling BMP file+" << std::endl;
		std::cout << "			[4] number of tiles in x-direction;" << std::endl;
		std::cout << "			[5] number of tiles in y-direction." << std::endl;
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


	// Test tiling
	wangTiling tiling = tileSet.give_stochastic_tiling(nTx, nTy);
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