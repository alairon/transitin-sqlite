#include "stdafx.h"
#include "database.hpp"

using namespace std;

database::database()
{
}

int database::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
	}
	cout << "\n";
	return 0;
}

void database::initDatabase(void* dbx) {
	sqlite3 *db = (sqlite3*)dbx;
	const int STATEMENTS = 13;
	char *zErrMsg = 0;
	const char *pSQL[STATEMENTS];
	int rc;

	pSQL[0] = "CREATE TABLE IF NOT EXISTS agency ("
		"agency_id varchar(10) UNIQUE ON CONFLICT REPLACE,"
		"agency_name varchar(99),"
		"agency_url varchar(256),"
		"agency_timezone varchar(64),"
		"agency_lang varchar(2),"
		"agency_phone varchar(15),"
		"agency_fare_url varchar(256),"
		"agency_email varchar(64)"
		")";
	pSQL[1] = "CREATE TABLE IF NOT EXISTS stops ("
		"stop_id varchar(20) UNIQUE ON CONFLICT REPLACE,"
		"stop_code varchar(10) UNIQUE ON CONFLICT REPLACE,"
		"stop_name varchar(99),"
		"stop_desc varchar(99),"
		"stop_lat decimal(8,6),"
		"stop_lon decimal(9,6),"
		"zone_id varchar(32),"
		"stop_url varchar(256),"
		"location_type integer,"
		"parent_station varchar(99),"
		"stop_timezone varchar(99),"
		"wheelchair_boarding integer"
		")";
	pSQL[2] = "CREATE TABLE IF NOT EXISTS routes ("
		"route_id varchar(20) UNIQUE ON CONFLICT REPLACE,"
		"agency_id varchar(30),"
		"route_short_name varchar(16),"
		"route_long_name varchar(99),"
		"route_desc varchar(256),"
		"route_type varchar(128),"
		"route_url varchar(256),"
		"route_color varchar(6),"
		"route_text_color varchar(6),"
		"route_sort_order integer unsigned"
		")";
	pSQL[3] = "CREATE TABLE IF NOT EXISTS trips ("
		"route_id varchar(20),"
		"service_id varchar(10),"
		"trip_id varchar(32) UNIQUE ON CONFLICT REPLACE,"
		"trip_headsign varchar(99),"
		"trip_short_name varchar(99),"
		"direction_id boolean,"
		"block_id varchar(32),"
		"shape_id varchar(8)"
		")";
	pSQL[4] = "CREATE TABLE IF NOT EXISTS stop_times ("
		"trip_id varchar(10),"
		"arrival_time varchar(8),"
		"departure_time varchar(8),"
		"stop_id varchar(10),"
		"stop_sequence integer unsigned,"
		"stop_headsign varchar(99),"
		"pickup_type varchar(99),"
		"drop_off_type varchar(99),"
		"shape_dist_traveled integer,"
		"timepoint varchar(99)"
		")";
	pSQL[5] = "CREATE TABLE IF NOT EXISTS calendar ("
		"service_id varchar(10) UNIQUE ON CONFLICT REPLACE,"
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
	pSQL[6] = "CREATE TABLE IF NOT EXISTS calendar_dates ("
		"service_id varchar(20),"
		"date varchar(8),"
		"exception_type integer"
		")";
	pSQL[7] = "CREATE TABLE IF NOT EXISTS fare_attributes ("
		"fare_id varchar(10) UNIQUE ON CONFLICT REPLACE,"
		"price varchar(10),"
		"currency_type varchar(3),"
		"payment_method boolean,"
		"transfers integer,"
		"agency_id varchar(10),"
		"transfer_duration integer unsigned"
		")";
	pSQL[8] = "CREATE TABLE IF NOT EXISTS fare_rules ("
		"fare_id varchar(10),"
		"route_id varchar(10),"
		"origin_id varchar(10),"
		"destination_id varchar(10),"
		"contains_id varchar(10)"
		")";
	pSQL[9] = "CREATE TABLE IF NOT EXISTS shapes ("
		"shape_id varchar(16),"
		"shape_pt_lat decimal(8,6),"
		"shape_pt_lon decimal(9,6),"
		"shape_pt_sequence integers unsigned,"
		"shape_dist_draveled integer"
		")";
	pSQL[10] = "CREATE TABLE IF NOT EXISTS frequencies ("
		"trip_id varchar(10),"
		"start_time varchar(8),"
		"end_time varchar(8),"
		"headway_secs integer unsigned,"
		"exact_times boolean"
		")";
	pSQL[11] = "CREATE TABLE IF NOT EXISTS transfers ("
		"from_stop_id varchar(10),"
		"to_stop id varchar(10),"
		"transfer_type integer,"
		"min_transfer_time integer unsigned"
		")";
	pSQL[12] = "CREATE TABLE IF NOT EXISTS feed_info ("
		"feed_publisher_name varchar(99),"
		"feed_publisher_url varchar(256),"
		"feed_lang varchar(8),"
		"feed_start_date varchar(8),"
		"feed_end_date varchar(8),"
		"feed_version varchar(24)"
		")";

	for (int i = 0; i < STATEMENTS; i++) {
		rc = sqlite3_exec(db, pSQL[i], callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			cout << endl << "SQL Error on line" << i << ". " << sqlite3_errmsg(db) << endl << endl;
			sqlite3_free(zErrMsg);
		}
	}

	sqlite3_close(db);
}