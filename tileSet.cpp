#include "tileSet.h"



tileSet::tileSet()
{
}


tileSet::~tileSet()
{
}

int tileSet::load_tileSet_data(string inFile)
{
	ifstream inF;

	inF.open(inFile);
	inF >> nTiles;
	inF >> dimension;
	tilesCodes = new int[nTiles*dimension*2];
	for (int i = 0; i < nTiles; i++) {
		for (int j = 0; j < dimension; j++) {
			inF >> tilesCodes[i*dimension * 2 + j * 2];
			inF >> tilesCodes[i*dimension * 2 + j * 2 + 1];
		}
	}
	inF.close();
	return 0;
}
