#pragma once

extern std::string defaultGTFSFilename;
extern std::string GTFSErrorFilename;
extern std::string GTFSDir;

extern std::string mandatoryFiles[6];
extern std::string optionalFiles[7];

bool mandatoryFileCheck();

void initErrorFile(time_t currentTime);
