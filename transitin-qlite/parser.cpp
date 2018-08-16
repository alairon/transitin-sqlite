#include "stdafx.h"
#include "parser.hpp"

using namespace std;

//Checks if the line contains a byte order mark. Returns the string without such a mark, or leaves it as-is
string checkEncoding(string header) {
	if (header.at(0) == char(0xEF) && header.at(1) == char(0xBB) && header.at(2) == char(0xBF)) {
		//cout << "Encoding: UTF-8 (BOM)" << endl;
		return(header.substr(3, header.length()));
	}
	else if (header.at(0) == char(0xFF) && header.at(1) == char(0xFE)) {
		//cout << "Encoding: UTF-16 (BE)" << endl;
	}
	else if (header.at(0) == char(0xFE) && header.at(1) == char(0xFF)) {
		//cout << "Encoding: UTF-16 (LE)" << endl;
	}
	return (header);
}

//Adds quotations to each element in the line
string quoteLine(string line) {
	string returnLine;
	stringstream iss(line);
	string token;

	//Loops through to add quotes if necessary
	while (getline(iss, token, ',')) {
		//If a token contains a double quotation, do not add quotations
		if (token.find('"') != string::npos) {
			returnLine = returnLine + token + ",";
		}
		//Add quotes to every token in the command
		else {
			returnLine = returnLine + "\"" + token + "\",";
		}
	}
	//Check for trailing blank commas
	if (!iss && token.empty()) {
		returnLine = returnLine + "\"\"";
	}
	else {
		//Remove the extra comma
		returnLine.pop_back();
	}

	return returnLine;
}