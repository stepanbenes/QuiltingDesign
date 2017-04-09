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

double wangTile::get_quiltErr() const
{
	return quiltErr;
}

pixelArray wangTile::get_tile_image() const
{
	return imageData;
}

pixelArray * wangTile::getptr_tile_image()
{
	return &imageData;
}

pixel wangTile::get_pixel_at(int iCoord, int jCoord)
{
	return imageData.get_pixel_at(iCoord,jCoord);
}

pixel * wangTile::getptr_pixel_at(int iCoord, int jCoord)
{
	return imageData.getptr_pixel_at(iCoord, jCoord);
}

void wangTile::construct_tile_image(std::vector<sample> & allSamples, int nO, int nT)
{
	//
	int tileSize[2] = {nT, nT};
	double tileScale = 2.0/sqrt(2.0);
	double tileOrigin[2] = { (nO - 1.0) / 2.0, ((2.0*nT+nO)-1.0)/2.0 };
	double sampleQuiltErr;
	double totalQuiltErr = 0.0;

	sampleQuiltErr = 0.0;
	sample merged03 = allSamples.at(codes[0]).merge_sample(&allSamples.at(codes[3]), nO, 1, sampleQuiltErr);
	totalQuiltErr += sampleQuiltErr;
	sample sample2rot = allSamples.at(codes[2]).rotate_n90(2);
	sample sample1rot = allSamples.at(codes[1]).rotate_n90(2);
	sampleQuiltErr = 0.0;
	sample merged21rot = sample1rot.merge_sample(&sample2rot, nO, 1, sampleQuiltErr);
	totalQuiltErr += sampleQuiltErr;
	sample merged21 = merged21rot.rotate_n90(2);

	sample merged03rot = merged03.rotate_n90(1);
	merged21rot = merged21.rotate_n90(1);
	sampleQuiltErr = 0.0;
	sample merged0321rot = merged03rot.merge_sample(&merged21rot, nO, 2, sampleQuiltErr);
	totalQuiltErr += sampleQuiltErr;
	sample merged0321 = merged0321rot.rotate_n90(3);

	imageData = merged0321.extract_patch(tileOrigin, tileSize, tileScale, -1);
	quiltErr = totalQuiltErr;
}

void wangTile::save_tile_BMP(std::string outFile)
{
	this->imageData.save_BMP(outFile);
}

void wangTile::load_tile_BMP(std::string inFile)
{
	imageData.load_BMP(inFile);
}

void wangTile::compute_averaged_tile_lightness()
{
	averagedLightness = imageData.compute_averaged_lightness();
}

double wangTile::get_averaged_lightness()
{
	return averagedLightness;
}

wangTile::~wangTile()
{
}

std::ostream & operator<<(std::ostream & out, const wangTile & t)
{
	// Define insertion operator for auxiliary checks
	return out << "Tile #" << t.get_ID() << ": [ " << t.get_code(0) << ", " << t.get_code(1) << ", " << t.get_code(2) << ", " << t.get_code(3) << "]";
}
