#ifndef DB_TOOLS_H_
#define DB_TOOLS_H_

#include "sqlite3.h"
#include <string>

class Db{
    public:

    /**
     * Constructor creates a database
     */
    Db(std::string db_path);

    /**
     * Insert a row into the table of the database
     */
    void InsertRow(std::string path, std::string tag);

    /**
     * Select rows from the table of the database
     */
    void SelectRows(std::string tag);

    /**
     * Desctuctor closes the database
     */
    ~Db();

    /**
     * Check if database is open
     */
    bool IsOpen();

    private:

    /**
     * Execute SQL for the open database without a callback
     */
    void Db::ExecSqlNoCallback(std::string sql);

    /**
     * Create file_tags table
     */
    void Db::CreateFileTagsTable();

    /**
     * Callback for select statement
     */
    static int SelectCallback(void* not_used, int n_cols, char** values, char** headers);

    sqlite3* dbc = NULL;
};

#endif // DB_TOOLS_H_
