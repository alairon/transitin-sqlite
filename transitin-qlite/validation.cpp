#include "stdafx.h"
#include "system.hpp"

using namespace std;

/*Checks if a file exists*/
bool fileExists(string file) {
	ifstream inFile(file);

	return (inFile.good());
}