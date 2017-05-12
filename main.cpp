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
#include <chrono>
#include <ctime>
#include <iomanip>
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

#pragma warning(disable : 4996)		// Disable _CRT_SECURE_NO_WARNINGS that occurs due to ctime() use

// ****** QUILTING DESIGN MAIN SCRIPT *****
#ifdef QA

int main(int argc, char * argv[]) throw(...)
{
	std::string inputJSONfile = "test-iofiles/muLib_ImageInput.json";
	std::string outputJSONfile = "log.json";
	std::string inputSpecimenFile = "test-iofiles/_specimen.bmp";
	std::string outputFolder = "test-iofiles/";
	std::string outputTilingImage = "test-iofiles/_tiling.bmp";
	const std::string sampleStencil = "sample";
	const std::string sampleSuffix = ".bmp";
	const std::string tileStencil = "tile";
	const std::string tileSuffix = ".bmp";
	std::chrono::time_point<std::chrono::system_clock> startWatch, stopWatch, allStartWatch, allStopWatch;
	std::chrono::duration<double> timeInterval;
	std::time_t currentTime;
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
	allStartWatch = std::chrono::system_clock::now();

	// Load setting from JSON file
	startWatch = std::chrono::system_clock::now();
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
	stopWatch = std::chrono::system_clock::now();
	timeInterval = stopWatch - startWatch;
	currentTime = std::chrono::system_clock::to_time_t(stopWatch);
	std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Input JSON file loaded in " << timeInterval.count() << " s." << std::endl;


	// Load specimen data
	startWatch = std::chrono::system_clock::now();
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
	stopWatch = std::chrono::system_clock::now();
	timeInterval = stopWatch - startWatch;
	currentTime = std::chrono::system_clock::to_time_t(stopWatch);
	std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Specimen BMP file loaded in " << timeInterval.count() << " s." << std::endl;


	// Extract samples
	startWatch = std::chrono::system_clock::now();
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
	stopWatch = std::chrono::system_clock::now();
	timeInterval = stopWatch - startWatch;
	currentTime = std::chrono::system_clock::to_time_t(stopWatch);
	std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Samples extracted in " << timeInterval.count() << " s." << std::endl;


	// Construct tiles
	startWatch = std::chrono::system_clock::now();
	tileSet.construct_tiles(allSamples, myParams.nO, myParams.nT);
	tileSet.save_tiles_BMP(outputFolder, tileStencil, tileSuffix);
	stopWatch = std::chrono::system_clock::now();
	timeInterval = stopWatch - startWatch;
	currentTime = std::chrono::system_clock::to_time_t(stopWatch);
	std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Tiles created in " << timeInterval.count() << " s with overall quilting errror " << tileSet.get_quiltErr() << "." << std::endl;


	// Test tiling
	startWatch = std::chrono::system_clock::now();
	wangTiling tiling = tileSet.give_stochastic_tiling(4, 3);
	stopWatch = std::chrono::system_clock::now();
	timeInterval = stopWatch - startWatch;
	currentTime = std::chrono::system_clock::to_time_t(stopWatch);
	std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Stochastic tiling generated in " << timeInterval.count() << " s." << std::endl;

	startWatch = std::chrono::system_clock::now();
	tileSet.construct_tiling_image(&tiling);
	stopWatch = std::chrono::system_clock::now();
	timeInterval = stopWatch - startWatch;
	currentTime = std::chrono::system_clock::to_time_t(stopWatch);
	std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Tiling image generated in " << timeInterval.count() << " s." << std::endl;
	tiling.save_tiling_BMP(outputTilingImage);


	allStopWatch = std::chrono::system_clock::now();
	timeInterval = stopWatch - startWatch;
	std::stringstream datestream;
	datestream << std::put_time(std::localtime(&currentTime), "%c");
	save_JSON_results(outputFolder + outputJSONfile, datestream.str(), timeInterval.count(), tileSet.get_quiltErr());

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
	std::string inputFolder = "test-iofiles/fromJP/type_e/";
	std::string outputTilingImage = "test-iofiles/_tiling.bmp";
	std::string inputReferenceImage = "test-iofiles/fromJP/reference.bmp";
	int nTx = 40;
	int nTy = 19;
	const std::string tileStencil = "tile";
	const std::string tileSuffix = ".bmp";
	std::chrono::time_point<std::chrono::system_clock> startWatch, stopWatch, allStartWatch, allStopWatch;
	std::chrono::duration<double> timeInterval;
	std::time_t currentTime;
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
	allStartWatch = std::chrono::system_clock::now();


	// Check if BMP or WTF is required
	bool wtfOnly = false;
	size_t charLen = outputTilingImage.length();
	if (outputTilingImage.compare(charLen - 4, 4, ".wtf") == 0) {
		wtfOnly = true;
	};

	// Load setting from JSON file
	startWatch = std::chrono::system_clock::now();
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
	stopWatch = std::chrono::system_clock::now();
	timeInterval = stopWatch - startWatch;
	currentTime = std::chrono::system_clock::to_time_t(stopWatch);
	std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Input JSON file loaded in " << timeInterval.count() << " s." << std::endl;


	// Load reference image and define average lightness map
	if (useRefImg) {
		startWatch = std::chrono::system_clock::now();
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
		pixelArray lightnessImg = convert_lightnessMap_to_pixelArray(lightnessMap, nTx, nTy);
		//lightnessImg.save_BMP("lightnessMap.bmp");
		stopWatch = std::chrono::system_clock::now();
		timeInterval = stopWatch - startWatch;
		currentTime = std::chrono::system_clock::to_time_t(stopWatch);
		std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Input JSON file loaded in " << timeInterval.count() << " s." << std::endl;
	}


	// Load tiles
	if (!wtfOnly) {
		startWatch = std::chrono::system_clock::now();
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
		if (useRefImg) {
			tileSet.compute_averaged_tile_lightness();
		}
		stopWatch = std::chrono::system_clock::now();
		timeInterval = stopWatch - startWatch;
		currentTime = std::chrono::system_clock::to_time_t(stopWatch);
		std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Tile images loaded in " << timeInterval.count() << " s." << std::endl;

	}

	// Construct stochastic tiling
	startWatch = std::chrono::system_clock::now();
	if (useRefImg) {
		tiling = tileSet.give_stochastic_tiling(nTx, nTy, lightnessMap);
	}
	else {
		tiling = tileSet.give_stochastic_tiling(nTx, nTy);
	}
	stopWatch = std::chrono::system_clock::now();
	timeInterval = stopWatch - startWatch;
	currentTime = std::chrono::system_clock::to_time_t(stopWatch);
	std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Stochastic tiling[" << nTx << ", " << nTy << "] generated in " << timeInterval.count() << " s." << std::endl;

	// Assemble tiling image
	if (!wtfOnly) {
		startWatch = std::chrono::system_clock::now();
		tileSet.construct_tiling_image(&tiling);
		tiling.save_tiling_BMP(outputTilingImage);
		stopWatch = std::chrono::system_clock::now();
		timeInterval = stopWatch - startWatch;
		currentTime = std::chrono::system_clock::to_time_t(stopWatch);
		std::cout << std::put_time(std::localtime(&currentTime), "%T") << ": Tiling image generated and saved in " << timeInterval.count() << " s." << std::endl;
	}
	else {
		tiling.save_tiling_WTF(outputTilingImage);
	}


	allStopWatch = std::chrono::system_clock::now();

#ifdef _DEBUG
	std::cout << "Press any key to abort the program...";
	std::cin.get();
#endif

	return 0;
}

#endif