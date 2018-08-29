#pragma once

/* A standard callback function */
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

/** initDatabase 
	Initializes a database. 
	Requires a sqlite3 database pointer to be passed in.
	Returns nothing.
*/
void initDatabase(void* dbx);

/**  writeValues
	Writes contents of a GTFS file into the database.
	Requires a sqlite3 database pointer to be passed in.
*/
void writeValues(void* dbx, std::string file);

void tableColExists(void* dbx, std::string table, std::string header);