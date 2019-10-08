#include "db_tools.hpp"
#include "sqlite3.h"
#include <string>
#include <queue>
#include <regex>
#include <boost/filesystem.hpp>
#include <iostream>

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
        std::cout << "Error executing sql:\n\t" << err_msg << std::endl;
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
std::vector<std::string> Db::SelectTagQuery(std::string tag_query) {

    std::vector<std::string> result_paths;

    // Generate sql statement
    std::string sql;
    try{
        sql = this->GenSql(tag_query);
    }
    catch(const std::invalid_argument& e){
        std::cout << "GenSql Exception: \n\t" << e.what() << std::endl;
    }

    std::cout << sql << std::endl;

    // // Execute sql
    // char* err_msg = 0;
    // int ec;
    // ec = sqlite3_exec(this->dbc, sql.c_str(), this->SelectCallback, static_cast<void*>(&result_paths), &err_msg);
    // if (ec != SQLITE_OK) {
    //     std::cout << "Error executing select statement:\n\t" << err_msg << std::endl;
    //     sqlite3_free(err_msg);
    //     return result_paths;
    // }

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

/**
 * Tokenize an input query
 */
std::vector<std::string> Db::Tokenize(std::string query) {

    std::vector<std::string> result;

    // Regex expression
    std::regex ex("\\w+");

    std::regex_iterator<std::string::iterator> pos(query.begin(),query.end(), ex);
    std::regex_iterator<std::string::iterator> end; // Default constructor defines past-the-end iterator
    for (; pos != end; pos++) {
        result.push_back(pos->str(0));
    }

    return result;
}

/**
 * Generate sql from tag query
 */
std::string Db::GenSql(std::string tag_query) {

    // // Create sql statement
    // std::string sql =
    // "select * from file_tags as t0 inner join ( select * from file_tags where tag == \"x\" ) as t1 on t1.path == t0.path inner join ( select * from file_tags where tag == \"figure\" ) as t2 on t2.path == t0.path group by t0.path ;";

    std::vector<std::string> tokens = this->Tokenize(tag_query);

    if(tokens.size() == 0){
        return "select * from file_tags;";
    }

    std::queue<std::string> qu;
    for(auto &token: tokens){
        qu.push(token);
    }

    std::string result = "select * from file_tags as t0 ";

    int count = 1;

    while(!qu.empty()){

        if(qu.front() == "not"){
            qu.pop();
            if(qu.empty()){
                throw std::invalid_argument("token required after not ");
            }
            result += " inner join ( select * from file_tags where tag != \"" + qu.front() + "\" ) as t" + std::to_string(count) + " on t" + std::to_string(count) + ".path == t0.path ";
            qu.pop();
            if(!qu.empty()){
                if(qu.front() == "not"){
                    throw std::invalid_argument("not can't follow not");
                }
            }
            count++;
        }
    }

    result += " group by t0.path;";

    return result;
}
