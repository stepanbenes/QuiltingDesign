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
	double averagedLightness = 0.0;
	double quiltErr = 0.0;
public:
	wangTile();
	wangTile(int inId, int inCodes[4]);
	wangTile(int inId, int inCode1, int inCode2, int inCode3, int inCode4);
	int get_ID() const;
	int get_code(int iC) const;
	double get_quiltErr() const;
	pixelArray get_tile_image() const;
	pixelArray * getptr_tile_image();
	pixel get_pixel_at(int iCoord, int jCoord);
	pixel * getptr_pixel_at(int iCoord, int jCoord);
	void construct_tile_image(std::vector<sample> & allSamples, int nO, int nT);
	void save_tile_BMP(std::string outFile);
	void load_tile_BMP(std::string inFile);
	void compute_averaged_tile_lightness();
	double get_averaged_lightness();
	friend std::ostream& operator<<(std::ostream & out, const wangTile & t);
	~wangTile();
};

