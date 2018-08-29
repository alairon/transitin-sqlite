#include "stdafx.h"
#include "validation.hpp"

using namespace std;

string defaultGTFSFilename = "GTFS_Data.sqlite";
string GTFSErrorFilename = "GTFS_Errors.txt";
string GTFSDir = "GTFS_Samples";

string mandatoryFiles[6] = { "agency", "stops", "routes", "trips", "stop_times", "calendar" };
string optionalFiles[7] = { "calendar_dates", "fare_attributes", "fare_rules", "shapes", "frequencies", "transfers", "feed_info" };

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

//Initialize/Clear the error output file
void initErrorFile(time_t currentTime) {
//The file output name can be changed under files.cpp
	ofstream initErrFile;
	initErrFile.open(GTFSErrorFilename, ios::trunc);
	char time[26];
	ctime_s(time, sizeof time, &currentTime);
	initErrFile << "This file was created on " << time << endl;
	initErrFile.close();
}