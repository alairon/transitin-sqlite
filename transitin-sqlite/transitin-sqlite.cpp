// transitin-sqlite.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "database.hpp"
#include "validation.hpp"
#include "system.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	int status = 0;
	bool continueFlag = true;
	string dbFile;
	clock_t startTime;
	clock_t lapsedTime;
	time_t now;

	sqlite3 *db;
	int rc;

	//Hide the cursor
	showConsoleCursor(false);

	//Initialize the error file for writing.
	time(&now);
	initErrorFile(now);

	//Perform a quick check to see if the required files exist
	continueFlag = mandatoryFileCheck();

	if (!continueFlag) {
		cout << "The program was not able to open the files necessary to operate. Please check for the missing files listed prior to continuing." << endl;
		exit(1);
	}

	//If the user specified a file name, use that. Otherwise, use a default name.
	//Default values are listed under files.cpp
	if (argc == 2) {
		dbFile = (string)argv[1] + ".sqlite";
	}
	else {
		cout << "No file was specified, the program will use the default name." << endl;
		dbFile = defaultGTFSFilename;
	}

	//Attempt to open the database file, or create one if it doesn't already exist.
	rc = sqlite3_open(dbFile.c_str(), &db);
	if (rc) {
		cout << "The database \"" << dbFile << "\" could not be used." << endl << "  Msg: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		exit(1);
	}
	cout << "Connected to the database: " << dbFile << endl;

	startTime = clock();
	//Create table headers with GTFS datatypes
	cout << "  Initializing the database.";
	initDatabase(db);
	cout << "\r" << "  Database initialized.      " << endl;

	//Load values from the .txt files into the database
	//Write the required files
	for (int i = 0; i < sizeof(mandatoryFiles)/sizeof(mandatoryFiles[0]); i++) {
		writeValues(db, mandatoryFiles[i]);
	}
	
	//Write the optional files
	for (int j = 0; j < sizeof(optionalFiles)/sizeof(optionalFiles[0]); j++) {
		writeValues(db, optionalFiles[j]);
	}

	//Close the database
	sqlite3_close(db);
	lapsedTime = clock();

	cout << "Disconnected from the database." << endl;

	cout << "All operations completed in " << double(double(lapsedTime) - startTime)/CLOCKS_PER_SEC <<" seconds." << endl;

	return (0);
}
