#pragma once
#include <vector>
#include <fstream>
#include "pixelArray.h"

class wangTiling
{
private:
	int nTx, nTy;
	std::vector<int> tilingPlan;
	pixelArray tilingImage;
public:
	wangTiling();
	wangTiling(int inNtx, int inNty, std::vector<int> inTilingPlan);
	~wangTiling();
	void get_tiling_size(int & nTx, int & nTy);
	int get_tileID_at(int iTx, int iTy);
	void set_tiling_image(pixelArray & inImage);
	void save_tiling_BMP(std::string outFile);
	void save_tiling_WTF(std::string outFile);
};

