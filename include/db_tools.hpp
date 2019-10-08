#ifndef DB_TOOLS_H_
#define DB_TOOLS_H_

#include "sqlite3.h"
#include <string>
#include <vector>

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
     * Select paths from the table of the database
     */
    std::vector<std::string> SelectTagQuery(std::string tag);

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

    /**
     * Tokenize an input query
     */
    std::vector<std::string> Db::Tokenize(std::string query);

    /**
     * Generate sql from tag query
     */
    std::string Db::GenSql(std::string tag_query);

    sqlite3* dbc = NULL;
};

#endif // DB_TOOLS_H_
