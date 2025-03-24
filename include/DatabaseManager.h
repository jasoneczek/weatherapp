#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>

class DatabaseManager {
    public:
        DatabaseManager(const std::string &dbName);
        ~DatabaseManager();

        bool initialize();
        bool registerUser(const std::string &username, const std::string &password);
        bool loginUser(const std::string &username, const std::string &password);

    private:
        std::string dbName;
        sqlite3 *db;
};

#endif //DATABASEMANAGER_H
