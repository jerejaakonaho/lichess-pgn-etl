#include <iostream>
#include "database.hpp"

Database::Database(const std::string& db_name) {
    // Open db or create if there isn't one
    if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
        std::cerr << "DB failed to open: " << sqlite3_errmsg(db) << std::endl;
    }

    create_table();

    // SQL command, ? replaced with hash later
    const char* sql = "INSERT INTO positions (hash) VALUES (?);";
    sqlite3_prepare_v2(db, sql, -1, &insert_statement, nullptr);

    sqlite3_exec(db, "PRAGMA synchronous = OFF;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "PRAGMA journal_mode = MEMORY;", nullptr, nullptr, nullptr);

    buffer.reserve(100000);
}

Database::~Database() {
    flush(); // Force flush even if the buffer isnt full
    sqlite3_finalize(insert_statement);
    sqlite3_close(db);
}

void Database::create_table() {
    const char* sql = "CREATE TABLE IF NOT EXISTS positions (hash BIGINT);";
    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

void Database::insert_hashes(const std::vector<uint64_t>& hashes) {
    // move game hashes to db buffer
    for (uint64_t h : hashes) {
        buffer.push_back(h);
    }

    if (buffer.size() >= 100000) {
        flush();
    }
}

void Database::flush() {
    if (buffer.empty()) return;

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    for (uint64_t h : buffer) {
        // replace ? with hash
        sqlite3_bind_int64(insert_statement, 1, h);
        sqlite3_step(insert_statement); // execute
        sqlite3_reset(insert_statement); // reset for next hash
    }

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

    buffer.clear();
}