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
     * Desctuctor closes the database
     */
    ~Db();

    /**
     * Check if database is open
     */
    bool IsOpen();

    private:
    void ExecSql(std::string sql);

    sqlite3* dbc = NULL;
};

#endif // DB_TOOLS_H_
