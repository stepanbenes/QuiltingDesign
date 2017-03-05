#include <sstream>
#include <random>
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

wangTiling wangSet::give_stochastic_tiling(int nTx, int nTy)
{
	std::random_device rd;							//Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd());							//Standard mersenne_twister_engine seeded with rd()
	std::vector<int> tiling(nTx*nTy);
	std::vector<int> candidates(tiles.size());
	int nCand = 0;
	bool addTile = false;

	for (int iY = 0; iY < nTy; iY++) {
		for (int iX = 0; iX < nTx; iX++) {

			nCand = 0;
			
			// Loop over tiles and identify candidate tiles to place
			for (std::vector<wangTile>::const_iterator it = tiles.begin(); it != tiles.end(); ++it) {
				addTile = true;
				if (iX != 0) {
					addTile = addTile && (tiles.at( tiling.at(iY*nTx+(iX-1)) ).get_code(1) == it->get_code(0));
				}
				if (iY != 0) {
					addTile = addTile && (tiles.at( tiling.at((iY-1)*nTx + iX) ).get_code(3) == it->get_code(2));
				}
				if (addTile) {
					nCand++;
					candidates[nCand - 1] = (it - tiles.begin());
				}
			}

			// Choose one tile 
			std::uniform_int_distribution<> uid(0, nCand-1);
			tiling[iY*nTx+iX] = candidates.at( uid(gen) );

		}
	}

	return wangTiling(nTx, nTy, tiling);
}

void wangSet::construct_tiling_image(wangTiling * inTiling)
{
	// Create image representation of provided stochastic tiling and store it in the tiling itself

	// Acquire tile resolution
	int resI, resJ;
	tiles.at(0).get_tile_image().get_resolution(resI,resJ);
	int nTx, nTy;
	inTiling->get_tiling_size(nTx, nTy);

	// Initialize new image
	pixelArray tilImg(nTy*resI, nTx*resJ);

	// Populate array
	int iTx, iTy;
	int locI, locJ;
	int tID;
	for (int i = 0; i < (nTy*resI); i++) {
		for (int j = 0; j < (nTx*resJ); j++) {

			iTy = nTy - 1 - (i / resI);
			iTx = j / resJ;
			locI = i % resI;
			locJ = j % resJ;
			tID = inTiling->get_tileID_at(iTx, iTy);

			tilImg.add_pixel( tiles.at(tID).get_tile_image().get_pixel_at(locI,locJ) );
		}
	}

	// Store image in tiling object
	inTiling->set_tiling_image(tilImg);
}

std::ostream & operator<<(std::ostream & out, const wangSet & s)
{
	out << "Wang tiles set contains " << s.nT << " tiles:" << std::endl;

	for (std::vector<wangTile>::const_iterator iT = s.tiles.begin(); iT != s.tiles.end(); ++iT) {
		out << "    " << *iT << std::endl;
	}
	return out;
}
