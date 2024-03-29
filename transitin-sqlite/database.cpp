#include "stdafx.h"
#include "database.hpp"
#include "system.hpp"
#include "parser.hpp"

using namespace std;

/*The callback function used when SELECT statements are called*/
int callback(void *pArg, int argc, char **argv, char **colName)
{
	for (int i = 0; i < argc; i++)
	{
		cout << colName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
	}
	cout << endl;
	return 0;
}

/*Initalizes the database with the standard headers specified under the GTFS*/
void initDatabase(void* dbx) {
	sqlite3 *db = (sqlite3*)dbx;
	const int STATEMENTS = 13;
	char *zErrMsg = 0;
	const char *pSQL[STATEMENTS];
	int rc;

	//agency
	pSQL[0] = "CREATE TABLE IF NOT EXISTS agency ("
		"agency_id varchar(16) UNIQUE ON CONFLICT REPLACE,"
		"agency_name varchar(128),"
		"agency_url varchar(256),"
		"agency_timezone varchar(64),"
		"agency_lang varchar(2),"
		"agency_phone varchar(16),"
		"agency_fare_url varchar(256),"
		"agency_email varchar(64)"
		")";
	//stops
	pSQL[1] = "CREATE TABLE IF NOT EXISTS stops ("
		"stop_id varchar(32) UNIQUE ON CONFLICT REPLACE,"
		"stop_code varchar(16) UNIQUE ON CONFLICT REPLACE,"
		"stop_name varchar(128),"
		"stop_desc varchar(128),"
		"stop_lat decimal(8,6),"
		"stop_lon decimal(9,6),"
		"zone_id varchar(32),"
		"stop_url varchar(256),"
		"location_type integer,"
		"parent_station varchar(128),"
		"stop_timezone varchar(128),"
		"wheelchair_boarding integer"
		")";
	//routes
	pSQL[2] = "CREATE TABLE IF NOT EXISTS routes ("
		"route_id varchar(32) UNIQUE ON CONFLICT REPLACE,"
		"agency_id varchar(32),"
		"route_short_name varchar(16),"
		"route_long_name varchar(128),"
		"route_desc varchar(256),"
		"route_type integer,"
		"route_url varchar(256),"
		"route_color varchar(6),"
		"route_text_color varchar(6),"
		"route_sort_order integer unsigned"
		")";
	//trips
	pSQL[3] = "CREATE TABLE IF NOT EXISTS trips ("
		"route_id varchar(32),"
		"service_id varchar(16),"
		"trip_id varchar(32) UNIQUE ON CONFLICT REPLACE,"
		"trip_headsign varchar(128),"
		"trip_short_name varchar(128),"
		"direction_id boolean,"
		"block_id varchar(32),"
		"shape_id varchar(8),"
		"wheelchair_accessible integer unsigned,"
		"bikes_allowed integer unsigned"
		")";
	//stop_times
	pSQL[4] = "CREATE TABLE IF NOT EXISTS stop_times ("
		"trip_id varchar(16),"
		"arrival_time varchar(8),"
		"departure_time varchar(8),"
		"stop_id varchar(16),"
		"stop_sequence integer unsigned,"
		"stop_headsign varchar(128),"
		"pickup_type varchar(128),"
		"drop_off_type varchar(128),"
		"shape_dist_traveled integer,"
		"timepoint varchar(128)"
		")";
	//calendar
	pSQL[5] = "CREATE TABLE IF NOT EXISTS calendar ("
		"service_id varchar(16) UNIQUE ON CONFLICT REPLACE,"
		"monday boolean,"
		"tuesday boolean,"
		"wednesday boolean,"
		"thursday boolean,"
		"friday boolean,"
		"saturday boolean,"
		"sunday boolean,"
		"start_date varchar(8),"
		"end_date varchar(8)"
		")";
	//calendar_dates
	pSQL[6] = "CREATE TABLE IF NOT EXISTS calendar_dates ("
		"service_id varchar(16),"
		"date varchar(8),"
		"exception_type integer"
		")";
	//fare_attributes
	pSQL[7] = "CREATE TABLE IF NOT EXISTS fare_attributes ("
		"fare_id varchar(16) UNIQUE ON CONFLICT REPLACE,"
		"price varchar(16),"
		"currency_type varchar(3),"
		"payment_method boolean,"
		"transfers integer,"
		"agency_id varchar(16),"
		"transfer_duration integer unsigned"
		")";
	//fare_rules
	pSQL[8] = "CREATE TABLE IF NOT EXISTS fare_rules ("
		"fare_id varchar(16),"
		"route_id varchar(16),"
		"origin_id varchar(16),"
		"destination_id varchar(16),"
		"contains_id varchar(16)"
		")";
	//shapes
	pSQL[9] = "CREATE TABLE IF NOT EXISTS shapes ("
		"shape_id varchar(16),"
		"shape_pt_lat decimal(8,6),"
		"shape_pt_lon decimal(9,6),"
		"shape_pt_sequence integers unsigned,"
		"shape_dist_traveled integer"
		")";
	//frequencies
	pSQL[10] = "CREATE TABLE IF NOT EXISTS frequencies ("
		"trip_id varchar(16),"
		"start_time varchar(8),"
		"end_time varchar(8),"
		"headway_secs integer unsigned,"
		"exact_times boolean"
		")";
	//transfers
	pSQL[11] = "CREATE TABLE IF NOT EXISTS transfers ("
		"from_stop_id varchar(16),"
		"to_stop id varchar(16),"
		"transfer_type integer,"
		"min_transfer_time integer unsigned"
		")";
	//feed_info
	pSQL[12] = "CREATE TABLE IF NOT EXISTS feed_info ("
		"feed_publisher_name varchar(128),"
		"feed_publisher_url varchar(256),"
		"feed_lang varchar(8),"
		"feed_start_date varchar(8),"
		"feed_end_date varchar(8),"
		"feed_version varchar(32)"
		")";

	//Insert the tables into the database
	for (int i = 0; i < STATEMENTS; i++) {
		rc = sqlite3_exec(db, pSQL[i], callback, 0, &zErrMsg);
		//Appears if one of the pSQL commands has an SQLite error.
		if (rc != SQLITE_OK) {
			cout << "  SQL Error on command " << i << ". " << sqlite3_errmsg(db) << endl;
			sqlite3_free(zErrMsg);
		}
	}
}

