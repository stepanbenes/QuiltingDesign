#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class tileSet
{
public:
	int nTiles = 0;
	int dimension = 0;
	int * tilesCodes = nullptr;

	tileSet();
	~tileSet();
	int load_tileSet_data(string inFile);
};

