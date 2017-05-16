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

void wangTiling::save_tiling_WTF(std::string outFile)
{

	std::ofstream outF;
	outF.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::vector<int>::const_iterator iD;

	// Write output file 
	try {
		outF.open(outFile);
	}
	catch (std::ios_base::failure& inErr) {
		std::cerr << "The requested output file cannot be created. System error: " << inErr.what() << std::endl;
	}
	for (int iTy = nTy - 1; iTy >= 0; iTy--) {
		for (int iTx = 0; iTx < nTx; iTx++) {
			outF << tilingPlan.at(iTy*nTx+iTx) << " ";
		}
		outF << std::endl;
	}
	outF.close();


}
