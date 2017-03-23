#pragma once
#include <vector>
#include "wangTile.h"
#include "wangTiling.h"

class wangSet
{
private:
	int nT = 0;						// Number of tiles within the set
	std::vector<wangTile> tiles;	// Vector of Wang tiles
public:
	wangSet();
	~wangSet();
	int get_nTiles();
	void set_nTiles(int inNt);
	void add_tile(wangTile inTile);
	wangTile get_tile_at(int tID);
	void construct_tiles(std::vector<sample> & allSamples, int nO, int nT);
	void save_tiles_BMP(std::string outputFolder, std::string tileStencil, std::string tileSuffix);
	void load_tiles_BMP(std::string folder, std::string tileStencil, std::string tileSuffix);
	wangTiling give_stochastic_tiling(int nTx, int nTy);
	void construct_tiling_image(wangTiling * inTiling);
	friend std::ostream& operator<<(std::ostream & out, const wangSet & s);
};

