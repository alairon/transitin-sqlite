#include "stdafx.h"
#include "system.hpp"

using namespace std;

bool fileExists(string file) {
	ifstream inFile(file);

	return inFile.good();
}