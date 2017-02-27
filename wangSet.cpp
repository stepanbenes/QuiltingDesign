#include <sstream>
#include "wangSet.h"

wangSet::wangSet()
{
}

wangSet::~wangSet()
{
}

int wangSet::get_nTiles()
{
	return nT;
}

void wangSet::set_nTiles(int inNt)
{
	// Set number of tiles and pre-allocate space in wangTile vector
	nT = inNt;
	if (!tiles.empty()) {
		std::cout << "Warning: tiles vector of wangTile is not empty. Setting new number of tiles erases tiles vector.." << std::endl;
		tiles.clear();
	}
	tiles.reserve(nT);
}

void wangSet::add_tile(wangTile inTile)
{
	tiles.push_back(inTile);
}

wangTile wangSet::get_tile_at(int tID)
{
	return tiles.at(tID);
}

void wangSet::construct_tiles(std::vector<sample> & allSamples, int nO, int nT)
{
	for (std::vector<wangTile>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
		it->construct_tile_image(allSamples, nO, nT);
	}
}

void wangSet::save_tiles_BMP(std::string outputFolder, std::string tileStencil, std::string tileSuffix)
{
	for (std::vector<wangTile>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
		std::string fileName = outputFolder;
		fileName += tileStencil;
		fileName += std::to_string(it-tiles.begin());
		fileName += tileSuffix;
		std::cout << fileName << std::endl;
		it->save_tile_BMP(fileName);
	}
}

std::ostream & operator<<(std::ostream & out, const wangSet & s)
{
	out << "Wang tiles set contains " << s.nT << " tiles:" << std::endl;

	for (std::vector<wangTile>::const_iterator iT = s.tiles.begin(); iT != s.tiles.end(); ++iT) {
		out << "    " << *iT << std::endl;
	}
	return out;
}
