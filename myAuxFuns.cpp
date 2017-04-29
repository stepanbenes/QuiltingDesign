#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "wangTile.h"
#include "wangSet.h"
#include "picojson.h"
#include "myAuxFuns.h"



void load_JSON_setting(std::string inFile, wangSet & tileSet, parameters & inParameters, std::vector<sample> & inSamples)
{
	// Load script setting provided in JSON file

	// Load input string
	std::ifstream JSONfile(inFile);
	if (JSONfile.fail()) {
		throw std::exception("Input JSON file does not exist.");
	}
	std::string JSONstring((std::istreambuf_iterator<char>(JSONfile)), std::istreambuf_iterator<char>());

	// Parse buffer string
	picojson::value JSONvalue;
	std::string JSONerr = picojson::parse(JSONvalue, JSONstring);
	if (!JSONerr.empty()) {
		throw std::exception("Loaded JOSN string couldn't be parsed.");
	}

	// Test parsed JSON values	
	if (!JSONvalue.is<picojson::object>()) {
		throw std::exception("JSON is not an object.");
	}
	if ( !(JSONvalue.contains("tileSize") && JSONvalue.contains("sampleOverlap") && JSONvalue.contains("samples") && JSONvalue.contains("tiles")) ) {
		throw std::exception("Loaded JSON doesn't contain all requried fields.");
	}

	// Load all data
	inParameters.nT = (int)JSONvalue.get("tileSize").get<double>();
	inParameters.nO = (int)JSONvalue.get("sampleOverlap").get<double>();
	inParameters.nS = inParameters.nT + inParameters.nO - 1;					// Define sample edge size in terms of ROTATED pixels

	picojson::array tilesArray = JSONvalue.get("tiles").get<picojson::array>();
	int nTiles = (int)tilesArray.size();
	tileSet.set_nTiles(nTiles);
	for (picojson::array::const_iterator it = tilesArray.begin(); it != tilesArray.end(); it++) {
		int auxID = 0;
		int auxCodes[4] = { 0,0,0,0 };
		auxID = (int)it->get("id").get<double>();
		for (int i = 0; i < 4; i++) {
			auxCodes[i] = (int)it->get("codes").get<picojson::array>()[i].get<double>();
		}
		wangTile auxTile(auxID, auxCodes);
		tileSet.add_tile(auxTile);
	}

	picojson::array sampleArray = JSONvalue.get("samples").get<picojson::array>();
	for (picojson::array::const_iterator it = sampleArray.begin(); it != sampleArray.end(); it++) {
		if (it->contains("originX") && it->contains("originY")) {				// Allow for obsolete syntax
			inSamples.push_back(sample((int)it->get("originX").get<double>(), (int)it->get("originY").get<double>(), inParameters.nS));
		}
		else if (it->contains("originI") && it->contains("originJ")) {			// Corect parameters names (originX -> originI, originY -> originJ)
			inSamples.push_back(sample((int)it->get("originI").get<double>(), (int)it->get("originJ").get<double>(), inParameters.nS));
		}
		else {
			throw std::exception("JSON samples do not contain originX/originY or originI/originJ entries.");
		}
	}


	//// Print all data
	//std::cout << "Identified JSON values:" << std::endl;
	//std::cout << "    tileSize:      " << (int)JSONvalue.get("tileSize").get<double>() << std::endl;
	//std::cout << "    sampleOverlap: " << (int)JSONvalue.get("sampleOverlap").get<double>() << std::endl;

	////picojson::array tilesArray = JSONvalue.get("tiles").get<picojson::array>();
	//std::cout << "    # tiles:       " << tilesArray.size() << std::endl;
	//for (picojson::array::const_iterator it = tilesArray.begin(); it != tilesArray.end(); it++) {
	//	std::cout << "        Tile " << it->get("id").get<double>() << ": [ "
	//		<< it->get("codes").get<picojson::array>()[0] << ", "
	//		<< it->get("codes").get<picojson::array>()[1] << ", "
	//		<< it->get("codes").get<picojson::array>()[2] << ", "
	//		<< it->get("codes").get<picojson::array>()[3] << " ]"
	//		<< std::endl;
	//}

	////picojson::array sampleArray = JSONvalue.get("samples").get<picojson::array>();
	//std::cout << "    # samples:       " << sampleArray.size() << std::endl;
	//for (picojson::array::const_iterator it = sampleArray.begin(); it != sampleArray.end(); it++) {
	//	std::cout << "        Sample " << it - sampleArray.begin() << ": [ "
	//		<< it->get("originX").get<double>() << ", "
	//		<< it->get("originY").get<double>() << " ]"
	//		<< std::endl;
	//}

}

void save_JSON_results(std::string & outFile, std::string & date, double duration, double quiltError) {

	picojson::object outJSON;
	std::ofstream outF;

	// Create JSON entries
	outJSON["date"] = picojson::value(date);
	outJSON["duration"] = picojson::value(duration);
	outJSON["quiltError"] = picojson::value(quiltError);

	// Write into file
	outF.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		outF.open(outFile, std::ios::binary | std::ios::out);
	}
	catch (std::ios_base::failure& inErr) {
		std::cerr << "The requested output file cannot be created. System error: " << inErr.what() << std::endl;
	}
	outF << picojson::value(outJSON).serialize();
	outF.close();

}

pixelArray convert_lightnessMap_to_pixelArray(std::vector<double> & lightnessMap, int nTx, int nTy)
{
	int val = 0;
	pixelArray out(nTy,nTx);
	for (int i = 0; i < nTy; i++){
		for (int j = 0; j < nTx; j++) {
			val = (int)(lightnessMap.at((nTy-1-i)*nTx+j)*255);
			out.add_pixel(val,val,val);
		}
	}
	return out;
}
