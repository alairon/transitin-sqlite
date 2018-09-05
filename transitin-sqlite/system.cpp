#include "stdafx.h"
#include "system.hpp"
#include "validation.hpp"

using namespace std;

string GTFSDir = "GTFS_Samples";
string defaultGTFSFilename = "GTFS_Data.sqlite";
string GTFSErrorFilename = "GTFS_Errors.txt";

string mandatoryFiles[6] = { "agency", "stops", "routes", "trips", "stop_times", "calendar" };
string optionalFiles[7] = { "calendar_dates", "fare_attributes", "fare_rules", "shapes", "frequencies", "transfers", "feed_info" };

/*Checks if files listed as "Required" under the GTFS are present*/
bool mandatoryFileCheck() {
	bool fileFlag;
	bool returnFlag = true;
	bool calendarFlag = false; //Check for a substitute for calendar.txt if the flag is true

	for (int i = 0; i < sizeof(mandatoryFiles) / sizeof(mandatoryFiles[0]); i++) {
		string fileLoc = GTFSDir + "/" + mandatoryFiles[i] + ".txt";

		fileFlag = fileExists(fileLoc);

		//Set flag if calendar doesn't exist
		if (mandatoryFiles[i] == "calendar" && !fileFlag) {
			fileLoc = GTFSDir + "/calendar_dates.txt";
			calendarFlag = fileExists(fileLoc);
			if (!calendarFlag) {
				cout << "Required file missing: calendar and calendar_dates" << endl;
				returnFlag = false;
			}
			else {
				cout << "Required file missing: calendar. Using calendar_dates as a substitute." << endl;
			}
		}
		else if (!fileFlag) {
			cout << "Required file missing: " << mandatoryFiles[i] << endl;
			returnFlag = false;
		}
	}
	return (returnFlag);
}

/*Shows/Hides the blinking console cursor*/
void showConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

/*Initialize/Clear the error output file*/
void initErrorFile(time_t currentTime) {
//The file output name can be changed under files.cpp
	ofstream initErrFile;
	initErrFile.open(GTFSErrorFilename, ios::trunc);
	char time[26];
	ctime_s(time, sizeof time, &currentTime);
	initErrFile << "This file was created on " << time << endl;
	initErrFile.close();
}