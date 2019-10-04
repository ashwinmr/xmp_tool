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

    // Create file tags table if not already present
    this->CreateFileTagsTable();
}

/**
 * Create file_tags table
 */
void Db::CreateFileTagsTable() {

    // Create sql statement
    std::string sql =
        "create table if not exists file_tags ("
        "path text,"
        "tag text);";

    this->ExecSqlNoCallback(sql);
}

/**
 * Execute SQL for the open database without a callback
 */
void Db::ExecSqlNoCallback(std::string sql) {
    // Execute sql
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

    // Execute sql
    this->ExecSqlNoCallback(sql);
}

/**
 * Select paths from the table of the database
 */
std::vector<std::string> Db::SelectPaths(std::string tag) {
    // Create sql statement
    std::string sql = "select path from file_tags where tag == \"" + tag + "\" group by path;";

    std::vector<std::string> result_paths;

    // Execute sql
    char* err_msg = 0;
    int ec;
    ec = sqlite3_exec(this->dbc, sql.c_str(), this->SelectCallback, static_cast<void*>(&result_paths), &err_msg);
    if (ec != SQLITE_OK) {
        std::cout << "Error executing select statement:\n\t" << *err_msg << std::endl;
        sqlite3_free(err_msg);
        return result_paths;
    }

    return result_paths;
}

/**
 * Callback for select statement
 */
int Db::SelectCallback(void* data, int n_cols, char** values, char** headers) {

    std::vector<std::string>* result_paths = static_cast<std::vector<std::string>*>(data);

    // Store result
    result_paths->push_back(values[0]);

    return 0;
}
