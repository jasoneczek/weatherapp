#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string &dbName) {
    int result = sqlite3_open(dbName.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "Can't open database " << dbName << std::endl;
        db = nullptr;
    } else {
        std::cout << "Opened database " << dbName << std::endl;
    }
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
#ifdef DEBUG
        std::cout << "Database closed" << std::endl;
#endif
    }
}

bool DatabaseManager::initialize() {
    if (!db) return false;

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL
        );
    )";

    char* errMsg = nullptr;
    int result = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (result != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    std::cout << "Users table ready." << std::endl;
    return true;
}

bool DatabaseManager::registerUser(const std::string &username, const std::string &password) {
    if (!db) return false;

    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result == SQLITE_DONE) {
        std::cout << "User " << username << " registered." << std::endl;
        return true;
    } else if (result == SQLITE_CONSTRAINT) {
        std::cerr << "That username is already taken.\n";
        return false;
    } else {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
}

bool DatabaseManager::loginUser(const std::string &username, const std::string &password) {
    if (!db) return false;

    const char* sql = "SELECT * FROM users WHERE username = ? AND password = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return success;
}


