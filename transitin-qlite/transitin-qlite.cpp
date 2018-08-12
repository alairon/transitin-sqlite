// transitin-qlite.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "database.hpp"

using namespace std;

//Hide the cursor. Windows OS only.
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

int main(int argc, char* argv[])
{
	ShowConsoleCursor(false);
	clock_t startTime;
	clock_t lapsedTime;
	string dbFile;
	string mandatoryFiles[] = { "agency", "stops", "routes", "trips", "stop_times", "calendar" };
	string optionalFiles[] = { "calendar_dates", "fare_attributes", "fare_rules", "shapes", "frequencies", "transfers", "feed_info" };

	sqlite3 *db;
	int rc;

	//If the user specified a file name, use that. Otherwise, use a default name.
	if (argc == 2) {
		dbFile = (string)argv[1] + ".sqlite3";
	}
	else {
		cout << "No file was specified, the program will use the default name." << endl;
		dbFile = "GTFS_Data.sqlite3";
	}

	//Attempt to open the database file, or create one if it doesn't already exist.
	cout << "Connecting to the database: " + dbFile;
	rc = sqlite3_open(dbFile.c_str(), &db);
	if (rc) {
		cout << "  " << dbFile << " could not be opened." << endl << "  Msg: " << sqlite3_errmsg(db) << endl;
		exit(1);
	}
	cout << "\r" << "Connected to the database: " << dbFile << "\t\t\t\t" << endl;

	startTime = clock();
	//Create table headers with GTFS datatypes
	cout << "  Initializing the database.";
	initDatabase(db);
	cout << "\r" << "  Database initialized.      " << endl;

	//Write the required files
	//Load values from the .txt files into the database
	for (int i = 0; i < sizeof(mandatoryFiles)/sizeof(mandatoryFiles[0]); i++) {
		writeValues(db, mandatoryFiles[i]);
	}
	
	//Write the optional files
	for (int j = 0; j < sizeof(optionalFiles)/sizeof(optionalFiles[0]); j++) {
		writeValues(db, optionalFiles[j]);
	}

	//Close the database
	sqlite3_close(db);
	cout << "Disconnected from the database." << endl;

	lapsedTime = clock();
	cout << "All operations completed in " << double(lapsedTime - startTime)/CLOCKS_PER_SEC <<" seconds. Press the RETURN key to exit." << endl;
	getchar();

    return 0;  
}
