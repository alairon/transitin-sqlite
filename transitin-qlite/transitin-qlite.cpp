// transitin-qlite.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "database.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	string dbFile;
	sqlite3 *db;
	int rc;

	if (argc == 2) {
		dbFile = (string)argv[1] + ".sqlite3";
	}
	else {
		cout << "No file was specified, the program will use the default name." << endl;
		dbFile = "GTFS_Data.sqlite3";
	}
	
	cout << "Connecting to the database: " + dbFile << endl;
	rc = sqlite3_open(dbFile.c_str(), &db);

	if (rc) {
		cout << "  " << dbFile << " could not be opened." << endl << "  Msg: " << sqlite3_errmsg(db) << endl;
		exit(1);
	}
	cout << "Connected to the database." << endl;

	cout << "  Initializing the database." << endl;
	initDatabase(db);
	cout << "  Database initialized." << endl;




	sqlite3_close(db);
	cout << "Disconnected from the database." << endl;

	cout << "All operations completed. Press the RETURN key to exit." << endl;
	getchar();

    return 0;  
}
