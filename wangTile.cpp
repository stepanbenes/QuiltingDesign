#include "wangTile.h"

wangTile::wangTile()
{
}

wangTile::wangTile(int inId, int inCodes[4])
{
	id = inId;
	codes[0] = inCodes[0];
	codes[1] = inCodes[1];
	codes[2] = inCodes[2];
	codes[3] = inCodes[3];
}

wangTile::wangTile(int inId, int inCode1, int inCode2, int inCode3, int inCode4)
{
	id = inId;
	codes[0] = inCode1;
	codes[1] = inCode2;
	codes[2] = inCode3;
	codes[3] = inCode4;
}

int wangTile::get_ID() const
{
	return id;
}

int wangTile::get_code(int iC) const
{
	if ((-1 < iC) && (iC < 4)) {
		return codes[iC];
	}
	else
	{
		return -1;
	}
}

pixelArray wangTile::get_tile_image() const
{
	return imageData;
}

void wangTile::construct_tile_image(std::vector<sample> & allSamples, int nO, int nT)
{
	//
	int tileSize[2] = {nT, nT};
	double tileScale = 2.0/sqrt(2.0);
	double tileOrigin[2] = { (nO - 1.0) / 2.0, ((2.0*nT+nO)-1.0)/2.0 };

	sample merged03 = allSamples[codes[0]].merge_sample(&allSamples[codes[3]], nO, 1);
	sample sample2rot = allSamples[codes[2]].rotate_n90(2);
	sample sample1rot = allSamples[codes[1]].rotate_n90(2);
	sample merged21rot = sample1rot.merge_sample(&sample2rot, nO, 1);
	sample merged21 = merged21rot.rotate_n90(2);

	sample merged03rot = merged03.rotate_n90(1);
	merged21rot = merged21.rotate_n90(1);
	sample merged0321rot = merged03rot.merge_sample(&merged21rot, nO, 2);
	sample merged0321 = merged0321rot.rotate_n90(3);

	imageData = merged0321.extract_patch(tileOrigin, tileSize, tileScale, -1);
}

void wangTile::save_tile_BMP(std::string outFile)
{
	this->imageData.save_BMP(outFile);
}

wangTile::~wangTile()
{
}

std::ostream & operator<<(std::ostream & out, const wangTile & t)
{
	// Define insertion operator for auxiliary checks
	return out << "Tile #" << t.get_ID() << ": [ " << t.get_code(0) << ", " << t.get_code(1) << ", " << t.get_code(2) << ", " << t.get_code(3) << "]";
}
