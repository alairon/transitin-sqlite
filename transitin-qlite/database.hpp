#pragma once
class database
{
public:
	database();
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);
	void initDatabase(void* dbx);
};

