#include "stdafx.h"
#include "parser.hpp"

using namespace std;

//Checks if the line contains a byte order mark. Returns the string without such a mark, or leaves it as-is. Only supports UTF-8
string checkEncoding(string header) {
	if (header.at(0) == char(0xEF) && header.at(1) == char(0xBB) && header.at(2) == char(0xBF)) {
		return(header.substr(3, header.length()));
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
		//Remove the extra comma left from the while loop
		returnLine.pop_back();
	}

	return returnLine;
}