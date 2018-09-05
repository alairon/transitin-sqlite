#pragma once

extern std::string defaultGTFSFilename;
extern std::string GTFSErrorFilename;
extern std::string GTFSDir;

extern std::string mandatoryFiles[6];
extern std::string optionalFiles[7];

/*Checks if files listed as "Required" under the GTFS are present*/
bool mandatoryFileCheck();

/*Shows/Hides the blinking console cursor*/
void showConsoleCursor(bool showFlag);

/*Initialize/Clear the error output file*/
void initErrorFile(time_t currentTime);
