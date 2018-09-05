#pragma once

/*The callback function used when SELECT statements are called*/
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

/*Initalizes the database with the standard headers specified under the GTFS*/
void initDatabase(void* dbx);

/*Inserts values into the database*/
void writeValues(void* dbx, std::string file);

/*Checks if a table exists from the file header. If a column doesn't exist, create one*/
void checkTableCol(void* dbx, std::string table, std::string header);

/*Creates a new table column. Acts as a wizard.*/
void createTableCol(void* dbx, std::string table, std::string token);