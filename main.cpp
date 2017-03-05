#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdint> 
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

#define DEBUG 1		// Define DEBUG macro such that certain parts are neglected or printed (set 0 for release)

// TODO Unresolved issues:
// NOTE No error control has been implemented (e.g., during opening files)
// NOTE Discuss validity of the wedge mask
// NOTE Interpolating RGB pixel (linear element-wise interpolation is not optimal)
// NOTE Decide whether to allow non-integer sample origins
// NOTE Change rgbArray into more general imgArray capable of handling both grayscale and RGB

int main(int argc, char * argv[]) throw(...)
{
	std::string inputJSONfile     = "test-iofiles/muLib_ImageInput.json";
	std::string inputSpecimenFile = "test-iofiles/_specimen.bmp";
	std::string outputFolder      = "test-iofiles/";
	std::string outputTilingImage = "test-iofiles/_tiling.bmp";
	const std::string sampleStencil = "sample";
	const std::string sampleSuffix  = ".bmp";
	const std::string tileStencil   = "tile";
	const std::string tileSuffix    = ".bmp";
	wangSet tileSet;
	pixelArray specimen;
	parameters myParams;					// Structure encapsulating settings (defined in myAuxFuns.h)
	std::vector<sample> allSamples;			// Vector containing loaded samples


	// Check input parameters
	if (argc != 3) {
		std::cout << "The script requires three inputs: " << std::endl;
		std::cout << "			[1] absolute path to the JSON input file;" << std::endl;
		std::cout << "			[2] absolute path to an input BMP image;" << std::endl;
		std::cout << "			[3] an absolute path to the output folder." << std::endl;
#if DEBUG == 1
		// With DEBUG macro, use predefined input/output folders/files
		std::cout << "In DEBUG mode, script uses default I/O files..." << std::endl;
#else
		return 1;
#endif
	}
	else {
		inputJSONfile = (std::string)argv[1];
		inputSpecimenFile = (std::string)argv[2];
		outputFolder = (std::string)argv[3];
	}


	// Load setting from JSON file
	load_JSON_setting(inputJSONfile, tileSet, myParams, allSamples);

	// Load specimen data
	specimen.load_BMP(inputSpecimenFile);

	// Extract samples
	for (std::vector<sample>::iterator i = allSamples.begin(); i != allSamples.end(); ++i) {
		try {
			i->acquire_data_from_specimen(specimen);
		}
		catch(const std::exception & e){
			std::cout << e.what() << std::endl;
			std::cin.get();
			return 1;
		}
#if DEBUG == 1
		std::string fileName = "";
		fileName += outputFolder;
		fileName += sampleStencil;
		fileName += std::to_string(i-allSamples.begin());
		fileName += sampleSuffix;
		std::cout << fileName << std::endl;
		i->save_BMP(fileName);
#endif
	}

	// Construct tiles
	tileSet.construct_tiles(allSamples, myParams.nO, myParams.nT);
	tileSet.save_tiles_BMP(outputFolder, tileStencil, tileSuffix);

	// Test tiling
	wangTiling tiling = tileSet.give_stochastic_tiling(4, 3);
	tileSet.construct_tiling_image(&tiling);
	tiling.save_tiling_BMP(outputTilingImage);

#if DEBUG == 1
	std::cout << "Press any key to abort the program...";
	std::cin.get();
#endif

	return 0;
}