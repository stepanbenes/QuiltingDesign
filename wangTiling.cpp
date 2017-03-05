#include "wangTiling.h"

wangTiling::wangTiling()
{
}

wangTiling::wangTiling(int inNtx, int inNty, std::vector<int> inTilingPlan)
{
	nTx = inNtx;
	nTy = inNty;
	tilingPlan = inTilingPlan;
}

wangTiling::~wangTiling()
{
}

void wangTiling::get_tiling_size(int & outNtx, int & outNty)
{
	outNtx = nTx;
	outNty = nTy;
}

int wangTiling::get_tileID_at(int iTx, int iTy)
{
	return tilingPlan.at(iTy*nTx+iTx);
}

void wangTiling::set_tiling_image(pixelArray & inImage)
{
	tilingImage = inImage;
}

void wangTiling::save_tiling_BMP(std::string outFile)
{
	this->tilingImage.save_BMP(outFile);
}
