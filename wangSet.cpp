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

std::ostream & operator<<(std::ostream & out, const wangSet & s)
{
	// TODO: insert return statement here
	out << "Wang tiles set contains " << s.nT << " tiles:" << std::endl;

	for (std::vector<wangTile>::const_iterator iT = s.tiles.begin(); iT != s.tiles.end(); ++iT) {
		out << "    " << *iT << std::endl;
	}
	return out;
}
