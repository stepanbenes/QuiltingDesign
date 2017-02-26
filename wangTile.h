#pragma once
#include <iostream>
#include "pixelArray.h"

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
	friend std::ostream& operator<<(std::ostream & out, const wangTile & t);
	~wangTile();
};

