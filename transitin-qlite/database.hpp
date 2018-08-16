#pragma once

/* A standard callback function */
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

/** initDatabase 
	Initializes a database. Requires a sqlite3 database file to be passed in.
**/
void initDatabase(void* dbx);
void writeValues(void* dbx, std::string file);
