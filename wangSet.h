#pragma once
#include <vector>
#include "wangTile.h"

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
	friend std::ostream& operator<<(std::ostream & out, const wangSet & s);
};

