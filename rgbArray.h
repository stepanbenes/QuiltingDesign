#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class rgbArray
{
private:
public:
	int size[2] = {};
	int * data = nullptr;
	rgbArray();
	~rgbArray();
	void initialize_array(int inSize[2]);
	void load_textified_image(string inFile);
	void save_textified_image(string outFile);
	int convert_3D_to_1D(int i, int j, int k);
	rgbArray * rotate_n90(int n);
	rgbArray * extract_sample(double o[2], int s[2], double a, int d);
};
