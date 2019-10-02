#include "db_tools.hpp"
#include "sqlite3.h"
#include <iostream>

void OpenDB(){
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("temp/example.db", &db);

   if( rc ) {
      std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
      return;
   } else {
      std::cout << "Opened database successfully" << std::endl;
   }
   sqlite3_close(db);
}
