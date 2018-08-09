#include "stdafx.h"
#include "parser.hpp"

using namespace std;

//Checks if the line contains a byte order mark. Returns the string without such a mark, or leaves it as-is
string checkEncoding(string header) {
	cout << header << endl;
	if (header.at(0) == char(0xEF) && header.at(1) == char(0xBB) && header.at(2) == char(0xBF)) {
		cout << "Encoding: UTF-8 (BOM)" << endl;
		return(header.substr(3, header.length()));
	}
	else if (header.at(0) == char(0xFF) && header.at(1) == char(0xFE)) {
		cout << "Encoding: UTF-16 (BE)" << endl;
	}
	else if (header.at(0) == char(0xFE) && header.at(1) == char(0xFF)) {
		cout << "Encoding: UTF-16 (LE)";
	}
	return (header);
}

//Adds quotations to each element in the line
string quoteLine(string line) {
	string returnLine;
	istringstream iss(line);
	string token;

	//Add quotes to every word in the command
	while (getline(iss, token, ',')) {
		returnLine = returnLine + "'" + token + "',";
	}

	//Remove the extra comma
	returnLine.pop_back();

	return returnLine;
}