#pragma once

/*Checks if the line contains a byte order mark. Returns the string without such a mark, or leaves it as-is.
Only supports ANSI or UTF-8*/
std::string checkEncoding(std::string header);

/*Adds quotes around each comma delimited word in a string*/
std::string quoteLine(std::string line);