/*Inserts values into the database*/
void writeValues(void* dbx, string file) {
	sqlite3 *db = (sqlite3*)dbx;
	char* zErrMsg;
	int rc;

	//Counters
	int linesProcessed = 0; //Counts the number of lines processed
	int bytesProcessed = 0; //Used by seekg(). Used to calculate % completion
	int errors = 0; //Counts the number of errors
	int totalBytes = 0; //Stores the total number of bytes from seekg(<file pointer>.end)
	double completion = 0; //Determines % completion

	string headers; //The columns from the GTFS file to be inserted
	string line; //The line being read from the file
	string sqlCommand;

	ifstream fp(GTFSDir + "/" + file + ".txt");

	//Skip everything else if the file could not be opened.
	if (!fp.is_open()) {
		cout << "  " << file << ": <N/A>" << endl;
		return;
	}

	//Determine number of lines that need to be processed
	fp.seekg(0, fp.end);
	totalBytes = static_cast<int>(fp.tellg());
	fp.seekg(0, fp.beg);

	//Read the first line to gather headers
	getline(fp, headers);

	//Check if the file is encoded with a byte-order mark
	headers = checkEncoding(headers); 
	checkTableCol(db, file, headers);

	//Read the read of the lines and insert into the table
	sqlite3_exec(db, "BEGIN TRANSACTION", callback, 0, &zErrMsg);
	while (getline(fp, line)) {
		sqlCommand = "INSERT OR REPLACE INTO " + file + "(" + headers + ") VALUES (" + quoteLine(line) + ")";
		rc = sqlite3_exec(db, sqlCommand.c_str(), callback, 0, &zErrMsg);
		linesProcessed++;
		bytesProcessed = static_cast<int>(fp.tellg());

		//If an error appears, write it into the error output file
		if (rc) {
			errors++;
			ofstream errorOut;
			errorOut.open("GTFS_Errors.txt", ios_base::app); //Append to end of file
			errorOut << "An error occurred on line " << linesProcessed << " while processing '" << file << "'" << endl << "  Msg:" << zErrMsg << endl << "  SQL: " << sqlCommand << endl << endl;
			errorOut.close();
		}

		completion = (bytesProcessed / (double)totalBytes) * 100.00;

		//Set double decimal precision
		cout << fixed;
		cout << setprecision(2);

		//Output current status with number of errors, if any
		if (errors > 0) {
			cout << "\r" << "  " << file << ": " << linesProcessed << " line(s) processed with " << errors << " errors [" << completion << "%].";
		}
		else {
			cout << "\r" << "  " << file << ": " << linesProcessed << " line(s) processed " << "[" << completion << "%].                              ";
		}
	}

	//Finalize changes
	sqlite3_exec(db, "COMMIT", callback, 0, &zErrMsg);
	cout << "\r" << "  " << file << ": " << linesProcessed << " line(s) processed with " << errors << " error(s).                             ";
	cout << endl;
	fp.close();
}

