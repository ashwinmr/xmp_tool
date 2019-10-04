#include "db_tools.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include "sqlite3.h"

namespace fs = boost::filesystem;

/**
 * Constructor opens the database
 */
Db::Db(std::string db_path) {
    // Create database connection
    sqlite3* dbc;
    int ec;

    if (!db_path.empty()) {

        // Check if database already exists
        if (fs::is_regular_file(db_path)) {
            std::cout << "Database already exists" << std::endl;
            return;
        }
        ec = sqlite3_open(db_path.c_str(), &dbc);
    } else {
        // Create table in memory
        ec = sqlite3_open(NULL, &dbc);
    }

    // Return if error opening database
    if (ec != SQLITE_OK) {
        std::cout << "Could not open database: " << sqlite3_errmsg(dbc) << std::endl;
        return;
    }

    // Store database connection
    this->dbc = dbc;

    // Create the file_tags table

    // Create sql statement
    std::string sql =
        "create table if not exists file_tags ("
        "path text,"
        "tag text);";

    this->ExecSql(sql);
}

/**
 * Execute SQL for the open database
 */
void Db::ExecSql(std::string sql) {
    /* Execute SQL statement */
    char* err_msg = 0;
    int ec;
    ec = sqlite3_exec(this->dbc, sql.c_str(), NULL, NULL, &err_msg);
    if (ec != SQLITE_OK) {
        std::cout << "Error executing sql:\n\t" << *err_msg << std::endl;
        sqlite3_free(err_msg);
        return;
    }
}

/**
 * Destructor closes the database
 */
Db::~Db() {
    // Close database
    if(this->dbc){
        sqlite3_close(this->dbc);
    }
}

/**
 * Check if database is open
 */
bool Db::IsOpen(){
    if(this->dbc){
        return true;
    }
    else{
        return false;
    }
}

/**
 * Insert a row into the table of the database
 */
void Db::InsertRow(std::string path, std::string tag) {
    // Create sql statement
    std::string sql = "insert into file_tags values(\"" + path + "\",\"" + tag + "\");";

    this->ExecSql(sql);
}
