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

wangTile::~wangTile()
{
}

std::ostream & operator<<(std::ostream & out, const wangTile & t)
{
	// Define insertion operator for auxiliary checks
	return out << "Tile #" << t.get_ID() << ": [ " << t.get_code(0) << ", " << t.get_code(1) << ", " << t.get_code(2) << ", " << t.get_code(3) << "]";
}
