#pragma once

static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void initDatabase(void* dbx);
void writeValues(void* dbx, std::string file);
void writeErrors(std::string errorLine);