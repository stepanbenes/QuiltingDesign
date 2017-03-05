#pragma once
#include <iostream>
#include "pixelArray.h"
#include "sample.h"

class wangTile
{
protected:
	int codes[4] = { 0,0,0,0 };
	int id = 0;
	pixelArray imageData;
public:
	wangTile();
	wangTile(int inId, int inCodes[4]);
	wangTile(int inId, int inCode1, int inCode2, int inCode3, int inCode4);
	int get_ID() const;
	int get_code(int iC) const;
	pixelArray get_tile_image() const;
	void construct_tile_image(std::vector<sample> & allSamples, int nO, int nT);
	void save_tile_BMP(std::string outFile);
	friend std::ostream& operator<<(std::ostream & out, const wangTile & t);
	~wangTile();
};