/*Checks if a table exists from the file header. If a column doesn't exist, create one*/
void checkTableCol(void * dbx, string table, string header){
	sqlite3 *db = (sqlite3*)dbx;

	stringstream iss(header);
	string token;

	//Output the affected table for the user to see
	cout << "  " << table << ":";

	//Go through the headers
	while (getline(iss, token, ',')) {
		//Check if the column would exist
		string sqlCommand = "SELECT " + token + " FROM " + table + " LIMIT 0";
		int rc = sqlite3_exec(db, sqlCommand.c_str(), 0, 0, NULL);

		//If the command wasn't able to find the column in the table, ask the user to create one
		if (rc != SQLITE_OK) {
			createTableCol(db, token, table);
		}
	}

	return;
}

/*Creates a new table column. Acts as a wizard.*/
void createTableCol(void* dbx, string table, string token) {
	sqlite3* db = (sqlite3*)dbx;
	bool validInputFlag = true;
	int totalPlaces = 8; //Default total number of digits
	int decimalPoints = 2; //Default decimal points
	int varcharSize = 128; //Set a default value
	string userAnswer;
	string sqlCommand;

	cout << endl;
	cout << "    \"" << token << "\" currently doesn't exist in the database." << endl;
	cout << "    Would you like to add this column? [y/N]? ";

	showConsoleCursor(true); //Enable cursor to indicate that the program wants user action
	cin >> userAnswer;
	cin.ignore();

	//If the user answered with 'Y' or 'y' to the previous question
	if (userAnswer[0] == 'Y' || userAnswer[0] == 'y') {
		do {
			cout << "    What kind of data will this field contain?" << endl;
			cout << "    1. True/False flags (boolean)" << endl;
			cout << "    2. Numbers without decimals" << endl;
			cout << "    3. Numbers with decimals" << endl;
			cout << "    4. Numbers and/or letters (small)" << endl;
			cout << "    5. Numbers and/or letters (large)" << endl;
			cout << "    : ";
			cin >> userAnswer;
			cin.ignore();

			//Perform action based on input
			switch (userAnswer[0]) {
			case '1': //boolean
				sqlCommand = "ALTER TABLE " + table + " ADD COLUMN " + token + " bool";
				sqlite3_exec(db, sqlCommand.c_str(), 0, 0, NULL);
				validInputFlag = true;
				break;
			case '2': //integer
				sqlCommand = "ALTER TABLE " + table + " ADD COLUMN " + token + " integer";
				sqlite3_exec(db, sqlCommand.c_str(), 0, 0, NULL);
				validInputFlag = true;
				break;
			case '3': //decimals
				sqlCommand = "ALTER TABLE " + table + " ADD COLUMN " + token + " decimal(" + to_string(totalPlaces) + "," + to_string(decimalPoints) + ")";
				sqlite3_exec(db, sqlCommand.c_str(), 0, 0, NULL);
				validInputFlag = true;
				break;
			case '4': //small varchar (set to 8 characters), such as a date written as 'YYYYMMDD'
				sqlCommand = "ALTER TABLE " + table + " ADD COLUMN " + token + " varchar(8)";
				sqlite3_exec(db, sqlCommand.c_str(), 0, 0, NULL);
				validInputFlag = true;
				break;
			case '5': //large varchar (set to 128 characters)
				sqlCommand = "ALTER TABLE " + table + " ADD COLUMN " + token + " varchar(" + to_string(varcharSize) + ")";
				sqlite3_exec(db, sqlCommand.c_str(), 0, 0, NULL);
				validInputFlag = true;
				break;
			default:
				cout << "    Invalid option. Please try again" << endl;
				validInputFlag = false;
			}
		} while (validInputFlag == false);
	}
	showConsoleCursor(false); //Hide the cursor
}