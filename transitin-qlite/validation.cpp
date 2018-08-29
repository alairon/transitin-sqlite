#include "stdafx.h"
#include "files.hpp"

using namespace std;

bool fileExists(string file) {
	ifstream inFile(file);

	return inFile.good();
}